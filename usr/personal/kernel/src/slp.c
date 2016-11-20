/*
 * The main loop of the scheduling (swapping)
 * process.
 * The basic idea is:
 *  see if anyone wants to be swapped in;
 *  swap out processes until there is room;
 *  swap him in;
 *  repeat.
 * Although it is not remarkably evident, the basic
 * synchronization here is on the runin flag, which is
 * slept on and is set once per second by the clock routine.
 * Core shuffling therefore takes place once per second.
 *
 * panic: swap error -- IO error while swapping.
 *	this is the one panic that should be
 *	handled in a less drastic way. Its
 *	very hard.
 */
sched()
{
	struct proc *p1;
	register struct proc *rp;
	register a, n;
 
	/*
	 * find user to swap in
	 * of users ready, select one out longest
	 */
 
	goto loop;
 
sloop:
	runin++;
	sleep(&runin, PSWP);
 
loop:
	spl6();
	n = -1;
	for(rp = &proc[0]; rp < &proc[NPROC]; rp++)
	if(rp->p_stat==SRUN && (rp->p_flag&SLOAD)==0 &&
	    rp->p_time > n) {
		p1 = rp;
		n = rp->p_time;
	}
	if(n == -1) {
		runout++;
		sleep(&runout, PSWP);
		goto loop;
	}
 
	/*
	 * see if there is core for that process
	 */
 
	spl0();
	rp = p1;
	a = rp->p_size;
	if((rp=rp->p_textp) != NULL)
		if(rp->x_ccount == 0)
			a =+ rp->x_size;

	b = ShmSize(p1, 1);
	a += b;
	if((a=malloc(coremap, a)) != NULL)
		goto found2;
 
	/*
	 * none found,
	 * look around for easy core
	 */
 
	spl6();
	for(rp = &proc[0]; rp < &proc[NPROC]; rp++)
	if((rp->p_flag&(SSYS|SLOCK|SLOAD))==SLOAD &&
	    (rp->p_stat == SWAIT || rp->p_stat==SSTOP))
		goto found1;
 
	/*
	 * no easy core,
	 * if this process is deserving,
	 * look around for
	 * oldest process in core
	 */
 
	if(n < 3)
		goto sloop;
	n = -1;
	for(rp = &proc[0]; rp < &proc[NPROC]; rp++)
	if((rp->p_flag&(SSYS|SLOCK|SLOAD))==SLOAD &&
	   (rp->p_stat==SRUN || rp->p_stat==SSLEEP) &&
	    rp->p_time > n) {
		p1 = rp;
		n = rp->p_time;
	}
	if(n < 2)
		goto sloop;
	rp = p1;
 
	/*
	 * swap user out
	 */
 
found1:
	spl0();
	rp->p_flag =& ~SLOAD;
	xswap(rp, 1, 0);
	goto loop;
 
	/*
	 * swap user in
	 */
 
found2:
	if((rp=p1->p_textp) != NULL) {
		if(rp->x_ccount == 0) {
			if(swap(rp->x_daddr, a, rp->x_size, B_READ))
				goto swaper;
			rp->x_caddr = a;
			a =+ rp->x_size;
		}
		rp->x_ccount++;
	}
	rp = p1;
	if(swap(rp->p_addr, a, rp->p_size, B_READ))
		goto swaper;
	if(b>0)
	{
		if(swap(rp->p_addr+rp->p_size, a+rp->p_size,b/PHY_MEM_BLOCK_SIZE,B_READ))
  		goto swaper;
		EstablishShm(rp, a+rp->p_size);
	}

	mfree(swapmap, (rp->p_size+7)/8, rp->p_addr);
	rp->p_addr = a;
	rp->p_flag =| SLOAD;
	rp->p_time = 0;
	goto loop;
 
swaper:
	panic("swap error");
}
 
/*
 * This routine is called to reschedule the CPU.
 * if the calling process is not in RUN state,
 * arrangements for it to restart must have
 * been made elsewhere, usually by calling via sleep.
 */
