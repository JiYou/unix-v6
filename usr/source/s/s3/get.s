<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s3/get.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s3/get.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s3/get.s>
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
/ getw/getc -- get words/characters from input file
/ fopen -- open a file for use by get(c|w)
/
/ calling sequences --
/
/   mov $filename,r0
/   jsr r5,fopen; ioptr
/
/  on return ioptr buffer is set up or error bit is set if
/  file could not be opened.
/
/   jsr r5,get(c|w)1; ioptr
/
/  on return char/word is in r0; error bit is
/  set on error or end of file.
/
/  ioptr is the address of a 518-byte buffer
/  whose layout is as follows:
/
/  ioptr: .=.+2    / file descriptor
/         .=.+2    / charact+2    / pointer to next character (reset if no. chars=0)
/         .=.+512. / the buffer

	.globl	getc,getw,fopen

fopen:
	mov	r1,-(sp)
	mov	(r5)+,r1
	mov	r0,0f
	sys	0; 9f
.data
9:
	sys	open; 0:..; 0
.text
	bes	1f
	mov	r0,(r1)+
	clr	(r1)+
	mov	(sp)+,r1
	rts	r5
1:
	mov	$-1,(r1)
	mov	(sp)+,r1
	sec
	rts	r5

.data
getw:
	mov	(r5),9f
	mov	(r5)+,8f
	jsr	r5,getc; 8:..
	bec	1f
	rts	r5
1:
	mov	r0,-(sp)
	jsr	r5,getc; 9:..
	swab	r0
	bis	(sp)+,r0
	rts	r5
.text

getc:
	mov	r1,-(sp)
	mov	(r5)+,r1
	dec	2(r1)
	bge	1f
	mov	r1,r0
	add	$6,r0
	mov	r0,0f
	mov	r0,4(r1)
	mov	(r1),r0
	sys	0; 9f
.data
9:
	sys	read; 0:..; 512.
.text
	bes	2f
	tst	r0
	bne	3f
2:
	mov	(sp)+,r1
	sec
	rts	r5
3:
	dec	r0
	mov	r0,2(r1)
1:
	clr	r0
	bisb	*4(r1),r0
	inc	4(r1)
	mov	(sp)+,r1
	rts	r5












