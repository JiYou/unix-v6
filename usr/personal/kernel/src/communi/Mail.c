#include "../../include/comm.h"
/* 
 * 信箱缓存初始化
 */
void mbxinit()
{
	register int i;
	Mail *m, *m2 = NULL;
	MailBox *mbx, mbx2 = NULL;
	for(m = mailBuf; m < mailBuf+MSG_BUF_COUNT; m++)
	{
		m->next = m2;
		m2 = m;
	}
	mailFree.next = m2;
	mailFree.flag = MAIL_BUF_COUNT;

	//初始化mbxFree链表
	for(mbx = mbxBuf; mbx < mbxBuf+MSG_BUF_COUNT; mbx ++)
	{
		mbx->next = mbx2;
		mbx2 = mbx;
	}
	mbxFree.next = mbx2;
	mbxFree.flag = MBX_BUF_COUNT;
}


/* 
* 判断名为name的信箱是否已存在，如果存在，则返回信箱指针，否则返回NULL。
* 函数只是简单遍历，如想提高性能，可用hash表。
*/
MailBox *mbxexist(char *name, int all)
{
	register MailBox *mbx ;
	if(all)	//查找所有信箱
	{
		for(mbx = mbxBuf; mbx < mbxBuf + MBX_COUNT; mbx++)
		{
			if(mbx->flag && strcmp(mbx->name, name) == 0)
			{
				return mbx;
			}
		}
	}
	else	//查找公共信箱
	{
		mbx = g_pPubBox;
		while(mbx != NULL)
		{
			if(mbx->flag && strcmp(mbx->name, name) == 0)
			{
				return mbx;
			}
			mbx = mbx->next;
		}
	}
	return NULL;
}

/* 
 * 从信箱缓存中分配一个信箱。成功返回信箱指针，否则返回NULL。
*/
MailBox *mbxalloc()
{
	register MailBox *m = NULL;
	while((mbxFree.flag & ~MB_WANT) == 0)
	{
		mbxFree.flag |= MB_WANT;
		sleep(&mbxFree.flag, PMBX);
	}
	m = mbxFree.next;
	mbxFree.next = m->next;
	mbxFree.flag--;
	return m;
}

void mbxfree(MailBox *m)
{
	m->next = mbxFree.next;
	m->flag = 0;
	mbxFree.next = m;
	if(mbxFree.flag & MB_WANT)
	{
		mbxFree.flag &= ~MB_WANT;
		wakeup(&mbxFree.flag);
	}
	mbxFree.flag++;
}


/* 
 * 从u_ombx中分配一个信箱句柄。成功返回句柄(>=0)，否则返回-1。
 */
int mbxdalloc()
{
	register int i;
	for(i = 0; i < MAX_EVENTS_PROC; i++)
	{
		if(u_ombx[i] == NULL)
			return i;
	}
	u.u_error = EMFILE;
	return -1;
}

void mbxdfree(int md)
{
	register MailBox *m = u.u_ombx[md];
	m->ref--;
	u.u_ombx[md] = NULL;
}

/* 
 * 读取第一个入参名称到缓存name中，缓存长度是maxlen。成功返回0，否则返回-1。
       */
int getname(char *name, int maxlen)
{
	while((c=uchar()) > 0)
	{
		name[len++] = c;
		if(len >= maxlen-1)
			break;
	}
	if(c < 0)
		return -1;
	name[len] = ‘\0’;
	return 0;
}



/* 
 * 系统调用CreatePrivMbx的内核实现。
       */
void CreatePrivMbx()
{
	register int c, len=0;
	char name[14];
	register MailBox *m;
	if(getname(name, 14))
		return;
	if(mbxexist(name, 1))
	{
		u.u_error = EEXIST;
		return;
	}		
	//名称检验通过，下面开始分配
	if((c=mbxdalloc()) < 0)
		return;
	
	m = mbxalloc();
	m->flag |= MBX_PRIVATE;
	m->ref = 1;
	u.u_ar0[R0] = c;
	u.u_ombx[c] = m;
}


/* 
	 * 关闭句柄是md的信箱。
 */
void closembx(int md)
{
	register MailBox *m;
	if(md <0 || md >= MBX_COUNT_PROC || (m=u.u_ombx[md]) == NULL)
	{
		u.u_error = EINVAL;
		return;
	}
	mbxdfree(md);
	if(m->ref == 0)
	{
		mbxfree(m);
	}
}

/* 
	 * 系统调用CloseMbx的内核实现。
         */
