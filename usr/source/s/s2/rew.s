<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/rew.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/rew.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/rew.s>
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
/ rew -- rewind dec/mag tape

	cmp	(sp)+,$2
	blt	1f
	tst	(sp)+
	mov	(sp)+,r0
	movb	(r0)+,r1
	cmp	r1,$'m
	beq	rewm
	movb	r1,tapx+8
	tstb	(r0)
	bne	error
1:
	sys	open; tapx; 0
	br	rew

rewm:
	movb	(r0)+,r1
	beq	1f
	movb	r1,mtx+7
	tstb	(r0)
	bne	error
1:
	sys	open; mtx; 0

rew:
	bes	error
	sys	read; word; 2
	bes	error
	sys	exit

error:
	mov	$1,r0
	sys	write; 0f; 2
	sys	exit
0:
	&lt;?\n&gt;

tapx:
	&lt;/dev/tap0\0&gt;
mtx:
	&lt;/dev/mt0\0&gt;
	.even

.bss
word:	.=.+2












