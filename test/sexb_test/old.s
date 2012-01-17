	.file	"./sexb_test/test.bc"
	.text
	.globl	r
	.type	r,@function
r:                                      ; @r
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	sexb r0,r0
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp0:
	.size	r, .tmp0-r

	.globl	ui
	.type	ui,@function
ui:                                     ; @ui
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	mov r0,4
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp1:
	.size	ui, .tmp1-ui

	.globl	li
	.type	li,@function
li:                                     ; @li
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	mov r0,127
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp2:
	.size	li, .tmp2-li


