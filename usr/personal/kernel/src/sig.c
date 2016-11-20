... ...

psig()
{
        register n, p, i;
        register *rp;
register struct U_Thread *uthd = &u.u_threads[u.u_curthu];
            struct *Thread thd;
	    char bTerm[NTHREAD_PROC];

    rp = u.u_procp;
    n = rp->p_sig; 
    rp->p_sig = 0;
    if((p=u.u_signal[n]) != 0) {
		uthd->u_error = 0;
		if(n != SIGINS && n != SIGTRC)
			u.u_signal[n] = 0;
		n =  u.u_ar0[R6] - 4;
		grow(n);
		suword(n+2, u.u_ar0[RPS]);
		suword(n, u.u_ar0[R7]);
		u.u_ar0[R6] = n; 
		u.u_ar0[RPS] =& ~TBIT;
		u.u_ar0[R7] = p;  
		return;  
    }
    switch(n) {

        case SIGQIT:
    case SIGINS:
    case SIGTRC:
    case SIGIOT:
    case SIGEMT:
    case SIGFPT:
    case SIGBUS:
    case SIGSEG:
case SIGSYS:
case SIGSTK:
		u.u_arg[0] = n;
		if(core())
			n =+ 0200;
		break;
	  case SIGTERM:	//终止指定线程
		for(i = 0, p=0, n=0; i < NTHREAD_PROC&& n < rp->p_threadNum; i++)
		{
			thd = rp->p_threads[i];
			if(thd != NULL)
			{
				if(thd->t_flag & STERM)	
				{
					bTerm[i] = TRUE;	//可能会有多个线程被同时终止
					p++;
					rp = thd;
				}
				else
					bTerm[i] = FALSE;
				n++;
}
		       }
	      if(p == 1)
	     {
		if(bTerm[u.u_curthu])   			//退出当前线程
			ExitThread();
		EndOtherThd(rp);
		return;
}
if(p > 1)
{
		rp = u.u_procp;
		for(i=0, n = 0; n < p && i < NTHREAD_PROC;i++)
		{
			if(bTerm[i])
		   {
				if(i != u.u_curthu)
				{
					EndOtherThd(rp->p_threads[i]);
				}
			   n++;
			}
		}
		if(bTerm[u.u_curthu])
			ExitThread();
	}				
	return;
  	    }
    u.u_arg[0] = (u.u_ar0[R0]<<8) | n;
    exit();
 }

... ...