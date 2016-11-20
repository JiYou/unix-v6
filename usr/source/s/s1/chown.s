<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/chown.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/chown.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/chown.s>
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
/ chown -- change owner


	.globl	fopen, getc, mesg

	mov	sp,r5
	mov	(r5),r4
	cmp	r4,$3
	bge	1f
	jsr	r5,mesg; &lt;chown uid f1 ...\n\0&gt;; .even
1:
	add	$4,r5
	mov	(r5),r3
	cmpb	(r3),$'0
	blt	1f
	cmpb	(r3),$'9
	bgt	1f
	jsr	r5,cvnum; geta
	br	do
1:
	mov	$uids,r0
	jsr	r5,fopen; ubuf
	bec	1f
	jsr	r5,mesg; &lt;Can't open /etc/uids\n\0&gt;; .even
	sys	exit
1:
	mov	r3,r2
2:
	jsr	r5,getc; ubuf
	bcc	3f
who:
	jsr	r5,mesg; &lt;Who?\n\0&gt;; .even
	sys	exit
3:
	cmp	r0,$':
	beq	3f
	cmpb	(r2)+,r0
	beq	2b
2:
	jsr	r5,getc; ubuf
	bcs	who
	cmp	r0,$'\n
	bne	2b
	br	1b
3:
	tstb	(r2)
	bne	2b
3:
	jsr	r5,getc; ubuf
	cmpb	r0,$':
	bne	3b
	jsr	r5,cvnum; getc
do:
	sub	$2,r4
	mov	r1,0f+2
	tst	(r5)+
1:
	mov	(r5),0f
	mov	(r5)+,9f
	sys	stat; 9:..; statb
	movb	statb+8,0f+3
	sys	chown; 0:..; 0
	bec	2f
	mov	0b,r0
	mov	r0,0f
	clr	0f+2
3:
	tstb	(r0)+
	beq	3f
	inc	0f+2
	br	3b
3:
	mov	$1,r0
	sys	write; 0:..; ..
	jsr	r5,mesg; &lt;?\n\0&gt;; .even
2:
	dec	r4
	bgt	1b
	sys	exit

cvnum:
	clr	r1
1:
	jsr	r5,*(r5); ubuf
	bcs	1f
	sub	$'0,r0
	cmp	r0,$9.
	bhi	1f
	mpy	$10.,r1
	add	r0,r1
	br	1b
1:
	tst	(r5)+
	rts	r5

geta:
	movb	(r3)+,r0
	tst	(r5)+
	rts	r5

uids:	&lt;/etc/passwd\0&gt;

	.bss
statb:	.=.+36.
ubuf:	.=.+518.











