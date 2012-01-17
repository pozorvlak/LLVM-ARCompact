	.file	"./cc_test/test.bc"
	.text
	.globl	eq
	.type	eq,@function
eq:                                     ; @eq
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	cmp r0,r1
	add.eq r0,r0,2
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp0:
	.size	eq, .tmp0-eq

	.globl	ne
	.type	ne,@function
ne:                                     ; @ne
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	cmp r0,r1
	add.ne r0,r0,2
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp1:
	.size	ne, .tmp1-ne

	.globl	ugt
	.type	ugt,@function
ugt:                                    ; @ugt
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	cmp r0,r1
	add.hi r0,r0,2
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp2:
	.size	ugt, .tmp2-ugt

	.globl	uge
	.type	uge,@function
uge:                                    ; @uge
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	cmp r0,r1
	add.hs r0,r0,2
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp3:
	.size	uge, .tmp3-uge

	.globl	ult
	.type	ult,@function
ult:                                    ; @ult
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	cmp r0,r1
	add.lo r0,r0,2
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp4:
	.size	ult, .tmp4-ult

	.globl	ule
	.type	ule,@function
ule:                                    ; @ule
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	cmp r0,r1
	add.ls r0,r0,2
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp5:
	.size	ule, .tmp5-ule

	.globl	sgt
	.type	sgt,@function
sgt:                                    ; @sgt
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	cmp r0,r1
	add.gt r0,r0,2
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp6:
	.size	sgt, .tmp6-sgt

	.globl	sge
	.type	sge,@function
sge:                                    ; @sge
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	cmp r0,r1
	add.ge r0,r0,2
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp7:
	.size	sge, .tmp7-sge

	.globl	slt
	.type	slt,@function
slt:                                    ; @slt
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	cmp r0,r1
	add.lt r0,r0,2
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp8:
	.size	slt, .tmp8-slt

	.globl	sle
	.type	sle,@function
sle:                                    ; @sle
; BB#0:
	; PROLOGUE START
	st.a fp,[sp,-4]
	mov fp,sp
	; PROLOGUE END
	cmp r0,r1
	add.le r0,r0,2
	; EPILOGUE START
	ld.ab fp,[sp,4]
	j [blink]
	; EPILOGUE END
.tmp9:
	.size	sle, .tmp9-sle


