<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/salloc/length.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/salloc/length.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/salloc/length.s>
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
.globl	length
.globl	position
.globl	w, r, a, l
/
/
/	routine to return the length of a string
/
/	mov	...,r1
/	jsr	pc,length
/	mov	r0,...
/
length:
	mov	w(r1),r0
	sub	a(r1),r0
	rts	pc
/
/
/	routine to return the read pointer position
/
/	mov	...,r1
/	jsr	pc,position
/	mov	r0,...
/
position:
	mov	r(r1),r0
	sub	a(r1),r0
	rts	pc











