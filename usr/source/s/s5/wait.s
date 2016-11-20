/ C library -- wait

/ pid = wait();
/   or,
/ pid = wait(&amp;status);
/
/ pid == -1 if error
/ status indicates fate of process, if given

.globl	_wait, _nargs, cerror

_wait:
	mov	r5,-(sp)
	mov	sp,r5
	jsr	pc,_nargs
	mov	r0,-(sp)
	sys	wait
	bec	1f
	tst	(sp)+
	jmp	cerror
1:
	tst	(sp)+
	beq	2f	//返回指令不是nargs中的指令。
	mov	r1,*4(r5)	/ status return
2:
	mov	(sp)+,r5
	rts	pc











