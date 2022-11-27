	.file	"decode_execute.c"
	.text
	.p2align 4
	.type	executeLoad, @function
executeLoad:
.LFB26:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsi, %rbp
	movq	%rdx, %rsi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	testl	$2147483648, %edx
	je	.L2
	movl	76(%rdi), %eax
	movq	48(%rdi), %rbx
	subl	$1, %eax
.L3:
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	andq	%rax, %rsi
	movq	%rbp, %rdi
	movl	%ecx, %edx
	addq	%rbx, %rsi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	memcpy@PLT
	.p2align 4,,10
	.p2align 3
.L2:
	.cfi_restore_state
	testl	$1073741824, %edx
	je	.L4
	movl	72(%rdi), %eax
	movq	40(%rdi), %rbx
	subl	$1, %eax
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L4:
	testl	$536870912, %edx
	jne	.L13
	testl	$268435456, %edx
	je	.L6
	movl	64(%rdi), %eax
	movq	16(%rdi), %rbx
	subl	$1, %eax
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L13:
	movl	68(%rdi), %eax
	movq	32(%rdi), %rbx
	subl	$1, %eax
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L6:
	testl	$134217728, %edx
	jne	.L14
	testl	$67108864, %edx
	je	.L8
	movl	56(%rdi), %eax
	movq	(%rdi), %rbx
	subl	$1, %eax
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L14:
	movl	60(%rdi), %eax
	movq	8(%rdi), %rbx
	subl	$1, %eax
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L8:
	leaq	318(%rdi), %rbx
	movl	$32767, %eax
	testl	$33554432, %edx
	je	.L3
	movq	%rbp, %rdx
	movl	%ecx, 12(%rsp)
	movq	24(%rdi), %rbx
	movq	%rsi, (%rsp)
	call	AX_IO_Read@PLT
	movl	12(%rsp), %ecx
	movq	(%rsp), %rsi
	movl	$524287, %eax
	jmp	.L3
	.cfi_endproc
.LFE26:
	.size	executeLoad, .-executeLoad
	.p2align 4
	.type	executeStore, @function
executeStore:
.LFB25:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rsi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdx, %rbx
	subq	$16, %rsp
	.cfi_def_cfa_offset 48
	testl	$2147483648, %edx
	je	.L16
	movl	76(%rdi), %eax
	movq	48(%rdi), %rbp
	leal	-1(%rax), %edx
.L17:
	andq	%rbx, %rdx
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	movq	%r12, %rsi
	leaq	0(%rbp,%rdx), %rdi
	popq	%rbx
	.cfi_def_cfa_offset 24
	movl	%ecx, %edx
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	memcpy@PLT
	.p2align 4,,10
	.p2align 3
.L16:
	.cfi_restore_state
	testl	$1073741824, %edx
	je	.L18
	movl	72(%rdi), %eax
	movq	40(%rdi), %rbp
	leal	-1(%rax), %edx
	jmp	.L17
	.p2align 4,,10
	.p2align 3
.L18:
	testl	$536870912, %edx
	jne	.L26
	testl	$268435456, %edx
	je	.L20
	movl	64(%rdi), %eax
	movq	16(%rdi), %rbp
	leal	-1(%rax), %edx
	jmp	.L17
	.p2align 4,,10
	.p2align 3
.L26:
	movl	68(%rdi), %eax
	movq	32(%rdi), %rbp
	leal	-1(%rax), %edx
	jmp	.L17
	.p2align 4,,10
	.p2align 3
.L20:
	testl	$134217728, %edx
	jne	.L27
	testl	$67108864, %edx
	je	.L22
	movl	56(%rdi), %eax
	movq	(%rdi), %rbp
	leal	-1(%rax), %edx
	jmp	.L17
	.p2align 4,,10
	.p2align 3
.L27:
	movl	60(%rdi), %eax
	movq	8(%rdi), %rbp
	leal	-1(%rax), %edx
	jmp	.L17
	.p2align 4,,10
	.p2align 3
.L22:
	leaq	318(%rdi), %rbp
	movl	$32767, %edx
	testl	$33554432, %ebx
	je	.L17
	movq	%rsi, %rdx
	movq	%rbx, %rsi
	movl	%ecx, 12(%rsp)
	movq	24(%rdi), %rbp
	call	AX_IO_Write@PLT
	movl	12(%rsp), %ecx
	movl	$524287, %edx
	jmp	.L17
	.cfi_endproc
.LFE25:
	.size	executeStore, .-executeStore
	.p2align 4
	.globl	executeDelayedInstruction
	.type	executeDelayedInstruction, @function
executeDelayedInstruction:
.LFB27:
	.cfi_startproc
	movzbl	187(%rdi), %eax
	movb	$0, 186(%rdi)
	movl	160(%rdi), %ecx
	subl	$76, %eax
	cmpb	$19, %al
	ja	.L28
	leaq	.L31(%rip), %rdx
	movzbl	%al, %eax
	movslq	(%rdx,%rax,4), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L31:
	.long	.L50-.L31
	.long	.L49-.L31
	.long	.L48-.L31
	.long	.L47-.L31
	.long	.L46-.L31
	.long	.L45-.L31
	.long	.L44-.L31
	.long	.L43-.L31
	.long	.L42-.L31
	.long	.L41-.L31
	.long	.L40-.L31
	.long	.L39-.L31
	.long	.L38-.L31
	.long	.L37-.L31
	.long	.L70-.L31
	.long	.L35-.L31
	.long	.L72-.L31
	.long	.L71-.L31
	.long	.L32-.L31
	.long	.L30-.L31
	.text
	.p2align 4,,10
	.p2align 3
.L48:
	movl	144(%rdi), %eax
	movb	$1, 188(%rdi)
	movl	$0, 144(%rdi)
	movl	%eax, 156(%rdi)
.L28:
	ret
	.p2align 4,,10
	.p2align 3
.L37:
	movzwl	184(%rdi), %eax
	andl	$3, %eax
	cmpw	$3, %ax
	je	.L28
