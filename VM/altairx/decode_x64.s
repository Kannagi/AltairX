	.file	"decode_x64.c"
	.text
#APP
	.section .rodata
	.align 16
	if_AX_EXE_IMM:
	.long if_AX_DEC_EXE
	
#NO_APP
	.p2align 4
	.globl	AX_decode_executex64
	.type	AX_decode_executex64, @function
AX_decode_executex64:
.LFB22:
	.cfi_startproc
	movq	624(%rdi), %rax
	movl	704(%rdi), %edx
	leaq	opcode1(%rip), %rcx
	leaq	-32(%rsp), %rdi
	leaq	opcode2(%rip), %rsi
#APP
# 34 "src/decode_x64.c" 1
	.intel_syntax noprefix
push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	mov r9d,%edx
	shl r9,2
	mov r8,[r9+%rax]
	mov r13,r8
	shr r13,26
	and r13,0x3F
	mov r14,r8
	shr r14,20
	and r14,0x3F
	mov r15,r8
	shr r15,14
	and r15,0x3F
	AX_EXE:
	mov r9,r8
	and r9,0xFE
	mov r10,[r9+%rcx]
	mov r11,r10
	and r10,0xFF
	and r11,0x7000
	mov r9,%rdi
	mov [r9+12],r10
	mov [r9+16],r11
	mov r11,r10
	and r11,0x8000
	cmp r11,0x8000
	jne  if_AX_DEC_EXE
	mov r15,0
	if_AX_DEC_EXE:
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	.att_syntax prefix

# 0 "" 2
#NO_APP
	leaq	reg(%rip), %rax
#APP
# 146 "src/decode_x64.c" 1
	.intel_syntax noprefix
.att_syntax prefix

# 0 "" 2
#NO_APP
	ret
	.cfi_endproc
.LFE22:
	.size	AX_decode_executex64, .-AX_decode_executex64
	.p2align 4
	.globl	decode_x64
	.type	decode_x64, @function
decode_x64:
.LFB23:
	.cfi_startproc
#APP
# 176 "src/decode_x64.c" 1
	.intel_syntax noprefix
push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	mov r8,%rdi
	shr r8,1
	and r8,0x7F
	mov r13,%rdi
	shr r13,26
	and r13,0x3F
	mov r13,%rdi
	shr r13,20
	and r13,0x3F
	mov r13,%rdi
	shr r13,14
	and r13,0x3F
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	.att_syntax prefix

# 0 "" 2
#NO_APP
	ret
	.cfi_endproc
.LFE23:
	.size	decode_x64, .-decode_x64
	.local	reg
	.comm	reg,128,32
	.section	.rodata
	.align 32
	.type	opcode2, @object
	.size	opcode2, 256
opcode2:
	.value	0
	.value	1
	.value	2
	.value	1539
	.value	4
	.value	5
	.value	772
	.value	1029
	.value	3334
	.value	2054
	.value	28
	.value	28
	.value	7
	.value	8
	.value	9
	.value	10
	.value	523
	.value	524
	.value	525
	.value	526
	.value	527
	.value	528
	.value	529
	.value	530
	.value	275
	.value	276
	.value	533
	.value	278
	.value	535
	.value	28
	.value	28
	.value	28
	.value	-27136
	.value	-27135
	.value	-27134
	.value	-27133
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	4096
	.value	4097
	.value	4098
	.value	4099
	.value	4864
	.value	4865
	.value	4866
	.value	4867
	.value	4104
	.value	4105
	.value	5128
	.value	5129
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	16386
	.value	16387
	.value	16384
	.value	16385
	.value	16390
	.value	16391
	.value	16392
	.value	28
	.value	16388
	.value	16389
	.value	16393
	.value	16394
	.value	28
	.value	28
	.value	28
	.value	28
	.value	20481
	.value	20482
	.value	20483
	.value	20484
	.value	20485
	.value	20486
	.value	20487
	.value	20488
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.align 32
	.type	opcode1, @object
	.size	opcode1, 256
opcode1:
	.value	0
	.value	1
	.value	2
	.value	1539
	.value	4
	.value	5
	.value	772
	.value	1029
	.value	3334
	.value	2054
	.value	28
	.value	28
	.value	7
	.value	8
	.value	9
	.value	10
	.value	523
	.value	524
	.value	525
	.value	526
	.value	527
	.value	528
	.value	529
	.value	530
	.value	275
	.value	276
	.value	533
	.value	278
	.value	535
	.value	28
	.value	28
	.value	28
	.value	-27136
	.value	-27135
	.value	-27134
	.value	-27133
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	28
	.value	4096
	.value	4097
	.value	4098
	.value	4099
	.value	4864
	.value	4865
	.value	4866
	.value	4867
	.value	4104
	.value	4105
	.value	5128
	.value	5129
	.value	28
	.value	28
	.value	28
	.value	28
	.value	24
	.value	1561
	.value	1816
	.value	2072
	.value	12318
	.value	12288
	.value	15134
	.value	15360
	.value	26
	.value	27
	.value	4106
	.value	28
	.value	8208
	.value	8209
	.value	8210
	.value	8211
	.value	10496
	.value	10497
	.value	10498
	.value	10499
	.value	10500
	.value	10501
	.value	10502
	.value	10503
	.value	10504
	.value	10505
	.value	10506
	.value	10507
	.value	10508
	.value	10509
	.value	10510
	.value	10511
	.value	12288
	.value	12289
	.value	12290
	.value	12291
	.value	12292
	.value	12293
	.value	12294
	.value	12295
	.value	12296
	.value	12297
	.value	12298
	.value	12299
	.value	12289
	.value	12293
	.value	12297
	.value	12303
	.value	12304
	.value	12305
	.value	15120
	.value	15121
	.value	12306
	.value	12307
	.value	12308
	.value	12309
	.value	12312
	.value	12313
	.value	12314
	.value	12315
	.value	12316
	.value	12317
	.value	12310
	.value	12311
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
