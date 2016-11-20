<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s4/crt0.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s4/crt0.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s4/crt0.s>
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
/ C runtime startoff

.globl	savr5
.globl	_exit

.globl	_main

start:
	setd
	mov	sp,r0
	mov	(r0),-(sp)
	tst	(r0)+
	mov	r0,2(sp)
	jsr	pc,_main
	mov	r0,(sp)
	jsr	pc,*$_exit
	sys	exit

.bss
savr5:	.=.+2











