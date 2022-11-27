	.file	"cycle.c"
	.text
	.p2align 4
	.globl	AX_Cache_miss
	.type	AX_Cache_miss, @function
AX_Cache_miss:
.LFB22:
	.cfi_startproc
	movl	144(%rdi), %eax
	shrl	$8, %eax
	movl	%eax, %edx
	andl	$63, %edx
	leaq	8272(%rdx), %rcx
	cmpl	%eax, 33088(%rdi,%rdx,4)
	je	.L2
	movl	%eax, (%rdi,%rcx,4)
	movdqu	104(%rdi), %xmm0
	addq	$94, 128(%rdi)
	paddq	.LC0(%rip), %xmm0
	movups	%xmm0, 104(%rdi)
.L2:
	movl	172(%rdi), %eax
	movl	%eax, %r8d
	movl	%eax, %ecx
	movl	%eax, %esi
	shrl	$10, %eax
	shrl	%r8d
	shrl	$20, %ecx
	movl	%r8d, %edx
	shrl	$14, %esi
	andl	$63, %ecx
	andl	$127, %edx
	andl	$63, %esi
	cmpl	$47, %edx
	ja	.L3
	cmpl	$43, %edx
	ja	.L4
	cmpl	$39, %edx
	jbe	.L14
	movq	37440(%rdi,%rsi,8), %rax
	addq	37440(%rdi,%rcx,8), %rax
	shrq	$7, %rax
	movq	%rax, %rdx
	andl	$1023, %edx
	leaq	8336(%rdx), %rcx
	movl	33344(%rdi,%rdx,4), %edx
	cmpq	%rax, %rdx
	je	.L15
.L8:
	movl	%eax, (%rdi,%rcx,4)
	subq	$-128, 104(%rdi)
	addq	$1, 120(%rdi)
	addq	$38, 136(%rdi)
	ret
	.p2align 4,,10
	.p2align 3
.L3:
	cmpl	$74, %edx
	jne	.L16
	movq	37440(%rdi,%rsi,8), %rax
	salq	$6, %rax
	addq	%rax, 104(%rdi)
.L1:
	ret
	.p2align 4,,10
	.p2align 3
.L14:
	andl	$96, %r8d
	je	.L1
	movzwl	%ax, %eax
.L12:
	addq	37440(%rdi,%rcx,8), %rax
	shrq	$7, %rax
	movq	%rax, %rdx
	andl	$1023, %edx
	leaq	8336(%rdx), %rcx
	movl	33344(%rdi,%rdx,4), %edx
	cmpq	%rax, %rdx
	jne	.L8
	jmp	.L1
	.p2align 4,,10
	.p2align 3
.L15:
	ret
	.p2align 4,,10
	.p2align 3
.L16:
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	andl	$1023, %eax
	jmp	.L12
	.cfi_endproc
.LFE22:
	.size	AX_Cache_miss, .-AX_Cache_miss
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"Cycle %d\n"
	.text
	.p2align 4
	.globl	AX_Pipeline_stall
	.type	AX_Pipeline_stall, @function
AX_Pipeline_stall:
.LFB23:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	leaq	189(%rdi), %rax
	leaq	253(%rdi), %rcx
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	.p2align 4,,10
	.p2align 3
.L19:
	movzbl	(%rax), %edx
	testb	%dl, %dl
	je	.L18
	subl	$1, %edx
	movb	%dl, (%rax)
.L18:
	addq	$1, %rax
	cmpq	%rcx, %rax
	jne	.L19
	leaq	317(%rbp), %rcx
	.p2align 4,,10
	.p2align 3
.L21:
	movzbl	(%rax), %edx
	testb	%dl, %dl
	je	.L20
	subl	$1, %edx
	movb	%dl, (%rax)
.L20:
	addq	$1, %rax
	cmpq	%rcx, %rax
	jne	.L21
	movzbl	317(%rbp), %eax
	testb	%al, %al
	je	.L22
	subl	$1, %eax
	movb	%al, 317(%rbp)
.L22:
	movl	172(%rbp), %edi
	leaq	opcode1_rw(%rip), %r9
	movl	%edi, %ecx
	movl	%edi, %esi
	movl	%edi, %r8d
	shrl	%ecx
	shrl	$20, %esi
	andl	$127, %ecx
	shrl	$14, %r8d
	andl	$63, %esi
	movl	%ecx, %edx
	andl	$63, %r8d
	movzbl	(%r9,%rdx), %edx
	leal	-80(%rcx), %r9d
	cmpl	$16, %r9d
	movl	$0, %r9d
	cmovnb	%r9d, %eax
	movl	%edi, %r9d
	movl	%edx, %r10d
	shrl	$26, %r9d
	movzbl	%al, %ebx
	movl	%edi, %eax
	shrl	$3, %eax
	andl	$1, %eax
	testb	$64, %dl
	cmove	%r9d, %eax
	andl	$16, %r10d
	testb	$2, %dl
	je	.L26
	cmpb	$61, %al
	movzbl	%al, %r9d
	setne	%r11b
	testb	$32, %dl
	je	.L27
	movzbl	253(%rbp,%r9), %r12d
	cmpl	%r12d, %ebx
	jnb	.L30
	testb	%r11b, %r11b
	je	.L30
	movl	%r12d, %ebx
	testb	%r10b, %r10b
	je	.L49
