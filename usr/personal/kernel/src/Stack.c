#include "../include/Thread_U.h"
/*
 * 分配大小为iSize*64的栈空间
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
 * 释放起始地址为pAddr、大小为iSize*64的栈空间
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
	
	if(bp->m_size == 0)	//最后
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
	
	if(pAddr+iSize == bp->m_addr)	//和后面的成员合并
	{
		bp->m_addr = pAddr;
		bp->m_size += iSize;
		//和前面的成员合并
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
	
	//和前面的成员合并
	if(bp > &u.u_stackmap[0] && (bp-1)->m_addr + (bp-1)->m_size == pAddr)
	{
		(bp-1)->m_size += bp->m_size;
		return 0;
	}
	
	//不和任何成员合并，插入
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
* 本函数用来设置线程首次执行时的内核栈数据，当swtch要返回从而执行该线程时，
* 首先会跳转到cret中。而cret会把t_ustackaddr写到r2中，把p_thdsysentry（即
* ThreadSysEntry）写到r3中，把t_entry写到r4中，把t_param写到r5中；
* 最后再跳转到ReturnToThread中。
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