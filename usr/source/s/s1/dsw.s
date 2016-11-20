<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/dsw.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/dsw.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/dsw.s>
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
/ dsw - delete from tty

	cmp	(sp)+,$2
	blt	1f
	tst	(sp)+
	mov	(sp)+,0f
1:
	sys	stat; 0:dot; stbuf
	bes	error
	mov	stbuf+4,r0
	bic	$!60000,r0
	cmp	r0,$40000
	bne	error
	mov	0b,0f
	sys	open; 0:..; 0
	bes	error
	mov	r0,r1
1:
	clrb	buf+17.
	mov	r1,r0
	sys	read; buf; 16.
	bes	done
	tst	r0
	beq	done
	tst	buf
	beq	1b
	mov	0b,r2
	mov	$obuf,r3
2:
	movb	(r2)+,(r3)+
	bne	2b
	mov	$buf+2,r2
	dec	r3
	cmpb	-1(r3),$'/
	beq	2f
	movb	$'/,(r3)+
2:
	movb	(r2)+,(r3)+
	bne	2b
	sys	stat; obuf; stbuf
	bes	error
	bit	$60000,stbuf+4
	bne	1b
	mov	$buf+2,r2
2:
	tstb	(r2)+
	bne	2b
	movb	$' ,-(r2)
	sub	$buf+1,r2
	mov	r2,0f

2:
	mov	$1,r0
	sys	write; buf+2; 0:..
	clr	r0
	sys	read; ch; 1
	cmpb	ch,$'\n
	beq	1b
	clr	r0
	sys	read; ch1; 1
	cmpb	ch1,$'\n
	beq	3f
4:
	clr	r0
	sys	read; ch; 1
	cmpb	ch,$'\n
	beq	2b
	br	4b
3:
	cmpb	ch,$'x
	beq	done
	cmpb	ch,$'y
	bne	2b
	sys	unlink; obuf
	bes	error
	br	1b

done:
	sys	exit

error:
	mov	$1,r0
	sys	write; mes; 2
	sys	exit

dot:	&lt;.\0&gt;
mes:	&lt;?\n&gt;

	.bss
obuf:	.=.+100.
stbuf:	.=.+40.
buf:	.=.+18.
ch:	.=.+1
ch1:	.=.+1












