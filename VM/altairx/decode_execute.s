	.file	"decode_execute.c"
	.text
	.p2align 4
	.globl	AX_execute_unit1
	.type	AX_execute_unit1, @function
AX_execute_unit1:
.LFB24:
	.cfi_startproc
	cmpl	$29, %r8d
	ja	.L1
	leaq	.L4(%rip), %r10
	movl	%r8d, %r8d
	movslq	(%r10,%r8,4), %rax
	addq	%r10, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L4:
	.long	.L1-.L4
	.long	.L9-.L4
	.long	.L8-.L4
	.long	.L7-.L4
	.long	.L5-.L4
	.long	.L5-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L3-.L4
	.text
	.p2align 4,,10
	.p2align 3
.L3:
	movq	37992(%rdi,%rdx,8), %rax
	xorl	%edx, %edx
	divq	37992(%rdi,%rcx,8)
.L24:
	movq	%rax, 37992(%rdi,%rsi,8)
.L1:
	ret
	.p2align 4,,10
	.p2align 3
.L5:
	leaq	sizemask(%rip), %rax
	movl	%r9d, %r9d
	movq	37992(%rdi,%rsi,8), %r11
	movq	(%rax,%r9,8), %rcx
	movzwl	732(%rdi), %eax
	andl	$-8, %eax
	andq	%rcx, %r11
	andq	37992(%rdi,%rdx,8), %rcx
	movl	%eax, %edx
	orl	$4, %edx
	cmpq	%rcx, %r11
	cmovb	%edx, %eax
	movw	%ax, 732(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L9:
	addq	$4748, %rdx
	movq	8(%rdi,%rdx,8), %rax
	testl	%r9d, %r9d
	jne	.L10
	testb	$-128, %al
	je	.L24
	orq	$-256, %rax
	movq	%rax, 8(%rdi,%rdx,8)
	jmp	.L24
	.p2align 4,,10
	.p2align 3
.L8:
	movq	37992(%rdi,%rdx,8), %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	setne	%al
	movzbl	%al, %eax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L7:
	movq	37992(%rdi,%rdx,8), %rax
	sall	$4, %r9d
	movl	%r9d, %ecx
	salq	%cl, %rax
	orq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L10:
	cmpl	$1, %r9d
	je	.L26
	cmpl	$2, %r9d
	jne	.L24
	testl	$2147483648, %eax
	je	.L24
	movabsq	$-4294967296, %rcx
	orq	%rcx, %rax
	movq	%rax, 8(%rdi,%rdx,8)
	jmp	.L24
	.p2align 4,,10
	.p2align 3
.L26:
	testb	$-128, %ah
	je	.L24
	orq	$-65536, %rax
	movq	%rax, 8(%rdi,%rdx,8)
	jmp	.L24
	.cfi_endproc
.LFE24:
	.size	AX_execute_unit1, .-AX_execute_unit1
	.p2align 4
	.globl	AX_execute_unit0
	.type	AX_execute_unit0, @function
AX_execute_unit0:
.LFB25:
	.cfi_startproc
	movq	%rdx, %r10
	cmpl	$28, %r8d
	ja	.L27
	leaq	.L30(%rip), %rdx
	movl	%r8d, %r8d
	movslq	(%rdx,%r8,4), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L30:
	.long	.L41-.L30
	.long	.L55-.L30
	.long	.L54-.L30
	.long	.L53-.L30
	.long	.L52-.L30
	.long	.L51-.L30
	.long	.L50-.L30
	.long	.L49-.L30
	.long	.L48-.L30
	.long	.L47-.L30
	.long	.L27-.L30
	.long	.L27-.L30
	.long	.L46-.L30
	.long	.L45-.L30
	.long	.L44-.L30
	.long	.L43-.L30
	.long	.L42-.L30
	.long	.L41-.L30
	.long	.L40-.L30
	.long	.L39-.L30
	.long	.L38-.L30
	.long	.L37-.L30
	.long	.L35-.L30
	.long	.L35-.L30
	.long	.L34-.L30
	.long	.L32-.L30
	.long	.L32-.L30
	.long	.L29-.L30
	.long	.L29-.L30
	.text
	.p2align 4,,10
	.p2align 3
.L27:
	ret
	.p2align 4,,10
	.p2align 3
.L35:
	movl	8(%rsp), %edx
	movq	37992(%rdi,%r10,8), %rax
	movl	%r9d, %r9d
	shrl	$11, %edx
	andl	$511, %edx
	testl	$1024, 8(%rsp)
	cmove	37992(%rdi,%rcx,8), %edx
	movl	%edx, %ecx
	leaq	sizemask(%rip), %rdx
	shrq	%cl, %rax
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L32:
	movq	37992(%rdi,%r10,8), %rax
	xorl	%edx, %edx
	divq	37992(%rdi,%rcx,8)
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L41:
	movq	37992(%rdi,%r10,8), %rax
	movq	37992(%rdi,%rcx,8), %rdx
	testl	$1024, 8(%rsp)
	je	.L83
	movl	8(%rsp), %edx
	shrl	$11, %edx
	andl	$511, %edx
.L83:
	subq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L29:
	movq	37992(%rdi,%r10,8), %rax
	xorl	%edx, %edx
	divq	37992(%rdi,%rcx,8)
	movq	%rdx, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L38:
	movq	37992(%rdi,%r10,8), %rax
	movq	37992(%rdi,%rcx,8), %rdx
	testl	$1024, 8(%rsp)
	je	.L86
	movl	8(%rsp), %edx
	shrl	$11, %edx
	andl	$511, %edx
.L86:
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %r8
	andq	(%r8,%r9,8), %rax
	andq	%rdx, %rax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L39:
	movq	37992(%rdi,%r10,8), %rax
	movq	37992(%rdi,%rcx,8), %rdx
	testl	$1024, 8(%rsp)
	je	.L85
	movl	8(%rsp), %edx
	shrl	$11, %edx
	andl	$511, %edx
.L85:
	orq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L50:
	movl	%r9d, %edx
	leaq	sizemask(%rip), %rcx
	movq	37992(%rdi,%rsi,8), %rax
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
	jne	.L76
	movl	%eax, %esi
	movl	%eax, %eax
	movq	%rax, %rdx
	orq	$-256, %rdx
	cmpl	$128, %esi
	cmova	%rdx, %rax
.L77:
	movzwl	732(%rdi), %edx
	andl	$-8, %edx
	movl	%edx, %esi
	orl	$4, %esi
	cmpq	%rcx, %rax
	cmovb	%esi, %edx
	movw	%dx, 732(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L51:
	leaq	sizemask(%rip), %rax
	movl	%r9d, %r9d
	movq	37992(%rdi,%rsi,8), %rcx
	movq	(%rax,%r9,8), %rdx
	movzwl	732(%rdi), %eax
	andq	%rdx, %rcx
	andl	$-8, %eax
	andq	37992(%rdi,%r10,8), %rdx
	movq	%rcx, %rsi
	movl	%eax, %ecx
	orl	$4, %ecx
	cmpq	%rdx, %rsi
	cmovb	%ecx, %eax
	movw	%ax, 732(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L40:
	movq	37992(%rdi,%r10,8), %rax
	movq	37992(%rdi,%rcx,8), %rdx
	testl	$1024, 8(%rsp)
	je	.L84
	movl	8(%rsp), %edx
	shrl	$11, %edx
	andl	$511, %edx
.L84:
	xorq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L44:
	movq	%r10, %rdx
	andl	$3, %edx
	jne	.L93
	movl	712(%rdi), %eax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L45:
	movq	640(%rdi), %rax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L46:
	movq	632(%rdi), %rax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L47:
	movl	8(%rsp), %eax
	addq	$4748, %rsi
	shrl	$8, %eax
	andl	$262143, %eax
	movq	%rax, 8(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L48:
	movl	8(%rsp), %eax
	shrl	$8, %eax
	orq	$-262144, %rax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L49:
	leaq	sizemask(%rip), %rax
	movl	%r9d, %r9d
	movq	37992(%rdi,%rsi,8), %rcx
	andq	(%rax,%r9,8), %rcx
	movl	8(%rsp), %eax
	shrl	$10, %eax
	andl	$1023, %eax
	movq	%rax, %rsi
	movl	%eax, %edx
	orq	$-1024, %rsi
	cmpl	$512, %eax
	movzwl	732(%rdi), %eax
	cmova	%rsi, %rdx
	andl	$-8, %eax
	movl	%eax, %esi
	orl	$4, %esi
	cmpq	%rdx, %rcx
	cmovb	%esi, %eax
	movw	%ax, 732(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L34:
	movq	37992(%rdi,%r10,8), %rax
	imulq	37992(%rdi,%rcx,8), %rax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L43:
	movq	%r10, %rdx
	andl	$3, %edx
	jne	.L95
	movq	37992(%rdi,%rsi,8), %rax
	movl	%eax, 712(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L54:
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	movq	37992(%rdi,%r10,8), %rax
	andq	(%rdx,%r9,8), %rax
	setne	%al
	movzbl	%al, %eax
.L127:
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L53:
	movq	37992(%rdi,%r10,8), %rax
	sall	$4, %r9d
	movl	%r9d, %ecx
	salq	%cl, %rax
	orq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L42:
	movq	37992(%rdi,%r10,8), %rdx
	movq	37992(%rdi,%rcx,8), %rax
	testl	$1024, 8(%rsp)
	je	.L82
	movl	8(%rsp), %eax
	shrl	$11, %eax
	andl	$511, %eax
.L82:
	addq	%rdx, %rax
	movl	%r9d, %r9d
	leaq	sizemask(%rip), %rdx
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L52:
	leaq	sizemask(%rip), %rdx
	movl	%r9d, %eax
	movq	37992(%rdi,%rsi,8), %rcx
	movq	(%rdx,%rax,8), %rax
	andq	%rax, %rcx
	andq	37992(%rdi,%r10,8), %rax
	movq	%rax, %rdx
	testl	%r9d, %r9d
	jne	.L61
	cmpl	$128, %ecx
	movl	%ecx, %ecx
	jbe	.L62
	orq	$-256, %rcx
	cmpl	$128, %edx
	movl	%edx, %edx
	ja	.L63
.L126:
	movzwl	732(%rdi), %eax
	andl	$-8, %eax
.L128:
	movw	%ax, 732(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L55:
	leaq	4748(%r10), %rdx
	movq	8(%rdi,%rdx,8), %rax
	testl	%r9d, %r9d
	jne	.L58
	testb	$-128, %al
	je	.L127
	orq	$-256, %rax
	movq	%rax, 8(%rdi,%rdx,8)
	jmp	.L127
	.p2align 4,,10
	.p2align 3
.L37:
	movl	8(%rsp), %edx
	movq	37992(%rdi,%r10,8), %rax
	movl	%r9d, %r9d
	shrl	$11, %edx
	andl	$511, %edx
	testl	$1024, 8(%rsp)
	cmove	37992(%rdi,%rcx,8), %edx
	movl	%edx, %ecx
	leaq	sizemask(%rip), %rdx
	salq	%cl, %rax
	andq	(%rdx,%r9,8), %rax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L76:
	cmpl	$1, %r9d
	je	.L131
	cmpl	$2, %r9d
	jne	.L77
	cmpl	$-2147483648, %eax
	movl	%eax, %eax
	jbe	.L77
	movabsq	$-4294967296, %rdx
	orq	%rdx, %rax
	jmp	.L77
	.p2align 4,,10
	.p2align 3
.L93:
	cmpq	$1, %rdx
	je	.L132
	cmpq	$2, %rdx
	jne	.L27
	movzwl	732(%rdi), %eax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
	.p2align 4,,10
	.p2align 3
.L95:
	cmpq	$1, %rdx
	je	.L133
	cmpq	$2, %rdx
	jne	.L27
	movq	37992(%rdi,%rsi,8), %rax
	jmp	.L128
	.p2align 4,,10
	.p2align 3
.L61:
	cmpl	$1, %r9d
	je	.L134
	cmpl	$2, %r9d
	je	.L135
.L65:
	movzwl	732(%rdi), %eax
	andl	$-8, %eax
	movl	%eax, %esi
	orl	$4, %esi
	cmpq	%rdx, %rcx
	cmovb	%esi, %eax
	jmp	.L128
	.p2align 4,,10
	.p2align 3
.L58:
	cmpl	$1, %r9d
	je	.L136
	cmpl	$2, %r9d
	jne	.L127
	testl	$2147483648, %eax
	je	.L127
	movabsq	$-4294967296, %rcx
	orq	%rcx, %rax
	movq	%rax, 8(%rdi,%rdx,8)
	jmp	.L127
.L62:
	cmpl	$128, %edx
	movl	%edx, %edx
	jbe	.L65
.L63:
	orq	$-256, %rdx
	jmp	.L65
.L131:
	movl	%eax, %esi
	movl	%eax, %eax
	movq	%rax, %rdx
	orq	$-65536, %rdx
	cmpl	$32768, %esi
	cmova	%rdx, %rax
	jmp	.L77
.L132:
	movl	708(%rdi), %eax
	movq	%rax, 37992(%rdi,%rsi,8)
	ret
.L136:
	testb	$-128, %ah
	je	.L127
	orq	$-65536, %rax
	movq	%rax, 8(%rdi,%rdx,8)
	jmp	.L127
.L133:
	movq	37992(%rdi,%rsi,8), %rax
	movl	%eax, 708(%rdi)
	ret
.L134:
	cmpl	$32768, %ecx
	movl	%ecx, %ecx
	jbe	.L67
	orq	$-65536, %rcx
	cmpl	$32768, %edx
	movl	%edx, %edx
	jbe	.L126
.L68:
	orq	$-65536, %rdx
	jmp	.L65
.L135:
	cmpl	$-2147483648, %ecx
	movl	%ecx, %ecx
	jbe	.L70
	movabsq	$-4294967296, %rax
	orq	%rax, %rcx
	cmpl	$-2147483648, %edx
	movl	%edx, %edx
	jbe	.L126
.L71:
	movabsq	$-4294967296, %rax
	orq	%rax, %rdx
	jmp	.L65
.L67:
	cmpl	$32768, %edx
	movl	%edx, %edx
	ja	.L68
	jmp	.L65
.L70:
	cmpl	$-2147483648, %edx
	movl	%edx, %edx
	jbe	.L65
	jmp	.L71
	.cfi_endproc
.LFE25:
	.size	AX_execute_unit0, .-AX_execute_unit0
	.p2align 4
	.globl	AX_decode_execute2
	.type	AX_decode_execute2, @function
AX_decode_execute2:
.LFB26:
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
	movl	704(%rdi), %edx
	movq	624(%rdi), %rax
	movl	(%rax,%rdx,4), %ebx
	pushq	%rbx
	.cfi_def_cfa_offset 40
	movl	%ebx, %r9d
	movl	%ebx, %r8d
	movl	%ebx, %ecx
	shrl	$8, %r9d
	movl	%ebx, %edx
	shrl	%r8d
	movl	%ebx, %esi
	shrl	$14, %ecx
	shrl	$20, %edx
	andl	$3, %r9d
	andl	$127, %r8d
	andl	$63, %ecx
	andl	$63, %edx
	shrl	$26, %esi
	call	AX_execute_unit0
	cmpb	$1, 734(%rdi)
	popq	%rcx
	.cfi_def_cfa_offset 32
	je	.L144
.L138:
	andl	$1, %ebx
	jne	.L145
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
.L145:
	.cfi_restore_state
	movl	704(%rbp), %eax
	subq	$8, %rsp
	.cfi_def_cfa_offset 40
	movq	%rbp, %rdi
	leal	1(%rax), %edx
	movq	624(%rbp), %rax
	movl	(%rax,%rdx,4), %eax
	pushq	%rax
	.cfi_def_cfa_offset 48
	movl	%eax, %r9d
	movl	%eax, %r8d
	movl	%eax, %ecx
	shrl	$8, %r9d
	movl	%eax, %edx
	movl	%eax, %esi
	shrl	%r8d
	shrl	$14, %ecx
	shrl	$20, %edx
	andl	$3, %r9d
	andl	$127, %r8d
	andl	$63, %edx
	andl	$63, %ecx
	shrl	$26, %esi
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
.L144:
	.cfi_restore_state
	call	executeDelayedInstruction@PLT
	jmp	.L138
	.cfi_endproc
.LFE26:
	.size	AX_decode_execute2, .-AX_decode_execute2
	.section	.rodata
	.align 32
	.type	sizemask, @object
	.size	sizemask, 32
sizemask:
	.quad	255
	.quad	65535
	.quad	4294967295
	.quad	-1
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
