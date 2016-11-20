<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s5/mcrt0.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s5/mcrt0.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s5/mcrt0.s>
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
/ C runtime startoff including monitoring

cbufs	= 150.

.globl	_monitor
.globl	_sbrk
.globl	_main
.globl	_exit
.globl	_etext
.comm	countbase,2

.comm	savr5,2

start:
	setd
	mov	sp,r0
	mov	(r0),-(sp)
	tst	(r0)+
	mov	r0,2(sp)

	mov	$_etext,r1
	sub	$eprol,r1
	add	$7,r1
	ash	$-3,r1
	bic	$!17777,r1
	mov	$cbufs,-(sp)
	add	$3*[cbufs+1],r1
	mov	r1,-(sp)
	asl	r1
	mov	r1,-(sp)
	jsr	pc,_sbrk
	tst	(sp)+
	cmp	r0,$-1
	beq	9f
	mov	r0,-(sp)
	add	$6,r0
	mov	r0,countbase
	mov	$_etext,-(sp)
	mov	$eprol,-(sp)
	jsr	pc,_monitor
	add	$10.,sp
	jsr	pc,_main
	cmp	(sp)+,(sp)+
	jsr	pc,_exit

9:
	mov	$2,r0
	sys	write; 8f; 9f-8f

.data; 8: &lt;No space for monitor buffer\n&gt;; 9:.even; .text

_exit:
	mov	r5,-(sp)
	mov	sp,r5
	clr	-(sp)
	jsr	pc,_monitor
	tst	(sp)+
	mov	4(r5),r0
	sys	exit
eprol:











