	.file	"./and_test/test.bc"
	.text
	.globl	f
	.type	f,@function
f:                                      ; @f
; BB#0:
	; PROLOGUE START
	st fp,[sp,-4]
	sub sp,sp,4
	mov fp,sp
	sub sp,sp,28
	; PROLOGUE END
	st r0,[fp,-8]
	st r1,[fp,-12]
	ld r0,[fp,-8]
	and r0,r0,r1
	st r0,[fp,-16]
	ld r0,[fp,-8]
	and r0,r0,4
	st r0,[fp,-20]
	ld r0,[fp,-8]
	and r0,r0,-104
	st r0,[fp,-24]
	ld r0,[fp,-8]
	and r0,r0,6000
	st r0,[fp,-28]
	ld r0,[fp,-4]
	; EPILOGUE START
	add sp,sp,28
	add sp,sp,4
	j.d [blink]
	ld fp,[sp,-4]
	; EPILOGUE END
.tmp0:
	.size	f, .tmp0-f

	.globl	main
	.type	main,@function
main:                                   ; @main
; BB#0:
	; PROLOGUE START
	st fp,[sp,-4]
	sub sp,sp,4
	mov fp,sp
	sub sp,sp,4
	; PROLOGUE END
	st 0,[fp,-4]
	mov r0,0
	; EPILOGUE START
	add sp,sp,4
	add sp,sp,4
	j.d [blink]
	ld fp,[sp,-4]
	; EPILOGUE END
.tmp1:
	.size	main, .tmp1-main


