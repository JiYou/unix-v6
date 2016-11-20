<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/salloc/zero.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/salloc/zero.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/salloc/zero.s>
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
.globl	zero
.globl	putchar
.globl	w, r, a, l
/
/
/	routine to zero a string
/
/	mov	...,r1
/	jsr	pc,zero
/
zero:
	mov	r0,-(sp)
	mov	a(r1),w(r1)
	clrb	r0
1:	cmp	w(r1),l(r1)
	bhis	1f
	jsr	pc,putchar
	br	1b
1:	mov	a(r1),w(r1)
	mov	(sp)+,r0
	rts	pc











