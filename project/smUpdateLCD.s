	.arch msp430g2553
	.p2align 1,0

jt:
	.word default		; jt[0]
	.word case1		; jt[1]
	.word case2		; jt[2]
	.word case3		; jt[3]
	
	.text
	.global sm_update_lcd_assembly

sm_update_lcd_assembly:
	mov r12, r13		; save state var
	cmp #4, r12
	jc default
	add r12, r12
	mov jt(r12), r0		; jmp jt[state]

case1:
	mov #0x001f, &triangleColor ; COLOR_RED
	call #write_on_blackboard
	jmp end

case2:
	mov #0x053f, &triangleColor ; COLOR_ORANGE
	jmp end

case3:
	mov #0xf800, &triangleColor ; COLOR_BLUE
	jmp end

default:
	jmp end

end:
	mov r13, r12		; get state var
	pop r0			; return