.L31:
	testb	$4, %dl
	je	.L33
.L113:
	movq	%rsi, %rdi
	andl	$63, %edi
	movzbl	253(%rbp,%rdi), %edi
	cmpl	%edi, %ebx
	jnb	.L33
	cmpb	$61, %sil
	cmovne	%edi, %ebx
.L33:
	testb	$8, %dl
	je	.L112
	movq	%r8, %rsi
	andl	$63, %esi
	movzbl	253(%rbp,%rsi), %esi
	cmpl	%esi, %ebx
	jnb	.L112
	cmpb	$61, %r8b
	je	.L112
	movl	%esi, %ebx
.L112:
	movl	$3, %esi
	movl	$4, %r9d
.L35:
	testb	$1, %dl
	je	.L45
	andl	$48, %edx
	movzbl	%al, %eax
	je	.L46
	movb	%r9b, 253(%rbp,%rax)
.L45:
	subl	$64, %ecx
	cmpl	$7, %ecx
	ja	.L47
	movb	$3, 317(%rbp)
.L47:
	leal	1(%rbx), %esi
	leaq	.LC1(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	movl	%ebx, %eax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	addq	%rax, 96(%rbp)
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L26:
	.cfi_restore_state
	testb	%r10b, %r10b
	jne	.L31
.L37:
	testb	$4, %dl
	je	.L39
.L114:
	movq	%rsi, %r9
	andl	$63, %r9d
	movzbl	189(%rbp,%r9), %r9d
	cmpl	%r9d, %ebx
	jnb	.L39
	cmpb	$61, %sil
	cmovne	%r9d, %ebx
.L39:
	testb	$8, %dl
	je	.L41
	movq	%r8, %rsi
	andl	$63, %esi
	movzbl	189(%rbp,%rsi), %esi
	cmpl	%esi, %ebx
	jnb	.L41
	cmpb	$61, %r8b
	cmovne	%esi, %ebx
.L41:
	movl	$3, %esi
	movl	$4, %r9d
	testb	%dl, %dl
	jns	.L35
	andl	$1024, %edi
	jne	.L43
	movq	%r8, %rsi
	andl	$63, %esi
	movzbl	189(%rbp,%rsi), %esi
	cmpl	%esi, %ebx
	jnb	.L43
	cmpb	$61, %r8b
	cmovne	%esi, %ebx
.L43:
	cmpb	$62, %al
	sbbl	%esi, %esi
	andl	$-4, %esi
	addl	$7, %esi
	cmpb	$62, %al
	sbbl	%r9d, %r9d
	andl	$-4, %r9d
	addl	$8, %r9d
	jmp	.L35
.L46:
	movb	%sil, 189(%rbp,%rax)
	jmp	.L45
.L27:
	testb	%r10b, %r10b
	je	.L49
	movzbl	253(%rbp,%r9), %edi
	cmpl	%edi, %ebx
	jnb	.L31
	testb	%r11b, %r11b
	cmovne	%edi, %ebx
	testb	$4, %dl
	je	.L33
	jmp	.L113
.L30:
	testb	%r10b, %r10b
	jne	.L31
.L49:
	movl	%ebx, %r10d
	movzbl	189(%rbp,%r9), %ebx
	cmpl	%r10d, %ebx
	jbe	.L62
	testb	%r11b, %r11b
	jne	.L37
.L62:
	movl	%r10d, %ebx
	testb	$4, %dl
	je	.L39
	jmp	.L114
	.cfi_endproc
.LFE23:
	.size	AX_Pipeline_stall, .-AX_Pipeline_stall
	.section	.rodata
	.align 32
	.type	opcode1_rw, @object
	.size	opcode1_rw, 129
opcode1_rw:
	.string	""
	.string	"\005\005\001\r\r\r\r\r\001\001"
	.string	""
	.string	"\001\001\001\b\205\205\205\205\205\205\205\205\205\205\205\205\205"
	.string	""
	.string	""
	.string	"ABabABab\r\016-.\005\006%&"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	"\006"
	.string	"\002\002\026\026\022\022"
	.string	"\002"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.ascii	"\035\035\035\035\035\035\035\035\035\035\035\035\035\035\035"
	.ascii	"\035\025\025\021\021\025\025\025\025\025\025\025\025\025\025"
	.ascii	"\025\025"
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.quad	1024
	.quad	1
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
