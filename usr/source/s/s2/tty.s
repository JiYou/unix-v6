/ tty -- get tty number

.globl	ttyn

	clr	r0
	jsr	pc,ttyn
	movb	r0,nam
	mov	$1,r0
	sys	write; name; 5
	sys	exit

.data
name:
	&lt;tty&gt;
nam:
	&lt;x\n&gt;
.even











