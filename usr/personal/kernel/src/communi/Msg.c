#include "../../include/comm.h"
/* 
	 * 消息缓存初始化
	 */
void msginit()
{
	register int i;
	Message *msg, *msg2 = NULL;
	for(msg = msgBuf; msg < msgBuf+MSG_BUF_COUNT; msg++)
	{
		msg->next = msg2;
		msg2 = msg;
	}
	mbFree.next = msg2;
	mbFree.flag = MSG_BUF_COUNT;
}

/* 
 * 从消息缓存中分配一个消息体。成功返回消息体指针，否则返回NULL。
 */
Message *msgalloc(int time)
{
	register Message *m = NULL;
	if(time == WT_IMME)
	{
		if((mbFree.flag & ~MB_WANT) == 0)
		{
			return NULL;
		}
		m = mbFree.next;
		mbFree.next = m->next;
		mbFree.flag--;
		return m;
	}
	
//若等待时间time是正值，则不须启动定时器，因为它已在调用者中启动
	while((mbFree.flag & ~MB_WANT) == 0)
	{
		mbFree.flag |= MB_WANT;
		sleep(&mbFree.flag, PMSG);
	}
	if(u.u_procp->w_chan == -1) //定时器已超时
	{
		u.u_procp->w_chan = 0;
		return NULL;
	}
	m  = mbFree.next;
	mbFree.next = m->next;
	mbFree.flag--;
	return m;
}

/* 
 * 释放消息体m。 
 */
void msgfree(Message *m)
{
	m->len = 0;
	m->next = mbFree.next;
	mbFree.next = m;
	if(mbFree.flag & MB_WANT)
	{
		mbFree.flag &= ~MB_WANT;
		wakeup(&mbFree.flag);
	}
	mbFree.flag++;
}

/* 
	 * 消息处理超时函数。 
	 */
int msgexpr(proc *p)
{
	return evtexpr(p);
}
	
/* 
	 * 把消息m放入到进程p的消息队列中。 
	 */
void msginqueue(proc *p, Message *m)	
{
	//分配消息
	register Message *mc = m;
	register int i;
	register char c;

	//从用户空间拷贝消息内容到mc中
	u.u_count = min(MAX_MSG_LEN, u.u_arg[1]);
	u.u_base = u.u_arg[0];
	u.u_segflg = 0;
	for(i = 0; (c = cpass()) >= 0; i++)
	{
		mc->data[i] = c;
	}
	mc->len = i;
	
	//把消息mc放到队列p->msgQue中
	m->next = NULL;
	if(p->msgQue.pTail != NULL)
	{
		p->msgQue.pTail->next = mc;
	}
	else
	{
		p->msgQue.pHead = mc;
	}
	p->msgQue.pTail = mc;
	if(p->msgQue.flag & M_RCV_WANT)
	{
		p->msgQue.flag &= ~M_RCV_WANT;
		setrun(p);
	}
	p->msgQue.flag++;
}

/*
 * 用来检测接收进程是否已经退出，p是其进程指针，id是其ID。
 * 通常情况下该函数可以正确检测，但在极端情况下也可能不成功。
 * 那就是进程已退出，而proc结构业已被分配给新进程，且新进程的ID和id相等。
 * 当然，发生这种情况的概率微乎其微。
 */
int procexited(proc *p, int id)
{
	if(p->p_stat == SZOMB || !p->p_stat || id != p->p_pid)
	{
		u.u_error = EEXIT;
		return 1;
	}
	return 0;
}

