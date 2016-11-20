<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/salloc/getwd.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/salloc/getwd.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/salloc/getwd.s>
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
.globl	getword
.globl	putword
.globl lookword
.globl alterword
.globl alterchar
.globl	putchar
.globl	lookchar
.globl	w, r, a, l
/
/	routine to put a word onto the string
/	mov	...,r1
/	mov	...,r0
/	jsr	pc,putword
putword:
	jsr	pc,putchar
	swab	r0
	jsr	pc,putchar
	swab	r0
	rts	pc
/
/
/	routine to look at a word from the string
/	mov	...,r1
/	jsr	pc,lookword
/	mov	r0,...
lookword:
	jsr	pc,lookchar
	bes	1f
	movb	r0,nchar
	inc	r(r1)
	jsr	pc,lookchar
	bes	1f
	movb	r0,nchar+1
	dec	r(r1)
	mov	nchar,r0
1:	rts	pc
/
/
/	routine to get a word from the strng
/
getword:
	jsr	pc,lookword
	bes	1f
	add	$2,r(r1)
1:	rts	pc
/
/
/	routine to alter a word in the string
/
alterword:
	jsr	pc,alterchar
	swab	r0
	jsr	pc,alterchar
	swab	r0 
	rts	pc
nchar:	.=.+2











