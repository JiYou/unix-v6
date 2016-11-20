<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s3/sqrt.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s3/sqrt.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s3/sqrt.s>
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
ldfps = 170100^tst
stfps = 170200^tst
/
/	sqrt replaces the f.p. number in fr0 by its
/	square root.  newton's method
/
.globl	sqrt, _sqrt
/
/
_sqrt:
	mov	r5,-(sp)
	mov	sp,r5
	movf	4(r5),fr0
	jsr	pc,sqrt
	mov	(sp)+,r5
	rts	pc

sqrt:
	tstf	fr0
	cfcc
	bne	1f
	clc
	rts	pc		/sqrt(0)
1:
	bgt	1f
	clrf	fr0
	sec
	rts	pc		/ sqrt(-a)
1:
	mov	r0,-(sp)
	stfps	-(sp)
	mov	(sp),r0
	bic	$!200,r0		/ retain mode
	ldfps	r0
	movf	fr1,-(sp)
	movf	fr2,-(sp)
/
	movf	fr0,fr1
	movf	fr0,-(sp)
	asr	(sp)
	add	$20100,(sp)
	movf	(sp)+,fr0	/initial guess
	mov	$4,r0
1:
	movf	fr1,fr2
	divf	fr0,fr2
	addf	fr2,fr0
	mulf	$half,fr0	/ x = (x+a/x)/2
	sob	r0,1b
2:
	movf	(sp)+,fr2
	movf	(sp)+,fr1
	ldfps	(sp)+
	mov	(sp)+,r0
	clc
	rts	pc
/
half	= 40000











