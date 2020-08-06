	.arch msp430g2553
	.p2align 1,0

jt:
	.word default		; jt[0]
	.word case1		; jt[1]
	.word case2		; jt[2]
	.word case3		; jt[3]

	.text
	.global state_advance_assembly
	
state_advance_assembly:	
	cmp #4, r12
	jc default
	add r12, r12
	mov jt(r12), r0		; jmp jt[state]

case1:
	mov #1, r12
	jmp end

case2:	
	mov #2, r12
	jmp end

case3:
	mov #3, r12
	jmp end

default:
	mov #0, r12
	jmp end
	
end:
	pop r0			; return
