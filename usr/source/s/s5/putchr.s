<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s5/putchr.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s5/putchr.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s5/putchr.s>
Find at most <select name="num">
<option value="5">5</option>
<option value="10">10</option>
<option value="20">20</option>
<option value="50">50</option>
<option value="100">100</option>
<option value="500">500</option></select>
related files. <input type="submit" value="Search"><br>
<input type="checkbox" name="incl" value="1">
including files from this version of Unix.
</form><hr>
<pre>
/ C library -- putchar

	.globl	_putchar,_flush
	.globl	_fout

_putchar:
	mov	r5,-(sp)
	mov	sp,r5
	mov	_fout+4,r0
	bne	1f
	jsr	pc,fl
	mov	_fout+4,r0
1:
	movb	4(r5),(r0)+
	beq	1f
	inc	_fout+4
	dec	_fout+2
	bgt	1f
	jsr	pc,fl
1:
	mov	4(r5),r0
	mov	(sp)+,r5
	rts	pc

_flush:
	mov	r5,-(sp)
	mov	sp,r5
	jsr	pc,fl
	mov	(sp)+,r5
	rts	pc

fl:
	mov	_fout+4,r0
	beq	1f
	sub	$_fout+6,r0
	mov	r0,0f
	mov	_fout,r0
	bne	2f
	inc	r0
2:
	sys	0; 9f
.data
9:	sys	write; _fout+6; 0:..
.text
1:
	mov	$_fout+6,_fout+4
	mov	$512.,_fout+2
	cmp	_fout,$2
	bhi	1f
	mov	$1,_fout+2
1:
	rts	pc

.bss
_fout:	.=.+518.











