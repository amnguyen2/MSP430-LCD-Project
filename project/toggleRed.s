	#include <msp430.h>
	#include "stateMachines.h"
	#include "led.h"

	.file "stateMachines.c"

red_state:	 .byte 0
red_on:		 .byte 0

	.text
jt:
	.word default		; jt[0]
	.word case0		; jt[1]
	.word case1		; jt[2]

toggle_red:
	cmp.b #3, &red_state
	jnc default		; no carry

	mov &state, r12
	add r12, r12
	mov jt(r12), r0		; jmp jt[state]

case0:
	mov.b #1, &red_on	; red is on
	mov #1, r12		; state is 1

case1:
	mov.b #0, &red_on	; red is off
	mov #0, r12		; state is 0

default:
	pop #1

end:
	pop r0			; return
