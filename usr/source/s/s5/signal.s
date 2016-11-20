/ C library -- signal

/ signal(n, 0); /* default action on signal(n) */
/ signal(n, odd); /* ignore signal(n) */
/ signal(n, label); /* goto label on signal(n) */
/ returns old label, only one level.

rtt	= 6
signal	= 48.
.globl	_signal, cerror

_signal:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),r1	//sig no
	cmp	r1,$NSIG
	bhis	2f
	mov	6(r5),r0	//handler
	mov	r1,0f		
	asl	r1
	mov	dvect(r1),-(sp)	//��dvect���ϵ�handler��ջ
	mov	r0,dvect(r1)		//�����µ�handler��dvect��
	mov	r0,0f+2		//��handler�������ݲ�����
	beq	1f			//Ϊ0����ת
	bit	$1,r0		
	bne	1f			//Ϊ��������ת
	asl	r1			//ż������tvect�д���handler
	asl	r1
	add	$tvect,r1
	mov	r1,0f+2
1:
	sys	0; 9f
	bes	3f	//carrier set
	bit	$1,r0
	beq	1f		//Ϊ0��ż��������dvect�ж�Ӧ��handler����
	mov	r0,(sp)	//Ϊ����������u.u_signal�еĸ�handler����
1:
	mov	(sp)+,r0	
	mov	(sp)+,r5
	rts	pc
2:
	mov	$22.,r0		/ EINVAL
	clr	-(sp)
3:
	tst	(sp)+
	jmp	cerror

NSIG = 0
tvect:
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
	mov r0,-(sp); mov dvect+[NSIG*2],r0; br 1f; NSIG=NSIG+1
1:
	mov	r1,-(sp)
	mov	r2,-(sp)
	mov	r3,-(sp)
	mov	r4,-(sp)
	jsr	pc,(r0)
	mov	(sp)+,r4
	mov	(sp)+,r3
	mov	(sp)+,r2
	mov	(sp)+,r1
	mov	(sp)+,r0
	rtt
.data
9:
	sys	signal; 0:..; .. //signal no+handler(or $tvect��Ӧ��ַ)
.bss
dvect:	.=.+[NSIG*2]











