#include "../../include/comm.h"
/* 
	 * ��Ϣ�����ʼ��
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
 * ����Ϣ�����з���һ����Ϣ�塣�ɹ�������Ϣ��ָ�룬���򷵻�NULL��
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
	
//���ȴ�ʱ��time����ֵ������������ʱ������Ϊ�����ڵ�����������
	while((mbFree.flag & ~MB_WANT) == 0)
	{
		mbFree.flag |= MB_WANT;
		sleep(&mbFree.flag, PMSG);
	}
	if(u.u_procp->w_chan == -1) //��ʱ���ѳ�ʱ
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
 * �ͷ���Ϣ��m�� 
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
	 * ��Ϣ����ʱ������ 
	 */
int msgexpr(proc *p)
{
	return evtexpr(p);
}
	
/* 
	 * ����Ϣm���뵽����p����Ϣ�����С� 
	 */
void msginqueue(proc *p, Message *m)	
{
	//������Ϣ
	register Message *mc = m;
	register int i;
	register char c;

	//���û��ռ俽����Ϣ���ݵ�mc��
	u.u_count = min(MAX_MSG_LEN, u.u_arg[1]);
	u.u_base = u.u_arg[0];
	u.u_segflg = 0;
	for(i = 0; (c = cpass()) >= 0; i++)
	{
		mc->data[i] = c;
	}
	mc->len = i;
	
	//����Ϣmc�ŵ�����p->msgQue��
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
 * ���������ս����Ƿ��Ѿ��˳���p�������ָ�룬id����ID��
 * ͨ������¸ú���������ȷ��⣬���ڼ��������Ҳ���ܲ��ɹ���
 * �Ǿ��ǽ������˳�����proc�ṹҵ�ѱ�������½��̣����½��̵�ID��id��ȡ�
 * ��Ȼ��������������ĸ���΢����΢��
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
	 * ϵͳ����SendMessage���ں�ʵ�֡� 
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

	//���ҽ��ս���
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
	
		//�жϽ��ս���p����Ϣ�����Ƿ������������������ȴ���ֱ�ӷ���
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
			//��Ϣ�������ɹ�����ŵ����ս���p����Ϣ������
			msginqueue(p, m);
			u.u_ar0[R0] = 0;
			return;
		}
		
		if(u.u_arg[2] == WT_INFINITE)
		{
			//����ʹ�ö���ѭ��������Ϊ�����ж������ͬʱ����ս��̷�����Ϣ��
			//Ϊ�˱�������������
			do
			{
				while((p->msgQue.flag &~M_BITS) == MSG_QUEUE_LEN)
				{
					if(m != NULL)
						msgfree(m);
					p->msgQue.flag |= M_SEND_WANT;
					sleep(&p->msgQue.flag, PMSG);
					if(procexited(p, pid))		//�����ս����Ƿ����˳�
						return;
					m = msgalloc(WT_INFINITE);
					if(procexited(p, pid))		//�����ս����Ƿ����˳�
					{
						msgfree(m);
						return;
					}
				}
				if(m == NULL)
					m = msgalloc(WT_INFINITE);//��ʹ����ɹ������ܱ�֤���ն��в���
				if(procexited(p, pid))	//�����ս����Ƿ����˳�
				{
					msgfree(m);
					return;
				}
			}while(m == NULL || (p->msgQue.flag &~M_BITS) == MSG_QUEUE_LEN);
			//���˽��շ���Ϣ�����п�������Ϣ����m����ɹ����򿽱����ݵ�m�������
			msginqueue(p, m);
			u.u_ar0[R0] = 0;
			return;
		}
		
		//time/u.u_arg[2]����ֵ���ȴ�һ��ʱ��󷵻ء�	
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
					if(u.u_procp->w_chan == -1) //��ʱ���ѳ�ʱ
					{
						u.u_procp->w_chan = 0;
						u.u_ar0[R0] = 1;
						spl0();
						return;
					}
					if(procexited(p, pid))	//�����ս����Ƿ����˳�
					{
						rmtimer(msgexpr, u.u_procp);
						return;
					}
					m = msgalloc(2);
					if(m == NULL)		//��ʱ
					{
						u.u_ar0[R0] = 1;
						spl0();
						return;
					}
						
					if(procexited(p, pid))		//�����ս����Ƿ����˳�
					{
						rmtimer(msgexpr, u.u_procp);
						msgfree(m);
						return;
					}
			}
			if(m == NULL)
			{
				m = msgalloc(WT_INFINITE);	//��ʹ����ɹ������ܱ�֤���ն��в���
				if(m == NULL)		//��ʱ
				{
					u.u_ar0[R0] = 1;
					spl0();
					return;
				}
				if(procexited(p, pid))		//�����ս����Ƿ����˳�
				{
					rmtimer(msgexpr, u.u_procp);
					msgfree(m);
					return;
				}
			}
	}while((p->msgQue.flag &~M_BITS) == MSG_QUEUE_LEN);

	//���˽��շ���Ϣ�����п�������Ϣ����m����ɹ�����ֹͣ��ʱ�����������ݵ�m
	//�������
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
	 * �ӽ���p��ȡ��һ����Ϣ����������ָ�롣 
*/
	Message *msgoutqueue(proc *p)	
	{
		//������Ϣ
		register Message *m = p->msgQue.pHead;
		register char *pdata;
		
		//��m�����ݿ������û��ռ䣨msg��
u.u_count = min(m->len, u.u_arg[1]);
u.u_base = u.u_arg[0];
u.u_segflg = 0;
pdata = m->data[i];
m->len = u.u_count;
while(passc(*pdata++) >= 0);

//����Ϣm�Ӷ���p->msgQue���Ƴ�
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
 * ϵͳ����ReceiveMessage���ں�ʵ�֡� 
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

	//�ж���Ϣ�����Ƿ�Ϊ�գ����Ϊ�գ���ȴ���ֱ�ӷ���
	if(u.u_arg[2] == WT_IMME)
	{
		if (*pf &~M_BITS) == 0)
		{
			u.u_error = EQUEEPY;
			return;
		}
		m = msgoutqueue(u.u_procp);
		u.u_ar0[R0] = m->len;
		//�ͷ���Ϣ����
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

		//�ͷ���Ϣ����
		msgfree(m);
		return;
	}
	
	//u.u_arg[2]����ֵ���ȴ�һ��ʱ��󷵻ء�
	if((*pf & ~M_BITS) == 0)
	{
		*pf |= M_RCV_WANT;
		tmrstart  = 1;
		timeout(msgexpr, u.u_procp, (u.u_arg[2]+15)/16);
		spl6();
		sleep(pf, PMSG);
		if(u.u_procp->w_chan == -1) //��ʱ���ѳ�ʱ
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

	//�ͷ���Ϣ����
	msgfree(m);
	return;
}



