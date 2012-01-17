	.file	"./addn_test/test.bc"
	.text
	.globl	add1
	.type	add1,@function
add1:                                   ; @add1
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	add1 r0,r1,r0
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp0:
	.size	add1, .tmp0-add1

	.globl	add2
	.type	add2,@function
add2:                                   ; @add2
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	add2 r0,r1,r0
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp1:
	.size	add2, .tmp1-add2

	.globl	add3
	.type	add3,@function
add3:                                   ; @add3
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	add3 r0,r1,r0
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp2:
	.size	add3, .tmp2-add3


