<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/fed1.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/fed1.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/fed1.s>
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
signal = 48.
testing = 0
/
/	command interpreter for form letter editor
/
/
/
command:
	sys	signal; 2; inter
	jsr	pc,initl
	mov	sp,spi
loop:
	mov	spi,sp
	mov	$buffer,r2
	clr	r4
	clr	iflag
1:
	jsr	pc,tfiget
	cmpb	$' ,r0
	beq	1f
	cmpb	$'\n,r0
	bne	2f
	clrb	(r2)
	br	4f
2:
	movb	r0,(r2)+
	br	1b
1:
	clrb	(r2)
	mov	$argc,r3
3:
	mov	$arg,r2
2:
	jsr	pc,tfiget
	cmpb	$' ,r0
	beq	1f
	cmpb	$'\n,r0
	beq	1f
	movb	r0,(r2)+
	br	2b
1:
	clrb	(r2)+
	inc	r4
1:
	movb	-(r2),sv
	cmp	r2,$arg
	blos	1f
	movb	-(r2),-(sp)
	movb	sv,1(sp)
	cmp	r2,$arg
	bhi	1b
	mov	sp,(r3)+
	br	2f
1:
	clrb	-(sp)
	movb	sv,1(sp)
	mov	sp,(r3)
	add	$1,(r3)+
2:
	cmpb	r0,$'\n
	bne	3b
1:
	mov	-(r3),-(sp)
	cmp	r3,$argc
	bhi	1b
4:
	mov	r4,-(sp)
3:
	clr	r0
	mov	$tabl,r4
2:
	mov	(r4)+,r1
	cmp	r4,$tend
	bhi	error
	add	$2,r0
	mov	$buffer,r2
1:
	cmpb	(r1)+,(r2)+
	bne	2b
	tstb	(r1)
	bne	1b
	tstb	(r2)
	bne	2b
	sub	$2,r0
	add	$jtable,r0
	clr	vflag
	clr	qflag
	jmp	*(r0)
/
error:
	mov	$1,r0
	sys	write; err; 2
	br	loop
/
/
tabl: c1; c2; c3; c4; c5; c6; c7; c8; 
tend:	0
c1:	&lt;n\0&gt;
c2:	&lt;p\0&gt;
c3:	&lt;e\0&gt;
c4:	&lt;d\0&gt;
c5:	&lt;m\0&gt;
c6:	&lt;fin\0&gt;
c7:	&lt;q\0&gt;
c8:	&lt;c\0&gt;
err:	&lt;?\n&gt;
endt:	.even
jtable: list; listf; ed; remove; rename;  fin; q; memck;
spi:	.=.+2
sv:	.=.+2











