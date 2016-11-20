<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s2/write.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s2/write.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s2/write.s>
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
/ write -- write to another user

.globl	ttyn

	cmp	(sp)+,$2
	beq	1f
	bgt	2f
	mov	$1,r0
	sys	write; argm; eargm-argm
	sys	exit
2:
	movb	*4(sp),ltty
1:
	tst	(sp)+
	mov	(sp)+,r5
	sys	open; utmp; 0
	bec	1f
	mov	$1,r0
	sys	write; film; efilm-film
	sys	exit
1:
	mov	r0,ufil
1:
	mov	ufil,r0
	sys	read; ubuf; 16.
	tst	r0
	bne	2f
	jmp	8f
2:
	tstb	ltty
	beq	2f
	cmpb	ltty,8.+ubuf
	bne	1b
2:
	mov	$ubuf,r3
	mov	r5,r4
	mov	$9.,r2
2:
	dec	r2
	beq	2f
	cmpb	(r4)+,(r3)+
	beq	2b
	tstb	-1(r4)
	bne	1b
	cmpb	$' ,-1(r3)
	bne	1b
2:
	movb	8.+ubuf,ttyno
	sys	open; ttyx; 1
	bes	3f
	sys	stat; ttyx; statbuf
	bes	3f
	bit	$2,statbuf+4
	bne	2f
3:
	mov	$1,r0
	sys	write; dnymes; ednymes-dnymes
	sys	exit
2:
	mov	r0,ttyf
	clr	r0
	jsr	pc,ttyn
	mov	r0,r3
	mov	statbuf,r4
	mov	ufil,r0
	sys	seek; 0; 0
1:
	mov	ufil,r0
	sys	read; ubuf; 16.
	tst	r0
	beq	unknown
	cmp	r3,ubuf+8.
	bne	1b
	mov	$ubuf,r0
	mov	$8.,r1
1:
	cmpb	$' ,(r0)+
	beq	1f
	dec	r1
	bne	1b
1:
	neg	r1
	add	$8,r1
	mov	r1,0f
6:
	mov	ttyf,r0
	sys	write; mesg; emesg-mesg
	mov	ttyf,r0
	sys	0; 5f
.data
5:
	sys	write; ubuf; 0:2
.text
	mov	ttyf,r0
	sys	write; qnl; 4
	sys	signal; 2; 9f
7:
	clr	r0
	sys	read; ch; 1
	tst	r0
	beq	9f
	tst	nlflg
	beq	1f
	cmp	ch,$'!
	bne	1f
	sys	fork
		br mshproc
	sys	wait
	mov	$1,r0
	sys	write; excl; 2
	br	7b
1:
	clr	nlflg
	cmp	ch,$'\n
	bne	1f
	inc	nlflg
1:
	mov	ttyf,r0
	sys	write; ch; 1
	br	7b
8:
	movb	(r5)+,ch
	beq	8f
	mov	$1,r0
	sys	write; ch; 1
	br	8b
8:
	tstb	ltty
	beq	8f
	mov	$1,r0
	sys	write; ltty-1; 2
8:
	mov	$1,r0
	sys	write; errmsg; eerrmsg-errmsg
	sys	exit
9:
	mov	ttyf,r0
	sys	write; endmsg; eendmsg-endmsg
	sys	exit

unknown:
	mov	$"??,ubuf
	br	6b

mshproc:
	sys	exec; msh; mshp
	sys	exit

.data
nlflg:
	1
.text

mshp:
	msh
	minust
	0
msh:
	&lt;/bin/sh\0&gt;
minust:
	&lt;-t\0&gt;
argm:
	&lt;Arg count\n&gt;
eargm:
film:
	&lt;Cannot open utmp\n&gt;
efilm:
.data
	&lt; &gt;		/ is ltty -1
ltty:
	.=.+1
.text
excl:
	&lt;!\n&gt;
qnl:
	&lt;...\n&gt;
.data
ttyx:
	&lt;/dev/ttyx\0&gt;
ttyno	= .-2
.text
utmp:
	&lt;/etc/utmp\0&gt;
endmsg:
	&lt;EOT\n&gt;
eendmsg:
errmsg:
	&lt; not logged in.\n&gt;
eerrmsg:
mesg:
	&lt;\nMessage from &gt;
emesg:
dnymes:
	&lt;Permission denied.\n&gt;
ednymes:
	.even
	.bss

ttyf:	.=.+2
ubuf:	.=.+16.
statbuf:.=.+40.
ch:	.=.+2
ufil:	.=.+2
signal = 48.











