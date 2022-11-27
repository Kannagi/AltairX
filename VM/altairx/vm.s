	.file	"vm.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"\n%ld instructions\n"
.LC1:
	.string	"%ld cycle\n"
.LC2:
	.string	"IPC : %f\n\n"
.LC3:
	.string	"%ld inst cache cycle\n"
.LC4:
	.string	"%ld data cache cycle\n"
.LC5:
	.string	"%ld total cycle\n"
.LC6:
	.string	"%ld icache miss\n"
.LC7:
	.string	"%ld dcache miss\n"
	.text
	.p2align 4
	.type	AX_exe_core_printinfo, @function
AX_exe_core_printinfo:
.LFB29:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	88(%rdi), %rsi
	movq	%rdi, %rbx
	xorl	%eax, %eax
	leaq	.LC0(%rip), %rdi
	call	printf@PLT
	movq	96(%rbx), %rsi
	leaq	.LC1(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	movq	88(%rbx), %rax
	testq	%rax, %rax
	js	.L2
	pxor	%xmm0, %xmm0
	cvtsi2ssq	%rax, %xmm0
	movq	96(%rbx), %rax
	testq	%rax, %rax
	js	.L4
.L8:
	pxor	%xmm1, %xmm1
	cvtsi2ssq	%rax, %xmm1
.L5:
	divss	%xmm1, %xmm0
	leaq	.LC2(%rip), %rdi
	movl	$1, %eax
	cvtss2sd	%xmm0, %xmm0
	call	printf@PLT
	movq	128(%rbx), %rsi
	xorl	%eax, %eax
	leaq	.LC3(%rip), %rdi
	call	printf@PLT
	movq	136(%rbx), %rsi
	leaq	.LC4(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	movq	136(%rbx), %rsi
	leaq	.LC5(%rip), %rdi
	xorl	%eax, %eax
	addq	96(%rbx), %rsi
	addq	128(%rbx), %rsi
	call	printf@PLT
	movq	112(%rbx), %rsi
	leaq	.LC6(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	movq	120(%rbx), %rsi
	leaq	.LC7(%rip), %rdi
	xorl	%eax, %eax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
	.p2align 4,,10
	.p2align 3
.L2:
	.cfi_restore_state
	movq	%rax, %rdx
	andl	$1, %eax
	pxor	%xmm0, %xmm0
	shrq	%rdx
	orq	%rax, %rdx
	movq	96(%rbx), %rax
	cvtsi2ssq	%rdx, %xmm0
	addss	%xmm0, %xmm0
	testq	%rax, %rax
	jns	.L8
.L4:
	movq	%rax, %rdx
	andl	$1, %eax
	pxor	%xmm1, %xmm1
	shrq	%rdx
	orq	%rax, %rdx
	cvtsi2ssq	%rdx, %xmm1
	addss	%xmm1, %xmm1
	jmp	.L5
	.cfi_endproc
.LFE29:
	.size	AX_exe_core_printinfo, .-AX_exe_core_printinfo
	.p2align 4
	.globl	AX_Memory_Map
	.type	AX_Memory_Map, @function
AX_Memory_Map:
.LFB22:
	.cfi_startproc
	testl	$2147483648, %esi
	je	.L10
	movl	76(%rdi), %eax
	movq	48(%rdi), %rdx
	subl	$1, %eax
.L11:
	andq	%rax, %rsi
	leaq	(%rdx,%rsi), %rax
	ret
	.p2align 4,,10
	.p2align 3
.L10:
	testl	$1073741824, %esi
	je	.L12
	movl	72(%rdi), %eax
	movq	40(%rdi), %rdx
	subl	$1, %eax
	andq	%rax, %rsi
	leaq	(%rdx,%rsi), %rax
	ret
	.p2align 4,,10
	.p2align 3
.L12:
	testl	$536870912, %esi
	jne	.L18
	testl	$268435456, %esi
	je	.L14
	movl	64(%rdi), %eax
	movq	16(%rdi), %rdx
	subl	$1, %eax
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L18:
	movl	68(%rdi), %eax
	movq	32(%rdi), %rdx
	subl	$1, %eax
	andq	%rax, %rsi
	leaq	(%rdx,%rsi), %rax
	ret
	.p2align 4,,10
	.p2align 3
.L14:
	testl	$134217728, %esi
	jne	.L19
	testl	$67108864, %esi
	je	.L16
	movl	56(%rdi), %eax
	movq	(%rdi), %rdx
	subl	$1, %eax
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L19:
	movl	60(%rdi), %eax
	movq	8(%rdi), %rdx
	subl	$1, %eax
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L16:
	testl	$33554432, %esi
	je	.L17
	movl	%esi, %eax
	movq	24(%rdi), %rdx
	andl	$524287, %eax
	movl	%eax, 168(%rdi)
	movl	$524287, %eax
	jmp	.L11
.L17:
	leaq	318(%rdi), %rdx
	movl	$32767, %eax
	jmp	.L11
	.cfi_endproc
.LFE22:
	.size	AX_Memory_Map, .-AX_Memory_Map
	.section	.rodata.str1.1
.LC8:
	.string	"rb"
	.text
	.p2align 4
	.globl	AX_load_prog
	.type	AX_load_prog, @function
AX_load_prog:
.LFB23:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
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
	movq	%rsi, %rbx
	leaq	.LC8(%rip), %rsi
	call	fopen@PLT
	movq	%rax, %rbp
	movl	$1, %eax
	testq	%rbp, %rbp
	je	.L20
	movl	$2, %edx
	xorl	%esi, %esi
	movq	%rbp, %rdi
	call	fseek@PLT
	movq	%rbp, %rdi
	call	ftell@PLT
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%rbp, %rdi
	movq	%rax, %r12
	call	fseek@PLT
	movslq	%r12d, %r14
	movq	%r14, %rdi
	call	malloc@PLT
	movq	%rbp, %rcx
	movq	%r14, %rdx
	movl	$1, %esi
	movq	%rax, %rdi
	movq	%rax, %r13
	call	fread@PLT
	movq	%rbp, %rdi
	call	fclose@PLT
	movq	%r13, (%rbx)
	xorl	%eax, %eax
	movl	%r12d, 56(%rbx)
.L20:
	popq	%rbx
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
	.cfi_endproc
.LFE23:
	.size	AX_load_prog, .-AX_load_prog
	.p2align 4
	.globl	AX_boot_rom
	.type	AX_boot_rom, @function
AX_boot_rom:
.LFB24:
	.cfi_startproc
	movq	80(%rdi), %rdi
	movslq	%edx, %rdx
	jmp	memcpy@PLT
	.cfi_endproc
.LFE24:
	.size	AX_boot_rom, .-AX_boot_rom
	.p2align 4
	.globl	AX_init_mem
	.type	AX_init_mem, @function
AX_init_mem:
.LFB25:
	.cfi_startproc
	movd	%ecx, %xmm1
	movd	%edx, %xmm3
	movd	%r8d, %xmm0
	sall	$20, %esi
	movd	%r9d, %xmm4
	punpckldq	%xmm3, %xmm1
	movdqa	.LC9(%rip), %xmm2
	movl	%esi, 588(%rdi)
	punpckldq	%xmm4, %xmm0
	punpcklqdq	%xmm1, %xmm0
	movdqa	%xmm0, %xmm1
	psrlq	$32, %xmm0
	pmuludq	%xmm2, %xmm1
	pmuludq	%xmm2, %xmm0
	pshufd	$8, %xmm1, %xmm1
	pshufd	$8, %xmm0, %xmm0
	punpckldq	%xmm0, %xmm1
	movups	%xmm1, 572(%rdi)
	ret
	.cfi_endproc
.LFE25:
	.size	AX_init_mem, .-AX_init_mem
	.p2align 4
	.globl	AX_init_proc
	.type	AX_init_proc, @function
AX_init_proc:
.LFB26:
	.cfi_startproc
	movq	.LC10(%rip), %rax
	movl	$0, 600(%rdi)
	movq	%rax, 592(%rdi)
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE26:
	.size	AX_init_proc, .-AX_init_proc
	.p2align 4
	.globl	AX_init_proc_mem
	.type	AX_init_proc_mem, @function
AX_init_proc_mem:
.LFB27:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movl	588(%rdi), %edi
	call	malloc@PLT
	movl	584(%rbx), %edi
	movq	%rax, 560(%rbx)
	call	malloc@PLT
	movl	580(%rbx), %edi
	movq	%rax, 552(%rbx)
	call	malloc@PLT
	movl	572(%rbx), %edi
	movq	%rax, 544(%rbx)
	call	malloc@PLT
	movl	576(%rbx), %edi
	movq	%rax, 520(%rbx)
	call	malloc@PLT
	movl	$65536, %edi
	movq	%rax, 528(%rbx)
	call	malloc@PLT
	movq	%rax, 536(%rbx)
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE27:
	.size	AX_init_proc_mem, .-AX_init_proc_mem
	.p2align 4
	.globl	AX_add_core
	.type	AX_add_core, @function
AX_add_core:
.LFB28:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$1, %eax
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movl	596(%rdi), %ebp
	cmpl	$64, %ebp
	jg	.L31
	movq	%rdi, %rbx
	movl	$38976, %edi
	call	malloc@PLT
	movdqu	512(%rbx), %xmm2
	movdqu	528(%rbx), %xmm3
	pxor	%xmm1, %xmm1
	movq	%rax, %rdx
	movdqu	544(%rbx), %xmm4
	movdqu	560(%rbx), %xmm5
	movb	$0, 188(%rax)
	leaq	33352(%rdx), %rdi
	movl	%edx, %ecx
	movups	%xmm2, (%rax)
	pxor	%xmm0, %xmm0
	movdqu	576(%rbx), %xmm6
	andq	$-8, %rdi
	movb	$0, 186(%rax)
	movl	$64, 144(%rax)
	subl	%edi, %ecx
	movl	$-1, 180(%rax)
	addl	$35392, %ecx
	movl	$0, 168(%rax)
	shrl	$3, %ecx
	movups	%xmm3, 16(%rax)
	movups	%xmm4, 32(%rax)
	movups	%xmm5, 48(%rax)
	movups	%xmm6, 64(%rax)
	movups	%xmm1, 88(%rax)
	movq	560(%rbx), %rax
	movq	$0, 33344(%rdx)
	movq	%rax, 80(%rdx)
	xorl	%eax, %eax
	movq	$0, 35384(%rdx)
	rep stosq
	leaq	33096(%rdx), %rdi
	movl	%edx, %ecx
	movq	$0, 33088(%rdx)
	movq	$0, 33336(%rdx)
	andq	$-8, %rdi
	subl	%edi, %ecx
	addl	$33344, %ecx
	shrl	$3, %ecx
	rep stosq
	movslq	%ebp, %rax
	addl	$1, %ebp
	movups	%xmm0, 189(%rdx)
	movups	%xmm0, 205(%rdx)
	movups	%xmm0, 221(%rdx)
	movups	%xmm0, 237(%rdx)
	movups	%xmm0, 253(%rdx)
	movups	%xmm0, 269(%rdx)
	movups	%xmm0, 285(%rdx)
	movups	%xmm0, 301(%rdx)
	movq	$0, 136(%rdx)
	movq	%rdx, (%rbx,%rax,8)
	xorl	%eax, %eax
	movl	%ebp, 596(%rbx)
	movups	%xmm1, 104(%rdx)
	movups	%xmm1, 120(%rdx)
.L31:
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE28:
	.size	AX_add_core, .-AX_add_core
	.section	.rodata.str1.1
.LC12:
	.string	"Frequence : %.2f MHz\n"
	.text
	.p2align 4
	.globl	AX_exe_core
	.type	AX_exe_core, @function
AX_exe_core:
.LFB30:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	call	clock@PLT
	movl	$0, 164(%rbp)
	movq	%rax, %rdx
	xorl	%eax, %eax
.L37:
	leaq	1000000(%rdx), %r12
.L41:
	movl	$65537, %ebx
	jmp	.L39
	.p2align 4,,10
	.p2align 3
.L44:
	call	AX_decode_execute@PLT
	subl	$1, %ebx
	je	.L38
	movl	164(%rbp), %eax
.L39:
	movq	%rbp, %rdi
	testl	%eax, %eax
	je	.L44
	call	AX_exe_core_printinfo
	movl	164(%rbp), %eax
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
.L38:
	.cfi_restore_state
	call	clock@PLT
	cmpq	%r12, %rax
	jg	.L40
	movl	164(%rbp), %eax
	jmp	.L41
.L40:
	movq	96(%rbp), %rax
	testq	%rax, %rax
	js	.L42
	pxor	%xmm0, %xmm0
	cvtsi2ssq	%rax, %xmm0
.L43:
	leaq	.LC12(%rip), %rdi
	movl	$1, %eax
	divss	.LC11(%rip), %xmm0
	cvtss2sd	%xmm0, %xmm0
	call	printf@PLT
	movq	$0, 96(%rbp)
	call	clock@PLT
	movq	%rax, %rdx
	movl	164(%rbp), %eax
	jmp	.L37
.L42:
	movq	%rax, %rdx
	andl	$1, %eax
	pxor	%xmm0, %xmm0
	shrq	%rdx
	orq	%rax, %rdx
	cvtsi2ssq	%rdx, %xmm0
	addss	%xmm0, %xmm0
	jmp	.L43
	.cfi_endproc
.LFE30:
	.size	AX_exe_core, .-AX_exe_core
	.p2align 4
	.globl	AX_exe_core_mode0
	.type	AX_exe_core_mode0, @function
AX_exe_core_mode0:
.LFB31:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movl	$0, 164(%rdi)
	.p2align 4,,10
	.p2align 3
.L47:
	movq	%rbx, %rdi
	call	AX_decode_execute@PLT
	movq	%rbx, %rdi
	call	AX_syscall_emul@PLT
	movl	164(%rbx), %eax
	testl	%eax, %eax
	je	.L47
	movq	%rbx, %rdi
	call	AX_exe_core_printinfo
	movl	164(%rbx), %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE31:
	.size	AX_exe_core_mode0, .-AX_exe_core_mode0
	.p2align 4
	.globl	AX_exe_core_mode1
	.type	AX_exe_core_mode1, @function
AX_exe_core_mode1:
.LFB32:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movl	$0, 164(%rdi)
	.p2align 4,,10
	.p2align 3
.L51:
	movq	%rbx, %rdi
	call	AX_decode_execute@PLT
	movq	%rbx, %rdi
	call	AX_debug@PLT
	movq	%rbx, %rdi
	call	AX_Pipeline_stall@PLT
	movq	%rbx, %rdi
	call	AX_Cache_miss@PLT
	movq	%rbx, %rdi
	call	AX_syscall_emul@PLT
	movl	164(%rbx), %eax
	testl	%eax, %eax
	je	.L51
	movq	%rbx, %rdi
	call	AX_exe_core_printinfo
	movl	164(%rbx), %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE32:
	.size	AX_exe_core_mode1, .-AX_exe_core_mode1
	.p2align 4
	.globl	AX_exe_core_mode2
	.type	AX_exe_core_mode2, @function
AX_exe_core_mode2:
.LFB33:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movl	$0, 164(%rdi)
	.p2align 4,,10
	.p2align 3
.L55:
	movq	%rbx, %rdi
	call	AX_decode_execute@PLT
	movq	%rbx, %rdi
	call	AX_Pipeline_stall@PLT
	movq	%rbx, %rdi
	call	AX_Cache_miss@PLT
	movq	%rbx, %rdi
	call	AX_syscall_emul@PLT
	movl	164(%rbx), %eax
	testl	%eax, %eax
	je	.L55
	movq	%rbx, %rdi
	call	AX_exe_core_printinfo
	movl	164(%rbx), %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE33:
	.size	AX_exe_core_mode2, .-AX_exe_core_mode2
	.p2align 4
	.globl	AX_exe_core_mode3
	.type	AX_exe_core_mode3, @function
AX_exe_core_mode3:
.LFB34:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movl	$0, 164(%rdi)
	.p2align 4,,10
	.p2align 3
.L59:
	movq	%rbx, %rdi
	call	AX_decode_execute@PLT
	movl	164(%rbx), %eax
	addq	$1, 96(%rbx)
	testl	%eax, %eax
	je	.L59
	movq	%rbx, %rdi
	call	AX_exe_core_printinfo
	movl	164(%rbx), %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE34:
	.size	AX_exe_core_mode3, .-AX_exe_core_mode3
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC9:
	.long	1024
	.long	1024
	.long	1048576
	.long	1048576
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC10:
	.long	1
	.long	0
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC11:
	.long	1232348160
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
