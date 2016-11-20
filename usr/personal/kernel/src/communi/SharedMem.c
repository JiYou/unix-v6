/* 
 *分配句柄，p可能是u.u_procp->p_shm或u.u_procp->p_mmap。
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
 * 判断名为name的共享内存是否已存在，如果存在，则返回SharedMemory指针，
* 否则返回NULL。函数只是简单遍历，如想提高性能，可用hash表。
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
 * 分配SharedMemory结构。
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
* 系统调用 CreateSharedMemory 的内核实现。
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
 * 系统调用 FreeSharedMemory的内核实现。
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
 * 分配内核中MemMap结构。
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
 * 判断sm是否已被映射。
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
 * 系统调用 MapSharedMemory的内核实现。
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
	//共享内存已被换出到磁盘上
	if((sm->flag & SM_LOAD) == 0)
	{
		u.u_error = EIO; //当然也可以不直接返回,而是分配内存并加载它,然后继续映射。
		return;
	}

	//分配MemMap结构
	if((mm=mmalloc()) == NULL)
	{
		u.u_error = ENOSPC;
		return;
	}

	//建立映射
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
	suword(u.u_arg[1], mm->virtAddr);	//起始虚拟地址
	suword(u.u_arg[1]+2, mm->pSM->blocks* PHY_MEM_BLOCK_SIZE);		//大小
	return;
}

//仅考虑pdp 11/40芯片
int virtSpaceAvail(int reqBlocks)
{
	int nt = nseg(u.u_tsize), nd = nseg(u.u_dsize), ns = nseg(u.u_ssize);
	register int i, j, nsh = 0, a, b;
	register MemMap *mm;
	MemMap map[4] = {0, 0, 0, 0};	//把p_mmap中的虚拟地址从小到大排序
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
	if((8-(nt + nd + ns+nsh)) * 128 < reqBlocks)	//虚拟内存不够
		return 0;
	//找一个空隙，分配给reqBlocks所需的空间。
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
	if((8-ns)*128 – a >= reqBlocks)
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
* 把物理内存sm映射到虚拟内存mm。
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
 * 卸载句柄shd对应的共享内存
*/
void unmap(int shd)
{
	register MemMap *mm = u.u_procp->p_mmap[shd];

//卸载映射
	unmapmem(mm);
	mm->pSM->ref--;
	mm->pSM->refmem--;

	//释放MemMap结构
	mmfree(mm);
	shdfree(u.u_procp->p_mmap, shd);
}

/* 
 * 系统调用UnmapSharedMemory的内核实现。
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
 * 卸载虚存mm映射。
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
 * 判断共享内存需要被换入(swapin=1)或换出(swapin=0)的大小。
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
 *建立虚存映射，用于换入进程时。
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

//如果当前进程是最后一个映射共享内存的进程，则释放该共享内存。
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
* 重新建立进程虚存和共享内存的映射，共享内存的虚拟地址不变，
* 否则进程程序不能正常运行。
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
