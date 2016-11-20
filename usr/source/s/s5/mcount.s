<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s5/mcount.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s5/mcount.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s5/mcount.s>
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
/ count subroutine calls during profiling

.globl	mcount
.comm	countbase,2

mcount:
	mov	(r0),r1
	bne	1f
	mov	countbase,r1
	beq	2f
	add	$6,countbase
	mov	(sp),(r1)+
	mov	r1,(r0)
1:
	inc	2(r1)
	bne	2f
	inc	(r1)
2:
	rts	pc












