	.file	"./not_test/test.bc"
	.text
	.globl	r
	.type	r,@function
r:                                      ; @r
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	not r0,r0
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
	mov r0,-5
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp1:
	.size	ui, .tmp1-ui

	.globl	limm
	.type	limm,@function
limm:                                   ; @limm
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	mov r0,-6001
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp2:
	.size	limm, .tmp2-limm


