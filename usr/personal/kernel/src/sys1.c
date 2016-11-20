fork()
{
    register struct proc *p1, *p2;

    p1 = u.u_procp;
    for(p2 = &proc[0]; p2 < &proc[NPROC]; p2++)
		if(p2->p_stat == NULL)
		goto found;
    u.u_threads[u.u_curthu].u_error = EAGAIN;
    goto out;

 found:
    for(p2 = &Threads[0]; p2 < &Threads[NTHREAD]; p2++)
		if(p2->t_stat == NULL)
		goto found2;
    u.u_threads[u.u_curthu].u_error = EAGAIN;
    goto out;

found2:
    if(newproc()) {//���ӽ�����
			u.u_threads[u.u_curthu].u_ar0[R0] = p1->p_pid; 
			u.u_threads[u.u_curthu].u_cstime[0] = 0;
			u.u_threads[u.u_curthu].u_cstime[1] = 0;
			u.u_threads[u.u_curthu].u_stime = 0;
			u.u_threads[u.u_curthu].u_cutime[0] = 0;
			u.u_threads[u.u_curthu].u_cutime[1] = 0;
			u.u_threads[u.u_curthu].u_utime = 0;
			return;
    }
    u.u_threads[u.u_curthu].u_ar0[R0] = p2->p_pid;

 out:
    u.u_threads[u.u_curthu].u_ar0[R7] =+ 2;
 }



exit()
{
	struct Thread **t;
	register int *pf = &u.u_procp->msgQue.flag;
	register MailBox *mbx;
	register Mail *m;
	
	register MemMap *mm;
	register SharedMemory *sm;
	int i;
	... ... 
	
	for(i = 0; i < 4; i++)
	{
			u.u_rsema[i] = 0;
			if(u.u_osema[i] != NULL)
				closes(i);
	}

	for(i = 0; i < MAX_EVENTS_PROC; i++)
	{
		if(u.u_oevent[i] != NULL)
			closee(i);
	}

	for(t = &u.u_procp->p_threads[0]; t < &u.u_procp->p_threads[NTHREAD]; t++)
	{
		if(*t != NULL)
		{
		    for(q = &u.u_threads[(*t)->t_index].u_ofile[0]; q < & u.u_threads[(*t)->t_index].u_ofile[NOFILE]; q++)
				{
				if(a = *q) {
					*q = NULL;
					closef(a);
					}
				}
				iput(u.u_threads[(*t)->t_index].u_cdir);
				
				//�ͷ��߳̽ṹ
				thdfree((*t)->t_tid);
		}
	}

	while(*pf & ~M_BITS)
	{
			m = msgoutqueue(u.u_procp);	
			
			//�ͷ���Ϣ����
			msgfree(m);
	}

	for(i = 0; i < MBX_COUNT_PROC; i++)
	{
		if((mbx=u.u_ombx[i]) != NULL)
		{
			pf = &mbx->flag;
			while(*pf & ~MBX_BITS)
			{
					m = mailoutmbx(mbx);	
					//�ͷ��ʼ�����
					mailfree(m);
			}
			closembx(i);
		}
	}

	for(i=0; i < MAP_MEM_COUNT_PROC; i++)
	{
		if((mm=u.u_procp->p_mmap[i]) != NULL)
			unmap(i);
	}
	for(i=0; i < MAP_MEM_COUNT_PROC; i++)
	{
		if((sm = u.uprocp->p_shm[i]) != NULL)
		{
			//�ͷ��������ڴ�
			if(sm->ref == 0)
					phyfree(sm->phyAddr, sm->blocks);
			else 
			{
				/*TODO:���԰�sm������������ʹ�øù����ڴ�Ľ��̣�����޷����������
				�ռ�����)���򽻸�init���̣��������н���unmap�ڴ���ͷ�����
				��Ϊ���ֱ���ͷŸù����ڴ棬�������޷�Ԥ֪�Ĵ���(����Ϊʲô)��

				��һ�ּ򵥵ķ����ǣ�ǿ��ж���������н����ڸù����ڴ��ϵ�ӳ�䣬
				ͨ���źš���Ϣ�ȷ�ʽ֪ͨ�����쳣������Ȼ���ͷŸù����ڴ档*/
			}
		}
	}

... ...
}