swtch()
{
	static struct proc *p;
	register i, n;
	register struct proc *rp;
 
	if(p == NULL)
		p = &proc[0];
	/*
	 * Remember stack of caller
	 */
	savu(u.u_rsav);
	/*
	 * Switch to scheduler's stack
	 */
	retu(proc[0].p_addr);
 
loop:
	runrun = 0;
	rp = p;
	p = NULL;
	n = 128;
	/*
	 * Search for highest-priority runnable process
	 */
	i = NPROC;
	do {
		rp++;
		if(rp >= &proc[NPROC])
			rp = &proc[0];
		if(rp->p_stat==SRUN && (rp->p_flag&SLOAD)!=0) {
			if(rp->p_pri < n) {
				p = rp;
				n = rp->p_pri;
			}
		}
	} while(--i);
	/*
	 * If no process is runnable, idle.
	 */
	if(p == NULL) {
		p = rp;
		idle();
		goto loop;
	}
	rp = p;
	curpri = n;
	/*
	 * Switch to stack of the new process and set up
	 * his segmentation registers.
	 */
	retu(rp->p_addr);
	sureg();//EINTR
	/*
	 * If the new process paused because it was
	 * swapped out, set the stack level to the last call
	 * to savu(u_ssav).  This means that the return
	 * which is executed immediately after the call to aretu
	 * actually returns from the last routine which did
	 * the savu.
	 *
	 * You are not expected to understand this.
	 */
	if(rp->p_flag&SSWAP) {
		rp->p_flag =& ~SSWAP;
		aretu(u.u_ssav);
	}
	/*
	 * The value returned here has many subtle implications.
	 * See the newproc comments.
	 */
	return(1);
}

swtch()
{
		static struct Thread *t;
    register i, n;
    register struct Thread *rt, *prevTh;
    struct proc *	pCur = u.u_procp;
    if(t == NULL)
		t = &Threads[0];
    /*
     * Remember stack of caller
     */
    savu(u.u_threads[u.u_curthd].u_rsav); 
    /*
     * Switch to scheduler’s stack
     */
    retu(proc[0].p_addr, 0); 

 loop:
    runrun = 0;
    rt = t;			
    prevTh = t = NULL;
    n = 128; 
     /*
     * Search for highest-priority runnable thread
     */
    i = NTHREAD;
    do {
		rt++;
		if(rt >= &Threads[NTHREAD])
			rt = &Threads[0];
		if(rt->t_stat==SRUN && (rt->t_proc->p_flag&SLOAD)!=0) {  
			if(rt->p_pri < n) {
				t = rt;
				n = rp->p_pri;
			}
			else if(rt->p_pri == n && prevTh != NULL && prevTh->t_proc != pCur &&
 rt->t_proc == pCur) //如果优先级相同，则优先调度本进程内线程
			{
				t = rt;
			}
			prevTh = t;
		}
	}
    } while(--i); 
    /*
     * If no thread is runnable, idle.
     */
    if(t == NULL) {
		t = rt;
		idle();		
		goto loop;
    }
    rt = t;
    curpri = n;
    /* Switch to stack of the new thread and set up
     * his segmentation registers.
     */


//如果待调度线程不属于当前进程，则需要重新映射用户和内核空间
//重新映射内核地址0o140000到rt->t_proc->p_addr，并改变内核栈指针
retu(rt->t_proc->p_addr, rt->t_index); 

   if(rt->t_proc != u.u_procp) 
{
   	sureg();	//重新映射用户空间
}
else if(rt->t_index != u.u_curthu)//直接改变内核栈指针r6和环境指针r5到新线程rt
{
	aretu(u.u_threads[rt->t_index].u_rsav);
}
u.u_curthu = rt->t_index;
	
    /*
     * If the new process paused because it was
     * swapped out, set the stack level to the last call
     * to savu(u_ssav). This means that the return
     * which is executed immediately after the call to aretu 
     * actually returns from the last routine which did
     * the savu. 
     *
     * You are not expected to understand this.
     */
    if(rt->t_proc->p_flag&SSWAP) {
		rt->t_proc->p_flag =& ~SSWAP;
		aretu(u.u_threads[rt->t_index].u_rsav);
}

    /* The value returned here has many subtle implications. 
     * See the newproc comments.
     */
   return(1);
 }


