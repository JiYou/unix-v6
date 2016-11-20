#include "../../include/comm.h"
void einit()
{
	evts.flag = MAX_EVENTS;	
}

int edalloc()
{
	register int i;
	for(i = 0; i < MAX_EVENTS_PROC; i++)
	{
		if(u_oevent[i] == NULL)
			return i;
	}
	u.u_error = EMFILE;
	return -1;
}
	
Event *ealloc()
{
	register int i;
		
	for(i = 0; i < MAX_EVENTS; i++)
	{
		if((evts.events[i].flag & S_ALLOC) == 0)
		{
			evts.events[i].flag = S_ALLOC;
			evts.events[i].ref = 1;
			return evts.events+i;
		}
	}
	u.u_error = EFAULT;
	return NULL;
}
	
void edfree(int ed)
{
	register *event = u.u_oevent[ed];
	event->ref--;
	u.u_oevent[ed] = NULL;
}

void efree(Event *event)
{
	event->s = TRUE;
	event->flag &= ~S_ALLOC;
}

	//锁定等待有空余的Event结构
void elock()
{
	while((evts.flag & ~S_WANT) == 0)
	{
		evts.flag  |= S_WANT;
		sleep(&evts.flag, PEVENT);
	}	
evts.flag--;
}
	
//释放Event结构，唤醒等待进程
void eunlock()
{
	if((evts.flag & S_WANT) != 0)
	{
		evts.flag  &= ~S_WANT;
		wakeup(&evts.flag);
	}	
	evts.flag++;
}

void CreateEvent()
{
	register Event  *event;
	register int ed, init = u.u_ar0[R0];
	if((ed = edalloc()) < 0)
	{
		return;
	}
	elock();	
	if((event = ealloc()) == NULL)
	{
		return;
	}
	u_oevent[ed] = event;
	event->e = init;
	u.u_ar0[R0] = ed;
}


void closee(int ed)
{
	register Event  *event;
	if(ed < 0 || ed >= 4 || (event = u.u_osema[ed]) == NULL)
	{
		u.u_error = EINVAL;
		return;
	}
	edfree(ed);

//所有进程都不再使用该信号量，则释放Event结构。
	if(event->ref == 0)
	{
		efree(event);
		eunlock();
	}
}

void CloseEvent()
{
	closee(u.u_ar0[R0]);	
}

int rmtimer(int (*func)(), int arg)
{
	register callo *p = &callout[0], *p1, *p2;
	int tm;
	spl6();
	for(; p->c_func && p < &callout[NCALL]; p++)
	{
		if(func == p->c_func && arg == p->c_arg)
		{
			p1 = p;
			tm = p->c_time;
			p2 = p++;
			while(p2->c_func = p->c_func&&p<&callout[NCALL]) {
				p2->c_time = p->c_time;
				p2->c_arg = p->c_arg;
				p++;
				p2++;
			}
			if(p1->c_func)
				p1->c_time += tm;
			if(p == &callout[NCALL])
				callout[NCALL-1].c_func = NULL;
			spl0();
			return 0;
		}
	}
	spl0();
	return -1;
}

int evtexpr(proc *p)
{
	setrun(p);
	p->w_chan = -1;
	return 0;
}

void WaitEvent()
{
	register Event  *event; 

	register int ed = u.u_ar0[R0], tm = u.u_arg[0];
	if(ed < 0 || ed >= 4 || (event = u.u_oevent[ed]) == NULL || tm < WT_INFINITE)
	{
		u.u_error = EINVAL;
		return;
	}
	if(event->e)
	{
		event->e = FALSE;
		u.u_ar0[R0] = 0;
		return;
	}

	if(tm == WT_IMME)
	{
		u.u_ar0[R0] = -1;
		return -1;
	}

	if(tm == WT_INFINITE)
	{
		event->flag |= S_WANT;
		sleep(&event->flag, PEVENT);
		event->e = FALSE;
		u.u_ar0[R0] = 0;
		return;
	}

		//处理tm>0的情形。1个时钟滴答≈16毫秒。
	event->flag |= S_WANT;
	timeout(evtexpr, u.u_procp, (tm+15)/16);  

	//这里没有用while判断event->e，而只是简单地调用sleep，是为了多个进程调用
	//WaitEvent挂在同一个事件上时，可以都被唤醒返回执行，如果使用while那么最终
	//就只有一个能够返回执行。
	sleep(&event->flag, PEVENT); 
	if(u.u_procp->w_chan == -1) //定时器已超时
	{
		u.u_procp->w_chan = 0;
		u.u_ar0[R0] = 1;
	}
	else		//事件已触发, 则停止定时器，返回0
	{
		event->e = FALSE;
		if(rmtimer(evtexpr, u.u_procp) < 0)
		{
			u.u_error = EFAULT;
			return;
		}
		u.u_ar0[R0] = 0;
	}
}


void SetEvent()
{
	register Event *event;
	register int ed = u.u_ar0[R0];
	if(ed < 0 || ed >= 4 || (event = u.u_oevent[ed]) == NULL)
	{
		u.u_error = EINVAL;
		return;
	}

	if(event->e)
	{
		u.u_error = EEVENTSET;
		return;
	}
	event->e = TRUE;
	if(event->flag & S_WANT)
	{
		event->flag &= ~S_WANT;
		//唤醒调用WaitEvent而正在等待的进程
		wakeup(&event->flag);	
	}
}
