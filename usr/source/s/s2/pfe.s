<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/pfe.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/pfe.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/pfe.s>
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
/ print last floating error

stst	= 170300^tst

	stst	r1
	cmp	r1,$14
	blos	1f
	clr	r1
1:
	bic	$1,r1
	mov	mesg(r1),r1
1:
	movb	(r1)+,ch
	beq	1f
	mov	$1,r0
	sys	write; ch; 1
	br	1b
1:
	sys	exit

mesg:
	1f
	2f
	3f
	4f
	5f
	6f
	7f
	8f

1:	&lt;No error.\n\0&gt;
2:	&lt;Floating op code error\n\0&gt;
3:	&lt;Floating divide check\n\0&gt;
4:	&lt;Integer conversion error\n\0&gt;
5:	&lt;Floating overflow\n\0&gt;
6:	&lt;Floating underflow\n\0&gt;
7:	&lt;Floating undefined\n\0&gt;
8:	&lt;Floating maintennace trap\n\0&gt;

	.even

	.bss
ch:	.=.+2