//线程挂起
sleep(chan, pri) 
{
    register struct Thread*rt, s;

    s = PS->integ;
    rt = u.u_threads[u.u_curthu].u_threadp;
    if(pri >= 0) {
		if(issig())
			goto psig;
		spl6();
		rt->t_wchan = chan;
		rt->t_stat = SWAIT;
		rt->t_pri = pri;
		spl0();
		if(runin != 0) {
			runin = 0;
			wakeup(&runin); 
		}
		if(--u.u_procp->p_actThreadNum == 0)
			u.u_procp->p_stat = SWAIT;
		swtch();    
		++u.u_procp->p_actThreadNum;
		u.u_procp->p_stat = SRUN;
if(issig())
			goto psig;
    } 
    else {
		spl6();
		rt->t_wchan = chan;
		rt->t_stat = SSLEEP;
		rt->t_pri = pri;
		spl0();
if(--u.u_procp->p_actThreadNum == 0)
			u.u_procp->p_stat = SWAIT;
		swtch();
++u.u_procp->p_actThreadNum;
		u.u_procp->p_stat = SRUN;

    }
    PS->integ = s;
    return;

    /*
     * If priority was low (>=0) and 
     * there has been a signal,
     * execute non-local goto to
     * the qsav location.
     * (see trap1/trap.c)
     */
 psig:
    aretu(u.u_threads[u.u_curthu].u_qsav);
}

//唤醒挂起的线程
wakeup(chan) 
 {
    register struct Thread *t;
    register c, i;

    c = chan;
    t = &Threads[0];
    i = NTHREAD;
    do {
		if(t->t_wchan == c) { 
			setrun(t);
		}
		t++;
    } while(--i);
} 

//设置线程运行，同时也设置其所属进程为运行态
setrun(t)
{
    register struct proc *rp;
    register struct Thread *rt = t;
    rp = rt->t_proc;
rp->p_stat = SRUN;
++rp->p_actThreadNum;
rt->t_stat = SRUN;

    if(rt->p_pri < curpri)
		runrun++;
    if(runout != 0 && (rp->p_flag&SLOAD) == 0) {
	runout = 0;
	wakeup(&runout);	//唤醒调度线程
    }
}



//调整线程t的优先级
setpri(Thread *t) 
{
    register *tt, p;

    tt = t;
    p = (tt->p_cpu & 0377)/16;
    p =+ PUSER + tt->p_nice;
    if(p > 127)
		p = 127;
    if(p > curpri)     
		runrun++;
    tt->p_pri = p;
}

