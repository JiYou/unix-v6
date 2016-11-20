#define      RESUMTHREAD				77

#int ResumeThread(int tid)
#{
.globl _ResumeThread
_ResumeThread:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),r0
	sys	RESUMTHREAD
	jmp	cerror
	clr r0
	mov	(sp)+,r5
	rts	pc
#}