.L70:
	addl	%ecx, 144(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L30:
	movl	144(%rdi), %eax
	movl	%eax, 152(%rdi)
.L71:
	addl	148(%rdi), %ecx
.L72:
	movl	%ecx, 144(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L32:
	movl	144(%rdi), %eax
	movl	%ecx, 144(%rdi)
	movl	%eax, 152(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L50:
	movl	152(%rdi), %eax
	movl	%eax, 144(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L40:
	movzwl	184(%rdi), %eax
	andl	$3, %eax
	cmpw	$2, %ax
	je	.L70
	ret
	.p2align 4,,10
	.p2align 3
.L41:
	testb	$4, 184(%rdi)
	jne	.L28
	jmp	.L70
	.p2align 4,,10
	.p2align 3
.L42:
	testb	$5, 184(%rdi)
	jne	.L28
	jmp	.L70
	.p2align 4,,10
	.p2align 3
.L43:
	testb	$4, 184(%rdi)
	je	.L28
	jmp	.L70
	.p2align 4,,10
	.p2align 3
.L44:
	movzwl	184(%rdi), %eax
	andl	$5, %eax
	cmpw	$4, %ax
	je	.L70
	ret
	.p2align 4,,10
	.p2align 3
.L45:
	testb	$1, 184(%rdi)
	jne	.L28
	jmp	.L70
	.p2align 4,,10
	.p2align 3
.L35:
	movq	37920(%rdi), %rax
	testq	%rax, %rax
	je	.L28
	subq	$1, %rax
	movq	%rax, 37920(%rdi)
	jmp	.L70
	.p2align 4,,10
	.p2align 3
.L49:
	movl	156(%rdi), %eax
	movl	%eax, 144(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L38:
	testb	$3, 184(%rdi)
	jne	.L28
	jmp	.L70
	.p2align 4,,10
	.p2align 3
.L39:
	testb	$2, 184(%rdi)
	je	.L28
	jmp	.L70
	.p2align 4,,10
	.p2align 3
.L46:
	testb	$1, 184(%rdi)
	je	.L28
	jmp	.L70
	.p2align 4,,10
	.p2align 3
.L47:
	movl	144(%rdi), %eax
	movb	$2, 188(%rdi)
	movl	$2, 144(%rdi)
	movl	%eax, 156(%rdi)
	ret
	.cfi_endproc
.LFE27:
	.size	executeDelayedInstruction, .-executeDelayedInstruction
	.p2align 4
	.globl	AX_execute_unit1A
	.type	AX_execute_unit1A, @function
AX_execute_unit1A:
.LFB28:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	subl	$32, %r8d
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movl	32(%rsp), %eax
	cmpl	$89, %r8d
	ja	.L75
	movq	%rsi, %rbx
	leaq	.L78(%rip), %rsi
	movq	%rdi, %rbp
	movq	%rdx, %rdi
	movslq	(%rsi,%r8,4), %rdx
	movq	%rcx, %r10
	addq	%rsi, %rdx
	jmp	*%rdx
	.section	.rodata
	.align 4
	.align 4
.L78:
	.long	.L114-.L78
	.long	.L113-.L78
	.long	.L112-.L78
	.long	.L111-.L78
	.long	.L110-.L78
	.long	.L109-.L78
	.long	.L108-.L78
	.long	.L107-.L78
	.long	.L106-.L78
	.long	.L105-.L78
	.long	.L104-.L78
	.long	.L103-.L78
	.long	.L102-.L78
	.long	.L101-.L78
	.long	.L100-.L78
	.long	.L99-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L98-.L78
	.long	.L97-.L78
	.long	.L96-.L78
	.long	.L95-.L78
	.long	.L94-.L78
	.long	.L93-.L78
	.long	.L92-.L78
	.long	.L75-.L78
	.long	.L91-.L78
	.long	.L90-.L78
	.long	.L89-.L78
	.long	.L88-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L75-.L78
	.long	.L87-.L78
	.long	.L86-.L78
	.long	.L85-.L78
	.long	.L84-.L78
	.long	.L83-.L78
	.long	.L82-.L78
	.long	.L81-.L78
	.long	.L80-.L78
	.long	.L79-.L78
	.long	.L77-.L78
	.text
	.p2align 4,,10
	.p2align 3
.L87:
	shrl	$10, %eax
	addq	%rbx, %rbx
	movzwl	%ax, %ecx
	movl	%ecx, %edx
	shrl	$3, %edx
	andl	$1920, %edx
	testb	$64, %ah
	movl	%edx, %eax
	je	.L128
	sall	$16, %eax
	orl	$1073741824, %eax
	movl	%eax, %edx
.L129:
	movl	%ecx, %eax
	sall	$13, %ecx
	pxor	%xmm0, %xmm0
	sall	$16, %eax
	andl	$8380416, %ecx
	andl	$-2147483648, %eax
	orl	%ecx, %eax
	addl	%edx, %eax
	movd	%eax, %xmm2
	cvtss2sd	%xmm2, %xmm0
.L140:
	movsd	%xmm0, 37952(%rbp,%rbx,8)
.L75:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L77:
	.cfi_restore_state
	leaq	4744(%rbx,%rbx), %rax
	leaq	9488(,%rdi,4), %rdx
	pxor	%xmm0, %xmm0
	cvtss2sd	0(%rbp,%rdx,4), %xmm0
	movsd	%xmm0, 0(%rbp,%rax,8)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L79:
	leaq	9488(,%rbx,4), %rax
	leaq	4744(%rdi,%rdi), %rdx
	pxor	%xmm0, %xmm0
	cvtsd2ss	0(%rbp,%rdx,8), %xmm0
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L80:
	leaq	4744(%rcx,%rcx), %rax
	leaq	4744(%rdi,%rdi), %rdx
	movsd	0(%rbp,%rdx,8), %xmm0
	movsd	0(%rbp,%rax,8), %xmm1
	call	fmax@PLT
.L141:
	leaq	4744(%rbx,%rbx), %rax
	movsd	%xmm0, 0(%rbp,%rax,8)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L81:
	leaq	4744(%rcx,%rcx), %rax
	leaq	4744(%rdi,%rdi), %rdx
	movsd	0(%rbp,%rax,8), %xmm1
	movsd	0(%rbp,%rdx,8), %xmm0
	call	fmin@PLT
	leaq	4744(%rbx,%rbx), %rax
	movsd	%xmm0, 0(%rbp,%rax,8)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L82:
	leaq	4744(%rdi,%rdi), %rdx
	leaq	4744(%rbx,%rbx), %rax
	movsd	0(%rbp,%rdx,8), %xmm0
	xorpd	.LC2(%rip), %xmm0
	movsd	%xmm0, 0(%rbp,%rax,8)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L83:
	leaq	4744(%rdi,%rdi), %rax
	pxor	%xmm1, %xmm1
	movsd	0(%rbp,%rax,8), %xmm0
	comisd	%xmm0, %xmm1
	jbe	.L75
	jmp	.L141
	.p2align 4,,10
	.p2align 3
.L84:
	leaq	4744(%rdi,%rdi), %rcx
	leaq	4744(%r10,%r10), %rdx
	movsd	0(%rbp,%rcx,8), %xmm0
	mulsd	0(%rbp,%rdx,8), %xmm0
	leaq	4744(%rbx,%rbx), %rax
	movsd	%xmm0, 0(%rbp,%rax,8)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L85:
	leaq	4744(%rdi,%rdi), %rcx
	leaq	4744(%r10,%r10), %rdx
	movsd	0(%rbp,%rcx,8), %xmm0
	subsd	0(%rbp,%rdx,8), %xmm0
	leaq	4744(%rbx,%rbx), %rax
	movsd	%xmm0, 0(%rbp,%rax,8)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L86:
	leaq	4744(%rdi,%rdi), %rcx
	leaq	4744(%r10,%r10), %rdx
	movsd	0(%rbp,%rcx,8), %xmm0
	addsd	0(%rbp,%rdx,8), %xmm0
	leaq	4744(%rbx,%rbx), %rax
	movsd	%xmm0, 0(%rbp,%rax,8)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L88:
	leaq	4744(%rdi,%rdi), %rax
	movsd	0(%rbp,%rax,8), %xmm0
	call	sin@PLT
	leaq	4744(%rbx,%rbx), %rax
	movsd	%xmm0, 0(%rbp,%rax,8)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L89:
	movl	%r9d, %r9d
	pxor	%xmm0, %xmm0
	leaq	9488(%r9,%rdi,4), %rax
	cvtss2sd	0(%rbp,%rax,4), %xmm0
	call	sin@PLT
	leaq	9488(,%rbx,4), %rax
	cvtsd2ss	%xmm0, %xmm0
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L90:
	salq	$2, %rbx
	salq	$2, %rdi
	movl	$0x00000000, 37952(%rbp,%rbx,4)
	salq	$2, %r10
	testl	%r9d, %r9d
	je	.L75
	leal	-1(%r9), %edx
	leaq	37952(%rbp,%rbx,4), %rax
	subq	%rbx, %rdi
	subq	%rbx, %r10
	addq	%rbx, %rdx
	leaq	37956(%rbp,%rdx,4), %rdx
	.p2align 4,,10
	.p2align 3
.L123:
	movss	(%rax,%rdi,4), %xmm0
	mulss	(%rax,%r10,4), %xmm0
	addq	$4, %rax
	addss	-4(%rax), %xmm0
	movss	%xmm0, -4(%rax)
	cmpq	%rax, %rdx
	jne	.L123
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L91:
	salq	$4, %rdi
	leaq	9488(,%rbx,4), %rax
	addq	%rbp, %rdi
	movss	37952(%rdi), %xmm0
	addss	37956(%rdi), %xmm0
	addss	37960(%rdi), %xmm0
	addss	37964(%rdi), %xmm0
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L92:
	movl	%r9d, %r9d
	leaq	9488(%r9,%rdi,4), %rax
	movss	0(%rbp,%rax,4), %xmm0
	call	expf@PLT
	leaq	9488(,%rbx,4), %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L93:
	shrl	$10, %eax
	pxor	%xmm0, %xmm0
	pxor	%xmm1, %xmm1
	movl	%r9d, %r9d
	andl	$3, %eax
	leaq	9488(%r9,%rdi,4), %rdx
	leaq	9488(%rax,%rcx,4), %rax
	cvtss2sd	0(%rbp,%rdx,4), %xmm0
	cvtss2sd	0(%rbp,%rax,4), %xmm1
	call	atan2@PLT
	leaq	9488(,%rbx,4), %rax
	cvtsd2ss	%xmm0, %xmm0
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L94:
	movl	%r9d, %r9d
	pxor	%xmm0, %xmm0
	leaq	9488(%r9,%rdi,4), %rax
	cvtss2sd	0(%rbp,%rax,4), %xmm0
	call	atan@PLT
	leaq	9488(,%rbx,4), %rax
	cvtsd2ss	%xmm0, %xmm0
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L95:
	leaq	4744(%rdi,%rdi), %rax
	pxor	%xmm1, %xmm1
	addq	%rbx, %rbx
	movsd	0(%rbp,%rax,8), %xmm0
	ucomisd	%xmm0, %xmm1
	ja	.L138
	sqrtsd	%xmm0, %xmm0
	jmp	.L140
	.p2align 4,,10
	.p2align 3
.L96:
	leaq	4744(%rbx,%rbx), %rax
	leaq	4744(%rdi,%rdi), %rcx
	leaq	4744(%r10,%r10), %rdx
	movsd	0(%rbp,%rcx,8), %xmm0
	divsd	0(%rbp,%rdx,8), %xmm0
	movsd	%xmm0, 0(%rbp,%rax,8)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L97:
	movl	%r9d, %r9d
	pxor	%xmm1, %xmm1
	salq	$2, %rbx
	leaq	9488(%r9,%rdi,4), %rax
	movss	0(%rbp,%rax,4), %xmm0
	ucomiss	%xmm0, %xmm1
	ja	.L137
	sqrtss	%xmm0, %xmm0
.L121:
	movss	%xmm0, 37952(%rbp,%rbx,4)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L98:
	shrl	$10, %eax
	movl	%r9d, %r9d
	leaq	9488(,%rbx,4), %rdx
	andl	$3, %eax
	leaq	9488(%r9,%rdi,4), %rcx
	leaq	9488(%rax,%r10,4), %rax
	movss	0(%rbp,%rcx,4), %xmm0
	divss	0(%rbp,%rax,4), %xmm0
	movss	%xmm0, 0(%rbp,%rdx,4)
	jmp	.L75
	.p2align 4,,10
	.p2align 3
.L99:
	shrl	$10, %eax
	andl	$1023, %eax
	movq	%rax, %rcx
	movl	%eax, %edx
	orq	$-1024, %rcx
	cmpl	$512, %eax
	cmova	%rcx, %rdx
	leal	1(%r9), %ecx
	sall	$4, %ecx
.L145:
	addq	37440(%rbp,%rdi,8), %rdx
.L146:
	leaq	37952(%rbp,%rbx,4), %rsi
.L143:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	executeStore
	.p2align 4,,10
	.p2align 3
.L100:
	.cfi_restore_state
	shrl	$10, %eax
	andl	$1023, %eax
	movq	%rax, %rcx
	movl	%eax, %edx
	orq	$-1024, %rcx
	cmpl	$512, %eax
	cmova	%rcx, %rdx
	leal	1(%r9), %ecx
	addq	37440(%rbp,%rdi,8), %rdx
	sall	$4, %ecx
.L144:
	leaq	37952(%rbp,%rbx,4), %rsi
.L142:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	executeLoad
	.p2align 4,,10
	.p2align 3
.L101:
	.cfi_restore_state
	shrl	$10, %eax
	leaq	37440(%rbp,%rbx,8), %rsi
	andl	$1023, %eax
	movq	%rax, %rcx
	movl	%eax, %edx
	orq	$-1024, %rcx
	cmpl	$512, %eax
	movl	$1, %eax
	cmova	%rcx, %rdx
	movl	%r9d, %ecx
	addq	37440(%rbp,%rdi,8), %rdx
	sall	%cl, %eax
	movl	%eax, %ecx
	jmp	.L143
	.p2align 4,,10
	.p2align 3
.L102:
	shrl	$10, %eax
	leaq	37440(%rbp,%rbx,8), %rsi
	andl	$1023, %eax
	movq	%rax, %rcx
	movl	%eax, %edx
	orq	$-1024, %rcx
	cmpl	$512, %eax
	movl	$1, %eax
	cmova	%rcx, %rdx
	movl	%r9d, %ecx
	addq	37440(%rbp,%rdi,8), %rdx
	sall	%cl, %eax
	movl	%eax, %ecx
	jmp	.L142
	.p2align 4,,10
	.p2align 3
.L103:
	leal	1(%r9), %ecx
	movq	37440(%rbp,%r10,8), %rdx
	sall	$4, %ecx
	jmp	.L145
	.p2align 4,,10
	.p2align 3
.L104:
	leal	1(%r9), %ecx
	movq	37440(%rbp,%r10,8), %rdx
	addq	37440(%rbp,%rdi,8), %rdx
	sall	$4, %ecx
	jmp	.L144
	.p2align 4,,10
	.p2align 3
.L105:
	movl	%r9d, %ecx
	movl	$1, %eax
	movq	37440(%rbp,%r10,8), %rdx
	leaq	37440(%rbp,%rbx,8), %rsi
	sall	%cl, %eax
	addq	37440(%rbp,%rdi,8), %rdx
	movl	%eax, %ecx
	jmp	.L143
	.p2align 4,,10
	.p2align 3
.L106:
	movl	%r9d, %ecx
	movl	$1, %eax
	movq	37440(%rbp,%r10,8), %rdx
	leaq	37440(%rbp,%rbx,8), %rsi
	sall	%cl, %eax
	addq	37440(%rbp,%rdi,8), %rdx
	movl	%eax, %ecx
	jmp	.L142
	.p2align 4,,10
	.p2align 3
.L107:
	leal	1(%r9), %ecx
	shrl	$10, %eax
	movzwl	%ax, %edx
	sall	$4, %ecx
	addq	37448(%rbp), %rdx
	jmp	.L146
	.p2align 4,,10
	.p2align 3
.L108:
	leal	1(%r9), %ecx
	shrl	$10, %eax
	movzwl	%ax, %edx
	sall	$4, %ecx
	addq	37448(%rbp), %rdx
	jmp	.L144
	.p2align 4,,10
	.p2align 3
.L109:
	movl	%r9d, %ecx
	movl	$1, %edx
	shrl	$10, %eax
	leaq	37440(%rbp,%rbx,8), %rsi
	sall	%cl, %edx
	movl	%edx, %ecx
	movzwl	%ax, %edx
	addq	37448(%rbp), %rdx
	jmp	.L143
	.p2align 4,,10
	.p2align 3
.L110:
	movl	%r9d, %ecx
	movl	$1, %edx
	shrl	$10, %eax
	leaq	37440(%rbp,%rbx,8), %rsi
	sall	%cl, %edx
	movl	%edx, %ecx
	movzwl	%ax, %edx
	addq	37448(%rbp), %rdx
	jmp	.L142
	.p2align 4,,10
	.p2align 3
.L111:
	leal	1(%r9), %ecx
	shrl	$10, %eax
	movzwl	%ax, %edx
	sall	$4, %ecx
	addq	37440(%rbp), %rdx
	jmp	.L146
	.p2align 4,,10
	.p2align 3
.L112:
	leal	1(%r9), %ecx
	shrl	$10, %eax
	movzwl	%ax, %edx
	sall	$4, %ecx
	addq	37440(%rbp), %rdx
	jmp	.L144
	.p2align 4,,10
	.p2align 3
.L113:
	movl	%r9d, %ecx
	movl	$1, %edx
	shrl	$10, %eax
	leaq	37440(%rbp,%rbx,8), %rsi
	sall	%cl, %edx
	movl	%edx, %ecx
	movzwl	%ax, %edx
	addq	37440(%rbp), %rdx
	jmp	.L143
	.p2align 4,,10
	.p2align 3
.L114:
	movl	%r9d, %ecx
	movl	$1, %edx
	shrl	$10, %eax
	leaq	37440(%rbp,%rbx,8), %rsi
	sall	%cl, %edx
	movl	%edx, %ecx
	movzwl	%ax, %edx
	addq	37440(%rbp), %rdx
	jmp	.L142
	.p2align 4,,10
	.p2align 3
.L128:
	sall	$16, %eax
	orl	$939524096, %eax
	testl	%edx, %edx
	cmovne	%eax, %edx
	jmp	.L129
.L137:
	call	sqrtf@PLT
	jmp	.L121
.L138:
	call	sqrt@PLT
	jmp	.L140
	.cfi_endproc
.LFE28:
	.size	AX_execute_unit1A, .-AX_execute_unit1A
	.p2align 4
	.globl	AX_execute_unit1
	.type	AX_execute_unit1, @function
AX_execute_unit1:
.LFB29:
	.cfi_startproc
	movq	%rdx, %r10
	cmpl	$23, %r8d
	ja	.L147
	leaq	.L150(%rip), %rdx
	movl	%r8d, %r8d
	movslq	(%rdx,%r8,4), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L150:
	.long	.L147-.L150
	.long	.L166-.L150
	.long	.L165-.L150
	.long	.L164-.L150
	.long	.L163-.L150
	.long	.L162-.L150
	.long	.L161-.L150
	.long	.L160-.L150
	.long	.L159-.L150
	.long	.L158-.L150
	.long	.L147-.L150
	.long	.L147-.L150
	.long	.L147-.L150
	.long	.L147-.L150
	.long	.L147-.L150
	.long	.L147-.L150
	.long	.L157-.L150
	.long	.L156-.L150
	.long	.L155-.L150
	.long	.L154-.L150
	.long	.L153-.L150
	.long	.L152-.L150
	.long	.L149-.L150
	.long	.L149-.L150
	.text
	.p2align 4,,10
	.p2align 3
.L147:
	ret
	.p2align 4,,10
	.p2align 3
.L149:
	movl	8(%rsp), %edx
	movq	37440(%rdi,%r10,8), %rax
	movl	%r9d, %r9d
	shrl	$11, %edx
	andl	$511, %edx
	testl	$1024, 8(%rsp)
	cmove	37440(%rdi,%rcx,8), %edx
	movl	%edx, %ecx
	leaq	sizemask(%rip), %rdx
	shrq	%cl, %rax
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L166:
	addq	$4680, %r10
	movq	(%rdi,%r10,8), %rax
	testl	%r9d, %r9d
	je	.L231
	cmpl	$1, %r9d
	je	.L232
	cmpl	$2, %r9d
	jne	.L229
	testl	$2147483648, %eax
	je	.L229
	movabsq	$-4294967296, %rdx
	orq	%rdx, %rax
	movq	%rax, (%rdi,%r10,8)
	jmp	.L229
	.p2align 4,,10
	.p2align 3
.L165:
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	movq	37440(%rdi,%r10,8), %rax
	andq	(%rdx,%r9,8), %rax
	setne	%al
	movzbl	%al, %eax
.L229:
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L164:
	movl	8(%rsp), %eax
	sall	$4, %r9d
	movl	%r9d, %ecx
	shrl	$10, %eax
	movzwl	%ax, %eax
	salq	%cl, %rax
	orq	%rax, 37440(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L163:
	leaq	sizemask(%rip), %rdx
	movl	%r9d, %eax
	movq	37440(%rdi,%rsi,8), %rcx
	movq	(%rdx,%rax,8), %rax
	andq	%rax, %rcx
	andq	37440(%rdi,%r10,8), %rax
	movq	%rax, %rdx
	testl	%r9d, %r9d
	jne	.L170
	cmpl	$128, %ecx
	movl	%ecx, %ecx
	jbe	.L171
	orq	$-256, %rcx
	cmpl	$128, %edx
	movl	%edx, %edx
	ja	.L172
.L228:
	movzwl	184(%rdi), %eax
	andl	$-8, %eax
	jmp	.L230
	.p2align 4,,10
	.p2align 3
.L162:
	leaq	sizemask(%rip), %rax
	movl	%r9d, %r9d
	movq	37440(%rdi,%rsi,8), %rcx
	movq	(%rax,%r9,8), %rdx
	movzwl	184(%rdi), %eax
	andq	%rdx, %rcx
	andl	$-8, %eax
	andq	37440(%rdi,%r10,8), %rdx
	movq	%rcx, %rsi
	movl	%eax, %ecx
	orl	$4, %ecx
	cmpq	%rdx, %rsi
	cmovb	%ecx, %eax
.L230:
	movw	%ax, 184(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L161:
	movl	%r9d, %edx
	leaq	sizemask(%rip), %rcx
	movq	37440(%rdi,%rsi,8), %rax
	andq	(%rcx,%rdx,8), %rax
	movl	8(%rsp), %edx
	shrl	$10, %edx
	andl	$1023, %edx
	movq	%rdx, %rsi
	movl	%edx, %ecx
	orq	$-1024, %rsi
	cmpl	$512, %edx
	cmova	%rsi, %rcx
	testl	%r9d, %r9d
	je	.L233
	cmpl	$1, %r9d
	je	.L234
	cmpl	$2, %r9d
	je	.L235
.L186:
	movzwl	184(%rdi), %edx
	andl	$-8, %edx
	movl	%edx, %esi
	orl	$4, %esi
	cmpq	%rcx, %rax
	cmovb	%esi, %edx
	movw	%dx, 184(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L160:
	leaq	sizemask(%rip), %rax
	movl	%r9d, %r9d
	movq	37440(%rdi,%rsi,8), %rcx
	andq	(%rax,%r9,8), %rcx
	movl	8(%rsp), %eax
	shrl	$10, %eax
	andl	$1023, %eax
	movq	%rax, %rsi
	movl	%eax, %edx
	orq	$-1024, %rsi
	cmpl	$512, %eax
	movzwl	184(%rdi), %eax
	cmova	%rsi, %rdx
	andl	$-8, %eax
	movl	%eax, %esi
	orl	$4, %esi
	cmpq	%rdx, %rcx
	cmovb	%esi, %eax
	movw	%ax, 184(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L159:
	movl	8(%rsp), %eax
	shrl	$8, %eax
	orq	$-262144, %rax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L158:
	movl	8(%rsp), %eax
	addq	$4680, %rsi
	shrl	$8, %eax
	andl	$262143, %eax
	movq	%rax, (%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L157:
	movq	37440(%rdi,%r10,8), %rdx
	movq	37440(%rdi,%rcx,8), %rax
	testl	$1024, 8(%rsp)
	je	.L191
	movl	8(%rsp), %eax
	shrl	$11, %eax
	andl	$511, %eax
.L191:
	addq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L156:
	movq	37440(%rdi,%r10,8), %rax
	movq	37440(%rdi,%rcx,8), %rdx
	testl	$1024, 8(%rsp)
	je	.L192
	movl	8(%rsp), %edx
	shrl	$11, %edx
	andl	$511, %edx
.L192:
	subq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L155:
	movq	37440(%rdi,%r10,8), %rax
	movq	37440(%rdi,%rcx,8), %rdx
	testl	$1024, 8(%rsp)
	je	.L193
	movl	8(%rsp), %edx
	shrl	$11, %edx
	andl	$511, %edx
.L193:
	xorq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L153:
	movq	37440(%rdi,%r10,8), %rax
	movq	37440(%rdi,%rcx,8), %rdx
	testl	$1024, 8(%rsp)
	je	.L195
	movl	8(%rsp), %edx
	shrl	$11, %edx
	andl	$511, %edx
.L195:
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %r8
	andq	(%r8,%r9,8), %rax
	andq	%rdx, %rax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L154:
	movq	37440(%rdi,%r10,8), %rax
	movq	37440(%rdi,%rcx,8), %rdx
	testl	$1024, 8(%rsp)
	je	.L194
	movl	8(%rsp), %edx
	shrl	$11, %edx
	andl	$511, %edx
.L194:
	orq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L152:
	movl	8(%rsp), %edx
	movq	37440(%rdi,%r10,8), %rax
	movl	%r9d, %r9d
	shrl	$11, %edx
	andl	$511, %edx
	testl	$1024, 8(%rsp)
	cmove	37440(%rdi,%rcx,8), %edx
	movl	%edx, %ecx
	leaq	sizemask(%rip), %rdx
	salq	%cl, %rax
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L233:
	movl	%eax, %esi
	movl	%eax, %eax
	movq	%rax, %rdx
	orq	$-256, %rdx
	cmpl	$128, %esi
	cmova	%rdx, %rax
	jmp	.L186
	.p2align 4,,10
	.p2align 3
.L170:
	cmpl	$1, %r9d
	je	.L236
	cmpl	$2, %r9d
	je	.L237
.L174:
	movzwl	184(%rdi), %eax
	andl	$-8, %eax
	movl	%eax, %esi
	orl	$4, %esi
	cmpq	%rdx, %rcx
	cmovb	%esi, %eax
	jmp	.L230
	.p2align 4,,10
	.p2align 3
.L231:
	testb	$-128, %al
	je	.L229
	orq	$-256, %rax
	movq	%rax, (%rdi,%r10,8)
	jmp	.L229
	.p2align 4,,10
	.p2align 3
.L171:
	cmpl	$128, %edx
	movl	%edx, %edx
	jbe	.L174
.L172:
	orq	$-256, %rdx
	jmp	.L174
.L232:
	testb	$-128, %ah
	je	.L229
	orq	$-65536, %rax
	movq	%rax, (%rdi,%r10,8)
	jmp	.L229
.L236:
	cmpl	$32768, %ecx
	movl	%ecx, %ecx
	jbe	.L176
	orq	$-65536, %rcx
	cmpl	$32768, %edx
	movl	%edx, %edx
	jbe	.L228
.L177:
	orq	$-65536, %rdx
	jmp	.L174
.L234:
	movl	%eax, %esi
	movl	%eax, %eax
	movq	%rax, %rdx
	orq	$-65536, %rdx
	cmpl	$32768, %esi
	cmova	%rdx, %rax
	jmp	.L186
.L237:
	cmpl	$-2147483648, %ecx
	movl	%ecx, %ecx
	jbe	.L179
	movabsq	$-4294967296, %rax
	orq	%rax, %rcx
	cmpl	$-2147483648, %edx
	movl	%edx, %edx
	jbe	.L228
.L180:
	movabsq	$-4294967296, %rax
	orq	%rax, %rdx
	jmp	.L174
.L235:
	cmpl	$-2147483648, %eax
	movl	%eax, %eax
	jbe	.L186
	movabsq	$-4294967296, %rdx
	orq	%rdx, %rax
	jmp	.L186
.L176:
	cmpl	$32768, %edx
	movl	%edx, %edx
	jbe	.L174
	jmp	.L177
.L179:
	cmpl	$-2147483648, %edx
	movl	%edx, %edx
	ja	.L180
	jmp	.L174
	.cfi_endproc
.LFE29:
	.size	AX_execute_unit1, .-AX_execute_unit1
	.p2align 4
	.globl	AX_execute_unit0A
	.type	AX_execute_unit0A, @function
AX_execute_unit0A:
.LFB30:
	.cfi_startproc
	movq	%rcx, %r10
	leal	-12(%r8), %ecx
	movl	8(%rsp), %eax
	cmpl	$115, %ecx
	ja	.L238
	leaq	.L241(%rip), %r11
	movslq	(%r11,%rcx,4), %rcx
	addq	%r11, %rcx
	jmp	*%rcx
	.section	.rodata
	.align 4
	.align 4
.L241:
	.long	.L294-.L241
	.long	.L293-.L241
	.long	.L292-.L241
	.long	.L291-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L290-.L241
	.long	.L289-.L241
	.long	.L288-.L241
	.long	.L287-.L241
	.long	.L286-.L241
	.long	.L285-.L241
	.long	.L284-.L241
	.long	.L283-.L241
	.long	.L282-.L241
	.long	.L281-.L241
	.long	.L280-.L241
	.long	.L279-.L241
	.long	.L278-.L241
	.long	.L277-.L241
	.long	.L276-.L241
	.long	.L275-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L274-.L241
	.long	.L273-.L241
	.long	.L272-.L241
	.long	.L271-.L241
	.long	.L270-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L269-.L241
	.long	.L269-.L241
	.long	.L269-.L241
	.long	.L269-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L268-.L241
	.long	.L267-.L241
	.long	.L266-.L241
	.long	.L265-.L241
	.long	.L264-.L241
	.long	.L263-.L241
	.long	.L262-.L241
	.long	.L261-.L241
	.long	.L260-.L241
	.long	.L259-.L241
	.long	.L258-.L241
	.long	.L257-.L241
	.long	.L256-.L241
	.long	.L255-.L241
	.long	.L254-.L241
	.long	.L253-.L241
	.long	.L252-.L241
	.long	.L251-.L241
	.long	.L250-.L241
	.long	.L249-.L241
	.long	.L248-.L241
	.long	.L247-.L241
	.long	.L246-.L241
	.long	.L245-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L244-.L241
	.long	.L243-.L241
	.long	.L238-.L241
	.long	.L238-.L241
	.long	.L242-.L241
	.long	.L240-.L241
	.text
	.p2align 4,,10
	.p2align 3
.L269:
	movb	%r8b, 187(%rdi)
	movb	$1, 186(%rdi)
.L238:
	ret
.L270:
	movl	$1, 164(%rdi)
	ret
.L271:
	shrl	$10, %eax
	pxor	%xmm1, %xmm1
	cvtss2sd	37952(%rdi,%rsi,8), %xmm1
	movzwl	%ax, %ecx
	movl	%ecx, %edx
	shrl	$3, %edx
	andl	$1920, %edx
	testb	$64, %ah
	movl	%edx, %eax
	je	.L310
	sall	$16, %eax
	orl	$1073741824, %eax
	movl	%eax, %edx
.L311:
	movl	%ecx, %eax
	sall	$13, %ecx
	pxor	%xmm0, %xmm0
	sall	$16, %eax
	andl	$8380416, %ecx
	andl	$-2147483648, %eax
	orl	%ecx, %eax
	movl	$1, %ecx
	addl	%edx, %eax
	xorl	%edx, %edx
	movd	%eax, %xmm4
	movzwl	184(%rdi), %eax
	cvtss2sd	%xmm4, %xmm0
	subsd	%xmm0, %xmm1
	pxor	%xmm0, %xmm0
	andl	$-8, %eax
	ucomisd	%xmm0, %xmm1
	setp	%dl
	cmovne	%ecx, %edx
	orl	%edx, %eax
	movl	%eax, %edx
	orl	$6, %edx
	comisd	%xmm1, %xmm0
	cmova	%edx, %eax
	movw	%ax, 184(%rdi)
	ret
.L272:
	shrl	$10, %eax
	movl	%r9d, %r9d
	movzwl	%ax, %ecx
	leaq	9488(%r9,%rsi,4), %rdx
	movss	(%rdi,%rdx,4), %xmm0
	movl	%ecx, %edx
	shrl	$3, %edx
	andl	$1920, %edx
	testb	$64, %ah
	movl	%edx, %eax
	je	.L306
	sall	$16, %eax
	orl	$1073741824, %eax
	movl	%eax, %edx
.L307:
	movl	%ecx, %eax
	sall	$13, %ecx
	sall	$16, %eax
	andl	$8380416, %ecx
	andl	$-2147483648, %eax
	orl	%ecx, %eax
	addl	%edx, %eax
	movd	%eax, %xmm3
	subss	%xmm3, %xmm0
.L418:
	movzwl	184(%rdi), %eax
	pxor	%xmm1, %xmm1
	xorl	%edx, %edx
	movl	$1, %ecx
	andl	$-8, %eax
	ucomiss	%xmm1, %xmm0
	setp	%dl
	cmovne	%ecx, %edx
	orl	%edx, %eax
	movl	%eax, %edx
	orl	$6, %edx
	comiss	%xmm0, %xmm1
	cmova	%edx, %eax
	movw	%ax, 184(%rdi)
	ret
.L273:
	movzwl	184(%rdi), %eax
	pxor	%xmm1, %xmm1
	xorl	%edx, %edx
	movl	$1, %ecx
	andl	$-8, %eax
	ucomisd	%xmm1, %xmm1
	setp	%dl
	cmovne	%ecx, %edx
	orl	%edx, %eax
	movl	%eax, %edx
	orl	$6, %edx
	comisd	%xmm1, %xmm1
	cmova	%edx, %eax
	movw	%ax, 184(%rdi)
	ret
.L274:
	shrl	$10, %eax
	movl	%r9d, %r9d
	andl	$3, %eax
	leaq	9488(%r9,%rsi,4), %rcx
	leaq	9488(%rax,%rdx,4), %rax
	movss	(%rdi,%rcx,4), %xmm0
	subss	(%rdi,%rax,4), %xmm0
	jmp	.L418
.L275:
	shrl	$10, %eax
	leaq	37952(%rdi,%rsi,4), %rsi
	andl	$1023, %eax
	movq	%rax, %r8
	movl	%eax, %ecx
	orq	$-1024, %r8
	cmpl	$512, %eax
	cmova	%r8, %rcx
	addl	$1, %r9d
	addq	37440(%rdi,%rdx,8), %rcx
	sall	$4, %r9d
	movq	%rcx, %rdx
	movl	%r9d, %ecx
	jmp	executeStore
.L276:
	shrl	$10, %eax
	leaq	37952(%rdi,%rsi,4), %rsi
	andl	$1023, %eax
	movq	%rax, %r8
	movl	%eax, %ecx
	orq	$-1024, %r8
	cmpl	$512, %eax
	cmova	%r8, %rcx
	addl	$1, %r9d
	addq	37440(%rdi,%rdx,8), %rcx
	sall	$4, %r9d
	movq	%rcx, %rdx
	movl	%r9d, %ecx
	jmp	executeLoad
.L277:
	shrl	$10, %eax
	leaq	37440(%rdi,%rsi,8), %rsi
	andl	$1023, %eax
	movq	%rax, %rcx
	movl	%eax, %r8d
	orq	$-1024, %rcx
	cmpl	$512, %eax
	movl	$1, %eax
	cmova	%rcx, %r8
	movl	%r9d, %ecx
	addq	37440(%rdi,%rdx,8), %r8
	sall	%cl, %eax
	movq	%r8, %rdx
	movl	%eax, %ecx
	jmp	executeStore
.L278:
	shrl	$10, %eax
	leaq	37440(%rdi,%rsi,8), %rsi
	andl	$1023, %eax
	movq	%rax, %rcx
	movl	%eax, %r8d
	orq	$-1024, %rcx
	cmpl	$512, %eax
	movl	$1, %eax
	cmova	%rcx, %r8
	movl	%r9d, %ecx
	addq	37440(%rdi,%rdx,8), %r8
	sall	%cl, %eax
	movq	%r8, %rdx
	movl	%eax, %ecx
	jmp	executeLoad
.L279:
	movq	37440(%rdi,%r10,8), %rax
	leal	1(%r9), %ecx
	addq	37440(%rdi,%rdx,8), %rax
	leaq	37952(%rdi,%rsi,4), %rsi
	sall	$4, %ecx
	movq	%rax, %rdx
	jmp	executeStore
.L280:
	movq	37440(%rdi,%r10,8), %rax
	leal	1(%r9), %ecx
	addq	37440(%rdi,%rdx,8), %rax
	leaq	37952(%rdi,%rsi,4), %rsi
	sall	$4, %ecx
	movq	%rax, %rdx
	jmp	executeLoad
.L281:
	movl	%r9d, %ecx
	movl	$1, %eax
	leaq	37440(%rdi,%rsi,8), %rsi
	sall	%cl, %eax
	movl	%eax, %ecx
	movq	37440(%rdi,%r10,8), %rax
	addq	37440(%rdi,%rdx,8), %rax
	movq	%rax, %rdx
	jmp	executeStore
.L282:
	movl	%r9d, %ecx
	movl	$1, %eax
	leaq	37440(%rdi,%rsi,8), %rsi
	sall	%cl, %eax
	movl	%eax, %ecx
	movq	37440(%rdi,%r10,8), %rax
	addq	37440(%rdi,%rdx,8), %rax
	movq	%rax, %rdx
	jmp	executeLoad
.L283:
	leal	1(%r9), %ecx
	shrl	$10, %eax
	leaq	37952(%rdi,%rsi,4), %rsi
	movzwl	%ax, %edx
	sall	$4, %ecx
	addq	37448(%rdi), %rdx
	jmp	executeStore
.L284:
	leal	1(%r9), %ecx
	shrl	$10, %eax
	leaq	37952(%rdi,%rsi,4), %rsi
	movzwl	%ax, %edx
	sall	$4, %ecx
	addq	37448(%rdi), %rdx
	jmp	executeLoad
.L285:
	movl	%r9d, %ecx
	movl	$1, %edx
	shrl	$10, %eax
	leaq	37440(%rdi,%rsi,8), %rsi
	sall	%cl, %edx
	movl	%edx, %ecx
	movzwl	%ax, %edx
	addq	37448(%rdi), %rdx
	jmp	executeStore
.L286:
	movl	%r9d, %ecx
	movl	$1, %edx
	shrl	$10, %eax
	leaq	37440(%rdi,%rsi,8), %rsi
	sall	%cl, %edx
	movl	%edx, %ecx
	movzwl	%ax, %edx
	addq	37448(%rdi), %rdx
	jmp	executeLoad
.L287:
	leal	1(%r9), %ecx
	shrl	$10, %eax
	leaq	37952(%rdi,%rsi,4), %rsi
	movzwl	%ax, %edx
	sall	$4, %ecx
	addq	37440(%rdi), %rdx
	jmp	executeStore
.L288:
	leal	1(%r9), %ecx
	shrl	$10, %eax
	leaq	37952(%rdi,%rsi,4), %rsi
	movzwl	%ax, %edx
	sall	$4, %ecx
	addq	37440(%rdi), %rdx
	jmp	executeLoad
.L289:
	movl	%r9d, %ecx
	movl	$1, %edx
	shrl	$10, %eax
	leaq	37440(%rdi,%rsi,8), %rsi
	sall	%cl, %edx
	movl	%edx, %ecx
	movzwl	%ax, %edx
	addq	37440(%rdi), %rdx
	jmp	executeStore
.L290:
	movl	%r9d, %ecx
	movl	$1, %edx
	shrl	$10, %eax
	leaq	37440(%rdi,%rsi,8), %rsi
	sall	%cl, %edx
	movl	%edx, %ecx
	movzwl	%ax, %edx
	addq	37440(%rdi), %rdx
	jmp	executeLoad
.L291:
	andl	$3, %edx
	jne	.L298
	movq	37440(%rdi,%rsi,8), %rax
	movl	%eax, 152(%rdi)
	ret
.L292:
	andl	$3, %edx
	jne	.L295
	movl	152(%rdi), %eax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
.L293:
	movq	96(%rdi), %rax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
.L253:
	salq	$4, %rdx
	movl	%eax, %ecx
	salq	$2, %rsi
	addq	%rdi, %rdx
	shrl	$10, %ecx
	movss	37952(%rdx), %xmm0
	andl	$3, %ecx
	leaq	9488(%rsi,%rcx), %rcx
	movss	%xmm0, (%rdi,%rcx,4)
	movl	%eax, %ecx
	movss	37956(%rdx), %xmm0
	shrl	$12, %ecx
	andl	$3, %ecx
	leaq	9488(%rsi,%rcx), %rcx
	movss	%xmm0, (%rdi,%rcx,4)
	movl	%eax, %ecx
	movss	37960(%rdx), %xmm0
	shrl	$16, %eax
	shrl	$14, %ecx
	andl	$3, %eax
	andl	$3, %ecx
	leaq	9488(%rsi,%rax), %rax
	leaq	9488(%rsi,%rcx), %rcx
	movss	%xmm0, (%rdi,%rcx,4)
	movss	37964(%rdx), %xmm0
	movss	%xmm0, (%rdi,%rax,4)
	ret
.L254:
	shrl	$10, %eax
	salq	$2, %rsi
	andl	$3, %eax
	salq	$2, %rdx
	leaq	9488(%rax,%r10,4), %rax
	movss	(%rdi,%rax,4), %xmm1
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	leaq	37952(%rdi,%rsi,4), %rax
	subq	%rsi, %rdx
	addq	%rsi, %r9
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L322:
	movss	(%rax,%rdx,4), %xmm0
	addq	$4, %rax
	subss	%xmm1, %xmm0
	mulss	-4(%rax), %xmm0
	movss	%xmm0, -4(%rax)
	cmpq	%rax, %rcx
	jne	.L322
	ret
.L255:
	salq	$2, %rdx
	salq	$2, %r10
	cmpl	$-1, %r9d
	je	.L238
	salq	$2, %rsi
	movl	%r9d, %r9d
	addq	%rsi, %r9
	leaq	37952(%rdi,%rsi,4), %rax
	subq	%rsi, %rdx
	subq	%rsi, %r10
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L317:
	movss	(%rax,%rdx,4), %xmm0
	subss	(%rax,%r10,4), %xmm0
	addq	$4, %rax
	mulss	-4(%rax), %xmm0
	movss	%xmm0, -4(%rax)
	cmpq	%rax, %rcx
	jne	.L317
	ret
.L256:
	shrl	$10, %eax
	movl	%r9d, %r9d
	leaq	9488(,%rsi,4), %rcx
	andl	$3, %eax
	leaq	9488(%r9,%rdx,4), %rdx
	leaq	9488(%rax,%r10,4), %rax
	movss	(%rdi,%rdx,4), %xmm0
	subss	(%rdi,%rax,4), %xmm0
	mulss	(%rdi,%rcx,4), %xmm0
	movss	%xmm0, (%rdi,%rcx,4)
	ret
.L257:
	shrl	$10, %eax
	salq	$2, %rsi
	andl	$3, %eax
	salq	$2, %rdx
	leaq	9488(%rax,%r10,4), %rax
	movss	(%rdi,%rax,4), %xmm1
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	leaq	37952(%rdi,%rsi,4), %rax
	subq	%rsi, %rdx
	addq	%rsi, %r9
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L321:
	movss	(%rax,%rdx,4), %xmm0
	addq	$4, %rax
	addss	%xmm1, %xmm0
	mulss	-4(%rax), %xmm0
	movss	%xmm0, -4(%rax)
	cmpq	%rcx, %rax
	jne	.L321
	ret
.L258:
	shrl	$10, %eax
	salq	$2, %rsi
	andl	$3, %eax
	salq	$2, %rdx
	leaq	9488(%rax,%r10,4), %rax
	movss	(%rdi,%rax,4), %xmm1
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	leaq	37952(%rdi,%rdx,4), %rax
	subq	%rdx, %rsi
	addq	%rdx, %r9
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L320:
	movss	(%rax), %xmm0
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%rax,%rsi,4)
	addq	$4, %rax
	cmpq	%rax, %rcx
	jne	.L320
	ret
.L259:
	shrl	$10, %eax
	salq	$2, %rsi
	andl	$3, %eax
	salq	$2, %rdx
	leaq	9488(%rax,%r10,4), %rax
	movss	(%rdi,%rax,4), %xmm1
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	leaq	37952(%rdi,%rdx,4), %rax
	subq	%rdx, %rsi
	addq	%rdx, %r9
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L319:
	movss	(%rax), %xmm0
	subss	%xmm1, %xmm0
	movss	%xmm0, (%rax,%rsi,4)
	addq	$4, %rax
	cmpq	%rax, %rcx
	jne	.L319
	ret
.L260:
	shrl	$10, %eax
	salq	$2, %rsi
	andl	$3, %eax
	salq	$2, %rdx
	leaq	9488(%rax,%r10,4), %rax
	movss	(%rdi,%rax,4), %xmm1
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	leaq	37952(%rdi,%rdx,4), %rax
	subq	%rdx, %rsi
	addq	%rdx, %r9
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L318:
	movss	(%rax), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, (%rax,%rsi,4)
	addq	$4, %rax
	cmpq	%rax, %rcx
	jne	.L318
	ret
.L261:
	salq	$2, %rdx
	salq	$2, %r10
	cmpl	$-1, %r9d
	je	.L238
	salq	$2, %rsi
	movl	%r9d, %r9d
	addq	%rsi, %r9
	leaq	37952(%rdi,%rsi,4), %rax
	subq	%rsi, %rdx
	subq	%rsi, %r10
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L316:
	movss	(%rax,%rdx,4), %xmm0
	addss	(%rax,%r10,4), %xmm0
	addq	$4, %rax
	mulss	-4(%rax), %xmm0
	movss	%xmm0, -4(%rax)
	cmpq	%rax, %rcx
	jne	.L316
	ret
.L262:
	salq	$2, %rdx
	salq	$2, %r10
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	salq	$2, %rsi
	leaq	37952(%rdi,%rdx,4), %rax
	subq	%rdx, %r10
	addq	%rdx, %r9
	subq	%rdx, %rsi
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L315:
	movss	(%rax,%r10,4), %xmm0
	mulss	(%rax), %xmm0
	movss	%xmm0, (%rax,%rsi,4)
	addq	$4, %rax
	cmpq	%rax, %rcx
	jne	.L315
	ret
.L263:
	salq	$2, %rdx
	salq	$2, %r10
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	salq	$2, %rsi
	leaq	37952(%rdi,%rdx,4), %rax
	subq	%rdx, %r10
	addq	%rdx, %r9
	subq	%rdx, %rsi
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L314:
	movss	(%rax), %xmm0
	subss	(%rax,%r10,4), %xmm0
	movss	%xmm0, (%rax,%rsi,4)
	addq	$4, %rax
	cmpq	%rax, %rcx
	jne	.L314
	ret
.L264:
	salq	$2, %rdx
	salq	$2, %r10
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	salq	$2, %rsi
	leaq	37952(%rdi,%rdx,4), %rax
	subq	%rdx, %r10
	addq	%rdx, %r9
	subq	%rdx, %rsi
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L313:
	movss	(%rax,%r10,4), %xmm0
	addss	(%rax), %xmm0
	movss	%xmm0, (%rax,%rsi,4)
	addq	$4, %rax
	cmpq	%rax, %rcx
	jne	.L313
	ret
.L265:
	shrl	$10, %eax
	movl	%r9d, %r9d
	leaq	9488(,%rsi,4), %rcx
	andl	$3, %eax
	leaq	9488(%rax,%r10,4), %rsi
	leaq	9488(%r9,%rdx,4), %rax
	movss	(%rdi,%rsi,4), %xmm0
	addss	(%rdi,%rax,4), %xmm0
	mulss	(%rdi,%rcx,4), %xmm0
	movss	%xmm0, (%rdi,%rcx,4)
	ret
.L266:
	shrl	$10, %eax
	movl	%r9d, %r9d
	leaq	9488(,%rsi,4), %rcx
	andl	$3, %eax
	leaq	9488(%rax,%r10,4), %rsi
	leaq	9488(%r9,%rdx,4), %rax
	movss	(%rdi,%rsi,4), %xmm0
	mulss	(%rdi,%rax,4), %xmm0
	movss	%xmm0, (%rdi,%rcx,4)
	ret
.L267:
	shrl	$10, %eax
	movl	%r9d, %r9d
	leaq	9488(,%rsi,4), %rcx
	andl	$3, %eax
	leaq	9488(%r9,%rdx,4), %rdx
	leaq	9488(%rax,%r10,4), %rax
	movss	(%rdi,%rdx,4), %xmm0
	subss	(%rdi,%rax,4), %xmm0
	movss	%xmm0, (%rdi,%rcx,4)
	ret
.L268:
	shrl	$10, %eax
	movl	%r9d, %r9d
	leaq	9488(,%rsi,4), %rcx
	andl	$3, %eax
	leaq	9488(%rax,%r10,4), %rsi
	leaq	9488(%r9,%rdx,4), %rax
	movss	(%rdi,%rsi,4), %xmm0
	addss	(%rdi,%rax,4), %xmm0
	movss	%xmm0, (%rdi,%rcx,4)
	ret
.L245:
	salq	$2, %rsi
	salq	$2, %rdx
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	leaq	37952(%rdi,%rdx,4), %rax
	pxor	%xmm1, %xmm1
	subq	%rdx, %rsi
	addq	%rdx, %r9
	movss	.LC3(%rip), %xmm2
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L333:
	movss	(%rax), %xmm0
	comiss	%xmm0, %xmm1
	jbe	.L331
	xorps	%xmm2, %xmm0
	movss	%xmm0, (%rax,%rsi,4)
.L331:
	addq	$4, %rax
	cmpq	%rcx, %rax
	jne	.L333
	ret
.L246:
	salq	$2, %rsi
	salq	$2, %rdx
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	leaq	37952(%rdi,%rdx,4), %rax
	movss	.LC3(%rip), %xmm1
	subq	%rdx, %rsi
	addq	%rdx, %r9
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L330:
	movss	(%rax), %xmm0
	xorps	%xmm1, %xmm0
	movss	%xmm0, (%rax,%rsi,4)
	addq	$4, %rax
	cmpq	%rcx, %rax
	jne	.L330
	ret
.L247:
	shrl	$10, %eax
	pxor	%xmm1, %xmm1
	andl	$3, %eax
	leaq	9488(%rax,%rdx,4), %rax
	movss	(%rdi,%rax,4), %xmm0
	comiss	%xmm0, %xmm1
	jbe	.L238
	xorps	.LC3(%rip), %xmm0
	movl	%r9d, %r9d
	leaq	9488(%r9,%rsi,4), %rax
	movss	%xmm0, (%rdi,%rax,4)
	ret
.L248:
	shrl	$10, %eax
	movl	%r9d, %r9d
	andl	$3, %eax
	leaq	9488(%r9,%rsi,4), %rcx
	leaq	9488(%rax,%rdx,4), %rax
	movss	(%rdi,%rax,4), %xmm0
	xorps	.LC3(%rip), %xmm0
	movss	%xmm0, (%rdi,%rcx,4)
	ret
.L249:
	shrl	$10, %eax
	salq	$2, %rsi
	movzwl	%ax, %ecx
	leaq	9488(%rsi), %r8
	movl	%ecx, %edx
	movss	(%rdi,%r8,4), %xmm0
	shrl	$3, %edx
	andl	$1920, %edx
	testb	$64, %ah
	movl	%edx, %eax
	je	.L326
	sall	$16, %eax
	orl	$1073741824, %eax
	movl	%eax, %edx
.L327:
	movl	%ecx, %eax
	sall	$13, %ecx
	sall	$16, %eax
	andl	$8380416, %ecx
	andl	$-2147483648, %eax
	orl	%ecx, %eax
	addl	%edx, %eax
	movd	%eax, %xmm6
	subss	%xmm6, %xmm0
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	leaq	(%rdi,%r8,4), %rax
	addq	%r9, %rsi
	leaq	37956(%rdi,%rsi,4), %rdx
	.p2align 4,,10
	.p2align 3
.L328:
	movss	%xmm0, (%rax)
	addq	$4, %rax
	cmpq	%rax, %rdx
	jne	.L328
	ret
.L250:
	shrl	$10, %eax
	movl	%r9d, %r9d
	movzwl	%ax, %ecx
	leaq	9488(%r9,%rsi,4), %rsi
	movl	%ecx, %edx
	movss	(%rdi,%rsi,4), %xmm0
	shrl	$3, %edx
	andl	$1920, %edx
	testb	$64, %ah
	movl	%edx, %eax
	je	.L324
	sall	$16, %eax
	orl	$1073741824, %eax
	movl	%eax, %edx
.L325:
	movl	%ecx, %eax
	sall	$13, %ecx
	sall	$16, %eax
	andl	$8380416, %ecx
	andl	$-2147483648, %eax
	orl	%ecx, %eax
	addl	%edx, %eax
	movd	%eax, %xmm5
	subss	%xmm5, %xmm0
	movss	%xmm0, (%rdi,%rsi,4)
	ret
.L251:
	salq	$2, %rdx
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	salq	$2, %rsi
	leaq	37952(%rdi,%rdx,4), %rax
	addq	%rdx, %r9
	subq	%rdx, %rsi
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L323:
	movss	(%rax), %xmm0
	movss	%xmm0, (%rax,%rsi,4)
	addq	$4, %rax
	cmpq	%rax, %rcx
	jne	.L323
	ret
.L252:
	shrl	$10, %eax
	movl	%r9d, %r9d
	andl	$3, %eax
	leaq	9488(%rax,%rdx,4), %rax
	movss	(%rdi,%rax,4), %xmm0
	leaq	9488(%r9,%rsi,4), %rax
	movss	%xmm0, (%rdi,%rax,4)
	ret
.L240:
	shrl	$10, %eax
	movl	%r9d, %r9d
	andl	$3, %eax
	leaq	9488(%rax,%rdx,4), %rax
	movss	(%rdi,%rax,4), %xmm0
	leaq	9488(%r9,%rsi,4), %rax
	comiss	(%rdi,%rax,4), %xmm0
	jbe	.L238
.L419:
	movss	%xmm0, (%rdi,%rax,4)
	ret
.L242:
	shrl	$10, %eax
	movl	%r9d, %r9d
	andl	$3, %eax
	leaq	9488(%rax,%rdx,4), %rax
	movss	(%rdi,%rax,4), %xmm0
	leaq	9488(%r9,%rsi,4), %rax
	movss	(%rdi,%rax,4), %xmm1
	comiss	%xmm0, %xmm1
	jbe	.L238
	jmp	.L419
.L243:
	salq	$2, %rsi
	salq	$2, %rdx
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	leaq	37952(%rdi,%rdx,4), %rax
	subq	%rdx, %rsi
	addq	%rdx, %r9
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L335:
	cvttss2sil	(%rax), %edx
	movl	%edx, (%rax,%rsi,4)
	addq	$4, %rax
	cmpq	%rcx, %rax
	jne	.L335
	ret
.L244:
	salq	$2, %rsi
	salq	$2, %rdx
	cmpl	$-1, %r9d
	je	.L238
	movl	%r9d, %r9d
	leaq	37952(%rdi,%rdx,4), %rax
	subq	%rdx, %rsi
	addq	%rdx, %r9
	leaq	37956(%rdi,%r9,4), %rcx
	.p2align 4,,10
	.p2align 3
.L334:
	pxor	%xmm0, %xmm0
	cvtsi2ssl	(%rax), %xmm0
	movss	%xmm0, (%rax,%rsi,4)
	addq	$4, %rax
	cmpq	%rcx, %rax
	jne	.L334
	ret
.L294:
	movq	88(%rdi), %rax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
.L326:
	sall	$16, %eax
	orl	$939524096, %eax
	testl	%edx, %edx
	cmovne	%eax, %edx
	jmp	.L327
.L298:
	cmpq	$1, %rdx
	je	.L420
	cmpq	$2, %rdx
	jne	.L238
	movq	37440(%rdi,%rsi,8), %rax
	movw	%ax, 184(%rdi)
	ret
.L324:
	sall	$16, %eax
	orl	$939524096, %eax
	testl	%edx, %edx
	cmovne	%eax, %edx
	jmp	.L325
.L310:
	sall	$16, %eax
	orl	$939524096, %eax
	testl	%edx, %edx
	cmovne	%eax, %edx
	jmp	.L311
.L306:
	sall	$16, %eax
	orl	$939524096, %eax
	testl	%edx, %edx
	cmovne	%eax, %edx
	jmp	.L307
.L295:
	cmpq	$1, %rdx
	je	.L421
	cmpq	$2, %rdx
	jne	.L238
	movzwl	184(%rdi), %eax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
.L420:
	movq	37440(%rdi,%rsi,8), %rax
	movl	%eax, 148(%rdi)
	ret
.L421:
	movl	148(%rdi), %eax
	movq	%rax, 37440(%rdi,%rsi,8)
	ret
	.cfi_endproc
.LFE30:
	.size	AX_execute_unit0A, .-AX_execute_unit0A
	.p2align 4
	.globl	AX_execute_unit0
	.type	AX_execute_unit0, @function
AX_execute_unit0:
.LFB31:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rsi, %r10
	movl	16(%rsp), %ebx
	movq	%rdx, %r11
	cmpl	$95, %r8d
	ja	.L423
	leaq	.L425(%rip), %rsi
	movl	%r8d, %edx
	movslq	(%rsi,%rdx,4), %rax
	addq	%rsi, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L425:
	.long	.L422-.L425
	.long	.L451-.L425
	.long	.L450-.L425
	.long	.L449-.L425
	.long	.L448-.L425
	.long	.L447-.L425
	.long	.L446-.L425
	.long	.L445-.L425
	.long	.L444-.L425
	.long	.L443-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L442-.L425
	.long	.L441-.L425
	.long	.L440-.L425
	.long	.L439-.L425
	.long	.L438-.L425
	.long	.L437-.L425
	.long	.L435-.L425
	.long	.L435-.L425
	.long	.L433-.L425
	.long	.L433-.L425
	.long	.L431-.L425
	.long	.L431-.L425
	.long	.L430-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L429-.L425
	.long	.L429-.L425
	.long	.L428-.L425
	.long	.L427-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L423-.L425
	.long	.L426-.L425
	.long	.L426-.L425
	.long	.L426-.L425
	.long	.L426-.L425
	.long	.L426-.L425
	.long	.L426-.L425
	.long	.L426-.L425
	.long	.L426-.L425
	.long	.L426-.L425
	.long	.L426-.L425
	.long	.L426-.L425
	.long	.L426-.L425
	.long	.L424-.L425
	.long	.L424-.L425
	.long	.L424-.L425
	.long	.L424-.L425
	.text
	.p2align 4,,10
	.p2align 3
.L427:
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rax
	shrl	$8, %ebx
	movq	37440(%rdi,%r10,8), %rdx
	andl	$262143, %ebx
	andq	(%rax,%r9,8), %rdx
.L523:
	movzwl	184(%rdi), %eax
	xorl	%ecx, %ecx
	andl	$-8, %eax
	cmpq	%rbx, %rdx
	setne	%cl
	orl	%ecx, %eax
	movl	%eax, %ecx
	orl	$2, %ecx
	cmpq	%rbx, %rdx
	cmovl	%ecx, %eax
	movl	%eax, %ecx
	orl	$4, %ecx
	cmpq	%rbx, %rdx
	cmovb	%ecx, %eax
.L522:
	movw	%ax, 184(%rdi)
.L422:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L429:
	.cfi_restore_state
	andw	$-8, 184(%rdi)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L433:
	.cfi_restore_state
	movq	37440(%rdi,%r11,8), %rax
	xorl	%edx, %edx
	divq	37440(%rdi,%rcx,8)
	movq	%rax, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L435:
	.cfi_restore_state
	movl	%ebx, %edx
	movq	37440(%rdi,%r11,8), %rax
	movl	%r9d, %r9d
	shrl	$11, %edx
	andl	$511, %edx
	andb	$4, %bh
	cmove	37440(%rdi,%rcx,8), %edx
	movl	%edx, %ecx
	leaq	sizemask(%rip), %rdx
	shrq	%cl, %rax
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L431:
	.cfi_restore_state
	movq	37440(%rdi,%r11,8), %rax
	xorl	%edx, %edx
	divq	37440(%rdi,%rcx,8)
	movq	%rdx, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L423:
	.cfi_restore_state
	movl	%ebx, 16(%rsp)
	movq	%r11, %rdx
	movq	%r10, %rsi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	jmp	AX_execute_unit0A
	.p2align 4,,10
	.p2align 3
.L426:
	.cfi_restore_state
	shrl	$10, %ebx
	movb	%r8b, 187(%rdi)
	movq	%rbx, %rax
	movb	$1, 186(%rdi)
	orq	$-4194304, %rax
	cmpl	$2097152, %ebx
	cmova	%eax, %ebx
	movl	%ebx, 160(%rdi)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L424:
	.cfi_restore_state
	shrl	$10, %ebx
	movb	%r8b, 187(%rdi)
	movl	%ebx, 160(%rdi)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movb	$1, 186(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L428:
	.cfi_restore_state
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rax
	shrl	$8, %ebx
	movq	37440(%rdi,%r10,8), %rdx
	orq	$-262144, %rbx
	andq	(%rax,%r9,8), %rdx
	jmp	.L523
	.p2align 4,,10
	.p2align 3
.L430:
	movq	37440(%rdi,%r11,8), %rax
	imulq	37440(%rdi,%rcx,8), %rax
	movq	%rax, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L437:
	.cfi_restore_state
	movl	%ebx, %edx
	movq	37440(%rdi,%r11,8), %rax
	movl	%r9d, %r9d
	shrl	$11, %edx
	andl	$511, %edx
	andb	$4, %bh
	cmove	37440(%rdi,%rcx,8), %edx
	movl	%edx, %ecx
	leaq	sizemask(%rip), %rdx
	salq	%cl, %rax
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L438:
	.cfi_restore_state
	movq	37440(%rdi,%r11,8), %rax
	movq	37440(%rdi,%rcx,8), %rdx
	testb	$4, %bh
	je	.L481
	shrl	$11, %ebx
	movl	%ebx, %edx
	andl	$511, %edx
.L481:
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rsi
	andq	(%rsi,%r9,8), %rax
	andq	%rdx, %rax
	movq	%rax, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L439:
	.cfi_restore_state
	movq	37440(%rdi,%r11,8), %rax
	movq	37440(%rdi,%rcx,8), %rdx
	testb	$4, %bh
	je	.L480
	shrl	$11, %ebx
	movl	%ebx, %edx
	andl	$511, %edx
.L480:
	orq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L448:
	.cfi_restore_state
	leaq	sizemask(%rip), %rdx
	movl	%r9d, %eax
	movq	37440(%rdi,%r10,8), %rcx
	movq	(%rdx,%rax,8), %rax
	andq	%rax, %rcx
	andq	37440(%rdi,%r11,8), %rax
	movq	%rax, %rdx
	testl	%r9d, %r9d
	jne	.L456
	cmpl	$128, %ecx
	movl	%ecx, %ecx
	jbe	.L457
	orq	$-256, %rcx
	cmpl	$128, %edx
	movl	%edx, %edx
	ja	.L458
.L519:
	movzwl	184(%rdi), %eax
	andl	$-8, %eax
	jmp	.L522
	.p2align 4,,10
	.p2align 3
.L449:
	shrl	$10, %ebx
	sall	$4, %r9d
	movzwl	%bx, %ebx
	movl	%r9d, %ecx
	salq	%cl, %rbx
	orq	%rbx, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L450:
	.cfi_restore_state
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	movq	37440(%rdi,%r11,8), %rax
	andq	(%rdx,%r9,8), %rax
	setne	%al
	movzbl	%al, %eax
.L521:
	movq	%rax, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L451:
	.cfi_restore_state
	addq	$4680, %r11
	movq	(%rdi,%r11,8), %rax
	testl	%r9d, %r9d
	jne	.L453
	testb	$-128, %al
	je	.L521
	orq	$-256, %rax
	movq	%rax, (%rdi,%r11,8)
	jmp	.L521
	.p2align 4,,10
	.p2align 3
.L440:
	movq	37440(%rdi,%r11,8), %rax
	movq	37440(%rdi,%rcx,8), %rdx
	testb	$4, %bh
	je	.L479
	shrl	$11, %ebx
	movl	%ebx, %edx
	andl	$511, %edx
.L479:
	xorq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L444:
	.cfi_restore_state
	shrl	$8, %ebx
	orq	$-262144, %rbx
	movq	%rbx, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L445:
	.cfi_restore_state
	shrl	$10, %ebx
	leaq	sizemask(%rip), %rax
	movl	%r9d, %r9d
	movq	37440(%rdi,%r10,8), %rcx
	andl	$1023, %ebx
	andq	(%rax,%r9,8), %rcx
	movq	%rbx, %rax
	movl	%ebx, %edx
	orq	$-1024, %rax
	cmpl	$512, %ebx
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	cmova	%rax, %rdx
	movzwl	184(%rdi), %eax
	andl	$-8, %eax
	movl	%eax, %esi
	orl	$4, %esi
	cmpq	%rdx, %rcx
	cmovb	%esi, %eax
	movw	%ax, 184(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L446:
	.cfi_restore_state
	shrl	$10, %ebx
	movl	%r9d, %edx
	leaq	sizemask(%rip), %rcx
	movq	37440(%rdi,%r10,8), %rax
	andl	$1023, %ebx
	andq	(%rcx,%rdx,8), %rax
	movq	%rbx, %rdx
	movl	%ebx, %ecx
	orq	$-1024, %rdx
	cmpl	$512, %ebx
	cmova	%rdx, %rcx
	testl	%r9d, %r9d
	jne	.L471
	movl	%eax, %esi
	movl	%eax, %eax
	movq	%rax, %rdx
	orq	$-256, %rdx
	cmpl	$128, %esi
	cmova	%rdx, %rax
.L472:
	movzwl	184(%rdi), %edx
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	andl	$-8, %edx
	movl	%edx, %esi
	orl	$4, %esi
	cmpq	%rcx, %rax
	cmovb	%esi, %edx
	movw	%dx, 184(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L447:
	.cfi_restore_state
	leaq	sizemask(%rip), %rax
	movl	%r9d, %r9d
	movq	37440(%rdi,%r10,8), %rsi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movq	(%rax,%r9,8), %rdx
	movzwl	184(%rdi), %eax
	andl	$-8, %eax
	andq	%rdx, %rsi
	andq	37440(%rdi,%r11,8), %rdx
	movl	%eax, %ecx
	orl	$4, %ecx
	cmpq	%rdx, %rsi
	cmovb	%ecx, %eax
	movw	%ax, 184(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L442:
	.cfi_restore_state
	movq	37440(%rdi,%r11,8), %rax
	movq	37440(%rdi,%rcx,8), %rdx
	testb	$4, %bh
	je	.L477
	shrl	$11, %ebx
	movl	%ebx, %edx
	andl	$511, %edx
.L477:
	addq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L443:
	.cfi_restore_state
	shrl	$8, %ebx
	addq	$4680, %r10
	andl	$262143, %ebx
	movl	%ebx, %eax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movq	%rax, (%rdi,%r10,8)
	ret
	.p2align 4,,10
	.p2align 3
.L441:
	.cfi_restore_state
	movq	37440(%rdi,%r11,8), %rax
	movq	37440(%rdi,%rcx,8), %rdx
	testb	$4, %bh
	je	.L478
	shrl	$11, %ebx
	movl	%ebx, %edx
	andl	$511, %edx
.L478:
	subq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37440(%rdi,%r10,8)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L453:
	.cfi_restore_state
	cmpl	$1, %r9d
	je	.L524
	cmpl	$2, %r9d
	jne	.L521
	testl	$2147483648, %eax
	je	.L521
	movabsq	$-4294967296, %rdx
	orq	%rdx, %rax
	movq	%rax, (%rdi,%r11,8)
	jmp	.L521
	.p2align 4,,10
	.p2align 3
.L456:
	cmpl	$1, %r9d
	je	.L525
	cmpl	$2, %r9d
	je	.L526
.L460:
	movzwl	184(%rdi), %eax
	andl	$-8, %eax
	movl	%eax, %esi
	orl	$4, %esi
	cmpq	%rdx, %rcx
	cmovb	%esi, %eax
	jmp	.L522
	.p2align 4,,10
	.p2align 3
.L471:
	cmpl	$1, %r9d
	je	.L527
	cmpl	$2, %r9d
	jne	.L472
	cmpl	$-2147483648, %eax
	movl	%eax, %eax
	jbe	.L472
	movabsq	$-4294967296, %rdx
	orq	%rdx, %rax
	jmp	.L472
.L457:
	cmpl	$128, %edx
	movl	%edx, %edx
	jbe	.L460
.L458:
	orq	$-256, %rdx
	jmp	.L460
.L527:
	movl	%eax, %esi
	movl	%eax, %eax
	movq	%rax, %rdx
	orq	$-65536, %rdx
	cmpl	$32768, %esi
	cmova	%rdx, %rax
	jmp	.L472
.L524:
	testb	$-128, %ah
	je	.L521
	orq	$-65536, %rax
	movq	%rax, (%rdi,%r11,8)
	jmp	.L521
.L525:
	cmpl	$32768, %ecx
	movl	%ecx, %ecx
	jbe	.L462
	orq	$-65536, %rcx
	cmpl	$32768, %edx
	movl	%edx, %edx
	jbe	.L519
.L463:
	orq	$-65536, %rdx
	jmp	.L460
.L526:
	cmpl	$-2147483648, %ecx
	movl	%ecx, %ecx
	jbe	.L465
	movabsq	$-4294967296, %rax
	orq	%rax, %rcx
	cmpl	$-2147483648, %edx
	movl	%edx, %edx
	jbe	.L519
.L466:
	movabsq	$-4294967296, %rax
	orq	%rax, %rdx
	jmp	.L460
.L462:
	cmpl	$32768, %edx
	movl	%edx, %edx
	jbe	.L460
	jmp	.L463
.L465:
	cmpl	$-2147483648, %edx
	movl	%edx, %edx
	ja	.L466
	jmp	.L460
	.cfi_endproc
.LFE31:
	.size	AX_execute_unit0, .-AX_execute_unit0
	.p2align 4
	.globl	AX_decode_execute
	.type	AX_decode_execute, @function
AX_decode_execute:
.LFB32:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movdqu	88(%rdi), %xmm0
	movq	80(%rdi), %rdx
	movl	144(%rdi), %ecx
	paddq	.LC4(%rip), %xmm0
	cmpb	$1, 186(%rdi)
	movl	(%rdx,%rcx,4), %ebx
	movups	%xmm0, 88(%rdi)
	movq	%rcx, %rax
	je	.L536
.L529:
	subq	$8, %rsp
	.cfi_def_cfa_offset 40
	addl	$1, %eax
	movl	%ebx, 172(%rbp)
	movl	%ebx, %r9d
	pushq	%rbx
	.cfi_def_cfa_offset 48
	movl	%ebx, %r8d
	movl	%ebx, %ecx
	movl	%ebx, %edx
	movl	%eax, 144(%rbp)
	shrl	$8, %r9d
	shrl	%r8d
	movl	%ebx, %esi
	shrl	$14, %ecx
	shrl	$20, %edx
	andl	$3, %r9d
	andl	$127, %r8d
	andl	$63, %ecx
	shrl	$26, %esi
	andl	$63, %edx
	movq	%rbp, %rdi
	call	AX_execute_unit0
	andl	$1, %ebx
	popq	%rcx
	.cfi_def_cfa_offset 40
	popq	%rsi
	.cfi_def_cfa_offset 32
	jne	.L537
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L537:
	.cfi_restore_state
	movl	144(%rbp), %eax
	subq	$8, %rsp
	.cfi_def_cfa_offset 40
	addq	$1, 88(%rbp)
	movq	%rbp, %rdi
	leal	1(%rax), %ecx
	movq	80(%rbp), %rax
	movl	(%rax,%rcx,4), %eax
	movl	%ecx, 144(%rbp)
	pushq	%rax
	.cfi_def_cfa_offset 48
	movl	%eax, %r9d
	movl	%eax, %r8d
	movl	%eax, %ecx
	movl	%eax, 176(%rbp)
	movl	%eax, %edx
	movl	%eax, %esi
	shrl	$8, %r9d
	shrl	%r8d
	shrl	$14, %ecx
	andl	$3, %r9d
	shrl	$20, %edx
	andl	$63, %ecx
	shrl	$26, %esi
	andl	$127, %r8d
	andl	$63, %edx
	call	AX_execute_unit1
	popq	%rax
	.cfi_def_cfa_offset 40
	popq	%rdx
	.cfi_def_cfa_offset 32
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L536:
	.cfi_restore_state
	call	executeDelayedInstruction
	movl	144(%rdi), %eax
	jmp	.L529
	.cfi_endproc
.LFE32:
	.size	AX_decode_execute, .-AX_decode_execute
	.section	.rodata
	.align 32
	.type	sizemask, @object
	.size	sizemask, 32
sizemask:
	.quad	255
	.quad	65535
	.quad	4294967295
	.quad	-1
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC2:
	.long	0
	.long	-2147483648
	.long	0
	.long	0
	.align 16
.LC3:
	.long	-2147483648
	.long	0
	.long	0
	.long	0
	.align 16
.LC4:
	.quad	1
	.quad	1
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
