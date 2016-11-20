/ C operator tables

.globl fltused; fltused=.
.globl	_instab
.globl	_branchtab

.data
_instab:
	80.; 1f; 2f; .text; 1:&lt;mov\0&gt;; 2:&lt;clr\0&gt;; .data
	60.; 1f; 2f; .text; 1: &lt;cmp\0&gt;; 2: &lt;tst\0&gt;; .data
	106.; 1b; 2b
	61.; 1b; 2b
	62.; 1b; 2b
	63.; 1b; 2b
	64.; 1b; 2b
	65.; 1b; 2b
	66.; 1b; 2b
	67.; 1b; 2b
	68.; 1b; 2b
	69.; 1b; 2b
	40.; 1f; 3f; .text; 1:&lt;add\0&gt;; .data
	70.; 1b; 3f
	41.; 2f; 4f; .text; 2:&lt;sub\0&gt;; .data
	71.; 2b; 4f
	30.; 1b; 3f; .text; 3:&lt;inc\0&gt;; .data
	31.; 2b; 4f; .text; 4:&lt;dec\0&gt;; .data
	32.; 1b; 3b
	33.; 2b; 4b
	42.; 5f; 5f; .text; 5:&lt;mul&gt;; .data
	72.; 5b; 5b
	43.; 6f; 6f; .text; 6:&lt;div\0&gt;; .data
	73.; 6b; 6b
	44.; 5b; 6b
	74.; 5b; 6b
	45.; 5f; 6f; .text; 	6:&lt;asr\0&gt;; .data
	75.; 5f; 6b
	46.; 5f; 6f; .text; 5:&lt;ash\0&gt;; 6:&lt;asl\0&gt;; .data
	76.; 5b; 6b
	47.; 5f; 5f; .text; 5:&lt;bic\0&gt;; 6:&lt;bic $1,\0&gt;; .data
	55.; 5b; 6b
	85.; 5b; 6b
	81.; 5f; 6f; .text; 5:&lt;bit\0&gt;; 6:&lt;bit $1,\0&gt;; .data
	48.; 5f; 6f; .text; 5:&lt;bis\0&gt;; 6:&lt;bis $1,\0&gt;; .data
	78.; 5b; 6b
	49.; 5f; 5f; .text; 5:&lt;xor\0&gt;; .data
	79.; 5b; 5b
	37.; 1f; 1f; .text; 1:&lt;neg\0&gt;; .data
	38.; 1f; 1f; .text; 1:&lt;com\0&gt;; .data

	98.; 1f; 1f; .text; 1:&lt;*$\0&gt;; .data
	99.; 1b+2; 1b+2
	91.; 1f; 1f; .text; 1: &lt;ashc\0&gt;; .data
	92.; 1b; 1b
	82.; 1f; 1f; .text; 1:&lt;lmul\0&gt;; .data
	83.; 1f; 1f; .text; 1:&lt;ldiv\0&gt;; .data
	84.; 1f; 1f; .text; 1:&lt;lrem\0&gt;; .data
	86.; 1f; 1f; .text; 1:&lt;almul\0&gt;; .data
	87.; 1f; 1f; .text; 1:&lt;aldiv\0&gt;; .data
	88.; 1f; 1f; .text; 1:&lt;alrem\0&gt;; .data
	0

.data
_branchtab:
	60.; 0f; 1f; .text; 0:&lt;jeq\0&gt;; 1:&lt;jne\0&gt;; .data
	61.; 1b; 0b
	62.; 2f; 5f; .text; 2:&lt;jle\0&gt;; 5:&lt;jgt\0&gt;; .data
	63.; 3f; 4f; .text; 3:&lt;jlt\0&gt;; 4:&lt;jge\0&gt;; .data
	64.; 4b; 3b
	65.; 5b; 2b
	66.; 6f; 9f; .text; 6:&lt;jlos\0&gt;; 9:&lt;jhi\0&gt;; .data
	67.; 7f; 8f; .text; 7:&lt;jlo\0&gt;; 8:&lt;jhis\0&gt;; .data
	68.; 8b; 7b
	69.; 9b; 6b

	260.; 0b; 1b
	261.; 1b; 0b
	262.; 2b; 5b
	263.; 3b; 4b
	264.; 4b; 3b
	265.; 5b; 2b
	266.; 0b; 1b
	267.; 7f; 8f; .text; 7:&lt;/nop\0&gt;; 8:&lt;jbr\0&gt;; .data
	268.; 8b; 7b
	269.; 1b; 0b
	0











