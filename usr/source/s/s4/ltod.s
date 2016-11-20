<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s4/ltod.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s4/ltod.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s4/ltod.s>
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
/ C library
/ return floating-point from long integer
/	d = ltod(l)

.globl	_ltod
_ltod:
	mov	r5,-(sp)
	mov	sp,r5
	setl
	movif	*4(r5),fr0
	seti
	mov	(sp)+,r5
	rts	pc

/ return long integer from floating
/	dtol(d, l)

.globl	_dtol
_dtol:
	mov	r5,-(sp)
	mov	sp,r5
	setl
	movf	4(r5),fr0
	movfi	fr0,*12.(r5)
	seti
	mov	(sp)+,r5
	rts	pc











