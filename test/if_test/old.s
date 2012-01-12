	.file	"./if_test/test.bc"
	.text
	.globl	f
	.type	f,@function
f:                                      ; @f
; BB#0:
	; PROLOGUE START
	st fp,[sp,-4]
	sub sp,sp,4
	mov fp,sp
	sub sp,sp,12
	; PROLOGUE END
	st r0,[fp,-4]
	st r1,[fp,-8]
	st 1,[fp,-12]
	ld r0,[fp,-4]
	ld r1,[fp,-8]
	cmp r0,r1
	bge @.BB0_2
; BB#1:
	st 2,[fp,-12]
.BB0_2:
	ld r0,[fp,-4]
	cmp r0,0
	bgt @.BB0_4
; BB#3:
	st 4,[fp,-12]
	b @.BB0_7
.BB0_4:
	ld r0,[fp,-12]
	cmp r0,6
	blt @.BB0_6
; BB#5:
	st 3,[fp,-12]
	b @.BB0_7
.BB0_6:
	st 7,[fp,-12]
.BB0_7:
	ld r0,[fp,-12]
	ld r1,[fp,-4]
	cmp r0,r1
	blt @.BB0_9
; BB#8:
	ld r0,[fp,-12]
	add r0,r0,2
	st r0,[fp,-12]
.BB0_9:
	ld r0,[fp,-12]
	ld r1,[fp,-8]
	cmp r0,r1
	bne @.BB0_11
; BB#10:
	ld r0,[fp,-12]
	add r0,r0,7
	b @.BB0_13
.BB0_11:
	ld r0,[fp,-12]
	ld r1,[fp,-4]
	cmp r0,r1
	beq @.BB0_14
; BB#12:
	ld r0,[fp,-12]
	add r0,r0,-6
.BB0_13:
	st r0,[fp,-12]
.BB0_14:
	ld r0,[fp,-12]
	; EPILOGUE START
	add sp,sp,12
	add sp,sp,4
	ld fp,[sp,-4]
	j [blink]
	; EPILOGUE END
.tmp0:
	.size	f, .tmp0-f

	.globl	main
	.type	main,@function
main:                                   ; @main
; BB#0:
	; PROLOGUE START
	st fp,[sp,-4]
	sub sp,sp,4
	mov fp,sp
	sub sp,sp,4
	; PROLOGUE END
	st 0,[fp,-4]
	mov r0,0
	; EPILOGUE START
	add sp,sp,4
	add sp,sp,4
	ld fp,[sp,-4]
	j [blink]
	; EPILOGUE END
.tmp1:
	.size	main, .tmp1-main


