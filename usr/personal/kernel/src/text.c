/*
 * Swap out process p.
 * The ff flag causes its core to be freed--
 * it may be off when called to create an image for a
 * child process in newproc.
 * Os is the old size of the data area of the process,
 * and is supplied during core expansion swaps.
 *
 * panic: out of swap space
 * panic: swap error -- IO error
 */
xswap(p, ff, os)
int *p;
{
	register *rp, a;
 
	rp = p;
	b = ShmSize(rp, 0);
	if(os == 0)
		os = rp->p_size;
	//a = malloc(swapmap, (rp->p_size+7)/8);
	 a = malloc(swapmap, (rp->p_size+b/PHY_MEM_BLOCK_SIZE+7)/8);
	if(a == NULL)
		panic("out of swap space");
	xccdec(rp->p_textp);
	rp->p_flag =| SLOCK;
	if(swap(a, rp->p_addr, os, 0))
		panic("swap error");
	if(ff)
	{
		mfree(coremap, os, rp->p_addr);
		FreeShm(rp);
	}
	rp->p_addr = a;
	rp->p_flag =& ~(SLOAD|SLOCK);
	rp->p_time = 0;
	if(runout) {
		runout = 0;
		wakeup(&runout);
	}
}