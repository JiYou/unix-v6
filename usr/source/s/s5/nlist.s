<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s5/nlist.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s5/nlist.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s5/nlist.s>
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
/ nlist -- extract values from name list
/

/	nlist(file, list);
/
.globl	_nlist

_nlist:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),0f
	mov	6(r5),r1
	mov	r2,-(sp)

	mov	r1,r0
1:
	tst	(r0)
	beq	1f
	add	$8.,r0
	clr	(r0)+
	clr	(r0)+			/ initialize to undefined
	br	1b
1:
	sys	0; 9f
.data
9:
	sys	open; 0:..; 0
.text
	bes	done
	mov	r0,r2
	sys	read; buf; 20
	cmp	r0,$20
	bne	cdone
	mov	buf+4,count			/ assume older a.out
	mov	buf+2,0f
	cmp	buf,$405
	beq	1f
	mov	buf+8.,count			/ now assume newer
	mov	buf+2,r0			/ txt
	add	buf+4,r0			/ data
	cmp	buf+16,$1			/ relocation?
	beq	4f
	asl	r0				/ txt+data reloc
4:
	add	$20,r0				/ header
	mov	r0,0f
	cmp	buf,$411
	beq	1f
	cmp	buf,$410
	beq	1f
	cmp	buf,$407
	bne	cdone
1:
	mov	r2,r0
	sys	0; 9f
.data
9:
	sys	seek; 0:..; 0
.text

1:
	sub	$12.,count
	blt	cdone
	mov	r2,r0
	sys	read; buf; 12.
	cmp	r0,$12.
	bne	cdone
	mov	r1,r0

2:
	tst	(r0)
	beq	1b
	cmp	(r0),buf
	bne	1f
	cmp	2(r0),buf+2
	bne	1f
	cmp	4(r0),buf+4
	bne	1f
	cmp	6(r0),buf+6
	bne	1f
	mov	buf+8.,8.(r0)
	mov	buf+10.,10.(r0)
1:
	add	$12.,r0
	br	2b

cdone:
	mov	r2,r0
	sys	close

done:
	mov	(sp)+,r2
	mov	(sp)+,r5
	rts	pc

.bss
buf:	.=.+20
count:	.=.+2











