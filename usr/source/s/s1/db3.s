<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML><HEAD><TITLE>V6/usr/source/s1/db3.s</TITLE></HEAD>
<BODY BGCOLOR="#ffffff"><H1>V6/usr/source/s1/db3.s</H1>
<form action="/cgi-bin/unixtree.cgi" method="POST">
<input type=hidden name=f value=V6/usr/source/s1/db3.s>
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
/ db3 -- debugger

psym:
	mov	$2,incdot
	jsr	pc,prints
	jsr	pc,pnl
	rts	pc

prints:
	mov	r0,-(sp)
	mov	$optab,r1
1:
	mov	(sp),r0
	bic	(r1)+,r0
	cmp	(r1)+,r0
	bne	2f
	mov	(r1)+,-(sp)
	jsr	pc,string
	jmp	*(sp)+
2:
	add	$8.,r1
	br	1b

optab:
	107777; 010000; double; &lt;mov\0\0\0&gt;
	107777; 020000; double; &lt;cmp\0\0\0&gt;
	107777; 030000; double; &lt;bit\0\0\0&gt;
	107777; 040000; double; &lt;bic\0\0\0&gt;
	107777; 050000; double; &lt;bis\0\0\0&gt;
	007777; 060000; double; &lt;add\0\0\0&gt;
	007777; 160000; double; &lt;su\0\0\0\0&gt;

	100077; 005000; single; &lt;clr\0\0\0&gt;
	100077; 005100; single; &lt;com\0\0\0&gt;
	100077; 005200; single; &lt;inc\0\0\0&gt;
	100077; 005300; single; &lt;dec\0\0\0&gt;
	100077; 005400; single; &lt;neg\0\0\0&gt;
	100077; 005500; single; &lt;adc\0\0\0&gt;
	100077; 005600; single; &lt;sbc\0\0\0&gt;
	100077; 005700; single; &lt;tst\0\0\0&gt;
	100077; 006000; single; &lt;ror\0\0\0&gt;
	100077; 006100; single; &lt;rol\0\0\0&gt;
	100077; 006200; single; &lt;asr\0\0\0&gt;
	100077; 006300; single; &lt;asl\0\0\0&gt;
	000077; 000100; single; &lt;jmp\0\0\0&gt;
	000077; 000300; single; &lt;swab\0\0&gt;
	000077; 170100; singlw; &lt;ldfps\0&gt;
	000077; 170200; singlw; &lt;stfps\0&gt;
	000077; 170300; singlw; &lt;stst\0\0&gt;
	000077; 170400; singlw; &lt;clrf\0\0&gt;
	000077; 170500; singlw; &lt;tstf\0\0&gt;
	000077; 170600; singlw; &lt;absf\0\0&gt;
	000077; 170700; singlw; &lt;negf\0\0&gt;
	000077; 006700; singlw; &lt;sxt\0\0\0&gt;
	000077; 006600; singlw; &lt;mtpi\0\0&gt;
	000077; 106600; singlw; &lt;mtpd\0\0&gt;
	000077; 006500; singlw; &lt;mfpi\0\0&gt;
	000077; 106500; singlw; &lt;mfpd\0\0&gt;
	000777; 070000; muldiv; &lt;mul\0\0\0&gt;
	000777; 071000; muldiv; &lt;div\0\0\0&gt;
	000777; 072000; muldiv; &lt;ash\0\0\0&gt;
	000777; 073000; muldiv; &lt;ashc\0\0&gt;

	000377; 000400; branch; &lt;br\0\0\0\0&gt;
	000377; 001000; branch; &lt;bne\0\0\0&gt;
	000377; 001400; branch; &lt;beq\0\0\0&gt;
	000377; 002000; branch; &lt;bge\0\0\0&gt;
	000377; 002400; branch; &lt;blt\0\0\0&gt;
	000377; 003000; branch; &lt;bgt\0\0\0&gt;
	000377; 003400; branch; &lt;ble\0\0\0&gt;
	000377; 100000; branch; &lt;bpl\0\0\0&gt;
	000377; 100400; branch; &lt;bmi\0\0\0&gt;
	000377; 101000; branch; &lt;bhi\0\0\0&gt;
	000377; 101400; branch; &lt;blos\0\0&gt;
	000377; 102000; branch; &lt;bvc\0\0\0&gt;
	000377; 102400; branch; &lt;bvs\0\0\0&gt;
	000377; 103000; branch; &lt;bhis\0\0&gt;
	000377; 103400; branch; &lt;blo\0\0\0&gt;

	000000; 000000; noaddr; &lt;halt\0\0&gt;
	000000; 000001; noaddr; &lt;wait\0\0&gt;
	000000; 000002; noaddr; &lt;rti\0\0\0&gt;
	000000; 000004; noaddr; &lt;iot\0\0\0&gt;
	000000; 000005; noaddr; &lt;reset\0&gt;

	000377; 171000; fltrev; &lt;mulf\0\0&gt;
	000377; 171400; fltrev; &lt;modf\0\0&gt;
	000377; 172000; fltrev; &lt;addf\0\0&gt;
	000377; 172400; fltrev; &lt;movf\0\0&gt;
	000377; 173000; fltrev; &lt;subf\0\0&gt;
	000377; 173400; fltrev; &lt;cmpf\0\0&gt;
	000377; 174000; fltnor; &lt;movf\0\0&gt;
	000377; 174400; fltrev; &lt;divf\0\0&gt;
	000377; 175000; fltnor; &lt;movei\0&gt;
	000377; 175400; fltnor; &lt;movfi\0&gt;
	000377; 176000; fltnor; &lt;movfo\0&gt;
	000377; 176400; fltrev; &lt;movie\0&gt;
	000377; 177000; fltrev; &lt;movif\0&gt;
	000377; 177400; fltrev; &lt;movof\0&gt;
	000000; 170000; noaddr; &lt;cfcc\0\0&gt;
	000000; 170001; noaddr; &lt;setf\0\0&gt;
	000000; 170002; noaddr; &lt;seti\0\0&gt;
	000000; 170011; noaddr; &lt;setd\0\0&gt;
	000000; 170012; noaddr; &lt;setl\0\0&gt;

	000777; 004000; specl1; &lt;jsr\0\0\0&gt;
	000777; 074000; specl1; &lt;xor\0\0\0&gt;
	000007; 000200; specl2; &lt;rts\0\0\0&gt;
	000017; 000240; specl3; &lt;cflg\0\0&gt;
	000017; 000260; specl3; &lt;sflg\0\0&gt;
	000377; 104000; specl4; &lt;emt\0\0\0&gt;
	000377; 104400; specl5; &lt;sys\0\0\0&gt;
	000077; 006400; specl7; &lt;mark\0\0&gt;
	000777; 077000; specl8; &lt;sob\0\0\0&gt;
	000007; 000230; specl9; &lt;spl\0\0\0&gt;
	177777; 000000; specl6; &lt;oct\0\0\0&gt;

