#define      SUSPTHREAD				76
#int SuspendThread(int tid)
#{
.globl _SuspendThread
_SuspendThread:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),r0
	sys	SUSPTHREAD
	jmp	cerror
	clr r0
	mov	(sp)+,r5
	rts	pc
#}
