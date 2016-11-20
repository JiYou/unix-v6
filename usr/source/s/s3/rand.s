<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s3/rand.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s3/rand.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s3/rand.s>
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
/ random fixed point number generator

.globl	rand, srand
.globl	_rand, _srand

_srand:
	mov	2(sp),ranx
	rts	pc

srand:
	mov	r0,ranx
	rts	pc

_rand:
rand:
	mov	r1,-(sp)
	mov	ranx,r1
	mpy	$13077.,r1
	add	$6925.,r1
	mov	r1,r0
	mov	r0,ranx
	bic	$100000,r0
	mov	(sp)+,r1
	rts	pc

.data
ranx:	1











