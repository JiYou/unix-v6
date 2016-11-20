<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s3/dpadd.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s3/dpadd.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s3/dpadd.s>
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
/ C library -- signed dp add

/ dpadd(l, i)
/	int l[2];
/ l =+ i;

.globl	_dpadd

_dpadd:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),r0
	tst	6(r5)
	sxt	r1
	add	r1,(r0)+
	add	6(r5),(r0)
	adc	-(r0)
	mov	(sp)+,r5
	rts	pc











