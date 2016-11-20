/ C library -- gtty

/ error = gtty(filep, ttyvec);

/ filep is descriptor of open tty
/ ttyvec[0, 1, 2] correspond to args of gtty

.globl	_gtty, cerror

_gtty:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),r0
	mov	6(r5),0f
	sys	0; 9f
	bec	1f
	jmp	cerror
1:
	clr	r0
	mov	(sp)+,r5
	rts	pc
.data
9:
	sys	gtty; 0:..











