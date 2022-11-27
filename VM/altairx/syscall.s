	.file	"syscall.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Altair K1 , VM Machine "
	.text
	.p2align 4
	.globl	AX_syscall_emul
	.type	AX_syscall_emul, @function
AX_syscall_emul:
.LFB22:
	.cfi_startproc
	cmpb	$1, 188(%rdi)
	jne	.L14
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	37472(%rdi), %rax
	movq	%rdi, %rbx
	testb	$-16, %al
	jne	.L3
	andl	$15, %eax
	movq	37480(%rdi), %rdi
	movq	37488(%rbx), %rsi
	movq	37496(%rbx), %rbp
	cmpq	$14, %rax
	ja	.L3
	leaq	.L5(%rip), %rcx
	movslq	(%rcx,%rax,4), %rax
	addq	%rcx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L5:
	.long	.L12-.L5
	.long	.L11-.L5
	.long	.L10-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L9-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L8-.L5
	.long	.L7-.L5
	.long	.L6-.L5
	.long	.L4-.L5
	.text
	.p2align 4,,10
	.p2align 3
.L4:
	movq	stdin(%rip), %rdi
	call	getc@PLT
	cltq
	movq	%rax, 37472(%rbx)
.L3:
	movb	$0, 188(%rbx)
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L14:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L6:
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	call	ftell@PLT
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L7:
	movl	%ebp, %edx
	call	fseek@PLT
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L8:
	call	fclose@PLT
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L9:
	movq	%rbx, %rdi
	call	AX_Memory_Map@PLT
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	movq	%rax, %r12
	call	AX_Memory_Map@PLT
	movq	%r12, %rdi
	movq	%rax, %rsi
	call	fopen@PLT
	movq	%rax, 37472(%rbx)
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L10:
	pxor	%xmm0, %xmm0
	movups	%xmm0, 37472(%rbx)
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L11:
	movq	%rdi, %rsi
	movq	%rbx, %rdi
	call	AX_Memory_Map@PLT
	movq	%rax, %rdi
	xorl	%eax, %eax
	call	printf@PLT
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L12:
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	jmp	.L3
	.cfi_endproc
.LFE22:
	.size	AX_syscall_emul, .-AX_syscall_emul
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
