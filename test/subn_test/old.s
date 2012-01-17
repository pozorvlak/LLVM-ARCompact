	.file	"./subn_test/test.bc"
	.text
	.globl	sub1
	.type	sub1,@function
sub1:                                   ; @sub1
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	sub1 r0,r1,r0
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp0:
	.size	sub1, .tmp0-sub1

	.globl	sub2
	.type	sub2,@function
sub2:                                   ; @sub2
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	sub2 r0,r1,r0
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp1:
	.size	sub2, .tmp1-sub2

	.globl	sub3
	.type	sub3,@function
sub3:                                   ; @sub3
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	sub3 r0,r1,r0
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp2:
	.size	sub3, .tmp2-sub3


