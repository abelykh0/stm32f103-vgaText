#include "settings.h"

.syntax unified

@ 640x480@24 > 40x30?

@ Arguments:
@  r0  characters
@  r1  vertical line
@  r2  lookup table
@  r7  ODR
.section	.text.vgaDraw,"ax",%progbits
@.section .ramcode,ax,%progbits
.balign 4
.global vgaDraw
.thumb_func
vgaDraw:
	@ parameters
	chr   .req r0
	vln   .req r1
	col   .req r2
	odr   .req r3

	push {r4, r5, r6, r7}          @ 5

	@ prepare
	ldr r5, [chr], #4              @ 2*
	ldrb r5, [r5, vln]             @ 2*
	lsr r4, r5, #2                 @ 1

	.rept HSIZE_CHARS

		@ first 4 pixels, each pixel one byte (r6)

		and r4, r4, #60            @ 1
		ldr r6, [col, r4]          @ 1*
		strb r6, [odr]             @ 2

		ror r6, #8                 @ 1
		nop                        @ 1
		strb r6, [odr]             @ 2

		ror r6, #8                 @ 1
		and r4, r5, #15            @ 1
		strb r6, [odr]             @ 2

		ror r6, #8                 @ 1
		ldr r7, [col, r4, LSL #2]  @ 1*
		strb r6, [odr]             @ 2

		@ next 4 pixels, each pixel one byte  (r7)

		nop                        @ 1
		ldr r5, [chr], #4          @ 1*
		strb r7, [odr]             @ 2

		ror r7, #8                 @ 1
		ldrb r5, [r5, vln]         @ 1*
		strb r7, [odr]             @ 2

		ror r7, #8                 @ 1
		lsr r4, r5, #2             @ 1
		strb r7, [odr]             @ 2

		ror r7, #8                 @ 1
		nop                        @ 1
		strb r7, [odr]             @ 2
	.endr

	@ 0 at the end
	mov r4, #0                     @ 1
	nop                            @ 1
	strb r4, [odr]                 @ 2

	pop	{r4, r5, r6, r7}           @ 5
	bx lr
