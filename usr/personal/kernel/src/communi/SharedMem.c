/* 
 *��������p������u.u_procp->p_shm��u.u_procp->p_mmap��
*/
int shdalloc(int *p)
{
	register int i;
	for(i = 0; i< MEM_MAP_COUNT_PROC; i++)
	{
		if(p[i] == NULL)
			return i;
	}
	u.u_error = EMFILE;
	return -1;
}	

void shdfree(int *p, int shd)
{
	p[shd] = NULL;
}

/* 
 * �ж���Ϊname�Ĺ����ڴ��Ƿ��Ѵ��ڣ�������ڣ��򷵻�SharedMemoryָ�룬
* ���򷵻�NULL������ֻ�Ǽ򵥱���������������ܣ�����hash��
*/
SharedMemory *shmexist(char *name)
{
	register SharedMemory *shm ;
	for(shm = shMem; shm < shMem + SHARED_MEM_COUNT; shm++)
	{
		if(shm->flag && strcmp(mbx->name, name) == 0)
		{
			return shm;
		}
	}
	return NULL;
}

/* 
 * ����SharedMemory�ṹ��
*/
SharedMemory *shmalloc()
{
	register SharedMemory *shm ;
	for(shm = shMem; shm < shMem + SHARED_MEM_COUNT; shm++)
	{
		if(!shm->flag)
		{
			shm->ref = shm->refmem = 0;
			shm->flag |= (SM_ALLOC|SM_LOAD);
			return shm;
		}
	}
	return NULL;
}
		
void shfree(SharedMemory * shm)
{
	shm->flag = 0;
}

/* 
* ϵͳ���� CreateSharedMemory ���ں�ʵ�֡�
*/
void CreateSharedMemory()
{
	char name[14];
	register SharedMemory *sm;
	register int i;
	if(getname(name, 14))
		return;
	if(shmexist(name) != NULL)
	{
		u.u_error = EEXIST;
		return;
	}
	if(i = shdalloc(u.u_procp->p_shm) == 0)
	{
		return;
	}
	if((sm=shmalloc()) == NULL)
	{
		u.u_error = ENOSPC;
		return;
	}
	if((sm->phyAddr = phymalloc(u.u_arg[1])) == NULL)
	{
		shfree(sm);
		u.u_error = ENOSPC;
		return;
	}
	sm->blocks = u.u_arg[1];
	sm->pOwner = u.u_procp;
	strcpy(sm->name, name);
	sm->permOwn = u.u_arg[2];
	sm->permOthers = u.u_arg[3];
	u.u_procp->p_shm[i] = sm;
	u.u_ar0[R0] = i;
	return;
}


/* 
 * ϵͳ���� FreeSharedMemory���ں�ʵ�֡�
*/
void FreeSharedMemory()
{
	char name[14];
	register SharedMemory *sm;
	register int i = u.u_ar0[R0];
	if(i < 0 || i >= MAP_MEM_COUNT_PROC || (sm = u.u_procp->p_shm[i]) == NULL)
	{
		u.u_error = EINVAL;
		return;
	}
		if(sm->ref > 0)
		{
			u.u_error = EINUSE;
		return;
	}
	phyfree(sm->phyAddr);
	shfree(sm);
	shdfree(u.u_procp->p_shm, i);	
	return;
}


/* 
 * �����ں���MemMap�ṹ��
*/
MemMap *mmalloc()
{
	register MemMap *mm ;
	for(mm = memMap; mm < memMap + MAP_MEM_COUNT; mm++)
	{
		if(!mm->flag)
		{
			mm->flag |= SM_ALLOC;
			return mm;
		}
	}
	return NULL;
}
			
void mmfree(MemMap * mm)
{
	mm->flag = 0;
}
/* 
 * �ж�sm�Ƿ��ѱ�ӳ�䡣
*/
int mapped(SharedMemory *sm)
{
		register int i; 
		for(i=0; i < MAP_MEM_COUNT_PROC; i++)
		{
			if(u.u_procp->p_mmap[i] && ((MemMap *)u.u_procp->p_mmap[i])->pSM == sm)
			{
				return 1;
			}
		}
		return 0;
}

