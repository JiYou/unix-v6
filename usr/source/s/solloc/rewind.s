<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/salloc/rewind.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/salloc/rewind.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/salloc/rewind.s>
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
.globl	rewind
.globl	create
.globl	fsfile
.globl	w, r, a, l
/
/
/	routine to rewind read pointer of string
/	pointed to by r1
/
/	mov	...,r1
/	jsr	pc,rewind
/
rewind:
	mov	a(r1),r(r1)
	rts	pc
/
/	routine to rewind write pointer of string
/	pointed to by r1
/
/	mov	...,r1
/	jsr	pc,create
/
create:
	mov	a(r1),w(r1)
	mov	a(r1),r(r1)
	rts	pc
/
/
/	routine to copy read pointer of string to end of string
/
/	mov	...,r1
/	jsr	pc,fsfile
/
fsfile:
	mov	w(r1),r(r1)
	rts	pc











