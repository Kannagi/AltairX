	.file	"AXS.c"
	.text
	.p2align 4
	.globl	AXS_decode_execute
	.type	AXS_decode_execute, @function
AXS_decode_execute:
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
	movl	144(%rdi), %ecx
	movq	80(%rdi), %rdx
	addq	$1, 96(%rdi)
	cmpb	$1, 186(%rdi)
	movl	(%rdx,%rcx,4), %ebx
	movq	%rcx, %rax
	je	.L10
.L2:
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
	xorl	%eax, %eax
	movq	%rbp, %rdi
	call	AX_execute_unit0@PLT
	andl	$1, %ebx
	popq	%rcx
	.cfi_def_cfa_offset 40
	popq	%rsi
	.cfi_def_cfa_offset 32
	jne	.L11
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
.L11:
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
	xorl	%eax, %eax
	shrl	$20, %edx
	andl	$63, %ecx
	shrl	$26, %esi
	andl	$127, %r8d
	andl	$63, %edx
	call	AX_execute_unit1@PLT
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
.L10:
	.cfi_restore_state
	xorl	%eax, %eax
	call	executeDelayedInstruction@PLT
	movl	144(%rbp), %eax
	jmp	.L2
	.cfi_endproc
.LFE26:
	.size	AXS_decode_execute, .-AXS_decode_execute
	.globl	swindow
	.bss
	.align 32
	.type	swindow, @object
	.size	swindow, 184
swindow:
	.zero	184
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
