main()
{
... ...
	sinit();	//initialize semphore
	einit();
	msginit();
	mbxinit();
	
	/*
     * set up system process
     */

  proc[0].p_addr = *ka6;	
  proc[0].p_size = USIZE; 
  proc[0].p_stat = SRUN;
  proc[0].p_flag =| SLOAD|SSYS;
	u.u_procp = &proc[0];
	
	//set up system thread
	u.u_curthu = 0;
	u.u_threads[0].t_proc = &proc[0];
	Threads[0].t_flag = SLOAD|SSYS;
	Threads[0].t_stat = SRUN;
	strcpy(Threads[0].t_name, ¡°scheduler¡±);
... ... 
}

/*
 * Load the user hardware segmentation
 * registers from the software prototype.
 * The software registers must have
 * been setup prior by estabur.
 */
sureg()
{
	register *up, *rp, a;
 
	a = u.u_procp->p_addr;
	up = &u.u_uisa[16];
	rp = &UISA->r[16];
	if(cputype == 40) {
		up =- 8;
		rp =- 8;
	}
	while(rp > &UISA->r[0])
		*--rp = *--up + a;
	if((up=u.u_procp->p_textp) != NULL)
		a =- up->x_caddr;
	up = &u.u_uisd[16];
	rp = &UISD->r[16];
	if(cputype == 40) {
		up =- 8;
		rp =- 8;
	}
	while(rp > &UISD->r[0]) {
		*--rp = *--up;
		if((*rp & WO) == 0)
			rp[(UISA-UISD)/2] =- a;
	}
	remapmem(u.u_procp);
}
 