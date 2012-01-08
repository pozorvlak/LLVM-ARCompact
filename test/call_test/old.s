	.file	"./call_test/test.bc"
	.text
	.globl	f
	.type	f,@function
f:                                      ; @f
; BB#0:
	; PROLOGUE START
	st fp,[sp,-4]
	sub sp,sp,4
	mov fp,sp
	sub sp,sp,4
	; PROLOGUE END
	st r0,[fp,-4]
	add r0,r0,2
	; EPILOGUE START
	add sp,sp,4
	add sp,sp,4
	j.d [blink]
	ld fp,[sp,-4]
	; EPILOGUE END
.tmp0:
	.size	f, .tmp0-f

	.globl	g
	.type	g,@function
g:                                      ; @g
; BB#0:
	; PROLOGUE START
	st fp,[sp,-4]
	sub sp,sp,4
	mov fp,sp
	sub sp,sp,4
	; PROLOGUE END
	st 2,[fp,-4]
	mov r0,2
	bl f
	; EPILOGUE START
	add sp,sp,4
	add sp,sp,4
	j.d [blink]
	ld fp,[sp,-4]
	; EPILOGUE END
.tmp1:
	.size	g, .tmp1-g

	.globl	main
	.type	main,@function
main:                                   ; @main
; BB#0:
	; PROLOGUE START
	st fp,[sp,-4]
	sub sp,sp,4
	mov fp,sp
	sub sp,sp,12
	; PROLOGUE END
	st 0,[fp,-4]
	st 5,[fp,-8]
	bl g
	st r0,[fp,-12]
	ld r1,[fp,-8]
	add r0,r1,r0
	; EPILOGUE START
	add sp,sp,12
	add sp,sp,4
	j.d [blink]
	ld fp,[sp,-4]
	; EPILOGUE END
.tmp2:
	.size	main, .tmp2-main


