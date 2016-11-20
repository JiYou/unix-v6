<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s3/mesg.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s3/mesg.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s3/mesg.s>
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
.globl	mesg

/ usage:
/   jsr r5,mesg
/       &lt;string ending in \0&gt;
/      .even
/   ...
/
/ string is output onto $1
/

mesg:
	mov	r0,-(sp)
	mov	r5,r0
	mov	r5,0f
1:
	tstb	(r5)+
	bne	1b
	sub	r5,r0
	com	r0
	mov	r0,0f+2
	mov	$1,r0
	sys	0; 9f
.data
9:
	sys	write; 0:..; ..
.text
	inc	r5
	bic	$1,r5
	mov	(sp)+,r0
	rts	r5












