#int CreateEvent(BOOL initStatus)
#{
.globl _CreateEvent
_CreateEvent:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),r0
	sys	54
	bec	1f
	jmp	cerror
1:
	mov	(sp)+,r5
	rts	pc
#}
