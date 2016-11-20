#define      EXITTHD				74
#int ExitThread(int exitCode)
#{
.globl _ExitThread
_ExitThread:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),r0
	sys	EXITTHD
	jmp	cerror
	mov	(sp)+,r5
	rts	pc
#}
