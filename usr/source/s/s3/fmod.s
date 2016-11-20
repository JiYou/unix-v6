<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s3/fmod.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s3/fmod.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s3/fmod.s>
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
/ C floating mod
/ fmod(a,b) returns a number n such that
/	i*b + n is a and 0 &lt;= n &lt; b

.globl	_fmod
_fmod:
	mov	r5,-(sp)
	mov	sp,r5
	movf	4(r5),fr0
	divf	12.(r5),fr0
	modf	$one,fr0
	mulf	12.(r5),fr1
	movf	4(r5),fr0
	subf	fr1,fr0
	mov	(sp)+,r5
	rts	pc
one = 40200











