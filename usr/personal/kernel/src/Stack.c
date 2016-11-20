#include "../include/Thread_U.h"
/*
 * �����СΪiSize*64��ջ�ռ�
 */
char *AllocStack(int iSize)
{
register map *bp, *bp2;
char *ret;
for(bp = &u.u_stackmap[0]; bp < &u.u_stackmap[NTHREAD_PROC];bp++)
{
	if(bp->m_size >= iSize)
	{
		ret = bp->m_addr;
		bp->m_size -= iSize;
		if(bp->m_size == 0)
		{
			for(bp2 = bp; bp2 < &u.u_stackmap[NTHREAD_PROC-1]; bp2++)
			{
				bp2->m_addr = (bp2+1)->m_addr;
				bp2->m_size = (bp2+1)->m_size;
			}	
			bp2->m_size = 0;
	    }
		else
			bp->m_addr+= iSize;
		return ret;
	}
}
return NULL;
}

/*
 * �ͷ���ʼ��ַΪpAddr����СΪiSize*64��ջ�ռ�
 */
int FreeStack(char *pAddr, int iSize)
{
	register map *bp, *bp2;
	char *ret;
	for(bp = &u.u_stackmap[0]; bp < &u.u_stackmap[NTHREAD_PROC];bp++)
	{
		if(pAddr < bp->m_addr || bp->m_size == 0)
		{
			break;
		}
	}
	
	if(bp == &u.u_stackmap[NTHREAD_PROC] || pAddr+iSize > bp->m_addr)
	{
		panic("Unexpected free memory error");
	}
	
	if(bp->m_size == 0)	//���
	{
		if(bp > &u.u_stackmap[0] && (bp-1)->m_addr + (bp-1)->m_size == pAddr)
		{
			(bp-1)->m_size += iSize;
		}
		Else
		{
			bp->m_addr = pAddr;
			bp->m_size = iSize;
		}
		return 0;
	}
	
	if(pAddr+iSize == bp->m_addr)	//�ͺ���ĳ�Ա�ϲ�
	{
		bp->m_addr = pAddr;
		bp->m_size += iSize;
		//��ǰ��ĳ�Ա�ϲ�
		if(bp > &u.u_stackmap[0] && (bp-1)->m_addr + (bp-1)->m_size == pAddr)
		{
			(bp-1)->m_size += bp->m_size;
			for(bp2 = bp; bp2 < &u.u_stackmap[NTHREAD_PROC-1]; bp2++)
			{
				bp2->m_addr = (bp2+1)->m_addr;
				bp2->m_size = (bp2+1)->m_size;
			}
			bp2->m_size = 0;
		}
		return 0;
	}
	
	//��ǰ��ĳ�Ա�ϲ�
	if(bp > &u.u_stackmap[0] && (bp-1)->m_addr + (bp-1)->m_size == pAddr)
	{
		(bp-1)->m_size += bp->m_size;
		return 0;
	}
	
	//�����κγ�Ա�ϲ�������
	for(bp2 = &u.u_stackmap[NTHREAD_PROC-1]; bp2 > bp; bp2--)
	{
		bp2->m_addr = (bp2-1)->m_addr;
		bp2->m_size = (bp2-1)->m_size;
	}	
	bp->m_addr = pAddr;
	bp->m_size = iSize;
	return 0;
}


/*
* ���������������߳��״�ִ��ʱ���ں�ջ���ݣ���swtchҪ���شӶ�ִ�и��߳�ʱ��
* ���Ȼ���ת��cret�С���cret���t_ustackaddrд��r2�У���p_thdsysentry����
* ThreadSysEntry��д��r3�У���t_entryд��r4�У���t_paramд��r5�У�
* �������ת��ReturnToThread�С�
*/
void SetupKernStack(U_Thread *u_thd)
{
	*((int *)&u_thd->u_kernstack[800]) = _ReturnToThread;
	*((int *)&u_thd->u_kernstack[798]) = u_thd->u_threadp->t_param;
	*((int *)&u_thd->u_kernstack[796]) = u_thd->u_threadp->t_entry;
	*((int *)&u_thd->u_kernstack[794]) = u_thd->u_threadp->t_proc->p_thdsysentry;
	*((int *)&u_thd->u_kernstack[792]) = u_thd->u_threadp->t_ustackaddr;
	*((int *)&u_thd->u_kernstack[790]) = _cret;
}