newproc()
{
	int a1, a2;
	struct proc *p, *up;
	register struct proc *rpp;
	register *rip, n;
	
	MailBox* pmbx[4];
	int i;
	struct Thread *t;
	register struct Thread *rtt;
	struct U_Thread *uthd = &u.u_threads[u.u_curthu];
	p = NULL;
	/*
	First, just locate a slot for a process
	and copy the useful info from this process into it.  
	The panic "cannot happen" because fork already
	checked for the existence of a slot.
	*/
	retry:
	mpid++;
	if(mpid < 0) {
	    mpid = 0;
	    goto retry;
	}
	for(rpp = &proc[0]; rpp < &proc[NPROC]; rpp++) {
	if(rpp->p_stat == NULL && p==NULL)
	    p = rpp;
	if (rpp->p_pid==mpid) 
	    goto retry; 
	}
	if ((rpp = p)==NULL)
	    panic("no procs");
	
	//找空闲的线程结构
	t = NULL;
	for(n=0, rtt = &Threads[0]; rtt < &Threads [NTHREAD]; rtt++,n++) {
	if(rtt->t_stat == NULL)
	{
	if((rtt->t_tid %NTHREAD)== n)
	{
	 rtt->t_tid += NTHREAD;
	}
	else
	{
		rtt->t_tid = n;
	}
	if(rtt->t_tid < 0)
		rtt->t_tid = n;
	t = rtt;
	break;
	}
	}
	if ((rtt=t)==NULL)
	    panic("no threads");
	
	
	/*
	make proc entry for new proc	
	*/
	rip = u.u_procp;
	up = rip;
	rpp->p_stat = SRUN;
	rpp->p_flag = SLOAD;
	rpp->p_uid = rip->p_uid;
	rpp->p_ttyp = rip->p_ttyp;
	rpp->p_nice = rip->p_nice;
	rpp->p_textp = rip->p_textp;
	rpp->p_pid = mpid;
	rpp->p_ppid = rip->p_pid;
	rpp->p_time = 0;
	
	for(n = 0; n < NTHREAD_PROC; n++)
	rpp->p_threads[n] = NULL;
		rpp->p_threads[u.u_curthu] = rtt;
		rpp->actThreadNum = rpp->p_threadNum = 1;
	
	//设置新线程数据
	rtt->t_stat = SRUN;
	rtt->t_flag = SLOAD;
	rtt->t_uid = uthd->u_threadp->t_uid;
	rtt->t_ttyp = uthd->u_threadp->t_ttyp;
	rtt->t_nice = uthd->u_threadp->t_nice;
	rtt->t_pid = mpid;
	rtt->t_time = 0;
	rtt->t_cpu = 0;
	
	/*
	make duplicate entries
	where needed
	*/
	
	for(rip = &uthd->u_ofile[0]; rip < & uthd->u_ofile[NOFILE];)
	    if((rpp = *rip++) != NULL)
	        rpp->f_count++;
	if((rpp=up->p_textp) != NULL) {
	        rpp->x_count++;          
	        rpp->x_ccount++;
	}
	uthd->u_cdir->i_count++;
	/*
	Partially simulate the environment
	of the new process so that when it is actually
	created (by copying) it will look right.
	*/
	savu(uthd->u_rsav);
	rpp = p;
	u.u_procp = rpp;	
	rip = up; 
	n = rip->p_size;
	a1 = rip->p_addr;
	rpp->p_size = n;
	a2 = malloc(coremap, n);
	
	//对子进程u_rsema的清除动作，并增加Semaphore结构的引用计数
	for(i = 0; i < 4; i++)
	{
		if(u.u_osema[i]  != NULL)
			((Semaphore *)u.u_osema[i])->ref++;
		rs[i] = u.u_rsema[i];
		u.u_rsema[i] = 0;
	}

	for(i = 0; i < MAX_EVENTS_PROC; i++)
	{
		if(u.u_oevent[i]  != NULL)
			((Event *)u.u_oevent[i])->ref++;
	}
	for(i = 0; i < MBX_COUNT_PROC; i++)
	{
		if((pmbx[i]=u.u_ombx[i])  != NULL)
			if((pmbx[i]->flag & MBX_PUBLIC) == MBX_SHARED)
				pmbx[i]->ref++;
			else
				u.u_ombx[i] = NULL;
	}

	/*
	If there is not enough core for the
	new process, swap put/out the current process to 
	generate the copy.
	*/
	if(a2 == NULL) {
	    rip->p_stat = SIDL; 
	    rpp->p_addr = a1;
	    savu(uthd->u_ssav); 
	    xswap(rpp, 0, 0);
	    rpp->p_flag =| SSWAP;
	    rip->p_stat = SRUN;
	} else {
	/*
	There is core, so just copy.
	*/
	    rpp->p_addr = a2;
	    while(n--)
	        copyseg(a1++, a2++);
	}
	u.u_procp = rip;   
	for(i = 0; i < 4; i++)
	{
		u.u_rsema[i] = rs[i];
	}

	return(0);
}

... ...