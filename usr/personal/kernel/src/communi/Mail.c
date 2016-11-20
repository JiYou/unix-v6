#include "../../include/comm.h"
/* 
 * ���仺���ʼ��
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

	//��ʼ��mbxFree����
	for(mbx = mbxBuf; mbx < mbxBuf+MSG_BUF_COUNT; mbx ++)
	{
		mbx->next = mbx2;
		mbx2 = mbx;
	}
	mbxFree.next = mbx2;
	mbxFree.flag = MBX_BUF_COUNT;
}


/* 
* �ж���Ϊname�������Ƿ��Ѵ��ڣ�������ڣ��򷵻�����ָ�룬���򷵻�NULL��
* ����ֻ�Ǽ򵥱���������������ܣ�����hash��
*/
MailBox *mbxexist(char *name, int all)
{
	register MailBox *mbx ;
	if(all)	//������������
	{
		for(mbx = mbxBuf; mbx < mbxBuf + MBX_COUNT; mbx++)
		{
			if(mbx->flag && strcmp(mbx->name, name) == 0)
			{
				return mbx;
			}
		}
	}
	else	//���ҹ�������
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
 * �����仺���з���һ�����䡣�ɹ���������ָ�룬���򷵻�NULL��
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
 * ��u_ombx�з���һ�����������ɹ����ؾ��(>=0)�����򷵻�-1��
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
 * ��ȡ��һ��������Ƶ�����name�У����泤����maxlen���ɹ�����0�����򷵻�-1��
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
	name[len] = ��\0��;
	return 0;
}



/* 
 * ϵͳ����CreatePrivMbx���ں�ʵ�֡�
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
	//���Ƽ���ͨ�������濪ʼ����
	if((c=mbxdalloc()) < 0)
		return;
	
	m = mbxalloc();
	m->flag |= MBX_PRIVATE;
	m->ref = 1;
	u.u_ar0[R0] = c;
	u.u_ombx[c] = m;
}


/* 
	 * �رվ����md�����䡣
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
	 * ϵͳ����CloseMbx���ں�ʵ�֡�
         */
void CloseMbx()
{
	register int mbxd = u.u_ar0[R0];
	closembx(mbxd);
}



/* 
 * �����ʼ����档time�ǳ�ʱֵ��
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
	
//���ȴ�ʱ��time����ֵ������������ʱ������Ϊ�����ڵ�����������
	while((mailFree.flag & ~MB_WANT) == 0)
	{
		mailFree.flag |= MB_WANT;
		sleep(&mailFree.flag, PMBX);
	}
	if(u.u_procp->w_chan == -1) //��ʱ���ѳ�ʱ
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
 * �ͷ��ʼ�����m��
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
 * �ʼ�����ʱ������
       */
int mbxexpr(proc *p)
{
	return evtexpr(p);
}

/* 
 *���ʼ�m�ŵ�����mbx�С�
       */ 
void mailinmbx(MailBox *mbx, Mail *m)	
{
	//������Ϣ
	register Mail *mc = m;
	register int i;
	register char c;

	//���û��ռ俽���ʼ����ݵ�mc��
	u.u_count = min(MAX_MAIL_LEN, u.u_arg[2]);
	u.u_base = u.u_arg[0];
	u.u_segflg = 0;
	for(i = 0; (c = cpass()) >= 0; i++)
	{
		mc->data[i] = c;
	}
	mc->len = i;

	//���ʼ��ŵ�������
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
		wakeup(&mbx->ref); 	//���շ������ڴ˱�����
	}
	
	//�����������ʼ���
	mbx->flag = ((mbx->flag&~MBX_BITS)+1)|(mbx->flag&MBX_BITS);
}

/*
* ���������ս����Ƿ��Ѿ��˳���mbx������ָ�롣
* �ܶ�����¸ú���������ȷ��⣬����ĳЩ�����Ҳ���ܲ��ɹ���
* �Ǿ���mbx�ṹ�ڸ������ڵ���SendMail��������mbx�ϵĽ��̷���ǰ���ѱ������
* �µĽ��̣�������CreateXXXMbx����
* ������������ĸ��ʲ����С�����ԣ����Ҫ��ȷ����������SendMail�ڵĽ���
*  ��mbx�ͷź��������أ�����exit�еȴ������̴�SendMail���ء�����ֻ�Ǽ�ʵ��.
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
 * ϵͳ����SendMail���ں�ʵ�֡�
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
	//���������Ƿ����
	if((mbx = mbxexist(name, 1) == NULL)
	{
		u.u_error = ENOMBX;
		return;
	}
	smail(mbx);
}

/* 
 * ��һ�����䴦��ĳ�ʱ������
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
 * ���û�������ʼ����͵�����mbx��
 */
