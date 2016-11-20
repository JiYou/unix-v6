#int CreateSemphore(int initNum)
#{
.globl _CreateSemphore
_CreateSemphore:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),r0
	sys	50
	bec	1f
	jmp	cerror
1:
	mov	(sp)+,r5
	rts	pc
#}
