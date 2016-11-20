#int ReceiveMessage(char *msg, int len, int time)
#{
.globl _ReceiveMessage
_ReceiveMessage:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),0f
	mov	6(r5),0f+2
	mov	8(r5),0f+4
	sys	0; 9f
	bec	1f
	jmp	cerror
1:
	mov	(sp)+,r5
	rts	pc
.data
9:
	sys 59; 0:	..;..;..
#}
