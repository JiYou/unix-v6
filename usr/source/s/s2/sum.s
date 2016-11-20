<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/sum.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/sum.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/sum.s>
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
/ sum -- check sum file

	mov	(sp)+,r3
	tst	(sp)+
loop:	dec	r3
	bgt	1f
	sys	exit
1:
	clr	bcnt
	mov	(sp)+,0f
	sys	open; 0:..; 0
	bec	3f
	mov	$1,r0
	sys	write; 1f; 2f-1f
	br	loop
1:	&lt;oprd\n&gt;
2:	.even
3:	mov	r0,r1
	clr	r5
1:
	mov	r1,r0
	sys	read; buf; 512.
	bes	err
	tst	r0
	beq	print
	inc	bcnt
	mov	$buf,r2
2:
	movb	(r2)+,r4
	add	r4,r5
	adc	r5
	sob	r0,2b
	br	1b

err:
	mov	$1,r0
	sys	write; 1f; 2

print:
	jsr	pc,decml
	mov	$1,r0
	sys	write; bl; 1
	mov	bcnt,r5
	jsr	pc,decml
	mov	$1,r0
	sys	write; nl; 1
	mov	r1,r0
	sys	close
	br	loop
1:	&lt;? &gt;
nl:	&lt;\n&gt;
bl:	&lt; &gt;
	.even

decml:
	mov	r0,-(sp)
	mov	r1,-(sp)
	mov	r5,r1
	jsr	pc,1f
	mov	(sp)+,r1
	mov	(sp)+,r0
	rts	pc

1:
	clr	r0
	dvd	$10.,r0
	mov	r1,-(sp)
	mov	r0,r1
	beq	1f
	jsr	pc,1b
1:
	mov	(sp)+,ch
	add	$'0,ch
	mov	$1,r0
	sys	write; ch; 1
	rts	pc

.bss
ch:	.=.+2
bcnt:	.=.+2
buf: .=.+512.











