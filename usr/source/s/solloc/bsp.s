<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/salloc/bsp.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/salloc/bsp.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/salloc/bsp.s>
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
.globl	backspace
.globl	lookchar
.globl	w, r, a, l
/
/	routine to read a string backwards
/	the read pointer is decremented before reading
/
/	mov	...,r1
/	jsr	pc,backspace
/	mov	r0,...
/
backspace:
	cmp	a(r1),r(r1)
	bhis	nochc
	dec	r(r1)
	jsr	pc,lookchar
	rts	pc
nochc:	clr	r0
	sec
	rts	pc











