	.file	"io.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%x\n"
	.text
	.p2align 4
	.globl	AX_IO_Read
	.type	AX_IO_Read, @function
AX_IO_Read:
.LFB22:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	xorl	%eax, %eax
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	shrq	$16, %rbp
	movq	24(%rdi), %r12
	movq	%rsi, %rbx
	andl	$15, %ebp
	leaq	.LC0(%rip), %rdi
	movl	%ebp, %esi
	call	printf@PLT
	testl	%ebp, %ebp
	jne	.L1
	testw	%bx, %bx
	je	.L5
.L1:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L5:
	.cfi_restore_state
	andl	$1048575, %ebx
	movl	$2147483904, %eax
	movq	%rax, (%r12,%rbx)
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE22:
	.size	AX_IO_Read, .-AX_IO_Read
	.p2align 4
	.globl	AX_IO_Write
	.type	AX_IO_Write, @function
AX_IO_Write:
.LFB23:
	.cfi_startproc
	movq	(%rdx), %rsi
	leaq	.LC0(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
	.cfi_endproc
.LFE23:
	.size	AX_IO_Write, .-AX_IO_Write
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
