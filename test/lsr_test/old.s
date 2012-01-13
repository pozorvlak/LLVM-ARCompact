	.file	"./lsr_test/test.bc"
	.text
	.globl	f
	.type	f,@function
f:                                      ; @f
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	sub sp,sp,36
	; PROLOGUE END
	st r0,[fp,-4]
	st r1,[fp,-8]
	ld r0,[fp,-4]
	lsr r0,r0
	st r0,[fp,-12]
	st 50,[fp,-16]
	st 3000,[fp,-20]
	ld r0,[fp,-4]
	ld r1,[fp,-8]
	lsr r0,r0,r1
	st r0,[fp,-24]
	ld r0,[fp,-4]
	lsr r0,r0,4
	st r0,[fp,-28]
	mov r0,0
	; EPILOGUE START
	add sp,sp,36
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


