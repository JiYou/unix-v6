#int CreateThread(char *name, int iPriority, int iStatus, ThreadProc entry, int *pParam, int iStackSize)
#{
.globl _CreateThread
.globl _ThreadSysEntry
_CreateThread:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),0f
	mov	6(r5),0f+2
	mov	8(r5),0f+4
	mov	10(r5),0f+6
	mov	12(r5),0f+8
	mov	12(r5),0f+10

	#��ThreadSysEntry��Ϊ���Ӳ���������ں�ʵ�֣������߳��״�ִ��ʱ�������
	mov _ThreadSysEntry, 0f+12
	sys	0; 9f
	bec	1f
	jmp	cerror
1:
	mov	(sp)+,r5
	rts	pc
9:
	sys 73; 0:	..;..;..;..;..;..;..
#}

