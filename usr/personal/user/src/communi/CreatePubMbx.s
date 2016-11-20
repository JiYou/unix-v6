#int CreatePublicMbx(char *name)
#{
.globl _CreatePublicMbx
_CreatePublicMbx:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),0f
	sys	0; 9f
	bec	1f
	jmp	cerror
1:
	clr r0
	mov	(sp)+,r5
	rts	pc
9:
	sys 66; 0:	..
#}
