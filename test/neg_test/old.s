	.file	"./neg_test/test.bc"
	.text
	.globl	r
	.type	r,@function
r:                                      ; @r
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	neg r0,r0
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp0:
	.size	r, .tmp0-r