void CloseMbx()
{
	register int mbxd = u.u_ar0[R0];
	closembx(mbxd);
}



/* 
 * 分配邮件缓存。time是超时值。
 */
Mail *mailalloc(int time)
{
	register Mail *m = NULL;
	if(time == WT_IMME)
	{
		if((mailFree.flag & ~MB_WANT) == 0)
		{
			return NULL;
		}
		m = mailFree.next;
		mailFree.next = m->next;
		mailFree.flag--;
		return m;
	}
	
//若等待时间time是正值，则不须启动定时器，因为它已在调用者中启动
	while((mailFree.flag & ~MB_WANT) == 0)
	{
		mailFree.flag |= MB_WANT;
		sleep(&mailFree.flag, PMBX);
	}
	if(u.u_procp->w_chan == -1) //定时器已超时
	{
		u.u_procp->w_chan = 0;
		return NULL;
	}
	m  = mailFree.next;
	mailFree.next = m->next;
	mailFree.flag--;
	return m;
}

/* 
 * 释放邮件缓存m。
 */
void mailfree(Mail *m)
{
	m->len = 0;
	m->next = mailFree.next;
	mailFree.next = m;
	if(mailFree.flag & MB_WANT)
	{
		mailFree.flag &= ~MB_WANT;
		wakeup(&mailFree.flag);
	}
	mailFree.flag++;
}

/* 
 * 邮件处理超时函数。
       */
int mbxexpr(proc *p)
{
	return evtexpr(p);
}

/* 
 *把邮件m放到信箱mbx中。
       */ 
void mailinmbx(MailBox *mbx, Mail *m)	
{
	//分配消息
	register Mail *mc = m;
	register int i;
	register char c;

	//从用户空间拷贝邮件内容到mc中
	u.u_count = min(MAX_MAIL_LEN, u.u_arg[2]);
	u.u_base = u.u_arg[0];
	u.u_segflg = 0;
	for(i = 0; (c = cpass()) >= 0; i++)
	{
		mc->data[i] = c;
	}
	mc->len = i;

	//把邮件放到信箱中
	mc->next = NULL;
	if(mbx->pTail != NULL)
	{
		mbx->pTail->next = mc;
	}
	else
	{
		mbx->pHead = mc;
	}
	mbx->pTail = mc;
	if(mbx->flag & MBX_RCV_WANT)
	{
		mbx->flag &= ~MBX_RCV_WANT;
		wakeup(&mbx->ref); 	//接收方挂起在此变量上
	}
	
	//增加信箱中邮件数
	mbx->flag = ((mbx->flag&~MBX_BITS)+1)|(mbx->flag&MBX_BITS);
}

/*
* 用来检测接收进程是否已经退出，mbx是信箱指针。
* 很多情况下该函数可以正确检测，但在某些情况下也可能不成功。
* 那就是mbx结构在各个由于调用SendMail而挂起在mbx上的进程返回前，已被分配给
* 新的进程（它调用CreateXXXMbx）。
* 发生这种情况的概率不算很小，所以，如果要想确保各挂起在SendMail内的进程
*  在mbx释放后立即返回，可在exit中等待各进程从SendMail返回。这里只是简化实现.
*/
int rcvpexited(MailBox *mbx)
{
	if(m->flag == 0)
	{
		u.u_error = EEXIT;
	}
	return 0;
}