fltrev:
	bic	$!377,(sp)
muldiv:
	jsr	pc,psp
	mov	(sp)+,r0
	asl	r0
	asl	r0
	swab	r0
	aslb	r0
	aslb	r0
	aslb	r0
	aslb	r0
	asr	r0
	asr	r0
	asr	r0
	asr	r0
	bic	$!7707,r0
	mov	r0,-(sp)
	br	fltnor1

fltnor:
	bic	$!377,(sp)
	br	1f

double:
	tst	(sp)
	bge	1f
	jsr	pc,pb
1:
	jsr	pc,psp
	mov	(sp),r0
fltnor1:
	als	$-6,r0
	jsr	r5,decodadr; dot
	add	r0,incdot
	add	dot,r0
	mov	r0,temp
	jsr	pc,pcom
	mov	(sp)+,r0
	jsr	r5,decodadr; temp
	add	r0,incdot
	rts	pc

single:
	tst	(sp)
	bge	singlw
	jsr	pc,pb
singlw:
	jsr	pc,psp
	mov	(sp)+,r0
	jsr	r5,decodadr; dot
	add	r0,incdot
	rts	pc

specl8: / sob
	jsr	pc,psp
	mov	(sp),r3
	als	$-6,r3
	jsr	pc,preg
	jsr	pc,pcom
	mov	(sp)+,r0
	bic	$!77,r0
	neg	r0
	br	1f

branch:
	jsr	pc,psp
	mov	(sp)+,r0
	bic	$!377,r0
	bit	$200,r0
	beq	1f
	bis	$177400,r0
1:
	inc	r0
	asl	r0
	add	dot,r0
	jsr	pc,pname
	rts	pc

noaddr:
	tst	(sp)+
	rts	pc

specl1: / jsr
	jsr	pc,psp
	mov	(sp),r3
	als	$-6,r3
	jsr	pc,preg
	jsr	pc,pcom
	mov	(sp)+,r0
	jsr	r5,decodadr; dot
	add	r0,incdot
	rts	pc

