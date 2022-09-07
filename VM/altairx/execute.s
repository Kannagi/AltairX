	.file	"execute.c"
	.text
	.p2align 4
	.type	executeLS, @function
executeLS:
.LFB25:
	.cfi_startproc
	movq	%rsi, %r9
	movq	%rdx, %rax
	testl	$2147483648, %edx
	je	.L2
	movl	76(%rdi), %edx
	movq	48(%rdi), %rsi
	leal	-1(%rdx), %edi
.L3:
	andq	%rdi, %rax
	movl	%ecx, %edx
	leaq	(%rsi,%rax), %rdi
	testl	%r8d, %r8d
	je	.L10
.L12:
	movq	%r9, %rsi
	jmp	memcpy@PLT
	.p2align 4,,10
	.p2align 3
.L2:
	testl	$1073741824, %edx
	je	.L4
	movl	72(%rdi), %edx
	movq	40(%rdi), %rsi
	leal	-1(%rdx), %edi
	movl	%ecx, %edx
	andq	%rdi, %rax
	leaq	(%rsi,%rax), %rdi
	testl	%r8d, %r8d
	jne	.L12
.L10:
	movq	%rdi, %rsi
	movq	%r9, %rdi
	jmp	memcpy@PLT
	.p2align 4,,10
	.p2align 3
.L4:
	testl	$536870912, %edx
	jne	.L13
	testl	$268435456, %edx
	je	.L6
	movl	64(%rdi), %edx
	movq	16(%rdi), %rsi
	leal	-1(%rdx), %edi
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L13:
	movl	68(%rdi), %edx
	movq	32(%rdi), %rsi
	leal	-1(%rdx), %edi
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L6:
	testl	$134217728, %edx
	jne	.L14
	testl	$67108864, %edx
	je	.L8
	movl	56(%rdi), %edx
	movq	(%rdi), %rsi
	leal	-1(%rdx), %edi
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L14:
	movl	60(%rdi), %edx
	movq	8(%rdi), %rsi
	leal	-1(%rdx), %edi
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L8:
	testl	$33554432, %edx
	je	.L9
	movq	24(%rdi), %rsi
	movl	$8388607, %edi
	jmp	.L3
.L9:
	leaq	866(%rdi), %rsi
	movl	$32767, %edi
	jmp	.L3
	.cfi_endproc
.LFE25:
	.size	executeLS, .-executeLS
	.p2align 4
	.globl	executeDelayedInstruction
	.type	executeDelayedInstruction, @function
executeDelayedInstruction:
.LFB24:
	.cfi_startproc
	cmpb	$19, 735(%rdi)
	ja	.L15
	movzbl	735(%rdi), %eax
	leaq	.L18(%rip), %rdx
	movslq	(%rdx,%rax,4), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L18:
	.long	.L37-.L18
	.long	.L36-.L18
	.long	.L35-.L18
	.long	.L34-.L18
	.long	.L33-.L18
	.long	.L32-.L18
	.long	.L31-.L18
	.long	.L30-.L18
	.long	.L29-.L18
	.long	.L28-.L18
	.long	.L65-.L18
	.long	.L26-.L18
	.long	.L64-.L18
	.long	.L24-.L18
	.long	.L23-.L18
	.long	.L22-.L18
	.long	.L21-.L18
	.long	.L20-.L18
	.long	.L19-.L18
	.long	.L17-.L18
	.text
	.p2align 4,,10
	.p2align 3
.L28:
	movzwl	732(%rdi), %eax
	andl	$3, %eax
	cmpw	$3, %ax
	je	.L38
	.p2align 4,,10
	.p2align 3
.L65:
	addl	%esi, 704(%rdi)
.L38:
	movb	$0, 734(%rdi)
.L15:
	ret
	.p2align 4,,10
	.p2align 3
.L24:
	addl	708(%rdi), %esi
.L64:
	movl	%esi, 704(%rdi)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L17:
	movl	704(%rdi), %eax
	movb	$2, 737(%rdi)
	movl	$2, 704(%rdi)
	movl	%eax, 716(%rdi)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L29:
	testb	$3, 732(%rdi)
	je	.L65
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L30:
	testb	$2, 732(%rdi)
	jne	.L65
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L26:
	movq	38472(%rdi), %rax
	testq	%rax, %rax
	je	.L38
	subq	$1, %rax
	movq	%rax, 38472(%rdi)
	jmp	.L65
	.p2align 4,,10
	.p2align 3
.L23:
	movl	704(%rdi), %eax
	movl	%esi, 704(%rdi)
	movl	%eax, 712(%rdi)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L19:
	movl	704(%rdi), %eax
	movb	$1, 737(%rdi)
	movl	$0, 704(%rdi)
	movl	%eax, 716(%rdi)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L20:
	movl	716(%rdi), %eax
	movl	%eax, 704(%rdi)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L21:
	movl	712(%rdi), %eax
	movl	%eax, 704(%rdi)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L22:
	movl	704(%rdi), %eax
	addl	708(%rdi), %esi
	movl	%esi, 704(%rdi)
	movl	%eax, 712(%rdi)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L31:
	movzwl	732(%rdi), %eax
	andl	$3, %eax
	cmpw	$2, %ax
	jne	.L38
	jmp	.L65
	.p2align 4,,10
	.p2align 3
.L32:
	testb	$4, 732(%rdi)
	je	.L65
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L33:
	testb	$5, 732(%rdi)
	je	.L65
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L34:
	testb	$4, 732(%rdi)
	jne	.L65
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L35:
	movzwl	732(%rdi), %eax
	andl	$5, %eax
	cmpw	$4, %ax
	jne	.L38
	jmp	.L65
	.p2align 4,,10
	.p2align 3
.L36:
	testb	$1, 732(%rdi)
	je	.L65
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L37:
	testb	$1, 732(%rdi)
	je	.L38
	jmp	.L65
	.cfi_endproc
.LFE24:
	.size	executeDelayedInstruction, .-executeDelayedInstruction
	.p2align 4
	.globl	executeInstruction
	.type	executeInstruction, @function
executeInstruction:
.LFB26:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	movq	%rdi, %r9
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movl	24(%rsi), %r8d
	leaq	37992(%rdi), %rbx
	movl	32(%rsi), %r10d
	movl	28(%rsi), %edi
	movq	(%rsi), %r11
	movq	8(%rsi), %rdx
	movq	16(%rsi), %rcx
	movl	124(%rsi), %r12d
	testl	%r8d, %r8d
	jne	.L67
	cmpl	$26, %edi
	ja	.L68
	leaq	.L70(%rip), %rax
	movslq	(%rax,%rdi,4), %rsi
	addq	%rax, %rsi
	jmp	*%rsi
	.section	.rodata
	.align 4
	.align 4
.L70:
	.long	.L448-.L70
	.long	.L95-.L70
	.long	.L94-.L70
	.long	.L93-.L70
	.long	.L92-.L70
	.long	.L91-.L70
	.long	.L90-.L70
	.long	.L89-.L70
	.long	.L88-.L70
	.long	.L87-.L70
	.long	.L86-.L70
	.long	.L85-.L70
	.long	.L84-.L70
	.long	.L83-.L70
	.long	.L82-.L70
	.long	.L81-.L70
	.long	.L80-.L70
	.long	.L79-.L70
	.long	.L78-.L70
	.long	.L77-.L70
	.long	.L76-.L70
	.long	.L75-.L70
	.long	.L74-.L70
	.long	.L73-.L70
	.long	.L72-.L70
	.long	.L71-.L70
	.long	.L69-.L70
	.text
	.p2align 4,,10
	.p2align 3
.L67:
	leaq	38504(%r9), %rbp
	cmpl	$4096, %r8d
	je	.L454
	cmpl	$8192, %r8d
	je	.L455
	cmpl	$12288, %r8d
	je	.L456
	cmpl	$16384, %r8d
	je	.L66
	movsd	56(%rsi), %xmm0
	movsd	64(%rsi), %xmm1
	cmpl	$6, %edi
	ja	.L66
	leaq	.L286(%rip), %rdx
	movslq	(%rdx,%rdi,4), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L286:
	.long	.L292-.L286
	.long	.L291-.L286
	.long	.L290-.L286
	.long	.L289-.L286
	.long	.L288-.L286
	.long	.L287-.L286
	.long	.L285-.L286
	.text
	.p2align 4,,10
	.p2align 3
.L454:
	leaq	(%rbx,%r11,8), %rsi
	leaq	(%rdx,%rcx), %r12
	movq	(%rsi), %rax
	leaq	866(%r9,%rax), %r13
	cmpl	$9, %edi
	ja	.L66
	leaq	.L111(%rip), %r8
	movslq	(%r8,%rdi,4), %rax
	addq	%r8, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L111:
	.long	.L116-.L111
	.long	.L115-.L111
	.long	.L114-.L111
	.long	.L113-.L111
	.long	.L66-.L111
	.long	.L66-.L111
	.long	.L66-.L111
	.long	.L66-.L111
	.long	.L112-.L111
	.long	.L110-.L111
	.text
	.p2align 4,,10
	.p2align 3
.L456:
	movl	36(%rsi), %ebx
	leaq	88(%rsi), %r14
	leaq	104(%rsi), %r13
	leaq	0(,%r11,4), %rdx
	cmpl	$30, %edi
	ja	.L66
	leaq	.L120(%rip), %r8
	movslq	(%r8,%rdi,4), %rax
	addq	%r8, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L120:
	.long	.L148-.L120
	.long	.L147-.L120
	.long	.L146-.L120
	.long	.L145-.L120
	.long	.L144-.L120
	.long	.L143-.L120
	.long	.L142-.L120
	.long	.L141-.L120
	.long	.L140-.L120
	.long	.L139-.L120
	.long	.L138-.L120
	.long	.L137-.L120
	.long	.L136-.L120
	.long	.L135-.L120
	.long	.L134-.L120
	.long	.L133-.L120
	.long	.L132-.L120
	.long	.L131-.L120
	.long	.L130-.L120
	.long	.L129-.L120
	.long	.L128-.L120
	.long	.L127-.L120
	.long	.L66-.L120
	.long	.L66-.L120
	.long	.L126-.L120
	.long	.L125-.L120
	.long	.L124-.L120
	.long	.L123-.L120
	.long	.L122-.L120
	.long	.L121-.L120
	.long	.L119-.L120
	.text
	.p2align 4,,10
	.p2align 3
