#int CreateSharedMbx()
#{
.globl _CreateSharedMbx
_CreateSharedMbx:
	mov	r5,-(sp)
	mov	sp,r5
	sys	64
	bec	1f
	jmp	cerror
1:
	mov	(sp)+,r5
	rts	pc
#}
