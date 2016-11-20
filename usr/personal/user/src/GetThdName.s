#define      GETTHDNAME				80
#int GetThreadName(int tid, char name[12])
#{
.globl _GetThreadName
_GetThreadName:
	mov	r5,-(sp)
	mov	sp,r5
	mov	4(r5), 0f
mov	6(r5), 0f+2
	sys	0; 9f
	bec 1f
	jmp	cerror
	    1:
			clr r0
	mov	(sp)+,r5
	rts	pc
9:
	sys GETTHDNAME; 0:..;..
#}
