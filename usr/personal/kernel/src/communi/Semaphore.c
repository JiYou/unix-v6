#include "../../include/comm.h"
void sinit()
{
	semaphs.flag = MAX_SEMA;	
}

int sdalloc()
{
	register int i;
	for(i = 0; i < 4; i++)
	{
		if(u_osema[i] == NULL)
			return i;
	}
	u.u_error = EMFILE;
	return -1;
}
	
Semaphore *salloc()
{
	register int I, num = semaphs.flag & ~;
	for(i = 0; i < MAX_SEMA; i++)
	{
			if((semaphs.semas[i].flag & S_ALLOC) == 0)
			{
				semaphs.semas[i].flag = S_ALLOC;
				semaphs.semas[i].ref = 1;
				return semaphs.semas+i;
			}
		}
		u.u_error = EFAULT;
		return NULL;
}

void sdfree(int sd)
{
	register *sema = u.u_osema[sd];
	sema->ref--;
	u.u_osema[sd] = NULL;
}

void sfree(Semaphore *sema)
{
	sema->s= sema->init = 0;
	sema->flag &= ~S_ALLOC;
}

//锁定等待有空余的Semaphore结构
void slock()
{
	while((semaphs.flag & ~S_WANT) == 0)
	{
		semaphs.flag  |= S_WANT;
		sleep(&semaphs.flag, PSEMA);
	}	
	semaphs.flag--;
}

//释放Semaphore结构，唤醒等待进程
void sunlock()
{
	if((semaphs.flag & S_WANT) != 0)
	{
		semaphs.flag  &= ~S_WANT;
		wakeup(&semaphs.flag);
	}	
	semaphs.flag++;
}

void CreateSemaphore()
{
	register Semaphore  *sema;
	register int sd, init = u.u_ar0[R0];
	if(init <= 0)
	{
		u.u_error = EINVAL;
		return;
	}
	if((sd = sdalloc()) < 0)
	{
		return;
	}
	slock();	
	if((sema = salloc()) == NULL)
	{
		return;
	}
	u_osema[sd] = sema;
	sema->init = sema->s = init;
	u.u_ar0[R0] = sd;
}


void closes(int sd)
{
	register Semaphore  *sema;
	if(sd < 0 || sd  >= 4 || (sema = u.u_osema[sd]) == NULL)
	{
		u.u_error = EINVAL;
		return;
	}
	if(u.u_rsema[sd] > 0)
	{
		u.u_error = EINUSE;
		return;
	}

	sdfree(sd);

	//所有进程都不再使用该信号量，则释放Semaphore结构。
 	if(sema->ref == 0)
	{
		sfree(sema);
		sunlock();
	}
}

void CloseSemphore()
{
	closes(u.u_ar0[R0]);	
}


void AcquireSemaphore()
{
	register Semaphore  *sema; 

	register int sd = u.u_ar0[R0], a = 0;
	if(sd < 0 || sd >= 4 || (sema = u.u_osema[sd]) == NULL)
	{
		u.u_error = EINVAL;
		return;
	}
	
	while(sema->s == 0)
	{
		sema->flag =  a| (sema->flag & S_BITS);
		sleep(&sema->flag, PSEMA);
	}
	sema->s--;
	u.u_rsema[sd]++;
}

void ReleaseSemphore()
{
	register Semaphore  *sema;
	register int sd = u.u_ar0[R0], a = 0;
	if(sd < 0 || sd >= 4 || (sema = u.u_osema[sd]) == NULL)
	{
		u.u_error = EINVAL;
		return;
	}

	if(u.u_rsema[sd] == 0)	//以前未调过AcquireSemaphore或已全释放
	{
		u.u_error = EINVAL;
		return;
	}

	sema->s++;
	u.u_rsema[sd]--;
	if(sema->flag & S_WANT)
	{
		sema->flag &= ~S_WANT;
		//唤醒调用AcquireSemaphore而正在等待的进程
		wakeup(&sema->flag);	
	}
}