exec()	
{	
	int ap, na, nc, *bp;
	int ts, ds, sep;
	register c, *ip;
	register char *cp;
	extern uchar;
	
	/*
	pick up file names
	and check various modes
	for execute permission
	*/
	register U_Thread *uthd = &u.u_threads[u.u_curthu];
	ip = namei(&uchar, 0);	
	if(ip == NULL)
	        return; 
	while(execnt >= NEXEC)
	        sleep(&execnt, EXPRI); 
	execnt++;
	bp = getblk(NODEV);
	if(access(ip, IEXEC) || (ip->i_mode&IFMT)!=0)
	        goto bad; 
	
	/*
	pack up arguments into
	allocated disk buffer
	*/
	
	cp = bp->b_addr; //buf mem addr
	na = 0;
	nc = 0;
	while(ap = fuword(uthd->u_arg[1])) 
	        na++;
	        if(ap == -1)
	                goto bad;
	        uthd->u_arg[1] =+ 2;
	        for(;;) {
	                c = fubyte(ap++);
	                if(c == -1)
	                        goto bad;
	                *cp++ = c;
	                nc++;
	                if(nc > 510) {
	                        uthd->u_error = E2BIG;
	                        goto bad;
	                }
	                if(c == 0)
	                        break;
	                }
	        }
	        if((nc&1) != 0) { //ż����0
	        *cp++ = 0;
	        nc++;
	}
	
	/* read in first 8 bytes
	of file for segment
	sizes:
	w0 = 407/410/411 (410 -> RO text) (411 -> sep ID)
	w1 = text size
	w2 = data size
	w3 = bss size
	*/
	
	uthd->u_base = &uthd->u_arg[0];
	uthd->u_count = 8;
	uthd->u_offset[1] = 0;
	uthd->u_offset[0] = 0;
	uthd->u_segflg = 1;
	readi(ip);	//������ʼ��8���ֽڵ�uthd->u_base��
	uthd->u_segflg = 0;
	if(uthd->u_error)
	        goto bad;
	sep = 0;
	if(uthd->u_arg[0] == 0407) {
	        uthd->u_arg[2] =+ uthd->u_arg[1];
	        uthd->u_arg[1] = 0;
	} else
	if(uthd->u_arg[0] == 0411)
	        sep++; 
	else
	        if(uthd->u_arg[0] != 0410) {
	                uthd->u_error = ENOEXEC;
	                goto bad;
	        }
	
	if(uthd->u_arg[1]!=0&&(ip->i_flag&ITEXT)==0&&ip->i_count!=1){
	        uthd->u_error = ETXTBSY;
	        goto bad;
	}
	
	/*
	find text and data sizes
	try them out for possible
	exceed of max sizes
	*/
	
	ts = ((uthd->u_arg[1]+63)>>6) & 01777;		
	ds = ((uthd->u_arg[2]+uthd->u_arg[3]+63)>>6) & 0177; 
	if(estabur(ts, ds, SSIZE, sep))
	        goto bad;
	
	/*
	allocate and clear core
	at this point, committed
	to the new image
	*/
	
	uthd->u_prof[3] = 0;
	xfree();
	expand(USIZE); 
	xalloc(ip);
	c = USIZE+ds+SSIZE;
	expand(c); 
	while(--c >= USIZE)
	clearseg(u.u_procp->p_addr+c);
	
	/* read in data segment */
	
	estabur(0, ds, 0, 0);
	uthd->u_base = 0;
	uthd->u_offset[1] = 020+uthd->u_arg[1];
	uthd->u_count = uthd->u_arg[2];
	readi(ip);
	
	/*
	initialize stack segment
	*/
	
	u.u_tsize = ts;
	u.u_dsize = ds; 
	u.u_ssize = SSIZE;		//0���̣߳�main�������ڵ����̣߳�ջ��С
	u.u_sep = sep;
	estabur(u.u_tsize, u.u_dsize, u.u_ssize, u.u_sep);
	cp = bp->b_addr;
	ap = -nc - na*2 - 4;
	
	//��ʼ��ջ�����¼
	u.u_stackmap[0].m_addr = ap-u.u_ssize;
	u.u_stackmap[0].m_size = u.u_ssize;
	
	for(ds = 1; ds < NTHREAD_PROC; ds++)
		u.u_stackmap[ds].m_size = 0;
	
	uthd->u_ar0[R6] = ap;	//�������߳��û�ջ��ַ
	u.u_procp->p_threads[u.u_curthu]->t_ustackaddr = AllocStack(THREAD_SSIZE);
	u.u_procp->p_threads[u.u_curthu]->t_ustacksize = THREAD_SSIZE;
	suword(ap, na);   
	c = -nc;
	while(na--) {
	        suword(ap=+2, c);
	        do
	                subyte(c++, *cp);
	        while(*cp++);
	}
	suword(ap+2, -1);
	
	/*
	set SUID/SGID protections, if no tracing
	*/
	
	if ((u.u_procp->p_flag&STRC)==0) {
	        if(ip->i_mode&ISUID)
	                if(u.u_uid != 0) {
	                        u.u_uid = ip->i_uid;	
	                        u.u_procp->p_uid = ip->i_uid;
	                }
	        if(ip->i_mode&ISGID)
	                uthd->u_gid = ip->i_gid;
	}
	
	c = ip;
	
	//�ͷ������߳���Դ�����˵�ǰ�߳�----������Ϊ�½��̵����߳�
	for(na = 0; na < NTHREAD_PROC; na++)
	{
		if(u.u_procp->p_threads[na] != NULL && u.u_procp->p_threads[na]->t_stat != 0)
		{
			if(na != u.u_curthu)
			{
						thdfree(u.u_procp->p_threads[na]->t_tid);
						u.u_procp->p_threads[na] = NULL;
			}
			
			//����Ĵ���
			for(cp = &regloc[0]; cp < &regloc[6];)   
			    u.u_threads[na]->u_ar0[*cp++] = 0;
			for(ip = &u.u_threads[na]->u_fsav[0]; ip < &u.u_threads[na]->u_fsav[25];)
			    *ip++ = 0;
		}
	}
	u.u_procp->p_threadNum = 1;
	
	
	/* clear sigs, regs, and return */
	for(ip = &u.u_signal[0]; ip < & u.u_signal[NSIG]; ip++)
	        if((*ip & 1) == 0)
	                *ip = 0;
	uthd->u_ar0[R7] = 0; //���÷��ص��û��ռ�ĵ�ַΪ0
	ip = c;
	bad:
	iput(ip);
	brelse(bp);
	if(execnt >= NEXEC)
	        wakeup(&execnt);
	execnt--;
}
... ...