	.file	"./asr_test/test.bc"
	.text
	.globl	r1
	.type	r1,@function
r1:                                     ; @r1
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	asr r0,r0
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp0:
	.size	r1, .tmp0-r1

	.globl	ui1
	.type	ui1,@function
ui1:                                    ; @ui1
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	mov r0,50
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp1:
	.size	ui1, .tmp1-ui1

	.globl	li1
	.type	li1,@function
li1:                                    ; @li1
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	mov r0,3000
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp2:
	.size	li1, .tmp2-li1

	.globl	rr
	.type	rr,@function
rr:                                     ; @rr
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	asr r0,r0,r1
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp3:
	.size	rr, .tmp3-rr

	.globl	rui
	.type	rui,@function
rui:                                    ; @rui
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	asr r0,r0,4
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp4:
	.size	rui, .tmp4-rui

	.globl	rsi
	.type	rsi,@function
rsi:                                    ; @rsi
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp5:
	.size	rsi, .tmp5-rsi

	.globl	rli
	.type	rli,@function
rli:                                    ; @rli
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp6:
	.size	rli, .tmp6-rli


