	.file	"./add_test/test.bc"
	.text
	.globl	rr
	.type	rr,@function
rr:                                     ; @rr
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	add r0,r0,r1
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp0:
	.size	rr, .tmp0-rr

	.globl	rui
	.type	rui,@function
rui:                                    ; @rui
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	add r0,r0,4
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp1:
	.size	rui, .tmp1-rui

	.globl	rsi
	.type	rsi,@function
rsi:                                    ; @rsi
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	add r0,r0,-104
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp2:
	.size	rsi, .tmp2-rsi

	.globl	rli
	.type	rli,@function
rli:                                    ; @rli
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	add r0,r0,6000
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp3:
	.size	rli, .tmp3-rli


