	.file	"./not_test/test.bc"
	.text
	.globl	f
	.type	f,@function
f:                                      ; @f
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	sub sp,sp,16
	; PROLOGUE END
	not r1,r0
	st r0,[fp,-4]
	st r1,[fp,-8]
	st -5,[fp,-12]
	st -6001,[fp,-16]
	mov r0,0
	; EPILOGUE START
	add sp,sp,16
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp0:
	.size	f, .tmp0-f

	.globl	main
	.type	main,@function
main:                                   ; @main
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	sub sp,sp,4
	; PROLOGUE END
	st 0,[fp,-4]
	mov r0,0
	; EPILOGUE START
	add sp,sp,4
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp1:
	.size	main, .tmp1-main