/* 
 * 系统调用SendMail的内核实现。
*/
void SendMail()
{
	register MailBox *mbx;
	char name[14];

	if(u.u_arg[2] <= 0 || u.u_arg[3] < WT_INFINITE)
	{
		u.u_error = EINVAL;
		return;
	}
	if(getname(name, 14))
		return;
	//查找信箱是否存在
	if((mbx = mbxexist(name, 1) == NULL)
	{
		u.u_error = ENOMBX;
		return;
	}
	smail(mbx);
}

/* 
 * 另一个信箱处理的超时函数。
*/
void mbxtmout(MailBox *mbx)
{
	u.u_procp->w_chan = 0;
	u.u_ar0[R0] = 1;
	spl0();
	if((mbx->flag & MBX_PUBLIC) == MBX_PUBLIC)
		mbx->ref--;
}

/* 
 * 把用户传入的邮件发送到信箱mbx。
 */
void smail(MailBox *mbx)
{
	register Mail  *m;
	int capability = ((mbx->flag & MBX_PUBLIC)==MBX_PUBLIC? MAIL_BUF_COUNT: 
												MBX_LEN_PROC);
	//判断信箱是否已满，如果已满，则等待或直接返回
	m = NULL;
	if(u.u_arg[3] == WT_IMME)
	{
		if ((mbx->flag &~MBX_BITS) == capability)
		{
			u.u_error = EQUEFULL;
			return;
		}
		m = mailalloc(WT_IMME);
		if(m == NULL)
		{
			u.u_error = ENOSPC;
			return;
		}
		//消息缓存分配成功，则放到信箱中
		mailinmbx(mbx, m);
		u.u_ar0[R0] = 0;
		return;
	}

	if((mbx->flag & MBX_PUBLIC) == MBX_PUBLIC)
		mbx->ref++;
	if(u.u_arg[2] == WT_INFINITE)
	{
		//这里使用二重循环，是因为可能有多个进程同时向该信箱发送邮件，
		//为避免死锁而采用。
		do
		{
				while((mbx->flag &~MBX_BITS) == capability)
				{
					if(m != NULL)
					{
						mailfree(m);
						m = NULL;
					}
					mbx->flag |= MBX_SEND_WANT;
					sleep(&mbx->flag, PMBX);
					if(rcvpexited(mbx))		//检测接收进程是否已退出
						goto error;
					m = mailalloc(WT_INFINITE);
					if(rcvpexited(mbx))		//检测接收进程是否已退出
								goto error;
				}
				if(m == NULL)
					m = mailalloc(WT_INFINITE);//即使分配成功，不能保证接收队列不满
				if(rcvpexited(mbx))	//检测接收进程是否已退出
					goto error;
		
		}while(m == NULL || (mbx->flag &~MBX_BITS) == capability);

		//至此信箱有空闲且邮件缓存分配成功，则拷贝用户数据到m，并入m到信箱
		mailinmbx(mbx, m);
		u.u_ar0[R0] = 0;
		goto out;
	}
	
	//time/u.u_arg[2]是正值，等待一段时间后返回。	
	timeout(mbxexpr, u.u_procp, (u.u_arg[3]+15)/16);
	spl6();
	do
	{
		while((mbx->flag &~MBX_BITS) == MBX_LEN)
		{
			if(m != NULL)
			{
				msgfree(m);
				m = NULL;
			}
			mbx->flag |= MBX_SEND_WANT;
			sleep(&mbx->flag, PMBX);
			if(u.u_procp->w_chan == -1) //定时器已超时
			{
				mbxtmout(mbx);
				return;
			}
			if(rcvpexited(mbx))	//检测接收进程是否已退出
					goto error2;
			m = mailalloc(2);
			if(m == NULL)		//超时
			{
				mbxtmout(mbx);
				return;
			}
		
			if(rcvpexited(mbx))		//检测接收进程是否已退出
				goto error2;
		}
		
		if(m == NULL)
		{
			m = mailalloc(WT_INFINITE);	//即使分配成功，不能保证接收队列不满
			if(m == NULL)		//超时
			{
				mbxtmout(mbx);
				return;
			}
			if(rcvpexited(mbx))		//检测接收进程是否已退出
				goto error2;
		}
	}while((mbx->flag &~MBX_BITS) == capability);

	//至此信箱有空闲且邮件缓存分配成功，则停止定时器，拷贝用户数据到m，
	//并放入m到信箱。
	if(rmtimer(mbxexpr, u.u_procp) < 0)
	{
		mailfree(m);
		u.u_error = EFAULT;
		goto out;
	}
	mbxinqueue(mbx, m);
	u.u_ar0[R0] = 0;
	goto out;
error2:
	rmtimer(mbxexpr, u.u_procp);
error:
	if(m != NULL)
		mailfree(m);
	spl0();
out:
	if((mbx->flag & MBX_PUBLIC) == MBX_PUBLIC)
		mbx->ref--;
}


/* 
 * 从信箱mbx中读取一封邮件。
*/
Mail *mailoutmbx(MailBox *mbx)	
{
	//分配消息
	register Message *m = mbx->pHead;
	register char *pdata;
	
	//把m的数据拷贝到用户空间（msg）
	u.u_count = min(m->len, u.u_arg[1]);
	u.u_base = u.u_arg[0];
	u.u_segflg = 0;
	pdata = m->data[i];
	m->len = u.u_count;
	while(passc(*pdata++) >= 0);

	//把邮件m从信箱中移除
	mbx->pHead = m->next;
	if(mbx->flag & MBX_SEND_WANT)
	{
		mbx->flag &= ~MBX_SEND_WANT;
		wakeup(&mbx->flag);
	}

	//减少信箱中邮件数
	mbx->flag = ((mbx->flag&~MBX_BITS)-1)|(mbx->flag&MBX_BITS);
	return m;
}

/* 
 *系统调用ReceiveMail的内核实现。
*/
void ReceiveMail()
{
	register Mail  *m;
	register MailBox *mbx;
	register int *pf, mbxd = u.u_ar0[R0];
	int tmrstart  = 0;
	if(mbxd < 0 || mbxd >= MBX_COUNT_PROC  || u.u_arg[2] < WT_INFINITE 
				|| u.u_arg[1] <= 0 || (mbx = u.u_ombx[mbxd]) == NULL)
	{
		u.u_error = EINVAL;
		return;
	}
	
	pf = &mbx->flag;	
	//判断信箱是否为空，如果为空，则等待或直接返回
	if(u.u_arg[2] == WT_IMME)
	{
		if (*pf &~MBX_BITS) == 0)
		{
			u.u_error = EQUEEPY;
			return;
		}
		m = msgoutqueue(mbx);
		u.u_ar0[R0] = m->len;
		//释放邮件缓存
		mailfree(m);
		return;
	}
	
	if(u.u_arg[2] == WT_INFINITE)
	{
		if((*pf &~MBX_BITS) == 0)		//因为共享信箱也只能单发单收，所以用if
		{
			*pf |= MBX_RCV_WANT;
			sleep(&mbx->ref, PMBX);
		}	
		m = mailoutmbx(mbx);	
		u.u_ar0[R0] = m->len;

		//释放消息缓存
		mailfree(m);
		return;
	}
	
	//u.u_arg[2]是正值，等待一段时间后返回。
	if((*pf & ~MBX_BITS) == 0)
	{
		*pf |= MBX_RCV_WANT;
		tmrstart  = 1;
		timeout(mbxexpr, u.u_procp, (u.u_arg[2]+15)/16);
		spl6();
		sleep(&mbx->ref, PMBX);
		if(u.u_procp->w_chan == -1) //定时器已超时
		{
			u.u_procp->w_chan = 0;
			u.u_ar0[R0] = 1;
			spl0();
			return;
		}
	}
	if(tmrstart && rmtimer(mbxexpr, u.u_procp) < 0)
	{
		u.u_error = EFAULT;
		return;
	}
	m = mailoutmbx(mbx);	
	u.u_ar0[R0] = m->len;

	//释放消息缓存
	mailfree(m);
	return;
}


