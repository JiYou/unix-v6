#int CreateSharedMemory(char *name, int blocks, int ownpermin, int otherpermin)
#{
.globl _CreateSharedMemory
_CreateSharedMemory:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),0f
	mov	6(r5),0f+2
	mov	8(r5),0f+4
	mov	10(r5),0f+6
	sys	0; 9f
	bec	1f
	jmp	cerror
1:
	mov	(sp)+,r5
	rts	pc
9:
	sys 69; 0:	..;..;..;..
#}
