	.file	"./add_test/test.bc"
	.text
	.globl	main
	.type	main,@function
main:                                   ; @main
; BB#0:
	st 0,[fp,-4]
	st 1,[fp,-8]
	st 2,[fp,-12]
	ld r0,[fp,-8]
	add r0,r0,2
	st r0,[fp,-16]
	ld r0,[fp,-8]
	add r0,r0,4
	st r0,[fp,-20]
	ld r0,[fp,-8]
	add r0,r0,-104
	st r0,[fp,-24]
	ld r0,[fp,-12]
	add r0,r0,6000
	st r0,[fp,-28]
	ld r0,[fp,-4]
	j.d [blink]
.tmp0:
	.size	main, .tmp0-main