.L69:
	movl	$1, 724(%r9)
.L448:
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rdi
	movq	(%rsi), %rdx
	andq	(%rdi,%r10,8), %rdx
	.p2align 4,,10
	.p2align 3
.L97:
	movq	%rdx, (%rsi)
.L66:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L455:
	.cfi_restore_state
	movb	$1, 734(%r9)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L80:
	.cfi_restore_state
	salq	%cl, %rdx
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rax
	movq	%rdx, (%rsi)
	andq	(%rax,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L81:
	andq	%rcx, %rdx
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rax
	movq	%rdx, (%rsi)
	andq	(%rax,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L82:
	orq	%rcx, %rdx
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rax
	movq	%rdx, (%rsi)
	andq	(%rax,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L71:
	andw	$-8, 732(%r9)
	leaq	(%rbx,%r11,8), %rsi
	movq	(%rsi), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L72:
	leaq	sizemask.1(%rip), %rax
	movq	(%rbx,%r11,8), %rsi
	movzwl	732(%r9), %ecx
	movq	(%rax,%r10,8), %rax
	andl	$-8, %ecx
	andq	%rax, %rsi
	andq	%rax, %rdx
	xorl	%eax, %eax
	cmpq	%rdx, %rsi
	setne	%al
	orl	%eax, %ecx
	movl	%ecx, %eax
	orl	$2, %eax
	cmpq	%rsi, %rdx
	cmovl	%eax, %ecx
	movl	%ecx, %eax
	orl	$4, %eax
	cmpq	%rdx, %rsi
	leaq	(%rbx,%rsi,8), %rsi
	cmova	%eax, %ecx
	movw	%cx, 732(%r9)
.L450:
	movq	(%rsi), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L73:
	movq	%rdx, %rax
	xorl	%edx, %edx
	divq	%rcx
.L451:
	movq	%rdx, 38496(%r9)
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rdi
	movq	(%rsi), %rdx
	andq	(%rdi,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L74:
	movq	%rdx, %rax
	cqto
	idivq	%rcx
	jmp	.L451
	.p2align 4,,10
	.p2align 3
.L91:
	cmpq	%rcx, %rdx
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rax
	setb	%dl
	movzbl	%dl, %edx
	movq	%rdx, (%rsi)
	andq	(%rax,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L76:
	movq	%rdx, %rax
	cqto
	idivq	%rcx
.L449:
	movq	%rax, 38496(%r9)
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rdi
	movq	(%rsi), %rdx
	andq	(%rdi,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L77:
	imulq	%rdx, %rcx
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rdi
	movq	%rcx, 38488(%r9)
	movq	(%rsi), %rdx
	andq	(%rdi,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L78:
	shrq	%cl, %rdx
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rax
	movq	%rdx, (%rsi)
	andq	(%rax,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L79:
	sarq	%cl, %rdx
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rax
	movq	%rdx, (%rsi)
	andq	(%rax,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L75:
	movq	%rdx, %rax
	xorl	%edx, %edx
	divq	%rcx
	jmp	.L449
	.p2align 4,,10
	.p2align 3
.L95:
	testl	%r10d, %r10d
	jne	.L99
	movq	%rdx, %rax
	orq	$-256, %rax
	testb	$-128, %dl
	cmovne	%rax, %rdx
.L100:
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rax
	movq	%rdx, (%rsi)
	andq	(%rax,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L92:
	cmpq	%rcx, %rdx
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rax
	setl	%dl
	movzbl	%dl, %edx
	movq	%rdx, (%rsi)
	andq	(%rax,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L93:
	movl	%r10d, %ecx
	leaq	(%rbx,%r11,8), %rsi
	sall	$4, %ecx
	salq	%cl, %rdx
	orq	(%rsi), %rdx
	movq	%rdx, (%rsi)
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L94:
	leaq	sizemask.1(%rip), %rax
	leaq	(%rbx,%r11,8), %rsi
	movq	(%rax,%r10,8), %rax
	andq	%rax, (%rbx,%rdx,8)
	testq	%rdx, %rdx
	setne	%dl
	movzbl	%dl, %edx
	movq	%rdx, (%rsi)
	andq	%rax, %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L83:
	xorq	%rcx, %rdx
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rax
	movq	%rdx, (%rsi)
	andq	(%rax,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L84:
	subq	%rcx, %rdx
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rax
	movq	%rdx, (%rsi)
	andq	(%rax,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L85:
	addq	%rcx, %rdx
	leaq	(%rbx,%r11,8), %rsi
	leaq	sizemask.1(%rip), %rax
	movq	%rdx, (%rsi)
	andq	(%rax,%r10,8), %rdx
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L86:
	leaq	(%rbx,%r11,8), %rsi
	movq	(%rsi), %rdx
	andl	$3, %r12d
	jne	.L105
	movl	%edx, 712(%r9)
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L87:
	leaq	(%rbx,%r11,8), %rsi
	andl	$3, %r12d
	jne	.L102
	movl	712(%r9), %edx
	movq	%rdx, (%rsi)
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L89:
	movq	632(%r9), %rdx
	leaq	(%rbx,%r11,8), %rsi
	movq	%rdx, (%rsi)
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L88:
	movq	640(%r9), %rdx
	leaq	(%rbx,%r11,8), %rsi
	movq	%rdx, (%rsi)
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L90:
	leaq	(%rbx,%r11,8), %rsi
	movq	%rdx, (%rsi)
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L116:
	movl	%r10d, %ecx
	movl	$1, %eax
	xorl	%r8d, %r8d
	movq	%r12, %rdx
	sall	%cl, %eax
	movl	%eax, %ecx
.L452:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	movq	%r9, %rdi
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	jmp	executeLS
	.p2align 4,,10
	.p2align 3
.L112:
	.cfi_restore_state
	sall	$6, %ecx
	xorl	%r8d, %r8d
.L453:
	movq	%r13, %rsi
	jmp	.L452
	.p2align 4,,10
	.p2align 3
.L113:
	leal	1(%r10), %ecx
	salq	$4, %r11
	movl	$1, %r8d
	movq	%r12, %rdx
	sall	$4, %ecx
	leaq	0(%rbp,%r11), %rsi
	jmp	.L452
	.p2align 4,,10
	.p2align 3
.L114:
	leal	1(%r10), %ecx
	salq	$4, %r11
	xorl	%r8d, %r8d
	movq	%r12, %rdx
	sall	$4, %ecx
	leaq	0(%rbp,%r11), %rsi
	jmp	.L452
	.p2align 4,,10
	.p2align 3
.L115:
	movl	%r10d, %ecx
	movl	$1, %eax
	movl	$1, %r8d
	movq	%r12, %rdx
	sall	%cl, %eax
	movl	%eax, %ecx
	jmp	.L452
	.p2align 4,,10
	.p2align 3
.L110:
	sall	$6, %ecx
	movl	$1, %r8d
	jmp	.L453
	.p2align 4,,10
	.p2align 3
.L285:
	xorpd	.LC13(%rip), %xmm0
	salq	$4, %r11
	movsd	%xmm0, 0(%rbp,%r11)
	jmp	.L66
	.p2align 4,,10
	.p2align 3
.L291:
	salq	$4, %r11
	movsd	%xmm0, 0(%rbp,%r11)
	jmp	.L66
	.p2align 4,,10
	.p2align 3
.L292:
	movsd	48(%rsi), %xmm1
	movzwl	732(%r9), %eax
	xorl	%edx, %edx
	movl	$1, %ecx
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
	movw	%ax, 732(%r9)
	jmp	.L66
	.p2align 4,,10
	.p2align 3
.L287:
	pxor	%xmm1, %xmm1
	comisd	%xmm0, %xmm1
	jbe	.L66
	xorpd	.LC13(%rip), %xmm0
	salq	$4, %r11
	movsd	%xmm0, 0(%rbp,%r11)
	jmp	.L66
	.p2align 4,,10
	.p2align 3
.L288:
	mulsd	%xmm1, %xmm0
	salq	$4, %r11
	movsd	%xmm0, 0(%rbp,%r11)
	jmp	.L66
	.p2align 4,,10
	.p2align 3
.L289:
	subsd	%xmm1, %xmm0
	salq	$4, %r11
	movsd	%xmm0, 0(%rbp,%r11)
	jmp	.L66
	.p2align 4,,10
	.p2align 3
.L290:
	addsd	%xmm1, %xmm0
	salq	$4, %r11
	movsd	%xmm0, 0(%rbp,%r11)
	jmp	.L66
.L119:
	movss	72(%rsi,%r10,4), %xmm0
	subss	(%r14,%rbx,4), %xmm0
	pxor	%xmm1, %xmm1
	xorl	%edx, %edx
	movzwl	732(%r9), %eax
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
	movw	%ax, 732(%r9)
	jmp	.L66
.L121:
	salq	$4, %r11
	pxor	%xmm0, %xmm0
	cvtsd2ss	0(%rbp,%r12,8), %xmm0
	movss	%xmm0, 0(%rbp,%r11)
	jmp	.L66
.L122:
	pxor	%xmm0, %xmm0
	cvtss2sd	88(%rsi), %xmm0
	movsd	%xmm0, 0(%rbp,%rdx,4)
	jmp	.L66
.L123:
	testl	%r10d, %r10d
	je	.L66
	xorl	%eax, %eax
	movl	%r10d, %r8d
	movl	%eax, %ecx
	subl	%eax, %r8d
	notl	%ecx
	addl	%r10d, %ecx
	cmpl	$2, %ecx
	jbe	.L250
	movl	%r8d, %ecx
	addq	$88, %rsi
	leaq	38504(%r9,%rdx,4), %rdi
	xorl	%eax, %eax
	shrl	$2, %ecx
	salq	$4, %rcx
	.p2align 4,,10
	.p2align 3
.L251:
	movdqu	(%rsi,%rax), %xmm7
	cvtdq2ps	%xmm7, %xmm0
	movups	%xmm0, (%rdi,%rax)
	addq	$16, %rax
	cmpq	%rcx, %rax
	jne	.L251
	movl	%r8d, %eax
	andl	$-4, %eax
	cmpl	%eax, %r8d
	je	.L66
.L250:
	movl	%eax, %ecx
	pxor	%xmm0, %xmm0
	cvtsi2ssl	(%r14,%rcx,4), %xmm0
	leaq	(%rdx,%rcx), %rsi
	leal	1(%rax), %ecx
	movss	%xmm0, 0(%rbp,%rsi,4)
	cmpl	%ecx, %r10d
	jbe	.L66
	pxor	%xmm0, %xmm0
	leaq	(%rdx,%rcx), %rsi
	addl	$2, %eax
	cvtsi2ssl	(%r14,%rcx,4), %xmm0
	movss	%xmm0, 0(%rbp,%rsi,4)
	cmpl	%eax, %r10d
	jbe	.L66
	movl	%eax, %ecx
	pxor	%xmm0, %xmm0
	cvtsi2ssl	(%r14,%rcx,4), %xmm0
	leaq	(%rdx,%rcx), %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L124:
	testl	%r10d, %r10d
	je	.L66
	leal	-1(%r10), %eax
	cmpl	$2, %eax
	jbe	.L298
	movl	%r10d, %ecx
	leaq	38504(%r9,%rdx,4), %rdi
	xorl	%eax, %eax
	shrl	$2, %ecx
	salq	$4, %rcx
	.p2align 4,,10
	.p2align 3
.L247:
	movups	88(%rsi,%rax), %xmm7
	cvttps2dq	%xmm7, %xmm0
	movups	%xmm0, (%rdi,%rax)
	addq	$16, %rax
	cmpq	%rcx, %rax
	jne	.L247
	movl	%r10d, %eax
	andl	$-4, %eax
	testb	$3, %r10b
	je	.L66
.L246:
	movl	%eax, %ecx
	leaq	(%rdx,%rcx), %rsi
	cvttss2sil	(%r14,%rcx,4), %ecx
	movl	%ecx, 0(%rbp,%rsi,4)
	leal	1(%rax), %ecx
	cmpl	%ecx, %r10d
	jbe	.L66
	leaq	(%rdx,%rcx), %rsi
	addl	$2, %eax
	cvttss2sil	(%r14,%rcx,4), %ecx
	movl	%ecx, 0(%rbp,%rsi,4)
	cmpl	%eax, %r10d
	jbe	.L66
	movl	%eax, %ecx
	leaq	(%rdx,%rcx), %rax
	cvttss2sil	(%r14,%rcx,4), %edx
	movl	%edx, 0(%rbp,%rax,4)
	jmp	.L66
.L141:
	testl	%r10d, %r10d
	je	.L66
	leaq	38504(%r9,%rdx,4), %rcx
	leaq	92(%rsi), %r8
	movq	%rcx, %rax
	leal	-1(%r10), %edi
	subq	%r8, %rax
	cmpq	$24, %rax
	jbe	.L185
	cmpl	$2, %edi
	jbe	.L185
	movl	%r10d, %edi
	xorl	%eax, %eax
	shrl	$2, %edi
	salq	$4, %rdi
	.p2align 4,,10
	.p2align 3
.L186:
	movups	104(%rsi,%rax), %xmm0
	movups	88(%rsi,%rax), %xmm7
	movups	(%rcx,%rax), %xmm5
	addps	%xmm7, %xmm0
	mulps	%xmm5, %xmm0
	movups	%xmm0, (%rcx,%rax)
	addq	$16, %rax
	cmpq	%rdi, %rax
	jne	.L186
	movl	%r10d, %ecx
	andl	$-4, %ecx
	testb	$3, %r10b
	je	.L66
	movl	%ecx, %eax
	leaq	(%rax,%rdx), %rdi
	movss	(%r14,%rax,4), %xmm0
	addss	0(%r13,%rax,4), %xmm0
	leaq	0(%rbp,%rdi,4), %rdi
	leal	1(%rcx), %eax
	mulss	(%rdi), %xmm0
	movss	%xmm0, (%rdi)
	cmpl	%r10d, %eax
	jnb	.L66
	leaq	(%rax,%rdx), %rdi
	movss	(%r14,%rax,4), %xmm0
	addl	$2, %ecx
	addss	0(%r13,%rax,4), %xmm0
	leaq	0(%rbp,%rdi,4), %rdi
	mulss	(%rdi), %xmm0
	movss	%xmm0, (%rdi)
	cmpl	%r10d, %ecx
	jnb	.L66
	leaq	(%rdx,%rcx), %rax
	movss	104(%rsi,%rcx,4), %xmm0
	addss	88(%rsi,%rcx,4), %xmm0
	leaq	0(%rbp,%rax,4), %rax
	mulss	(%rax), %xmm0
	movss	%xmm0, (%rax)
	jmp	.L66
.L142:
	testl	%r10d, %r10d
	je	.L66
	leaq	38504(%r9,%rdx,4), %rdi
	leaq	92(%rsi), %rcx
	movq	%rdi, %rax
	subq	%rcx, %rax
	cmpq	$24, %rax
	jbe	.L179
	leal	-1(%r10), %eax
	cmpl	$2, %eax
	jbe	.L179
	movl	%r10d, %ecx
	xorl	%eax, %eax
	shrl	$2, %ecx
	salq	$4, %rcx
	.p2align 4,,10
	.p2align 3
.L180:
	movups	104(%rsi,%rax), %xmm0
	movups	88(%rsi,%rax), %xmm6
	mulps	%xmm6, %xmm0
	movups	%xmm0, (%rdi,%rax)
	addq	$16, %rax
	cmpq	%rcx, %rax
	jne	.L180
	movl	%r10d, %eax
	andl	$-4, %eax
	testb	$3, %r10b
	je	.L66
	movl	%eax, %ecx
	movss	(%r14,%rcx,4), %xmm0
	mulss	0(%r13,%rcx,4), %xmm0
	leaq	(%rcx,%rdx), %rdi
	leal	1(%rax), %ecx
	movss	%xmm0, 0(%rbp,%rdi,4)
	cmpl	%r10d, %ecx
	jnb	.L66
	movss	88(%rsi,%rcx,4), %xmm0
	mulss	104(%rsi,%rcx,4), %xmm0
	leaq	(%rcx,%rdx), %rdi
	addl	$2, %eax
	movss	%xmm0, 0(%rbp,%rdi,4)
	cmpl	%r10d, %eax
	jnb	.L66
	movl	%eax, %ecx
	movss	0(%r13,%rcx,4), %xmm0
	mulss	(%r14,%rcx,4), %xmm0
	leaq	(%rcx,%rdx), %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L145:
	salq	$4, %r11
	movss	(%r14,%r10,4), %xmm0
	addss	0(%r13,%rbx,4), %xmm0
	addq	%r11, %rbp
	mulss	0(%rbp), %xmm0
	movss	%xmm0, 0(%rbp)
	jmp	.L66
.L146:
	movss	(%r14,%r10,4), %xmm0
	mulss	0(%r13,%rbx,4), %xmm0
	salq	$4, %r11
	movss	%xmm0, 0(%rbp,%r11)
	jmp	.L66
.L125:
	testl	%r10d, %r10d
	je	.L66
	xorl	%eax, %eax
	movl	%r10d, %r8d
	movl	%eax, %ecx
	subl	%eax, %r8d
	notl	%ecx
	addl	%r10d, %ecx
	cmpl	$6, %ecx
	jbe	.L266
	movl	%r8d, %edi
	addq	$88, %rsi
	pxor	%xmm7, %xmm7
	xorl	%eax, %eax
	shrl	$3, %edi
	leaq	38504(%r9,%rdx,4), %rcx
	pxor	%xmm14, %xmm14
	movdqa	.LC7(%rip), %xmm15
	salq	$4, %rdi
	movaps	%xmm15, -16(%rsp)
	.p2align 4,,10
	.p2align 3
.L267:
	movdqu	(%rsi,%rax), %xmm0
	movdqa	%xmm7, %xmm15
	movdqa	.LC10(%rip), %xmm8
	movdqa	.LC10(%rip), %xmm10
	movdqa	%xmm0, %xmm6
	movdqa	%xmm0, %xmm5
	pand	.LC9(%rip), %xmm0
	punpcklwd	%xmm7, %xmm6
	punpckhwd	%xmm7, %xmm5
	pcmpeqw	%xmm7, %xmm0
	movdqa	%xmm6, %xmm3
	movdqa	%xmm5, %xmm2
	psrld	$3, %xmm3
	psrld	$3, %xmm2
	pand	.LC8(%rip), %xmm3
	pand	.LC8(%rip), %xmm2
	movdqa	%xmm0, %xmm9
	movdqa	%xmm0, %xmm4
	movdqa	%xmm3, %xmm0
	movdqa	%xmm2, %xmm11
	pcmpeqd	%xmm14, %xmm0
	por	%xmm2, %xmm8
	pcmpeqd	%xmm14, %xmm11
	pcmpeqw	%xmm7, %xmm4
	por	%xmm3, %xmm10
	pslld	$16, %xmm8
	pslld	$16, %xmm10
	movdqa	%xmm0, %xmm1
	movaps	%xmm8, -48(%rsp)
	movdqa	%xmm11, %xmm13
	pcmpeqd	%xmm14, %xmm1
	movdqa	%xmm4, %xmm8
	pcmpeqd	%xmm14, %xmm13
	movdqa	%xmm1, %xmm12
	punpcklwd	%xmm13, %xmm1
	punpckhwd	%xmm13, %xmm12
	movdqa	%xmm1, %xmm13
	punpcklwd	%xmm12, %xmm1
	punpckhwd	%xmm12, %xmm13
	movdqa	%xmm0, %xmm12
	punpcklwd	%xmm11, %xmm0
	punpckhwd	%xmm11, %xmm12
	movdqa	%xmm0, %xmm11
	punpcklwd	%xmm13, %xmm1
	punpckhwd	%xmm12, %xmm11
	punpcklwd	%xmm12, %xmm0
	pand	%xmm9, %xmm1
	punpcklwd	%xmm11, %xmm0
	movdqa	%xmm7, %xmm11
	movdqa	%xmm7, %xmm13
	pcmpgtw	%xmm4, %xmm11
	pcmpgtw	%xmm1, %xmm13
	pand	%xmm9, %xmm0
	pcmpgtw	%xmm0, %xmm15
	movdqa	%xmm1, %xmm9
	movdqa	%xmm0, %xmm12
	punpcklwd	%xmm11, %xmm8
	movaps	%xmm11, -32(%rsp)
	punpcklwd	%xmm13, %xmm9
	punpckhwd	-32(%rsp), %xmm4
	movdqa	%xmm8, %xmm11
	punpcklwd	%xmm15, %xmm12
	punpckhwd	%xmm13, %xmm1
	movdqa	-16(%rsp), %xmm8
	punpckhwd	%xmm15, %xmm0
	por	%xmm8, %xmm3
	por	%xmm8, %xmm2
	pslld	$16, %xmm3
	pslld	$16, %xmm2
	pand	%xmm11, %xmm3
	pandn	%xmm10, %xmm11
	pand	%xmm9, %xmm10
	por	%xmm3, %xmm11
	movdqa	%xmm6, %xmm3
	pand	%xmm4, %xmm2
	pslld	$16, %xmm3
	pslld	$13, %xmm6
	pand	.LC11(%rip), %xmm3
	pand	.LC12(%rip), %xmm6
	pandn	%xmm11, %xmm9
	por	%xmm9, %xmm10
	por	%xmm3, %xmm6
	pandn	%xmm10, %xmm12
	paddd	%xmm6, %xmm12
	movdqa	-48(%rsp), %xmm6
	movups	%xmm12, (%rcx,%rax,2)
	pandn	%xmm6, %xmm4
	pand	%xmm1, %xmm6
	por	%xmm2, %xmm4
	pandn	%xmm4, %xmm1
	por	%xmm6, %xmm1
	pandn	%xmm1, %xmm0
	movdqa	%xmm5, %xmm1
	pslld	$16, %xmm1
	pslld	$13, %xmm5
	pand	.LC11(%rip), %xmm1
	pand	.LC12(%rip), %xmm5
	por	%xmm1, %xmm5
	paddd	%xmm5, %xmm0
	movups	%xmm0, 16(%rcx,%rax,2)
	addq	$16, %rax
	cmpq	%rdi, %rax
	jne	.L267
	movl	%r8d, %eax
	andl	$-8, %eax
	cmpl	%eax, %r8d
	je	.L66
.L266:
	movl	%eax, %ecx
	movzwl	(%r14,%rcx,2), %edi
	addq	%rdx, %rcx
	leaq	0(%rbp,%rcx,4), %r8
	movl	%edi, %ecx
	shrl	$3, %ecx
	andl	$1920, %ecx
	testw	$16384, %di
	je	.L270
	sall	$16, %ecx
	orl	$1073741824, %ecx
.L271:
	movl	%edi, %esi
	sall	$13, %edi
	sall	$16, %esi
	andl	$8380416, %edi
	andl	$-2147483648, %esi
	orl	%edi, %esi
	addl	%esi, %ecx
	movl	%ecx, (%r8)
	leal	1(%rax), %ecx
	cmpl	%ecx, %r10d
	jbe	.L66
	movzwl	(%r14,%rcx,2), %edi
	addq	%rdx, %rcx
	leaq	0(%rbp,%rcx,4), %r8
	movl	%edi, %ecx
	shrl	$3, %ecx
	andl	$1920, %ecx
	testw	$16384, %di
	je	.L272
	sall	$16, %ecx
	orl	$1073741824, %ecx
.L273:
	movl	%edi, %esi
	sall	$13, %edi
	sall	$16, %esi
	andl	$8380416, %edi
	andl	$-2147483648, %esi
	orl	%edi, %esi
	addl	%esi, %ecx
	movl	%ecx, (%r8)
	leal	2(%rax), %ecx
	cmpl	%ecx, %r10d
	jbe	.L66
	movzwl	(%r14,%rcx,2), %edi
	addq	%rdx, %rcx
	leaq	0(%rbp,%rcx,4), %r8
	movl	%edi, %ecx
	shrl	$3, %ecx
	andl	$1920, %ecx
	testw	$16384, %di
	je	.L274
	sall	$16, %ecx
	orl	$1073741824, %ecx
.L275:
	movl	%edi, %esi
	sall	$13, %edi
	sall	$16, %esi
	andl	$8380416, %edi
	andl	$-2147483648, %esi
	orl	%edi, %esi
	addl	%esi, %ecx
	movl	%ecx, (%r8)
	leal	3(%rax), %ecx
	cmpl	%ecx, %r10d
	jbe	.L66
	movzwl	(%r14,%rcx,2), %edi
	addq	%rdx, %rcx
	leaq	0(%rbp,%rcx,4), %r8
	movl	%edi, %ecx
	shrl	$3, %ecx
	andl	$1920, %ecx
	testw	$16384, %di
	je	.L276
	sall	$16, %ecx
	orl	$1073741824, %ecx
.L277:
	movl	%edi, %esi
	sall	$13, %edi
	sall	$16, %esi
	andl	$8380416, %edi
	andl	$-2147483648, %esi
	orl	%edi, %esi
	addl	%esi, %ecx
	movl	%ecx, (%r8)
	leal	4(%rax), %ecx
	cmpl	%ecx, %r10d
	jbe	.L66
	movzwl	(%r14,%rcx,2), %edi
	addq	%rdx, %rcx
	leaq	0(%rbp,%rcx,4), %r8
	movl	%edi, %ecx
	shrl	$3, %ecx
	andl	$1920, %ecx
	testw	$16384, %di
	je	.L278
	sall	$16, %ecx
	orl	$1073741824, %ecx
.L279:
	movl	%edi, %esi
	sall	$13, %edi
	sall	$16, %esi
	andl	$8380416, %edi
	andl	$-2147483648, %esi
	orl	%edi, %esi
	addl	%esi, %ecx
	movl	%ecx, (%r8)
	leal	5(%rax), %ecx
	cmpl	%ecx, %r10d
	jbe	.L66
	movzwl	(%r14,%rcx,2), %edi
	addq	%rdx, %rcx
	leaq	0(%rbp,%rcx,4), %r8
	movl	%edi, %ecx
	shrl	$3, %ecx
	andl	$1920, %ecx
	testw	$16384, %di
	je	.L280
	sall	$16, %ecx
	orl	$1073741824, %ecx
.L281:
	movl	%edi, %esi
	sall	$13, %edi
	addl	$6, %eax
	sall	$16, %esi
	andl	$8380416, %edi
	andl	$-2147483648, %esi
	orl	%edi, %esi
	addl	%esi, %ecx
	movl	%ecx, (%r8)
	cmpl	%eax, %r10d
	jbe	.L66
	movzwl	(%r14,%rax,2), %ecx
	addq	%rdx, %rax
	leaq	0(%rbp,%rax,4), %rdi
	movl	%ecx, %eax
	shrl	$3, %eax
	andl	$1920, %eax
	testb	$64, %ch
	je	.L457
	sall	$16, %eax
	orl	$1073741824, %eax
.L284:
	movl	%ecx, %edx
	sall	$13, %ecx
	sall	$16, %edx
	andl	$8380416, %ecx
	andl	$-2147483648, %edx
	orl	%ecx, %edx
	addl	%edx, %eax
	movl	%eax, (%rdi)
	jmp	.L66
.L126:
	testl	%r10d, %r10d
	je	.L66
	xorl	%ecx, %ecx
	leaq	(%rdx,%rdx), %rax
	movl	%r10d, %r8d
	movl	%ecx, %edx
	subl	%ecx, %r8d
	notl	%edx
	addl	%r10d, %edx
	cmpl	$6, %edx
	jbe	.L254
	leaq	88(%rsi), %rdi
	leaq	88(%rsi,%rax,2), %rdx
	movl	%r8d, %esi
	xorl	%ecx, %ecx
	shrl	$3, %esi
	pxor	%xmm5, %xmm5
	movdqa	.LC3(%rip), %xmm7
	movdqa	.LC4(%rip), %xmm9
	movdqa	.LC5(%rip), %xmm8
	movdqa	.LC6(%rip), %xmm6
	salq	$4, %rsi
	movdqa	.LC7(%rip), %xmm15
	.p2align 4,,10
	.p2align 3
.L255:
	movups	(%rdi,%rcx,2), %xmm10
	movups	16(%rdi,%rcx,2), %xmm12
	movdqa	%xmm10, %xmm0
	movdqa	%xmm12, %xmm13
	movdqa	%xmm12, %xmm2
	psrld	$16, %xmm13
	psrld	$16, %xmm0
	movdqa	%xmm10, %xmm4
	psrld	$13, %xmm12
	psrld	$23, %xmm4
	movdqa	%xmm0, %xmm11
	punpcklwd	%xmm13, %xmm0
	punpckhwd	%xmm13, %xmm11
	movdqa	%xmm4, %xmm3
	movdqa	%xmm0, %xmm13
	punpcklwd	%xmm11, %xmm0
	pand	%xmm6, %xmm4
	pslld	$10, %xmm3
	pcmpeqd	%xmm5, %xmm4
	punpckhwd	%xmm11, %xmm13
	punpcklwd	%xmm13, %xmm0
	psrld	$23, %xmm2
	pand	%xmm7, %xmm3
	pand	%xmm9, %xmm0
	movdqa	%xmm2, %xmm1
	pand	%xmm6, %xmm2
	pslld	$10, %xmm1
	pcmpeqd	%xmm5, %xmm2
	movdqa	%xmm0, %xmm11
	movdqa	%xmm10, %xmm0
	pand	%xmm7, %xmm1
	psrld	$13, %xmm0
	movdqa	%xmm0, %xmm10
	punpcklwd	%xmm12, %xmm0
	punpckhwd	%xmm12, %xmm10
	movdqa	%xmm0, %xmm12
	punpckhwd	%xmm10, %xmm12
	punpcklwd	%xmm10, %xmm0
	punpcklwd	%xmm12, %xmm0
	pand	%xmm8, %xmm0
	paddw	%xmm11, %xmm0
	movdqa	%xmm3, %xmm11
	pand	%xmm4, %xmm3
	por	%xmm15, %xmm11
	movdqa	%xmm3, %xmm10
	pandn	%xmm11, %xmm4
	movdqa	%xmm4, %xmm3
	movdqa	%xmm1, %xmm4
	por	%xmm10, %xmm3
	movdqa	%xmm1, %xmm10
	pand	%xmm2, %xmm4
	por	%xmm15, %xmm10
	pandn	%xmm10, %xmm2
	movdqa	%xmm2, %xmm1
	movdqa	%xmm3, %xmm2
	por	%xmm4, %xmm1
	movdqa	%xmm3, %xmm4
	punpcklwd	%xmm1, %xmm2
	punpckhwd	%xmm1, %xmm4
	movdqa	%xmm2, %xmm1
	punpcklwd	%xmm4, %xmm2
	punpckhwd	%xmm4, %xmm1
	punpcklwd	%xmm1, %xmm2
	paddw	%xmm2, %xmm0
	movups	%xmm0, (%rdx,%rcx)
	addq	$16, %rcx
	cmpq	%rsi, %rcx
	jne	.L255
	movl	%r8d, %ecx
	andl	$-8, %ecx
	cmpl	%ecx, %r8d
	je	.L66
.L254:
	movl	%ecx, %edx
	movl	(%r14,%rdx,4), %esi
	leaq	(%rax,%rdx), %r9
	movl	%esi, %r8d
	movl	%esi, %edx
	shrl	$13, %esi
	shrl	$23, %r8d
	shrl	$16, %edx
	andl	$1023, %esi
	movl	%r8d, %edi
	andl	$32768, %edx
	sall	$10, %edi
	andl	$15360, %edi
	movl	%edi, %r11d
	orl	$16384, %r11d
	andl	$128, %r8d
	cmovne	%r11d, %edi
	addl	%esi, %edx
	addl	%edi, %edx
	movw	%dx, (%r14,%r9,2)
	leal	1(%rcx), %edx
	cmpl	%edx, %r10d
	jbe	.L66
	movl	(%r14,%rdx,4), %esi
	leaq	(%rax,%rdx), %r8
	movl	%esi, %r9d
	movl	%esi, %edx
	shrl	$13, %esi
	shrl	$23, %r9d
	shrl	$16, %edx
	andl	$1023, %esi
	movl	%r9d, %edi
	andl	$32768, %edx
	sall	$10, %edi
	andl	$15360, %edi
	movl	%edi, %r11d
	orl	$16384, %r11d
	andl	$128, %r9d
	cmovne	%r11d, %edi
	addl	%esi, %edx
	addl	%edi, %edx
	movw	%dx, (%r14,%r8,2)
	leal	2(%rcx), %edx
	cmpl	%edx, %r10d
	jbe	.L66
	movl	(%r14,%rdx,4), %esi
	leaq	(%rax,%rdx), %r8
	movl	%esi, %r9d
	movl	%esi, %edx
	shrl	$13, %esi
	shrl	$23, %r9d
	shrl	$16, %edx
	andl	$1023, %esi
	movl	%r9d, %edi
	andl	$32768, %edx
	sall	$10, %edi
	andl	$15360, %edi
	movl	%edi, %r11d
	orl	$16384, %r11d
	andl	$128, %r9d
	cmovne	%r11d, %edi
	addl	%esi, %edx
	addl	%edi, %edx
	movw	%dx, (%r14,%r8,2)
	leal	3(%rcx), %edx
	cmpl	%edx, %r10d
	jbe	.L66
	movl	(%r14,%rdx,4), %esi
	leaq	(%rax,%rdx), %r8
	movl	%esi, %r9d
	movl	%esi, %edx
	shrl	$13, %esi
	shrl	$23, %r9d
	shrl	$16, %edx
	andl	$1023, %esi
	movl	%r9d, %edi
	andl	$32768, %edx
	sall	$10, %edi
	andl	$15360, %edi
	movl	%edi, %r11d
	orl	$16384, %r11d
	andl	$128, %r9d
	cmovne	%r11d, %edi
	addl	%esi, %edx
	addl	%edi, %edx
	movw	%dx, (%r14,%r8,2)
	leal	4(%rcx), %edx
	cmpl	%edx, %r10d
	jbe	.L66
	movl	(%r14,%rdx,4), %esi
	leaq	(%rax,%rdx), %r8
	movl	%esi, %r9d
	movl	%esi, %edx
	shrl	$13, %esi
	shrl	$23, %r9d
	shrl	$16, %edx
	andl	$1023, %esi
	movl	%r9d, %edi
	andl	$32768, %edx
	sall	$10, %edi
	andl	$15360, %edi
	movl	%edi, %r11d
	orl	$16384, %r11d
	andl	$128, %r9d
	cmovne	%r11d, %edi
	addl	%esi, %edx
	addl	%edi, %edx
	movw	%dx, (%r14,%r8,2)
	leal	5(%rcx), %edx
	cmpl	%edx, %r10d
	jbe	.L66
	movl	(%r14,%rdx,4), %esi
	leaq	(%rax,%rdx), %r8
	movl	%esi, %r9d
	movl	%esi, %edx
	shrl	$13, %esi
	shrl	$23, %r9d
	shrl	$16, %edx
	andl	$1023, %esi
	movl	%r9d, %edi
	andl	$32768, %edx
	sall	$10, %edi
	andl	$15360, %edi
	movl	%edi, %r11d
	orl	$16384, %r11d
	andl	$128, %r9d
	cmovne	%r11d, %edi
	addl	%esi, %edx
	addl	$6, %ecx
	addl	%edi, %edx
	movw	%dx, (%r14,%r8,2)
	cmpl	%ecx, %r10d
	jbe	.L66
	movl	(%r14,%rcx,4), %edx
	addq	%rcx, %rax
	addq	%rax, %rax
	movl	%edx, %edi
	movl	%edx, %ecx
	shrl	$13, %edx
	addq	%r14, %rax
	shrl	$23, %edi
	shrl	$16, %ecx
	andl	$1023, %edx
	movl	%edi, %esi
	andl	$32768, %ecx
	sall	$10, %esi
	andl	$15360, %esi
	andb	$-128, %dil
	je	.L265
	orl	$16384, %esi
.L265:
	addl	%ecx, %edx
	addl	%esi, %edx
	movw	%dx, (%rax)
	jmp	.L66
.L127:
	testl	%r10d, %r10d
	je	.L66
	leaq	(%r14,%rbx,4), %rcx
	salq	$4, %r11
	leal	-1(%r10), %edx
	xorl	%eax, %eax
	movss	(%rcx), %xmm0
	pxor	%xmm1, %xmm1
	salq	$2, %rdx
	addq	%r11, %r9
	movss	.LC1(%rip), %xmm2
	comiss	%xmm0, %xmm1
	ja	.L300
.L445:
	cmpq	%rdx, %rax
	je	.L66
	addq	$4, %rax
	comiss	%xmm0, %xmm1
	jbe	.L66
.L300:
	movss	88(%rsi,%rax), %xmm0
	xorps	%xmm2, %xmm0
	movss	%xmm0, 38504(%r9,%rax)
	cmpq	%rdx, %rax
	je	.L66
	movss	(%rcx), %xmm0
	addq	$4, %rax
	comiss	%xmm0, %xmm1
	jbe	.L445
	jmp	.L300
.L128:
	testl	%r10d, %r10d
	je	.L66
	leaq	38504(%r9,%rdx,4), %rdi
	leaq	92(%rsi), %rcx
	movq	%rdi, %rax
	subq	%rcx, %rax
	cmpq	$8, %rax
	jbe	.L235
	leal	-1(%r10), %eax
	cmpl	$3, %eax
	jbe	.L235
	movl	%r10d, %ecx
	movaps	.LC2(%rip), %xmm1
	xorl	%eax, %eax
	shrl	$2, %ecx
	salq	$4, %rcx
	.p2align 4,,10
	.p2align 3
.L236:
	movups	88(%rsi,%rax), %xmm0
	xorps	%xmm1, %xmm0
	movups	%xmm0, (%rdi,%rax)
	addq	$16, %rax
	cmpq	%rcx, %rax
	jne	.L236
	movl	%r10d, %eax
	andl	$-4, %eax
	testb	$3, %r10b
	je	.L66
	movl	%eax, %ecx
	movss	.LC1(%rip), %xmm1
	movss	(%r14,%rcx,4), %xmm0
	leaq	(%rdx,%rcx), %rsi
	leal	1(%rax), %ecx
	xorps	%xmm1, %xmm0
	movss	%xmm0, 0(%rbp,%rsi,4)
	cmpl	%ecx, %r10d
	jbe	.L66
	movss	(%r14,%rcx,4), %xmm0
	leaq	(%rdx,%rcx), %rsi
	addl	$2, %eax
	xorps	%xmm1, %xmm0
	movss	%xmm0, 0(%rbp,%rsi,4)
	cmpl	%eax, %r10d
	jbe	.L66
	movl	%eax, %ecx
	movss	(%r14,%rcx,4), %xmm0
	leaq	(%rdx,%rcx), %rax
	xorps	%xmm1, %xmm0
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L143:
	testl	%r10d, %r10d
	je	.L66
	leaq	38504(%r9,%rdx,4), %rdi
	leaq	92(%rsi), %rcx
	movq	%rdi, %rax
	subq	%rcx, %rax
	cmpq	$24, %rax
	jbe	.L173
	leal	-1(%r10), %eax
	cmpl	$2, %eax
	jbe	.L173
	movl	%r10d, %ecx
	xorl	%eax, %eax
	shrl	$2, %ecx
	salq	$4, %rcx
	.p2align 4,,10
	.p2align 3
.L174:
	movups	88(%rsi,%rax), %xmm0
	movups	104(%rsi,%rax), %xmm5
	subps	%xmm5, %xmm0
	movups	%xmm0, (%rdi,%rax)
	addq	$16, %rax
	cmpq	%rcx, %rax
	jne	.L174
	movl	%r10d, %eax
	andl	$-4, %eax
	testb	$3, %r10b
	je	.L66
	movl	%eax, %ecx
	movss	(%r14,%rcx,4), %xmm0
	subss	0(%r13,%rcx,4), %xmm0
	leaq	(%rcx,%rdx), %rdi
	leal	1(%rax), %ecx
	movss	%xmm0, 0(%rbp,%rdi,4)
	cmpl	%r10d, %ecx
	jnb	.L66
	movss	(%r14,%rcx,4), %xmm0
	subss	0(%r13,%rcx,4), %xmm0
	leaq	(%rcx,%rdx), %rdi
	addl	$2, %eax
	movss	%xmm0, 0(%rbp,%rdi,4)
	cmpl	%r10d, %eax
	jnb	.L66
	movl	%eax, %ecx
	movss	88(%rsi,%rcx,4), %xmm0
	subss	104(%rsi,%rcx,4), %xmm0
	leaq	(%rdx,%rcx), %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L144:
	testl	%r10d, %r10d
	je	.L66
	leaq	38504(%r9,%rdx,4), %rdi
	leaq	92(%rsi), %rcx
	movq	%rdi, %rax
	subq	%rcx, %rax
	cmpq	$24, %rax
	jbe	.L167
	leal	-1(%r10), %eax
	cmpl	$2, %eax
	jbe	.L167
	movl	%r10d, %ecx
	xorl	%eax, %eax
	shrl	$2, %ecx
	salq	$4, %rcx
	.p2align 4,,10
	.p2align 3
.L168:
	movups	104(%rsi,%rax), %xmm0
	movups	88(%rsi,%rax), %xmm7
	addps	%xmm7, %xmm0
	movups	%xmm0, (%rdi,%rax)
	addq	$16, %rax
	cmpq	%rcx, %rax
	jne	.L168
	movl	%r10d, %eax
	andl	$-4, %eax
	testb	$3, %r10b
	je	.L66
	movl	%eax, %ecx
	movss	(%r14,%rcx,4), %xmm0
	addss	0(%r13,%rcx,4), %xmm0
	leaq	(%rdx,%rcx), %rdi
	leal	1(%rax), %ecx
	movss	%xmm0, 0(%rbp,%rdi,4)
	cmpl	%ecx, %r10d
	jbe	.L66
	movss	(%r14,%rcx,4), %xmm0
	addss	0(%r13,%rcx,4), %xmm0
	leaq	(%rcx,%rdx), %rdi
	addl	$2, %eax
	movss	%xmm0, 0(%rbp,%rdi,4)
	cmpl	%r10d, %eax
	jnb	.L66
	movl	%eax, %ecx
	movss	104(%rsi,%rcx,4), %xmm0
	addss	88(%rsi,%rcx,4), %xmm0
	leaq	(%rdx,%rcx), %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L133:
	movq	%rcx, %rax
	movss	88(%rsi), %xmm0
	andl	$3, %eax
	addq	%rdx, %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	movq	%rcx, %rax
	movss	92(%rsi), %xmm0
	shrq	$2, %rax
	andl	$3, %eax
	addq	%rdx, %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	movq	%rcx, %rax
	movss	96(%rsi), %xmm0
	shrq	$4, %rax
	andl	$3, %eax
	addq	%rdx, %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	movq	%rcx, %rax
	movss	100(%rsi), %xmm0
	shrq	$6, %rax
	andl	$3, %eax
	addq	%rdx, %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L134:
	testl	%r10d, %r10d
	je	.L66
	movl	%ebx, %r8d
	movl	%r10d, %eax
	leaq	38504(%r9,%rdx,4), %rcx
	salq	$2, %r8
	leaq	9626(%rdx,%rax), %rdi
	leaq	(%r9,%rdi,4), %rbx
	leaq	104(%rsi,%r8), %rdi
	addq	%r8, %r13
	cmpq	%rdi, %rbx
	leaq	108(%rsi,%r8), %r8
	leaq	92(%rsi), %rbx
	setbe	%dil
	cmpq	%r8, %rcx
	setnb	%r8b
	orl	%r8d, %edi
	movq	%rcx, %r8
	subq	%rbx, %r8
	cmpq	$8, %r8
	seta	%r8b
	testb	%r8b, %dil
	je	.L221
	leal	-1(%r10), %edi
	cmpl	$2, %edi
	jbe	.L221
	movl	%r10d, %edi
	movss	0(%r13), %xmm1
	xorl	%eax, %eax
	shrl	$2, %edi
	salq	$4, %rdi
	shufps	$0, %xmm1, %xmm1
	.p2align 4,,10
	.p2align 3
.L222:
	movups	88(%rsi,%rax), %xmm0
	movups	(%rcx,%rax), %xmm6
	subps	%xmm1, %xmm0
	mulps	%xmm6, %xmm0
	movups	%xmm0, (%rcx,%rax)
	addq	$16, %rax
	cmpq	%rdi, %rax
	jne	.L222
	movl	%r10d, %ecx
	andl	$-4, %ecx
	testb	$3, %r10b
	je	.L66
	movl	%ecx, %esi
	leaq	(%rdx,%rsi), %rax
	movss	(%r14,%rsi,4), %xmm0
	subss	0(%r13), %xmm0
	leaq	0(%rbp,%rax,4), %rax
	mulss	(%rax), %xmm0
	movss	%xmm0, (%rax)
	leal	1(%rcx), %eax
	cmpl	%eax, %r10d
	jbe	.L66
	leaq	(%rdx,%rax), %rsi
	movss	(%r14,%rax,4), %xmm0
	subss	0(%r13), %xmm0
	addl	$2, %ecx
	leaq	0(%rbp,%rsi,4), %rsi
	mulss	(%rsi), %xmm0
	movss	%xmm0, (%rsi)
	cmpl	%ecx, %r10d
	jbe	.L66
	leaq	(%rdx,%rcx), %rax
	movss	(%r14,%rcx,4), %xmm0
	subss	0(%r13), %xmm0
	leaq	0(%rbp,%rax,4), %rax
	mulss	(%rax), %xmm0
	movss	%xmm0, (%rax)
	jmp	.L66
.L147:
	movss	(%r14,%r10,4), %xmm0
	subss	0(%r13,%rbx,4), %xmm0
	salq	$4, %r11
	movss	%xmm0, 0(%rbp,%r11)
	jmp	.L66
.L148:
	movss	(%r14,%r10,4), %xmm0
	addss	0(%r13,%rbx,4), %xmm0
	salq	$4, %r11
	movss	%xmm0, 0(%rbp,%r11)
	jmp	.L66
.L129:
	movss	(%r14,%rbx,4), %xmm0
	pxor	%xmm1, %xmm1
	comiss	%xmm0, %xmm1
	jbe	.L66
	xorps	.LC1(%rip), %xmm0
	movl	%r10d, %eax
	addq	%rdx, %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L130:
	movss	(%r14,%rbx,4), %xmm0
	movl	%r10d, %eax
	xorps	.LC1(%rip), %xmm0
	addq	%rdx, %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L131:
	testl	%r10d, %r10d
	je	.L66
	leaq	38504(%r9,%rdx,4), %rdi
	leaq	92(%rsi), %rcx
	movq	%rdi, %rax
	subq	%rcx, %rax
	cmpq	$8, %rax
	jbe	.L228
	leal	-1(%r10), %eax
	cmpl	$3, %eax
	jbe	.L228
	movl	%r10d, %ecx
	xorl	%eax, %eax
	shrl	$2, %ecx
	salq	$4, %rcx
	.p2align 4,,10
	.p2align 3
.L229:
	movups	88(%rsi,%rax), %xmm7
	movups	%xmm7, (%rdi,%rax)
	addq	$16, %rax
	cmpq	%rcx, %rax
	jne	.L229
	movl	%r10d, %eax
	andl	$-4, %eax
	testb	$3, %r10b
	je	.L66
	movl	%eax, %ecx
	movss	(%r14,%rcx,4), %xmm0
	addq	%rdx, %rcx
	movss	%xmm0, 0(%rbp,%rcx,4)
	leal	1(%rax), %ecx
	cmpl	%r10d, %ecx
	jnb	.L66
	movss	(%r14,%rcx,4), %xmm0
	addl	$2, %eax
	addq	%rdx, %rcx
	movss	%xmm0, 0(%rbp,%rcx,4)
	cmpl	%eax, %r10d
	jbe	.L66
	movss	(%r14,%rax,4), %xmm0
	addq	%rdx, %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L132:
	movss	(%r14,%rbx,4), %xmm0
	movl	%r10d, %eax
	addq	%rdx, %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L137:
	testl	%r10d, %r10d
	je	.L66
	movl	%ebx, %r8d
	movl	%r10d, %eax
	leaq	38504(%r9,%rdx,4), %rcx
	salq	$2, %r8
	leaq	9626(%rdx,%rax), %rdi
	leaq	(%r9,%rdi,4), %rbx
	leaq	104(%rsi,%r8), %rdi
	addq	%r8, %r13
	cmpq	%rdi, %rbx
	leaq	108(%rsi,%r8), %r8
	leaq	92(%rsi), %rbx
	setbe	%dil
	cmpq	%r8, %rcx
	setnb	%r8b
	orl	%r8d, %edi
	movq	%rcx, %r8
	subq	%rbx, %r8
	cmpq	$8, %r8
	seta	%r8b
	testb	%r8b, %dil
	je	.L215
	leal	-1(%r10), %edi
	cmpl	$2, %edi
	jbe	.L215
	movl	%r10d, %edi
	movss	0(%r13), %xmm1
	xorl	%eax, %eax
	shrl	$2, %edi
	salq	$4, %rdi
	shufps	$0, %xmm1, %xmm1
	.p2align 4,,10
	.p2align 3
.L216:
	movups	88(%rsi,%rax), %xmm0
	movups	(%rcx,%rax), %xmm5
	addps	%xmm1, %xmm0
	mulps	%xmm5, %xmm0
	movups	%xmm0, (%rcx,%rax)
	addq	$16, %rax
	cmpq	%rdi, %rax
	jne	.L216
	movl	%r10d, %ecx
	andl	$-4, %ecx
	testb	$3, %r10b
	je	.L66
	movl	%ecx, %esi
	leaq	(%rdx,%rsi), %rax
	movss	(%r14,%rsi,4), %xmm0
	addss	0(%r13), %xmm0
	leaq	0(%rbp,%rax,4), %rax
	mulss	(%rax), %xmm0
	movss	%xmm0, (%rax)
	leal	1(%rcx), %eax
	cmpl	%r10d, %eax
	jnb	.L66
	leaq	(%rdx,%rax), %rsi
	movss	(%r14,%rax,4), %xmm0
	addss	0(%r13), %xmm0
	addl	$2, %ecx
	leaq	0(%rbp,%rsi,4), %rsi
	mulss	(%rsi), %xmm0
	movss	%xmm0, (%rsi)
	cmpl	%ecx, %r10d
	jbe	.L66
	leaq	(%rdx,%rcx), %rax
	movss	(%r14,%rcx,4), %xmm0
	addss	0(%r13), %xmm0
	leaq	0(%rbp,%rax,4), %rax
	mulss	(%rax), %xmm0
	movss	%xmm0, (%rax)
	jmp	.L66
.L138:
	testl	%r10d, %r10d
	je	.L66
	movl	%ebx, %r8d
	movl	%r10d, %eax
	leaq	38504(%r9,%rdx,4), %rcx
	salq	$2, %r8
	leaq	9626(%rdx,%rax), %rdi
	leaq	(%r9,%rdi,4), %rbx
	leaq	104(%rsi,%r8), %rdi
	addq	%r8, %r13
	cmpq	%rdi, %rbx
	leaq	108(%rsi,%r8), %r8
	leaq	92(%rsi), %rbx
	setbe	%dil
	cmpq	%r8, %rcx
	setnb	%r8b
	orl	%r8d, %edi
	movq	%rcx, %r8
	subq	%rbx, %r8
	cmpq	$8, %r8
	seta	%r8b
	testb	%r8b, %dil
	je	.L209
	leal	-1(%r10), %edi
	cmpl	$2, %edi
	jbe	.L209
	movl	%r10d, %edi
	movss	0(%r13), %xmm1
	xorl	%eax, %eax
	shrl	$2, %edi
	salq	$4, %rdi
	shufps	$0, %xmm1, %xmm1
	.p2align 4,,10
	.p2align 3
.L210:
	movups	88(%rsi,%rax), %xmm0
	mulps	%xmm1, %xmm0
	movups	%xmm0, (%rcx,%rax)
	addq	$16, %rax
	cmpq	%rdi, %rax
	jne	.L210
	movl	%r10d, %eax
	andl	$-4, %eax
	testb	$3, %r10b
	je	.L66
	movl	%eax, %ecx
	movss	(%r14,%rcx,4), %xmm0
	mulss	0(%r13), %xmm0
	leaq	(%rcx,%rdx), %rsi
	leal	1(%rax), %ecx
	movss	%xmm0, 0(%rbp,%rsi,4)
	cmpl	%ecx, %r10d
	jbe	.L66
	movss	(%r14,%rcx,4), %xmm0
	mulss	0(%r13), %xmm0
	leaq	(%rdx,%rcx), %rsi
	addl	$2, %eax
	movss	%xmm0, 0(%rbp,%rsi,4)
	cmpl	%eax, %r10d
	jbe	.L66
	movl	%eax, %ecx
	movss	(%r14,%rcx,4), %xmm0
	mulss	0(%r13), %xmm0
	leaq	(%rdx,%rcx), %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L139:
	testl	%r10d, %r10d
	je	.L66
	movl	%ebx, %r8d
	movl	%r10d, %eax
	leaq	38504(%r9,%rdx,4), %rcx
	salq	$2, %r8
	leaq	9626(%rdx,%rax), %rdi
	leaq	(%r9,%rdi,4), %rbx
	leaq	104(%rsi,%r8), %rdi
	addq	%r8, %r13
	cmpq	%rdi, %rbx
	leaq	108(%rsi,%r8), %r8
	leaq	92(%rsi), %rbx
	setbe	%dil
	cmpq	%r8, %rcx
	setnb	%r8b
	orl	%r8d, %edi
	movq	%rcx, %r8
	subq	%rbx, %r8
	cmpq	$8, %r8
	seta	%r8b
	testb	%r8b, %dil
	je	.L203
	leal	-1(%r10), %edi
	cmpl	$2, %edi
	jbe	.L203
	movl	%r10d, %edi
	movss	0(%r13), %xmm1
	xorl	%eax, %eax
	shrl	$2, %edi
	salq	$4, %rdi
	shufps	$0, %xmm1, %xmm1
	.p2align 4,,10
	.p2align 3
.L204:
	movups	88(%rsi,%rax), %xmm0
	subps	%xmm1, %xmm0
	movups	%xmm0, (%rcx,%rax)
	addq	$16, %rax
	cmpq	%rdi, %rax
	jne	.L204
	movl	%r10d, %eax
	andl	$-4, %eax
	testb	$3, %r10b
	je	.L66
	movl	%eax, %ecx
	movss	(%r14,%rcx,4), %xmm0
	subss	0(%r13), %xmm0
	leaq	(%rdx,%rcx), %rsi
	leal	1(%rax), %ecx
	movss	%xmm0, 0(%rbp,%rsi,4)
	cmpl	%ecx, %r10d
	jbe	.L66
	movss	(%r14,%rcx,4), %xmm0
	subss	0(%r13), %xmm0
	leaq	(%rdx,%rcx), %rsi
	addl	$2, %eax
	movss	%xmm0, 0(%rbp,%rsi,4)
	cmpl	%eax, %r10d
	jbe	.L66
	movl	%eax, %ecx
	movss	(%r14,%rcx,4), %xmm0
	subss	0(%r13), %xmm0
	leaq	(%rdx,%rcx), %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L140:
	testl	%r10d, %r10d
	je	.L66
	movl	%ebx, %r8d
	movl	%r10d, %eax
	leaq	38504(%r9,%rdx,4), %rcx
	salq	$2, %r8
	leaq	9626(%rdx,%rax), %rdi
	leaq	104(%rsi,%r8), %rbx
	leaq	(%r9,%rdi,4), %rdi
	addq	%r8, %r13
	cmpq	%rdi, %rbx
	leaq	108(%rsi,%r8), %r8
	leaq	92(%rsi), %rbx
	setnb	%dil
	cmpq	%r8, %rcx
	setnb	%r8b
	orl	%r8d, %edi
	movq	%rcx, %r8
	subq	%rbx, %r8
	cmpq	$8, %r8
	seta	%r8b
	testb	%r8b, %dil
	je	.L197
	leal	-1(%r10), %edi
	cmpl	$2, %edi
	jbe	.L197
	movl	%r10d, %edi
	movss	0(%r13), %xmm1
	xorl	%eax, %eax
	shrl	$2, %edi
	salq	$4, %rdi
	shufps	$0, %xmm1, %xmm1
	.p2align 4,,10
	.p2align 3
.L198:
	movups	88(%rsi,%rax), %xmm0
	addps	%xmm1, %xmm0
	movups	%xmm0, (%rcx,%rax)
	addq	$16, %rax
	cmpq	%rdi, %rax
	jne	.L198
	movl	%r10d, %eax
	andl	$-4, %eax
	testb	$3, %r10b
	je	.L66
	movl	%eax, %ecx
	movss	(%r14,%rcx,4), %xmm0
	addss	0(%r13), %xmm0
	leaq	(%rcx,%rdx), %rsi
	leal	1(%rax), %ecx
	movss	%xmm0, 0(%rbp,%rsi,4)
	cmpl	%r10d, %ecx
	jnb	.L66
	movss	(%r14,%rcx,4), %xmm0
	addss	0(%r13), %xmm0
	leaq	(%rcx,%rdx), %rsi
	addl	$2, %eax
	movss	%xmm0, 0(%rbp,%rsi,4)
	cmpl	%r10d, %eax
	jnb	.L66
	movl	%eax, %ecx
	movss	(%r14,%rcx,4), %xmm0
	addss	0(%r13), %xmm0
	leaq	(%rdx,%rcx), %rax
	movss	%xmm0, 0(%rbp,%rax,4)
	jmp	.L66
.L135:
	testl	%r10d, %r10d
	je	.L66
	leaq	38504(%r9,%rdx,4), %rcx
	leaq	92(%rsi), %r8
	movq	%rcx, %rax
	leal	-1(%r10), %edi
	subq	%r8, %rax
	cmpq	$24, %rax
	jbe	.L191
	cmpl	$2, %edi
	jbe	.L191
	movl	%r10d, %edi
	xorl	%eax, %eax
	shrl	$2, %edi
	salq	$4, %rdi
	.p2align 4,,10
	.p2align 3
.L192:
	movups	88(%rsi,%rax), %xmm0
	movups	104(%rsi,%rax), %xmm6
	movups	(%rcx,%rax), %xmm7
	subps	%xmm6, %xmm0
	mulps	%xmm7, %xmm0
	movups	%xmm0, (%rcx,%rax)
	addq	$16, %rax
	cmpq	%rdi, %rax
	jne	.L192
	movl	%r10d, %eax
	andl	$-4, %eax
	testb	$3, %r10b
	je	.L66
	movl	%eax, %ecx
	leaq	(%rdx,%rcx), %rdi
	movss	88(%rsi,%rcx,4), %xmm0
	subss	104(%rsi,%rcx,4), %xmm0
	leaq	0(%rbp,%rdi,4), %rdi
	leal	1(%rax), %ecx
	mulss	(%rdi), %xmm0
	movss	%xmm0, (%rdi)
	cmpl	%ecx, %r10d
	jbe	.L66
	leaq	(%rdx,%rcx), %rdi
	movss	88(%rsi,%rcx,4), %xmm0
	subss	104(%rsi,%rcx,4), %xmm0
	addl	$2, %eax
	leaq	0(%rbp,%rdi,4), %rdi
	mulss	(%rdi), %xmm0
	movss	%xmm0, (%rdi)
	cmpl	%eax, %r10d
	jbe	.L66
	movl	%eax, %ecx
	leaq	(%rdx,%rcx), %rax
	movss	88(%rsi,%rcx,4), %xmm0
	subss	104(%rsi,%rcx,4), %xmm0
	leaq	0(%rbp,%rax,4), %rax
	mulss	(%rax), %xmm0
	movss	%xmm0, (%rax)
	jmp	.L66
.L136:
	salq	$4, %r11
	movss	(%r14,%r10,4), %xmm0
	subss	0(%r13,%rbx,4), %xmm0
	addq	%r11, %rbp
	mulss	0(%rbp), %xmm0
	movss	%xmm0, 0(%rbp)
	jmp	.L66
.L68:
	movl	$-1, 724(%r9)
	jmp	.L66
.L99:
	cmpl	$1, %r10d
	je	.L458
	cmpl	$2, %r10d
	jne	.L100
	testl	$2147483648, %edx
	je	.L100
	movabsq	$-4294967296, %rax
	orq	%rax, %rdx
	jmp	.L100
.L105:
	cmpl	$1, %r12d
	je	.L459
	cmpl	$2, %r12d
	jne	.L97
	movw	%dx, 732(%r9)
	jmp	.L97
.L102:
	cmpl	$1, %r12d
	je	.L460
	cmpl	$2, %r12d
	jne	.L450
	movzwl	732(%r9), %edx
	movq	%rdx, (%rsi)
	jmp	.L97
.L458:
	movq	%rdx, %rax
	orq	$-65536, %rax
	testb	$-128, %dh
	cmovne	%rax, %rdx
	jmp	.L100
.L459:
	movl	%edx, 708(%r9)
	jmp	.L97
.L460:
	movl	708(%r9), %edx
	movq	%rdx, (%rsi)
	jmp	.L97
.L270:
	testl	%ecx, %ecx
	je	.L271
	sall	$16, %ecx
	orl	$939524096, %ecx
	jmp	.L271
.L272:
	testl	%ecx, %ecx
	je	.L273
	sall	$16, %ecx
	orl	$939524096, %ecx
	jmp	.L273
.L274:
	testl	%ecx, %ecx
	je	.L275
	sall	$16, %ecx
	orl	$939524096, %ecx
	jmp	.L275
.L276:
	testl	%ecx, %ecx
	je	.L277
	sall	$16, %ecx
	orl	$939524096, %ecx
	jmp	.L277
.L278:
	testl	%ecx, %ecx
	je	.L279
	sall	$16, %ecx
	orl	$939524096, %ecx
	jmp	.L279
.L280:
	testl	%ecx, %ecx
	je	.L281
	sall	$16, %ecx
	orl	$939524096, %ecx
	jmp	.L281
.L457:
	testl	%eax, %eax
	je	.L284
	sall	$16, %eax
	orl	$939524096, %eax
	jmp	.L284
.L191:
	salq	$4, %r11
	addq	%rdi, %rdx
	leaq	38504(%r9,%r11), %rax
	leaq	38508(%r9,%rdx,4), %rdx
.L195:
	movss	-16(%r13), %xmm0
	subss	0(%r13), %xmm0
	addq	$4, %rax
	addq	$4, %r13
	mulss	-4(%rax), %xmm0
	movss	%xmm0, -4(%rax)
	cmpq	%rdx, %rax
	jne	.L195
	jmp	.L66
.L203:
	salq	$4, %r11
	xorl	%edx, %edx
	addq	%r11, %r9
.L207:
	movss	88(%rsi,%rdx,4), %xmm0
	subss	0(%r13), %xmm0
	movss	%xmm0, 38504(%r9,%rdx,4)
	addq	$1, %rdx
	cmpq	%rax, %rdx
	jne	.L207
	jmp	.L66
.L167:
	salq	$4, %r11
	xorl	%eax, %eax
	addq	%r11, %r9
.L171:
	movss	104(%rsi,%rax,4), %xmm0
	addss	88(%rsi,%rax,4), %xmm0
	movss	%xmm0, 38504(%r9,%rax,4)
	addq	$1, %rax
	cmpq	%rax, %r10
	jne	.L171
	jmp	.L66
.L197:
	salq	$4, %r11
	xorl	%edx, %edx
	addq	%r11, %r9
.L201:
	movss	88(%rsi,%rdx,4), %xmm0
	addss	0(%r13), %xmm0
	movss	%xmm0, 38504(%r9,%rdx,4)
	addq	$1, %rdx
	cmpq	%rax, %rdx
	jne	.L201
	jmp	.L66
.L185:
	salq	$4, %r11
	addq	%rdi, %rdx
	leaq	38504(%r9,%r11), %rax
	leaq	38508(%r9,%rdx,4), %rdx
.L189:
	movss	0(%r13), %xmm0
	addss	-16(%r13), %xmm0
	addq	$4, %rax
	addq	$4, %r13
	mulss	-4(%rax), %xmm0
	movss	%xmm0, -4(%rax)
	cmpq	%rdx, %rax
	jne	.L189
	jmp	.L66
.L235:
	salq	$4, %r11
	xorl	%eax, %eax
	movss	.LC1(%rip), %xmm1
	addq	%r11, %r9
.L239:
	movss	88(%rsi,%rax,4), %xmm0
	xorps	%xmm1, %xmm0
	movss	%xmm0, 38504(%r9,%rax,4)
	addq	$1, %rax
	cmpq	%r10, %rax
	jne	.L239
	jmp	.L66
.L228:
	salq	$4, %r11
	xorl	%eax, %eax
	addq	%r11, %r9
.L232:
	movss	88(%rsi,%rax,4), %xmm0
	movss	%xmm0, 38504(%r9,%rax,4)
	addq	$1, %rax
	cmpq	%r10, %rax
	jne	.L232
	jmp	.L66
.L173:
	salq	$4, %r11
	xorl	%eax, %eax
	addq	%r11, %r9
.L177:
	movss	88(%rsi,%rax,4), %xmm0
	subss	104(%rsi,%rax,4), %xmm0
	movss	%xmm0, 38504(%r9,%rax,4)
	addq	$1, %rax
	cmpq	%r10, %rax
	jne	.L177
	jmp	.L66
.L215:
	salq	$4, %r11
	xorl	%edx, %edx
	addq	%r11, %r9
.L219:
	movss	88(%rsi,%rdx,4), %xmm0
	addss	0(%r13), %xmm0
	mulss	38504(%r9,%rdx,4), %xmm0
	movss	%xmm0, 38504(%r9,%rdx,4)
	addq	$1, %rdx
	cmpq	%rax, %rdx
	jne	.L219
	jmp	.L66
.L209:
	salq	$4, %r11
	xorl	%edx, %edx
	addq	%r11, %r9
.L213:
	movss	88(%rsi,%rdx,4), %xmm0
	mulss	0(%r13), %xmm0
	movss	%xmm0, 38504(%r9,%rdx,4)
	addq	$1, %rdx
	cmpq	%rax, %rdx
	jne	.L213
	jmp	.L66
.L179:
	salq	$4, %r11
	xorl	%eax, %eax
	addq	%r11, %r9
.L183:
	movss	104(%rsi,%rax,4), %xmm0
	mulss	88(%rsi,%rax,4), %xmm0
	movss	%xmm0, 38504(%r9,%rax,4)
	addq	$1, %rax
	cmpq	%r10, %rax
	jne	.L183
	jmp	.L66
.L221:
	salq	$4, %r11
	xorl	%edx, %edx
	addq	%r11, %r9
.L225:
	movss	88(%rsi,%rdx,4), %xmm0
	subss	0(%r13), %xmm0
	mulss	38504(%r9,%rdx,4), %xmm0
	movss	%xmm0, 38504(%r9,%rdx,4)
	addq	$1, %rdx
	cmpq	%rax, %rdx
	jne	.L225
	jmp	.L66
.L298:
	xorl	%eax, %eax
	jmp	.L246
	.cfi_endproc
.LFE26:
	.size	executeInstruction, .-executeInstruction
	.p2align 4
	.globl	AX_execute
	.type	AX_execute, @function
AX_execute:
.LFB27:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	cmpb	$1, 734(%rdi)
	movq	%rdi, %rbp
	je	.L465
.L462:
	leaq	80(%rbp), %rsi
	movq	%rbp, %rdi
	call	executeInstruction
	cmpb	$0, 736(%rbp)
	je	.L463
	leaq	216(%rbp), %rsi
	movq	%rbp, %rdi
	call	executeInstruction
.L463:
	xorl	%eax, %eax
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L465:
	.cfi_restore_state
	movl	720(%rdi), %esi
	call	executeDelayedInstruction
	jmp	.L462
	.cfi_endproc
.LFE27:
	.size	AX_execute, .-AX_execute
	.section	.rodata
	.align 32
	.type	sizemask.1, @object
	.size	sizemask.1, 32
sizemask.1:
	.quad	255
	.quad	65535
	.quad	4294967295
	.quad	-1
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC1:
	.long	-2147483648
	.long	0
	.long	0
	.long	0
	.align 16
.LC2:
	.long	-2147483648
	.long	-2147483648
	.long	-2147483648
	.long	-2147483648
	.align 16
.LC3:
	.long	15360
	.long	15360
	.long	15360
	.long	15360
	.align 16
.LC4:
	.value	-32768
	.value	-32768
	.value	-32768
	.value	-32768
	.value	-32768
	.value	-32768
	.value	-32768
	.value	-32768
	.align 16
.LC5:
	.value	1023
	.value	1023
	.value	1023
	.value	1023
	.value	1023
	.value	1023
	.value	1023
	.value	1023
	.align 16
.LC6:
	.long	128
	.long	128
	.long	128
	.long	128
	.align 16
.LC7:
	.long	16384
	.long	16384
	.long	16384
	.long	16384
	.align 16
.LC8:
	.long	1920
	.long	1920
	.long	1920
	.long	1920
	.align 16
.LC9:
	.value	16384
	.value	16384
	.value	16384
	.value	16384
	.value	16384
	.value	16384
	.value	16384
	.value	16384
	.align 16
.LC10:
	.long	14336
	.long	14336
	.long	14336
	.long	14336
	.align 16
.LC11:
	.long	-2147483648
	.long	-2147483648
	.long	-2147483648
	.long	-2147483648
	.align 16
.LC12:
	.long	8380416
	.long	8380416
	.long	8380416
	.long	8380416
	.align 16
.LC13:
	.long	0
	.long	-2147483648
	.long	0
	.long	0
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
