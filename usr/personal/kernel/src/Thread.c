#include "../include/Thread.h"
#include "../include/Thread_U.h"
#include "../../user/include/param.h"

/*
 * 系统调用CreateThread的内核实现
 */
void CreateThread()
{
	char name[12];
	register Thread *thread;
	register int id;
	register U_Thread *uthd = &u.u_threads[u.u_curthu];
	char *stack;

	if(u.u_procp->p_threadNum == NTHREAD_PROC)
	{
		uthd->u_error = ENOTHDSPC;
		return;
	}
	
	//获得线程名称，getname见10.7.2.1.2节
		if(getname(name, 12)) 
		return;
	if(threadexist(name) != NULL) //该名称线程已存在
	{
		uthd->u_error = EEXIST;
		return;
	}
	
	if((id=threadalloc()) == -1)
	{
		return;
	}

	thread = &Threads[id];
	if(thread->t_index = thpalloc(u.u_procp) == -1)
	{
		return;
	}

	//分配用户栈空间
	stack = AllocStack(uthd->u_arg[5]);
	if(stack == NULL)
	{
		uthd->u_error = ENOSPC;
		return;
	}
	thread->t_ustackaddr = stack;
	thread->t_ustacksize = uthd->u_arg[5];
	u.u_procp->p_threads[thread->t_index] = thread;
	strcpy(thread->t_name, name);

	thread->t_pri = uthd->u_arg[1];		//用户程序传递的iPriority参数
		thread->t_cpu = 0;		
	thread->t_time = 0;
	thread->t_nice = u.u_procp->t_nice;
	thread->t_ttyp = u.u_procp->p_ttyp;

	//从进程继承uid、gid等
	u.u_threads[thread->t_index].u_uid = u.u_uid;
	u.u_threads[thread->t_index].u_gid = u.u_gid;
	u.u_threads[thread->t_index].u_ruid = u.u_ruid;
	u.u_threads[thread->t_index].u_rgid = u.u_rgid;
	
	if(u.u_threads[u.u_curthu].u_arg[2] == THREAD_RUN)
	{
		thread->t_stat = SRUN;
		++u.u_procp->p_actThreadNum;
		rp->p_stat = SRUN;
	    if(thread->t_pri < curpri)	//新创建线程优先级更高，则设置调度标志
		runrun++;
	}
	else
	{
			spl6();
		thread->t_wchan = &thread->t_tid;
		thread->t_stat = SSLEEP;
		thread->t_pri = SUSPPRI;
	}
	
	thread->t_flag = SLOAD;
	
	//为了避免t_tid值固定
	if((thread->t_tid % NTHREAD) == id)
		thread->t_tid += NTHREAD; 	
	else
		thread->t_tid = id;
	if(thread->t_tid < 0)	
		thread->t_tid = id;
	
	thread->t_pid = u.u_procp->p_pid;
	thread->t_proc = u.u_procp;
	thread->t_entry = uthd->u.u_arg[3];
	thread->t_param = uthd->u_arg[4];
	u.u_procp->p_thdsysentry = uthd->u_arg[6];

	//搭建内核栈，为首次执行做准备
   	u.u_threads[thread->t_index].u_threadp = thread;
	SetupKernStack(&u.u_threads[thread->t_index]);

	//设置r6和r5的值
	u.u_threads[thread->t_index].u_rsav[0] = 
			&u.u_threads[thread->t_index].u_kernstack[790];
	u.u_threads[thread->t_index].u_rsav[1] = 
	&u.u_threads[thread->t_index].u_kernstack[798];

	uthd->u_ar0[R0] = thread->t_tid;//返回线程ID
	return;
}

/*
 * 分配进程proc结构中的线程指针
 */
int thpalloc(struct proc *p)
{
		register int i;
		for(i = 0; i< NTHREAD_PROC; i++)
		{
			if(p->p_threads[i] == NULL)
			{
				p->p_threadNum++;
				return i;
			}
		}
	panic(“Unexpected exception: no threads pointer space”);
		return -1;
}