/* 
 * ϵͳ���� MapSharedMemory���ں�ʵ�֡�
*/
void MapSharedMemory()
{
	char name[14];
	register SharedMemory *sm;
	register MemMap *mm;
	register int i;
	if(getname(name, 14))
		return;
	if((sm=shmexist(name)) == NULL)
	{
		u.u_error = EINVAL;
		return;
	}
	
	if(mapped(sm))
	{
		u.u_error = EEXIST;
		return;
	}

	if(i = shdalloc(u.u_procp->p_mmap)) < 0)
	{
		u.u_error = ENOSPC;
		return;
	}
	//�����ڴ��ѱ�������������
	if((sm->flag & SM_LOAD) == 0)
	{
		u.u_error = EIO; //��ȻҲ���Բ�ֱ�ӷ���,���Ƿ����ڴ沢������,Ȼ�����ӳ�䡣
		return;
	}

	//����MemMap�ṹ
	if((mm=mmalloc()) == NULL)
	{
		u.u_error = ENOSPC;
		return;
	}

	//����ӳ��
	if(mapmem(sm, mm))
	{
		mmfree(mm);
		u.u_error = ENOSPC;
		return;
	}
	sm->ref++;
	sm->refmem++;
	u.u_procp->p_mmap[i] = mm;
	u.u_ar0[R0] = i;
	suword(u.u_arg[1], mm->virtAddr);	//��ʼ�����ַ
	suword(u.u_arg[1]+2, mm->pSM->blocks* PHY_MEM_BLOCK_SIZE);		//��С
	return;
}

//������pdp 11/40оƬ
int virtSpaceAvail(int reqBlocks)
{
	int nt = nseg(u.u_tsize), nd = nseg(u.u_dsize), ns = nseg(u.u_ssize);
	register int i, j, nsh = 0, a, b;
	register MemMap *mm;
	MemMap map[4] = {0, 0, 0, 0};	//��p_mmap�е������ַ��С��������
	for(i=0, a=0; i < MAP_MEM_COUNT_PROC; i++)
		if(mm = u.u_procp->p_mmap[i])
		{
			nsh += nseg(mm->pSM->blocks);
			for(b=0; b < a; b++)
				if(mm->virtAddr < map[b]->virtAddr)
					break;
			for(j = b; j < a; j++)
				map[j+1] = map[j];
			map[b] = mm;
			a++;
		}
	if((8-(nt + nd + ns+nsh)) * 128 < reqBlocks)	//�����ڴ治��
		return 0;
	//��һ����϶�������reqBlocks����Ŀռ䡣
	j = a;
	a = (nt+nd)* 128;
	for(i=0; i < j; i++)
	{
		mm = map[i];
		b = mm->pSM->virtAddr/PHY_MEM_BLOCK_SIZE ;
		if(b-a>=reqBlocks)
		{
			return a;
		}
		a = b+nseg(mm->pSM->blocks)*128;
	}
	if((8-ns)*128 �C a >= reqBlocks)
		return a;
	return 0;
}

void setUAPRs(int index, int blk, int phyAddr, int perm)
{
	register int a = phyAddr/ PHY_MEM_BLOCK_SIZE;
  ap = UISA[index];
	dp = UISD[index];
  while(blk >= 128) {
		*dp++ = (127<<8) | perm;  
		*ap++ = a;
		a =+ 128;
		blk =- 128;
  }
  if(blk) {
		*dp++ = ((blk-1)<<8) | perm;
		*ap++ = a;
	}
}

/* 
* �������ڴ�smӳ�䵽�����ڴ�mm��
*/
int mapmem(SharedMemory *sm, MemMap *mm)
{
	register int a, *ap, *dp;
	register int blk = sm->blocks, ts = nseg(u.u_tsize), ds = nseg(u.u_dsize);
	int perm = ((u.u_procp ==sm->pOwner?sm->permOwn:sm->permOthers) <<1);
	if((a=virtSpaceAvail(blk)) == 0)
		return -1;
	setUAPRs(nseg(a), blk, sm->phyAddr, perm);
	mm->virtAddr = a * PHY_MEM_BLOCK_SIZE;
	mm->pSM = sm;
	return 0;
}

