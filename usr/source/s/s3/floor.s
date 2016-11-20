<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s3/floor.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s3/floor.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s3/floor.s>
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
.globl	_floor, _ceil
.globl	floor, ceil
one = 40200

_floor:
	mov	r5,-(sp)
	mov	sp,r5
	movf	4(r5),fr0
	jsr	pc,floor
	br	1f

_ceil:
	mov	r5,-(sp)
	mov	sp,r5
	movf	4(r5),fr0
	jsr	pc,ceil
1:
	mov	(sp)+,r5
	rts	pc

floor:
	tstf	fr0
	cfcc
	bge	1f
	modf	$one,fr0
	cfcc
	beq	2f
	subf	$one,fr1
	br	2f
1:
	modf	$one,fr0
2:
	movf	fr1,fr0
	rts	pc

ceil:
	negf	fr0
	jsr	pc,floor
	negf	fr0
	rts	pc











