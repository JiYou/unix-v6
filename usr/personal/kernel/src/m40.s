_retu:			
 bis $340,PS
 mov (sp)+,r1		//return pc
 mov (sp)+,KISA6	//pass the param to PAR6.
 mov (sp), r2		//t_index
 mov $_u,r0
 mul sizeof(struct U_Thread), r2
 add r2, r0	      
1:
 mov (r0)+,sp
 mov (r0)+,r5 	
 bic $340,PS	//clear the low 5 bits.
 jmp (r1)

.globl _ReturnToThread
_ReturnToThread:
	mov r2, -(sp)
	mtpi sp			/�����û��ռ�ջָ��
	mtpi r5, -(r1)		/ѹ��t_param���û�ջ�ռ�
	mtpi r4, -(r1)		/ѹ��t_entry���û�ջ�ռ�
	mov $170000, -(sp)		/����PSW
	mov r3, -(sp)		/���ص�ַThreadSysEntry���ں�ջ
	rtt
