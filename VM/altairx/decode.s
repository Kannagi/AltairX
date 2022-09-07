	.file	"decode.c"
	.text
	.p2align 4
	.type	decode, @function
decode:
.LFB26:
	.cfi_startproc
	movl	%ecx, %r9d
	movq	%rdx, %r11
	movl	%ecx, %edx
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	shrl	$10, %r9d
	shrl	$20, %edx
	movl	%ecx, %r8d
	movl	%ecx, %ebx
	shrl	$8, %r8d
	movl	%r9d, %eax
	andl	$63, %edx
	shrl	$26, %ebx
	andl	$3, %eax
	movl	%edx, 124(%rsi)
	movq	37992(%rdi,%rdx,8), %rdx
	movl	%r8d, %r10d
	movl	%eax, 36(%rsi)
	movl	%ecx, %eax
	andl	$3, %r10d
	shrl	$14, %eax
	movq	%rdx, 8(%rsi)
	andl	$63, %eax
	movl	%r10d, 32(%rsi)
	movl	%eax, 128(%rsi)
	movq	37992(%rdi,%rax,8), %rax
	movq	%rbx, (%rsi)
	movq	%rax, 16(%rsi)
	movl	%ecx, %eax
	shrl	%eax
	andl	$127, %eax
	movzwl	(%r11,%rax,2), %edx
	movzbl	%dl, %eax
	movl	%edx, %r11d
	movl	%eax, 28(%rsi)
	movl	%edx, %eax
	andw	$28672, %r11w
	andl	$28672, %eax
	movl	%eax, 24(%rsi)
	testw	%dx, %dx
	js	.L50
	movl	%edx, %eax
	andw	$3840, %ax
	cmpw	$1792, %ax
	je	.L4
	jbe	.L51
	cmpw	$2816, %ax
	je	.L14
	jbe	.L52
	cmpw	$3072, %ax
	je	.L18
	cmpw	$3328, %ax
	jne	.L48
	andl	$262143, %r8d
	movq	%r8, %rax
	orq	$-262144, %rax
	movq	%rax, 8(%rsi)
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L51:
	cmpw	$1024, %ax
	je	.L6
	jbe	.L53
	cmpw	$1280, %ax
	je	.L12
	cmpw	$1536, %ax
	jne	.L48
	movzwl	%r9w, %eax
	xorl	%r8d, %r8d
	movq	%rax, 8(%rsi)
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L53:
	cmpw	$512, %ax
	je	.L8
	cmpw	$768, %ax
	jne	.L54
	movl	%r9d, %r8d
	orl	$-1024, %r9d
	andl	$1023, %r8d
	movl	%r8d, %eax
	cmpl	$513, %r8d
	cmovb	%rax, %r9
	movq	%r9, 16(%rsi)
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L52:
	cmpw	$2304, %ax
	je	.L16
	cmpw	$2560, %ax
	jne	.L55
