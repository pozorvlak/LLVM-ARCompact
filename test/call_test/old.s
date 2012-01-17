	.file	"./call_test/test.bc"
	.text
	.globl	f
	.type	f,@function
f:                                      ; @f
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	add r0,r0,2
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp0:
	.size	f, .tmp0-f

	.globl	g
	.type	g,@function
g:                                      ; @g
; BB#0:
	; PROLOGUE START
	st.a blink,[sp,-4]
	st.a fp,[sp,-4]
	mov fp,sp
	sub sp,sp,4
	; PROLOGUE END
	st 2,[fp,-4]
	mov r0,2
	bl @f
	; EPILOGUE START
	add sp,sp,4
	ld.ab fp,[sp,4]
	ld.ab sp,[blink,4]
	j [blink]
	; EPILOGUE END
.tmp1:
	.size	g, .tmp1-g

	.globl	main
	.type	main,@function
main:                                   ; @main
; BB#0:
	; PROLOGUE START
	st.a blink,[sp,-4]
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	bl @g
	add r0,r0,5
	; EPILOGUE START
	ld.ab fp,[sp,4]
	ld.ab sp,[blink,4]
	j [blink]
	; EPILOGUE END
.tmp2:
	.size	main, .tmp2-main


