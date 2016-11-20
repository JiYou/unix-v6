<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s4/locv.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s4/locv.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s4/locv.s>
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
/ C library -- long output conversion

.globl	_locv

_locv:
	mov	r5,-(sp)
	mov	sp,r5
	mov	r4,-(sp)
	mov	r3,-(sp)
	mov	r2,-(sp)
	mov	$buf,r4
	mov	6(r5),r3
	mov	4(r5),r2
	bpl	1f
	neg	r2
	neg	r3
	sbc	r2
	movb	$'-,(r4)+
1:
	jsr	pc,1f
	clrb	(r4)+
	mov	$buf,r0
	mov	(sp)+,r2
	mov	(sp)+,r3
	mov	(sp)+,r4
	mov	(sp)+,r5
	rts	pc

1:
	jsr	pc,divid
	add	$'0,r1
	mov	r1,-(sp)
	ashc	$0,r2
	beq	1f
	jsr	pc,1b
1:
	movb	(sp)+,(r4)+
	rts	pc

divid:
	clr	r1
	mov	$32.,r0
1:
	ashc	$1,r2
	rol	r1
	cmp	r1,$10.
	blo	2f
	sub	$10.,r1
	inc	r3
2:
	sob	r0,1b
	rts	pc


.bss
buf:	.=.+12.











