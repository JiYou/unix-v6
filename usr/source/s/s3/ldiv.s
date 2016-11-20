<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s3/ldiv.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s3/ldiv.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s3/ldiv.s>
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
/ C library-- long divide/remainder

.globl	_ldiv, _ldivr
.globl	_lrem

_ldiv:
	mov	2(sp),r0
	mov	4(sp),r1
	div	6(sp),r0
	mov	r1,_ldivr
	rts	pc

_lrem:
	mov	2(sp),r0
	mov	4(sp),r1
	div	6(sp),r0
	mov	r1,r0
	rts	pc

.bss
_ldivr:	.=.+2