/* 
 * ж�ؾ��shd��Ӧ�Ĺ����ڴ�
*/
void unmap(int shd)
{
	register MemMap *mm = u.u_procp->p_mmap[shd];

//ж��ӳ��
	unmapmem(mm);
	mm->pSM->ref--;
	mm->pSM->refmem--;

	//�ͷ�MemMap�ṹ
	mmfree(mm);
	shdfree(u.u_procp->p_mmap, shd);
}

/* 
 * ϵͳ����UnmapSharedMemory���ں�ʵ�֡�
*/
void UnmapSharedMemory()
{
	register MemMap *mm;
	register int i = u.u_ar0[R0];
	if(i < 0 || i >= MAP_MEM_COUNT_PROC || (mm=u.u_procp->p_mmap[i]) == NULL)
	{
		u.u_error = EINVAL;
		return;
	}
	unmap(i);
}

/* 
 * ж�����mmӳ�䡣
*/
int unmapmem(MemMap *mm)
{
	register int *ap, *dp;
	register int blk = mm->pSM->blocks, ts = nseg(u.u_tsize), ds = nseg(u.u_dsize);
  ap = UISA[ts+=ds];
	dp = UISD[ts];
	ds = nseg(blk);
  while(ds >= 0) {
		*dp++ = 0;  
		*ap++ = 0;
		ds--;
  }
	return 0;
}

/* 
 * �жϹ����ڴ���Ҫ������(swapin=1)�򻻳�(swapin=0)�Ĵ�С��
*/
int ShmSize(proc *p, int swapin)
{
		register int i, size=0; 
		register MemMap *mm;
		for(i=0; i < MAP_MEM_COUNT_PROC; i++)
		{
			if(mm=p->p_mmap[i] && ((swapin && (mm->pSM->flag & SM_LOAD) == 0) ||
							(!swapin && mm->pSM->refmem == 1)))
			{
				size += mm->pSM->blocks * PHY_MEM_BLOCK_SIZE;
			}
		}
		return size;
}

/* 
 *�������ӳ�䣬���ڻ������ʱ��
*/
void EstablishShm(proc *p, int blkno)
{
	register int i;
	for(i=0; i < MAP_MEM_COUNT_PROC; i++)
	{
		if(mm=p->p_mmap[i] && (mm->pSM->flag & SM_LOAD) == 0)
		{
				mm->pSM->phyAddr = blkno* PHY_MEM_BLOCK_SIZE;
				blkno += mm->pSM->blocks ;
				mm->pSM->flag |= SM_LOAD;
		}
	}
}

//�����ǰ���������һ��ӳ�乲���ڴ�Ľ��̣����ͷŸù����ڴ档
void FreeShm(proc *p)
{
	register int i, size=0; 
	register MemMap *mm;
	for(i=0; i < MAP_MEM_COUNT_PROC; i++)
	{
		if((mm=p->p_mmap[i]) && --mm->pSM->refmem == 0)
		{
			phyfree(mm->pSM->phyAddr, mm->pSM->blocks);
		}
	}
}

/*
* ���½����������͹����ڴ��ӳ�䣬�����ڴ�������ַ���䣬
* ������̳������������С�
*/ 
void remapmem(proc *p)
{
	register MemMap *mm;
	register int i;
	for(i=0; i < MAP_MEM_COUNT_PROC; i++)
	{
		if((mm=p->p_mmap[i])
		{
			setUAPRs(mm->virtAddr/(PHY_MEM_BLOCK_SIZE*128), 
									mm->pSM->blocks, mm->pSM->phyAddr, 
										(p == mm->pSM->pOwner?mm->pSM->permOwn:	mm->pSM->permOthers)<<1);
		}
	}
}
