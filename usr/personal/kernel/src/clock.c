
#include "../param.h"
#include "../systm.h"
#include "../user.h"
#include "../proc.h"
 
#define	UMODE	0170000
#define	SCHMAG	10
 
/*
 * clock is called straight from
 * the real time clock interrupt.
 *
 * Functions:
 *	reprime clock
 *	copy *switches to display
 *	implement callouts
 *	maintain user/system times
 *	maintain date
 *	profile
 *	tout wakeup (sys sleep)
 *	lightning bolt wakeup (every 4 sec)
 *	alarm clock signals
 *	jab the scheduler
 */
#define 		SIGSTK				15		//栈溢出
clock(dev, sp, r1, nps, r0, pc, ps)
{
	register U_Thread *uthd= &u.u_threads[u.u_curthu];
	... ... 
  /*
     * lightning bolt time-out
     * and time of day
     */

 out:
    if((ps&UMODE) == UMODE) {
		uthd->u_utime++; 
		if(uthd->u_prof[3])
			incupc(pc, uthd->u_prof);
    } else
		uthd->u_stime++; 
		pp = uthd->u_threadp;

		/*
		* 检查栈是否溢出，如溢出则发出信号。通常，这应该由MMU实现，当它检测到访
		* 问了栈外未映射空间时，会触发异常自陷。现在由程序手动实现，是因为线程栈空
		* 间连续。如果采用现代页式虚存结构，可以在线程栈之间留出空白，实现自动检测。
		*/
		if(sp < uthd->u_threadp->t_ustackaddr || sp > uthd->u_threadp->t_ustackaddr+ 
				uthd->u_threadp->t_ustacksize)
		{
			psignal(u.u_procp, SIGSTK); //新增的栈溢出信号
		}

    if(++pp->p_cpu == 0)  
		pp->p_cpu--; 
    if(++lbolt >= HZ) {
		if((ps&0340) != 0)
			return; 
		lbolt =- HZ;	
		if(++time[1] == 0)
			++time[0];
		spl1();
		if(time[1]==tout[1] && time[0]==tout[0]) 
			wakeup(tout);    
		if((time[1]&03) == 0) {
			runrun++;
			wakeup(&lbolt); 
		}

	if(pp->p_time != 127)
			pp->p_time++;		
for(pp = &proc[0]; pp < & proc [NPROC]; pp++)
			if (pp->p_stat) {
		        	if(pp->p_time != 127)
					pp->p_time++;		
}		

		for(pp = &Threads[0]; pp < & Threads [NTHREAD]; pp++)
			if (pp->p_stat) {
				if((pp->p_cpu & 0377) > SCHMAG)
					pp->p_cpu =- SCHMAG; 
				else
					pp->p_cpu = 0; 
				if(pp->p_pri > PUSER) 
					setpri(pp);
			}
			
		if(runin!=0) {
			runin = 0;
			wakeup(&runin);
		}
		if((ps&UMODE) == UMODE) {
			u.u_ar0 = &r0;	
			if(issig())
				psig();
			setpri(uthd->u_threadp); //调整当前线程优先级
		}
    }
}

 
/*
 * timeout is called to arrange that
 * fun(arg) is called in tim/HZ seconds.
 * An entry is sorted into the callout
 * structure. The time in each structure
 * entry is the number of HZ's more
 * than the previous entry.
 * In this way, decrementing the
 * first entry has the effect of
 * updating all entries.
 */
timeout(fun, arg, tim)
{
	register struct callo *p1, *p2;
	register t;
	int s;
 
	t = tim;
	s = PS->integ;
	p1 = &callout[0];
	spl7();
	while(p1->c_func != 0 && p1->c_time <= t) {
		t =- p1->c_time;
		p1++;
	}
	p1->c_time =- t;
	p2 = p1;
	while(p2->c_func != 0)
		p2++;
	while(p2 >= p1) {
		(p2+1)->c_time = p2->c_time;
		(p2+1)->c_func = p2->c_func;
		(p2+1)->c_arg = p2->c_arg;
		p2--;
	}
	p1->c_time = t;
	p1->c_func = fun;
	p1->c_arg = arg;
	PS->integ = s;
}