/* 
 * 系统调用CreateSharedMbx的内核实现。
*/
void CreateSharedMbx()
{
	register int c;
	register MailBox *m;

	//开始分配句柄
	if((c=mbxdalloc()) < 0)
		return;
	
	m = mbxalloc();
	m->flag |= MBX_SHARED;
	m->ref = 1;
	u.u_ar0[R0] = c;
	u.u_ombx[c] = m;
}


/* 
 * 系统调用SendSharedMail的内核实现。
*/
void SendSharedMail()
{
	register MailBox *mbx;
	int mbxd = u.u_arg[0];
	if(mbxd < 0 || mbxd >= MBX_COUNT_PROC ||
			||(mbx=u.u_ombx[mbxd])==NULL|| u.u_arg[2] <= 0 || u.u_arg[3] < WT_INFINITE)
	{
		u.u_error = EINVAL;
		return;
	}
	smail(mbx);
}


MailBox *g_pPubBox=NULL;
/* 
 * 系统调用CreatePublicMbx的内核实现。
*/
void CreatePublicMbx()
{
	register int c, len=0;
	char name[14];
	register MailBox *m;
	if(getname(name, 14))
		return;
	if(mbxexist(name, 1))
	{
		u.u_error = EEXIST;
		return;
	}		
		//名称检验通过，下面开始分配
	m = mbxalloc();
	m->flag |= MBX_PUBLIC;
	m->ref = 0;
	m->next = g_pPubBox;
	g_pPubBox = m;
}


/* 
 * 系统调用ClosePublicMbx的内核实现。
*/
void ClosePublicMbx()
{
	register int c, len=0;
	char name[14];
	register MailBox *m;
	if(getname(name, 14))
		return;
	if((m = mbxexist(name, 0)) == NULL)
	{
			u.u_error = ENOMBX;
			return;
	}		
	//名称检验通过，下面释放
	if(m->ref)
	{
		u.u_error = EINUSE;
		return;
	}
	g_pPubBox = m->next;
	mbxfree(m);
}