.L11:
	cmpw	$8192, %r11w
	je	.L56
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L54:
	.cfi_restore_state
	cmpw	$256, %ax
	jne	.L48
	movl	%ecx, %r8d
	shrl	$11, %r8d
	andl	$511, %r8d
	andb	$1, %ch
	je	.L11
	movl	%r8d, %ecx
	movl	%r8d, %eax
	orl	$-256, %ecx
	cmpl	$129, %r8d
	cmovnb	%rcx, %rax
	movq	%rax, 16(%rsi)
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L55:
	cmpw	$2048, %ax
	jne	.L48
	andl	$262143, %r8d
	movl	%r8d, %eax
	xorl	%r8d, %r8d
	movq	%rax, 8(%rsi)
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L56:
	movb	%dl, 735(%rdi)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movb	$0, 734(%rdi)
	movl	%r8d, 720(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L50:
	.cfi_restore_state
	movq	37992(%rdi), %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movq	%rax, 16(%rsi)
	ret
	.p2align 4,,10
	.p2align 3
.L12:
	.cfi_restore_state
	movzwl	%r9w, %eax
	orl	$-32768, %r9d
	cmpl	$16385, %eax
	movq	%rax, %r8
	cmovb	%rax, %r9
	movq	%r9, 8(%rsi)
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L18:
	movzwl	%r9w, %r8d
	movl	%r8d, %eax
	shrl	$3, %eax
	andl	$1920, %eax
	andl	$16384, %r9d
	movl	%eax, %ecx
	jne	.L57
	sall	$16, %eax
	orl	$939524096, %eax
	testl	%ecx, %ecx
	cmovne	%eax, %ecx
.L32:
	movl	%r8d, %eax
	pxor	%xmm0, %xmm0
	movl	%r8d, %r9d
	sall	$16, %eax
	sall	$13, %r9d
	andl	$-2147483648, %eax
	andl	$8380416, %r9d
	orl	%r9d, %eax
	addl	%ecx, %eax
	movd	%eax, %xmm1
	cvtss2sd	%xmm1, %xmm0
	movsd	%xmm0, 56(%rsi)
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L8:
	movl	%ecx, %r8d
	shrl	$11, %r8d
	andl	$511, %r8d
	andb	$1, %ch
	je	.L11
	movl	%r8d, %eax
	movq	%rax, 16(%rsi)
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L16:
	movl	%r9d, %r8d
	orl	$-2097152, %r8d
	cmpl	$1048576, %r9d
	cmovbe	%r9d, %r8d
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L6:
	andl	$1023, %r9d
	movl	%r9d, %eax
	movq	%rax, 16(%rsi)
.L48:
	xorl	%r8d, %r8d
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L4:
	andl	$262143, %r8d
	movl	%r8d, %ecx
	movl	%r8d, %eax
	orl	$-131072, %ecx
	cmpl	$65537, %r8d
	cmovnb	%rcx, %rax
	movq	%rax, 8(%rsi)
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L14:
	movzwl	%r9w, %r8d
	addl	$1, %r10d
	movl	%r8d, %eax
	andl	$3, %r10d
	shrl	$3, %eax
	movl	%r10d, 36(%rsi)
	andl	$1920, %eax
	andl	$16384, %r9d
	movl	%eax, %ecx
	jne	.L58
	sall	$16, %eax
	orl	$939524096, %eax
	testl	%ecx, %ecx
	cmovne	%eax, %ecx
.L30:
	movl	%r8d, %eax
	movl	%r8d, %r9d
	sall	$16, %eax
	sall	$13, %r9d
	andl	$-2147483648, %eax
	andl	$8380416, %r9d
	orl	%r9d, %eax
	addl	%ecx, %eax
	movl	%eax, 88(%rsi,%r10,4)
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L58:
	sall	$16, %eax
	orl	$1073741824, %eax
	movl	%eax, %ecx
	jmp	.L30
	.p2align 4,,10
	.p2align 3
.L57:
	sall	$16, %eax
	orl	$1073741824, %eax
	movl	%eax, %ecx
	jmp	.L32
	.cfi_endproc
.LFE26:
	.size	decode, .-decode
	.p2align 4
	.globl	AX_decode_execute2
	.type	AX_decode_execute2, @function
AX_decode_execute2:
.LFB27:
	.cfi_startproc
	movq	624(%rdi), %rax
	movl	704(%rdi), %edx
	movl	(%rax,%rdx,4), %edx
	movl	%edx, %ecx
	movl	%edx, %eax
	shrl	$14, %edx
	shrl	$26, %ecx
	shrl	$20, %eax
	andl	$63, %edx
	andl	$63, %eax
	movl	%ecx, %esi
	movq	%rsi, 37992(%rdi)
	movl	%eax, %esi
	addl	%ecx, %eax
	movq	%rsi, 38024(%rdi)
	movl	%edx, %esi
	movq	%rsi, 38064(%rdi)
	movb	%al, 734(%rdi)
	cmpb	$1, %al
	je	.L61
	ret
	.p2align 4,,10
	.p2align 3
.L61:
	movl	720(%rdi), %esi
	jmp	executeDelayedInstruction@PLT
	.cfi_endproc
.LFE27:
	.size	AX_decode_execute2, .-AX_decode_execute2
	.p2align 4
	.globl	AX_decode_execute
	.type	AX_decode_execute, @function
AX_decode_execute:
.LFB28:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	leaq	80(%rdi), %r12
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	movl	704(%rdi), %ecx
	movq	%rdi, %rbp
	movq	624(%rdi), %rdx
	movq	%rcx, %rax
	movl	(%rdx,%rcx,4), %ecx
	leal	1(%rax), %esi
	movl	%ecx, 688(%rdi)
	movl	(%rdx,%rsi,4), %edx
	movl	%edx, 692(%rdi)
	testb	$1, %cl
	jne	.L63
	leaq	opcode1(%rip), %rdx
	movq	%r12, %rsi
	call	decode
	addq	$1, 632(%rdi)
	cmpb	$1, 734(%rdi)
	movb	$0, 736(%rdi)
	je	.L70
.L64:
	movq	%r12, %rsi
	movq	%rbp, %rdi
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	executeInstruction@PLT
	.p2align 4,,10
	.p2align 3
.L63:
	.cfi_restore_state
	addl	$2, %eax
	leaq	opcode1(%rip), %rdx
	movq	%r12, %rsi
	movl	%eax, 704(%rdi)
	leaq	216(%rdi), %r13
	call	decode
	movl	692(%rdi), %ecx
	leaq	opcode2(%rip), %rdx
	movq	%r13, %rsi
	call	decode
	addq	$2, 632(%rdi)
	cmpb	$1, 734(%rdi)
	movb	$1, 736(%rdi)
	je	.L71
	movq	%r12, %rsi
	movq	%rbp, %rdi
	call	executeInstruction@PLT
	movq	%r13, %rsi
.L72:
	movq	%rbp, %rdi
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	executeInstruction@PLT
	.p2align 4,,10
	.p2align 3
.L71:
	.cfi_restore_state
	movl	720(%rdi), %esi
	call	executeDelayedInstruction@PLT
	movq	%r12, %rsi
	movq	%rbp, %rdi
	call	executeInstruction@PLT
	movq	%r13, %rsi
	jmp	.L72
	.p2align 4,,10
	.p2align 3
.L70:
	movl	720(%rdi), %esi
	call	executeDelayedInstruction@PLT
	jmp	.L64
	.cfi_endproc
.LFE28:
	.size	AX_decode_execute, .-AX_decode_execute
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