specl2: / rts
	jsr	pc,psp
	mov	(sp)+,r3
	jsr	pc,preg
	rts	pc

specl3: / opr
	jsr	pc,psp
	mov	(sp)+,r2
	mov	$3f,r1
1:
	bit	$17,r2
	beq	1f
	bit	$10,r2
	beq	2f
	movb	(r1),r0
	jsr	pc,putc
2:
	inc	r1
	asl	r2
	br	1b
1:
	rts	pc
3:
	&lt;nzvc&gt;

specl7: / mark
	bic	$!77,(sp)
	br	specl4

specl9: / spl
	bic	$!7,(sp)
	br	specl4

specl4: / emt
	jsr	pc,psp
	mov	(sp)+,r0
	bic	$!377,r0
8:
	jsr	pc,printo
	rts	pc

specl5: / sys
	jsr	pc,psp
	mov	(sp)+,r2
	bic	$!377,r2
	mov	r2,r0
	asl	r2
	asl	r2
	cmp	r2,$esystab-systab
	bhis	8b
	mov	systab(r2),r1
	jsr	pc,string
	mov	systab+2(r2),-(sp)
	mov	dot,temp1
1:
	dec	(sp)
	bge	2f
	tst	(sp)+
	rts	pc
2:
	jsr	r5,mesg; &lt;;\0&gt;
	add	$2,temp1
	add	$2,incdot
	jsr	r5,get; temp1
	jsr	pc,pname
	br	1b

systab:
	1f; 0; .data; 1:&lt;rele\0&gt;; .text
	1f; 0; .data; 1:&lt;exit\0&gt;; .text
	1f; 0; .data; 1:&lt;fork\0&gt;; .text
	1f; 2; .data; 1:&lt;read\0&gt;; .text
	1f; 2; .data; 1:&lt;write\0&gt;; .text
	1f; 2; .data; 1:&lt;open\0&gt;; .text
	1f; 0; .data; 1:&lt;close\0&gt;; .text
	1f; 0; .data; 1:&lt;wait\0&gt;; .text
	1f; 2; .data; 1:&lt;creat\0&gt;; .text
	1f; 2; .data; 1:&lt;link\0&gt;; .text
	1f; 1; .data; 1:&lt;unlink\0&gt;; .text
	1f; 2; .data; 1:&lt;exec\0&gt;; .text
	1f; 1; .data; 1:&lt;chdir\0&gt;; .text
	1f; 0; .data; 1:&lt;time\0&gt;; .text
	1f; 1; .data; 1:&lt;makdir\0&gt;; .text
	1f; 2; .data; 1:&lt;chmod\0&gt;; .text
	1f; 2; .data; 1:&lt;chown\0&gt;; .text
	1f; 1; .data; 1:&lt;break\0&gt;; .text
	1f; 2; .data; 1:&lt;stat\0&gt;; .text
	1f; 2; .data; 1:&lt;seek\0&gt;; .text
	1f; 2; .data; 1:&lt;tell\0&gt;; .text
	1f; 2; .data; 1:&lt;mount\0&gt;; .text
	1f; 1; .data; 1:&lt;umount\0&gt;; .text
	1f; 0; .data; 1:&lt;setuid\0&gt;; .text
	1f; 0; .data; 1:&lt;getuid\0&gt;; .text
	1f; 0; .data; 1:&lt;stime\0&gt;; .text
	1f; 1; .data; 1:&lt;quit\0&gt;; .text
	1f; 1; .data; 1:&lt;intr\0&gt;; .text
	1f; 1; .data; 1:&lt;fstat\0&gt;; .text
	1f; 1; .data; 1:&lt;emt\0&gt;; .text
	1f; 1; .data; 1:&lt;smdate\0&gt;; .text
	1f; 1; .data; 1:&lt;stty\0&gt;; .text
	1f; 1; .data; 1:&lt;gtty\0&gt;; .text
	1f; 1; .data; 1:&lt;ilgins\0&gt;; .text
	1f; 0; .data; 1:&lt;hog\0&gt;; .text
	1f; 0; .data; 1:&lt;sleep\0&gt;; .text
	1f; 0; .data; 1:&lt;sync\0&gt;; .text
	1f; 0; .data; 1:&lt;kill\0&gt;; .text
	1f; 0; .data; 1:&lt;switches\0&gt;; .text
	1f; 0; .data; 1:&lt;boot\0&gt;; .text
esystab:
.data
.even
.text

specl6: / unknown
	jsr	pc,psp
	mov	(sp)+,r0
	jsr	pc,printo
	rts	pc