void smail(MailBox *mbx)
{
	register Mail  *m;
	int capability = ((mbx->flag & MBX_PUBLIC)==MBX_PUBLIC? MAIL_BUF_COUNT: 
												MBX_LEN_PROC);
	//�ж������Ƿ������������������ȴ���ֱ�ӷ���
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
		//��Ϣ�������ɹ�����ŵ�������
		mailinmbx(mbx, m);
		u.u_ar0[R0] = 0;
		return;
	}

	if((mbx->flag & MBX_PUBLIC) == MBX_PUBLIC)
		mbx->ref++;
	if(u.u_arg[2] == WT_INFINITE)
	{
		//����ʹ�ö���ѭ��������Ϊ�����ж������ͬʱ������䷢���ʼ���
		//Ϊ�������������á�
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
					if(rcvpexited(mbx))		//�����ս����Ƿ����˳�
						goto error;
					m = mailalloc(WT_INFINITE);
					if(rcvpexited(mbx))		//�����ս����Ƿ����˳�
								goto error;
				}
				if(m == NULL)
					m = mailalloc(WT_INFINITE);//��ʹ����ɹ������ܱ�֤���ն��в���
				if(rcvpexited(mbx))	//�����ս����Ƿ����˳�
					goto error;
		
		}while(m == NULL || (mbx->flag &~MBX_BITS) == capability);

		//���������п������ʼ��������ɹ����򿽱��û����ݵ�m������m������
		mailinmbx(mbx, m);
		u.u_ar0[R0] = 0;
		goto out;
	}
	
	//time/u.u_arg[2]����ֵ���ȴ�һ��ʱ��󷵻ء�	
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
			if(u.u_procp->w_chan == -1) //��ʱ���ѳ�ʱ
			{
				mbxtmout(mbx);
				return;
			}
			if(rcvpexited(mbx))	//�����ս����Ƿ����˳�
					goto error2;
			m = mailalloc(2);
			if(m == NULL)		//��ʱ
			{
				mbxtmout(mbx);
				return;
			}
		
			if(rcvpexited(mbx))		//�����ս����Ƿ����˳�
				goto error2;
		}
		
		if(m == NULL)
		{
			m = mailalloc(WT_INFINITE);	//��ʹ����ɹ������ܱ�֤���ն��в���
			if(m == NULL)		//��ʱ
			{
				mbxtmout(mbx);
				return;
			}
			if(rcvpexited(mbx))		//�����ս����Ƿ����˳�
				goto error2;
		}
	}while((mbx->flag &~MBX_BITS) == capability);

	//���������п������ʼ��������ɹ�����ֹͣ��ʱ���������û����ݵ�m��
	//������m�����䡣
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
 * ������mbx�ж�ȡһ���ʼ���
*/
Mail *mailoutmbx(MailBox *mbx)	
{
	//������Ϣ
	register Message *m = mbx->pHead;
	register char *pdata;
	
	//��m�����ݿ������û��ռ䣨msg��
	u.u_count = min(m->len, u.u_arg[1]);
	u.u_base = u.u_arg[0];
	u.u_segflg = 0;
	pdata = m->data[i];
	m->len = u.u_count;
	while(passc(*pdata++) >= 0);

	//���ʼ�m���������Ƴ�
	mbx->pHead = m->next;
	if(mbx->flag & MBX_SEND_WANT)
	{
		mbx->flag &= ~MBX_SEND_WANT;
		wakeup(&mbx->flag);
	}

	//�����������ʼ���
	mbx->flag = ((mbx->flag&~MBX_BITS)-1)|(mbx->flag&MBX_BITS);
	return m;
}

/* 
 *ϵͳ����ReceiveMail���ں�ʵ�֡�
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
	//�ж������Ƿ�Ϊ�գ����Ϊ�գ���ȴ���ֱ�ӷ���
	if(u.u_arg[2] == WT_IMME)
	{
		if (*pf &~MBX_BITS) == 0)
		{
			u.u_error = EQUEEPY;
			return;
		}
		m = msgoutqueue(mbx);
		u.u_ar0[R0] = m->len;
		//�ͷ��ʼ�����
		mailfree(m);
		return;
	}
	
	if(u.u_arg[2] == WT_INFINITE)
	{
		if((*pf &~MBX_BITS) == 0)		//��Ϊ��������Ҳֻ�ܵ������գ�������if
		{
			*pf |= MBX_RCV_WANT;
			sleep(&mbx->ref, PMBX);
		}	
		m = mailoutmbx(mbx);	
		u.u_ar0[R0] = m->len;

		//�ͷ���Ϣ����
		mailfree(m);
		return;
	}
	
	//u.u_arg[2]����ֵ���ȴ�һ��ʱ��󷵻ء�
	if((*pf & ~MBX_BITS) == 0)
	{
		*pf |= MBX_RCV_WANT;
		tmrstart  = 1;
		timeout(mbxexpr, u.u_procp, (u.u_arg[2]+15)/16);
		spl6();
		sleep(&mbx->ref, PMBX);
		if(u.u_procp->w_chan == -1) //��ʱ���ѳ�ʱ
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

	//�ͷ���Ϣ����
	mailfree(m);
	return;
}


/* 
 * ϵͳ����CreateSharedMbx���ں�ʵ�֡�
*/
void CreateSharedMbx()
{
	register int c;
	register MailBox *m;

	//��ʼ������
	if((c=mbxdalloc()) < 0)
		return;
	
	m = mbxalloc();
	m->flag |= MBX_SHARED;
	m->ref = 1;
	u.u_ar0[R0] = c;
	u.u_ombx[c] = m;
}


/* 
 * ϵͳ����SendSharedMail���ں�ʵ�֡�
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
 * ϵͳ����CreatePublicMbx���ں�ʵ�֡�
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
		//���Ƽ���ͨ�������濪ʼ����
	m = mbxalloc();
	m->flag |= MBX_PUBLIC;
	m->ref = 0;
	m->next = g_pPubBox;
	g_pPubBox = m;
}


/* 
 * ϵͳ����ClosePublicMbx���ں�ʵ�֡�
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
	//���Ƽ���ͨ���������ͷ�
	if(m->ref)
	{
		u.u_error = EINUSE;
		return;
	}
	g_pPubBox = m->next;
	mbxfree(m);
}
