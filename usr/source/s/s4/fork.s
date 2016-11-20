/ C library -- fork

/ pid = fork();
/
/ pid == 0 in child process; pid == -1 means error return
/ in child, parents id is in par_uid if needed

.globl	_fork, cerror, _par_uid

_fork:
	mov	r5,-(sp)
	mov	sp,r5
	sys	fork
	br 1f		//这里是子进程
	bec	2f    //carrir == 0? 返回到这里是父进程
	
	jmp	cerror
1:
	mov	r0,_par_uid
	clr	r0
2:
	mov	(sp)+,r5
	rts	pc
.bss
_par_uid: .=.+2