int thpfree(struct Thread *t)
{
		t->t_proc->p_threads[t->t_index] =NULL;
		t->t_proc->p_threadNum--;
		return 0;
}

/*
 * 分配线程句柄
 */
int thdalloc()
{
	register int i;
	register U_Thread *uthd = &u.u_threads[u.u_curthu];
	for(i = 0; i< NTHREAD; i++)
	{
		if(Threads[i].t_stat == 0)
			return i;
	}
	uthd->u_error = ENOTHDSPC;
	return -1;
}	

void thdfree(int thd)
{
	Threads[thd%NTHREAD].t_stat = 0;
}

	//判断名为name的线程是否已存在，如果存在，则返回线程结构指针，否则返回NULL。
Thread *threadexist(char *name)
{
		register Thread *thd ;
		for(thd = Threads; thd < Threads + NTHREAD; thd++)
		{
			if(thd->t_stat && strcmp(thd->t_name, name) == 0)
			{
				return thd;
			}
		}
		return NULL;
}


/*
 * 系统调用ExitThread的内核实现。
 */
void ExitThread()
{
	register int *q, a;
		register U_Thread *uthd = &u.u_threads[u.u_curthu];
		//关闭所有打开的文件
		for(q = uthd->u_ofile[0]; q < &uthd->u_ofile[NOFILE]; q++)
		{	
		if(a = *q) {
			*q = NULL;
			closef(a);
		}
	}
	iput(uthd->u_cdir);
	//释放栈空间
	FreeStack(uthd->u_threadp->t_ustackaddr, uthd->u_threadp->t_ustacksize);
	
	//释放线程结构
	thdfree(uthd->u_threadp);
	u.u_procp->p_threads[u.u_curthu] = NULL;
	if(--u.u_procp->p_threadNum == 0)	//是最后一个线程，则退出进程
			exit();
	if(uthd->u_threadp->t_stat == SRUN)
		{
			if(--u.u_procp->p_actThreadNum == 0)
				u.u_procp->p_stat = SWAIT;
	}
	swtch();
}


/*
 * 系统调用gettid的内核实现
 */
void gettid()
{
		u.u_threads[u.u_curthu].u_ar0[R0] = 
		u.u_threads[u.u_curthu].u_threadp->t_tid;
}



/*
 * 系统调用SuspendThread的内核实现
 */
