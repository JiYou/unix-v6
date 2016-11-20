<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s4/fltpr.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s4/fltpr.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s4/fltpr.s>
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
/ C library-- floating output

.globl	pfloat
.globl	pscien
.globl	fltused

.globl	_ndigit
.globl	ecvt
.globl	fcvt

fltused:		/ force loading
pfloat:
	mov	r0,_ndigit
	tst	r2
	bne	1f
	mov	$6,_ndigit
1:
	movf	(r4)+,fr0
	jsr	pc,fcvt
	tst	r1
	beq	1f
	movb	$'-,(r3)+
1:
	tst	r2
	bgt	1f
	movb	$'0,(r3)+
1:
	mov	r2,r1
	ble	1f
2:
	movb	(r0)+,(r3)+
	sob	r1,2b
1:
	mov	_ndigit,r1
	beq	1f
	movb	$'.,(r3)+
1:
	neg	r2
	ble	1f
2:
	dec	r1
	blt	1f
	movb	$'0,(r3)+
	sob	r2,2b
1:
	tst	r1
	ble	2f
1:
	movb	(r0)+,(r3)+
	sob	r1,1b
2:
	rts	pc

pscien:
	mov	r0,_ndigit
	tst	r2
	bne	1f
	mov	$6,_ndigit
1:
	movf	(r4)+,fr0
	jsr	pc,ecvt
	tst	r1
	beq	1f
	movb	$'-,(r3)+
1:
	movb	(r0)+,(r3)+
	movb	$'.,(r3)+
	mov	_ndigit,r1
	dec	r1
	ble	1f
2:
	movb	(r0)+,(r3)+
	sob	r1,2b
1:
	movb	$'e,(r3)+
	dec	r2
	mov	r2,r1
	bge	1f
	movb	$'-,(r3)+
	neg	r1
	br	2f
1:
	movb	$'+,(r3)+
2:
	clr	r0
	div	$10.,r0
	add	$'0,r0
	movb	r0,(r3)+
	add	$'0,r1
	movb	r1,(r3)+
	rts	pc











