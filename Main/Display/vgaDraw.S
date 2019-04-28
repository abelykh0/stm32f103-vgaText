#include "settings.h"

.syntax unified

// 640x480@24 > 40x30?

@ Arguments:
@  r0  font
@  r1  characters
@  r2  lookup table
@  r3  ODR
.section	.text.vgaDraw,"ax",%progbits
//.section .ramcode,ax,%progbits
.balign 4
.global vgaDraw
.thumb_func
vgaDraw:
	@ parameters
	fnt   .req r0
	chr   .req r1
	col   .req r2
	odr   .req r3

	push {r4, r5, r6, r7}          // 5

	// prepare
	ldrb r5, [chr], #1             // 1*
	ldrb r5, [fnt, r5, LSL #3]     // 2*
	lsr r4, r5, #2                 // 1

	.rept HSIZE_CHARS

		// first 4 pixels, each pixel one byte (r6)

		and r4, r4, #60            // 1
		ldr r6, [col, r4]          // 1*
		strb r6, [odr]             // 2

		ror r6, #8                 // 1
		nop                        // 1
		strb r6, [odr]             // 2

		ror r6, #8                 // 1
		and r4, r5, #15            // 1
		strb r6, [odr]             // 2

		ror r6, #8                 // 1
		ldr r7, [col, r4, LSL #2]  // 1*
		strb r6, [odr]             // 2

		// next 4 pixels, each pixel one byte  (r7)

		nop                        // 1
		ldrb r5, [chr], #1         // 1*
		strb r7, [odr]             // 2

		ror r7, #8                 // 1
		ldrb r5, [fnt, r5, LSL #3] // 1*
		strb r7, [odr]             // 2

		ror r7, #8                 // 1
		lsr r4, r5, #2             // 1
		strb r7, [odr]             // 2

		ror r7, #8                 // 1
		nop                        // 1
		strb r7, [odr]             // 2
	.endr

	// 0 at the end
	mov r4, #0                     // 1
	nop                            // 1
	strb r4, [odr]                 // 2

	pop	{r4, r5, r6, r7}           // 4
	bx lr