void SuspendThread()
{
	register U_Thread *uthd = &u.u_threads[u.u_curthu];
register int id = uthd->u_ar0[R0], a;
	register Thread *pthd;
	//判断ID的有效性
	if(id == 0)
	{
		uthd->u_error = EINVTHDID;
		return;
	}
	if(id < 0 || id == uthd->u_threadp->t_tid)	//挂起自己
	{
		sleep(&uthd->u_threadp->t, SUSPPRI);
		return;
	}
	pthd = &Threads[id%NTHREAD];
	if((pthd->t_stat == 0)
	{
		uthd->u_error = EINVTHDID;
		return;
	}
	
	if((pthd->t_stat != SRUN)
	{
		uthd->u_error = EINVTHDSTATE;
		return;
	}

	if(pthd->t_proc != u.u_procp)
	{
		if(!suser())
		{
			return;
		}
	}

	//挂起其他线程
	suspthd(pthd); 
}

/*
 * 挂起线程thd，同时切换CPU到其他线程。
 */
void suspthd(Thread *thd) 
{
    register struct Thread*rt = thd, s;

    s = PS->integ;
    spl6();
		rt->t_wchan = &rt->t_tid;
		rt->t_stat = SSLEEP;
		rt->t_pri = SUSPPRI;
		spl0();
		if(--u.u_procp->p_actThreadNum == 0)
		u.u_procp->p_stat = SWAIT;
		swtch();
		++u.u_procp->p_actThreadNum;
		u.u_procp->p_stat = SRUN;
  	PS->integ = s;
}



/*
     * 系统调用ResumeThread的内核实现。
 */
void ResumeThread()
{
	register U_Thread *uthd = &u.u_threads[u.u_curthu];
register int id = uthd->u_ar0[R0];
	register Thread *pthd;
	//判断ID的有效性
	if(id <= 0)
	{
		uthd->u_error = EINVTHDID;
		return;
	}
	pthd = Threads[id%NTHREAD];
	if((pthd->t_stat == 0)
	{
		uthd->u_error = EINVTHDID;
		return;
	}
	
	if((pthd->t_stat != SSLEEP || pthd->t_wchan != &pthd->t_tid)
	{
		uthd->u_error = EINVTHDSTATE;
		return;
	}

	if(pthd->t_proc != u.u_procp)
	{
		if(!suser())
		{
			return;
		}
	}
	
	//恢复该线程
	setrun(pthd); 
}


#define SIGTERM					14
/*
     * 系统调用TerminateThread的内核实现，线程 ID在u_ar0[R1]中。
 */
void TermThread()
{
register struct Thread *targetThd;
	register struct U_Thread *uthd = &u.u_threads[u.u_curthu];
	int id = uthd->u_ar0[R1];
	targetThd = &Threads[id%NTHREAD];
if(targetThd->t_stat == 0)
	{
		uthd->u_error = EINVTHDID;
		return;
	}
	if(targetThd->t_proc != u.u_procp)
		if(!suser())
			return;
	if(targetThd->t_proc != u.u_procp)
{
		targetThd->t_flag |= STERM;
		//向目标线程所在进程发送SIGTERM信号
		psignal(targetThd->t_proc, SIGTERM);
}
else
{
	EndOtherThd(targetThd);
}
	}
	
/*
 * 用来终止进程内其他线程。
 */ 
void EndOtherThd(Thread *thd)	//终止兄弟线程
{
	register struct U_Thread *uthd = &u.u_threads[thd->t_index];
		//关闭所有打开的文件
		for(q = uthd->u_ofile[0]; q < &uthd->u_ofile[NOFILE]; q++)
		{	
			if(a = *q) {
				*q = NULL;
				closef(a);
			}
		}
	iput(uthd->u_cdir);
	//释放栈空间
	FreeStack(thd->t_ustackaddr,thd->t_ustacksize);
	
	//释放线程结构
	thdfree(thd);
	u.u_procp->p_threads[thd->t_index] = NULL;
	--u.u_procp->p_threadNum;
	if(thd->t_stat == SRUN)
	{
			if(--u.u_procp->p_actThreadNum == 0)
				u.u_procp->p_stat = SWAIT;
	}
}


/*
 * 系统调用GetThreadID的内核实现
 */
void GetThreadID()
{
register struct Thread *thd;
char name[12];
	register struct U_Thread *uthd = &u.u_threads[u.u_curthu];
	if(getname(name, 12))
		return;
	if((thd = threadexist(name)) == NULL) //查找线程
	{
		uthd->u_error = ENOTHREAD;
		return;
	}
	uthd->u_ar0[R0] = thd->t_tid;
	return;
}

/*
 * 系统调用GetThreadName的内核实现
 */
void GetThreadName()
{
struct Thread *thd;
	register struct U_Thread *uthd = &u.u_threads[u.u_curthu];
	register char c, *psrc, *pdst = uthd->u_arg[1];

	thd = Threads[uthd->u_arg[0]%NTHREAD];
	if(thd->t_stat == 0)
	{
		uthd->u_error = ENOTHREAD;
		return;
	}
p = thd->t_name;
	do
	{
		if(subyte(pdst++, *p) < 0) {
			uthd->u_error = EFAULT;
			return;
		}
	} while(*p++);
}
