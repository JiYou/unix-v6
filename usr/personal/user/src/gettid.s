#define 			GETTID				75
#int gettid()
#{
.globl _gettid
_gettid:
	mov	r5,-(sp)
	mov	sp,r5
	sys	GETTID
	bec 1f
	jmp	cerror
1:
	mov	(sp)+,r5
	rts	pc
#}