/* 
	 * 系统调用SendMessage的内核实现。 
*/
void SendMessage()
{
	Message  *m;
	register proc *p=NULL, *p2;
	register int pid = u.u_ar0[R0];
	if(pid == 0 || pid == 1)
	{
		u.u_error = ESYSPID;
		return;
	}
	if(u.u_arg[2] < WT_INFINITE || u.u_arg[1] <= 0)
	{
		u.u_error = EINVAL;
		return;
	}

	//查找接收进程
	for(p2 = proc;  p2 < &proc[NPROC]; p2++)
	{
		if(p2->p_pid == pid)
		{
			if(p2->p_stat != SZOMB && p2->p_stat)
			{
				p = p2;
				break;
			}
			else
			{
				u.u_error = EEXIT;
				break;
			}
		}
		if(p == NULL)
		{
			u.u_error = ESRCH;
			return;
		}
	
		//判断接收进程p的消息队列是否已满，如果已满，则等待或直接返回
		m = NULL;
		if(u.u_arg[2] == WT_IMME)
		{
			if (p->msgQue.flag &~M_BITS) == MSG_QUEUE_LEN)
			{
				u.u_error = EQUEFULL;
				return;
			}
			m = msgalloc(WT_IMME);
			if(m == NULL)
			{
				u.u_error = ENOSPC;
				return;
			}
			//消息缓存分配成功，则放到接收进程p的消息队列中
			msginqueue(p, m);
			u.u_ar0[R0] = 0;
			return;
		}
		
		if(u.u_arg[2] == WT_INFINITE)
		{
			//这里使用二重循环，是因为可能有多个进程同时向接收进程发送消息，
			//为了避免死锁而采用
			do
			{
				while((p->msgQue.flag &~M_BITS) == MSG_QUEUE_LEN)
				{
					if(m != NULL)
						msgfree(m);
					p->msgQue.flag |= M_SEND_WANT;
					sleep(&p->msgQue.flag, PMSG);
					if(procexited(p, pid))		//检测接收进程是否已退出
						return;
					m = msgalloc(WT_INFINITE);
					if(procexited(p, pid))		//检测接收进程是否已退出
					{
						msgfree(m);
						return;
					}
				}
				if(m == NULL)
					m = msgalloc(WT_INFINITE);//即使分配成功，不能保证接收队列不满
				if(procexited(p, pid))	//检测接收进程是否已退出
				{
					msgfree(m);
					return;
				}
			}while(m == NULL || (p->msgQue.flag &~M_BITS) == MSG_QUEUE_LEN);
			//至此接收方消息队列有空闲且消息缓存m分配成功，则拷贝数据到m并入队列
			msginqueue(p, m);
			u.u_ar0[R0] = 0;
			return;
		}
		
		//time/u.u_arg[2]是正值，等待一段时间后返回。	
		timeout(msgexpr, u.u_procp, (u.u_arg[2]+15)/16);
		spl6();
		do
		{
			while((p->msgQue.flag &~M_BITS) == MSG_QUEUE_LEN)
			{
					if(m != NULL)
						msgfree(m);
					p->msgQue.flag |= M_SEND_WANT;
					sleep(&p->msgQue.flag, PMSG);
					if(u.u_procp->w_chan == -1) //定时器已超时
					{
						u.u_procp->w_chan = 0;
						u.u_ar0[R0] = 1;
						spl0();
						return;
					}
					if(procexited(p, pid))	//检测接收进程是否已退出
					{
						rmtimer(msgexpr, u.u_procp);
						return;
					}
					m = msgalloc(2);
					if(m == NULL)		//超时
					{
						u.u_ar0[R0] = 1;
						spl0();
						return;
					}
						
					if(procexited(p, pid))		//检测接收进程是否已退出
					{
						rmtimer(msgexpr, u.u_procp);
						msgfree(m);
						return;
					}
			}
			if(m == NULL)
			{
				m = msgalloc(WT_INFINITE);	//即使分配成功，不能保证接收队列不满
				if(m == NULL)		//超时
				{
					u.u_ar0[R0] = 1;
					spl0();
					return;
				}
				if(procexited(p, pid))		//检测接收进程是否已退出
				{
					rmtimer(msgexpr, u.u_procp);
					msgfree(m);
					return;
				}
			}
	}while((p->msgQue.flag &~M_BITS) == MSG_QUEUE_LEN);

	//至此接收方消息队列有空闲且消息缓存m分配成功，则停止定时器，拷贝数据到m
	//并入队列
	if(rmtimer(msgexpr, u.u_procp) < 0)
	{
		msgfree(m);
		u.u_error = EFAULT;
		return;
	}
	msginqueue(p, m);
	u.u_ar0[R0] = 0;
}

/* 
	 * 从进程p中取出一条消息，并返回其指针。 
*/
	Message *msgoutqueue(proc *p)	
	{
		//分配消息
		register Message *m = p->msgQue.pHead;
		register char *pdata;
		
		//把m的数据拷贝到用户空间（msg）
u.u_count = min(m->len, u.u_arg[1]);
u.u_base = u.u_arg[0];
u.u_segflg = 0;
pdata = m->data[i];
m->len = u.u_count;
while(passc(*pdata++) >= 0);

//把消息m从队列p->msgQue中移除
p->msgQue.pHead = m->next;
if(p->msgQue.flag & M_SEND_WANT)
{
	p->msgQue.flag &= ~M_ SEND_WANT;
	wakeup(&p->msgQue.flag);
}

p->msgQue.flag--;
return m;
}

/* 
 * 系统调用ReceiveMessage的内核实现。 
*/
void ReceiveMessage()
{
	register Message  *m;
	register int *pf = &u.u_procp->msgQue.flag;	
	int tmrstart  = 0;
	if(u.u_arg[2] < WT_INFINITE || u.u_arg[1] <= 0)
	{
		u.u_error = EINVAL;
		return;
	}

	//判断消息队列是否为空，如果为空，则等待或直接返回
	if(u.u_arg[2] == WT_IMME)
	{
		if (*pf &~M_BITS) == 0)
		{
			u.u_error = EQUEEPY;
			return;
		}
		m = msgoutqueue(u.u_procp);
		u.u_ar0[R0] = m->len;
		//释放消息缓存
		msgfree(m);
		return;
	}
	
	if(u.u_arg[2] == WT_INFINITE)
	{
		if((*pf &~M_BITS) == 0)
		{
			*pf |= M_RCV_WANT;
			sleep(pf, PMSG);
		}	
		m = msgoutqueue(u.u_procp);	
		u.u_ar0[R0] = m->len;

		//释放消息缓存
		msgfree(m);
		return;
	}
	
	//u.u_arg[2]是正值，等待一段时间后返回。
	if((*pf & ~M_BITS) == 0)
	{
		*pf |= M_RCV_WANT;
		tmrstart  = 1;
		timeout(msgexpr, u.u_procp, (u.u_arg[2]+15)/16);
		spl6();
		sleep(pf, PMSG);
		if(u.u_procp->w_chan == -1) //定时器已超时
		{
			u.u_procp->w_chan = 0;
			u.u_ar0[R0] = 0;
			spl0();
			return;
		}
	}
	if(tmrstart && rmtimer(msgexpr, u.u_procp) < 0)
	{
		u.u_error = EFAULT;
		return;
	}
	m = msgoutqueue(u.u_procp);	
	u.u_ar0[R0] = m->len;

	//释放消息缓存
	msgfree(m);
	return;
}



