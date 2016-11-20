<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s3/put.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s3/put.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s3/put.s>
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
/ putw/putc -- write words/characters on output file
/
/ fcreat -- create an output file for use by put(w|c)
/
/ calling sequences --
/
/   mov $filename,r0
/   jsr r5,fcreat; ioptr
/
/ on return ioptr is set up for use by put or error
/ bit is set if file could not be created.
/
/   mov(b) thing,r0
/   jsr r5,put(w|c)1; ioptr
/
/ the character or word is written out.
/
/   jsr r5,flush; ioptr
/
/ the buffer is fled.
/

	.globl	putc, putw, flush, fcreat

fcreat:
	mov	r1,-(sp)
	mov	(r5)+,r1
	mov	r0,0f
	sys	0; 9f
.data
9:
	sys	creat; 0:..; 666
.text
	bes	1f
	mov	r0,(r1)+
2:
	clr	(r1)+
	clr	(r1)+
	mov	(sp)+,r1
	rts	r5
1:
	mov	$-1,(r1)+
	mov	(sp)+,r1
	sec
	rts	r5

.data
putw:
	mov	(r5),8f
	mov	(r5)+,9f
	mov	r0,-(sp)
	jsr	r5,putc; 8:..
	mov	(sp)+,r0
	swab	r0
	jsr	r5,putc; 9:..
	rts	r5
.text

putc:
	mov	r1,-(sp)
	mov	(r5)+,r1
1:
	dec	2(r1)
	bge	1f
	mov	r0,-(sp)
	jsr	pc,fl
	mov	(sp)+,r0
	br	1b
1:
	movb	r0,*4(r1)
	inc	4(r1)
	mov	(sp)+,r1
	rts	r5

flush:
	mov	r0,-(sp)
	mov	r1,-(sp)
	mov	(r5)+,r1
	jsr	pc,fl
	mov	(sp)+,r1
	mov	(sp)+,r0
	rts	r5

fl:
	mov	r1,r0
	add	$6,r0
	mov	r0,-(sp)
	mov	r0,0f
	mov	4(r1),0f+2
	beq	1f
	sub	(sp),0f+2
	mov	(r1),r0
	sys	0; 9f
.data
9:
	sys	write; 0:..; ..
.text
1:
	mov	(sp)+,4(r1)
	mov	$512.,2(r1)
	rts	pc












