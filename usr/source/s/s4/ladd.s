<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s4/ladd.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s4/ladd.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s4/ladd.s>
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
/ C routine-- long integer subtract and add

/ ladd/lsub (a, b, c);
/	int a[2], b[2], c[2];
/	a = b +- c;

.globl	_lsub
.globl	_ladd
.globl	csv
.globl	cret

_lsub:
	jsr	r5,csv
	mov	6(r5),r2
	mov	(r2)+,r0
	mov	(r2),r1
	mov	10(r5),r2
	sub	(r2)+,r0
	sub	(r2),r1
	sbc	r0
	mov	4(r5),r2
	mov	r0,(r2)+
	mov	r1,(r2)
	jmp	cret

_ladd:
	jsr	r5,csv
	mov	6(r5),r2
	mov	(r2)+,r0
	mov	(r2),r1
	mov	10(r5),r2
	add	(r2)+,r0
	add	(r2),r1
	adc	r0
	mov	4(r5),r2
	mov	r0,(r2)+
	mov	r1,(r2)
	jmp	cret











