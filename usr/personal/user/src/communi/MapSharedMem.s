#int MapSharedMemory(char *name, MapShm *ms)
#{
.globl _MapSharedMemory
_MapSharedMemory:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5),0f
	mov	6(r5),0f+2
	sys	0; 9f
	bec	1f
	jmp	cerror
1:
	mov	(sp)+,r5
	rts	pc
9:
	sys 71; 0:	..;..
#}
