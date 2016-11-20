<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s3/switch.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s3/switch.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s3/switch.s>
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
/ switch -- switch on contents of r0
/
/
/ calling sequence --
/
/   jsr r5,switch; swtab
/
/ r0 is looked up in swtab and if found
/ control is returned to the corresponding place
/ if r0 is not found, the next inline instruction is 
/ executed
/
/ swtab format --
/
/ swtab:
/    val1; ret1
/   val2; ret2
/   ...
/   valn; retn
/   ..; 0
/

	.globl	switch
switch:
	mov	r1,-(sp)
	mov	(r5)+,r1
1:
	cmp	r0,(r1)+
	beq	1f
	tst	(r1)+
	bne	1b
2:
	mov	(sp)+,r1
	rts	r5
1:
	mov	(r1)+,r1
	beq	2b
	mov	r1,r5
	br	2b












