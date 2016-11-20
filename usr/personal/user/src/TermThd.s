#define      TERMTHD				78
int TerminateThread(int tid)
{
.globl _TerminateThread
_TerminateThread:
	mov	r5,-(sp)
	mov	sp,r5
	mov r1, -(sp)
	mov	4(r5),r1
	cmp 0, r1
	blt	me
	beq err
	mov r1, -(sp)
	jsr gettid
	cmp r0, r1
	beq me
	sys	TERMTHD
  bec 2f
	jmp	cerror
2:
	clr r0
	br 1f
err:
	mov EINVTHDID, _errno
	mov -1, r0
	br 1f
me:
	mov -1, -(sp)
	jsr ExitThread

1:
	mov (sp)+, r1
	mov	(sp)+,r5
	rts pc
#}
