	.file	"test.c"
	.text
	.p2align 4
	.type	to_string_tree_table.constprop.0, @function
to_string_tree_table.constprop.0:
.LFB28:
	.cfi_startproc
	movdqa	.LC0(%rip), %xmm0
	movups	%xmm0, (%rdi)
	ret
	.cfi_endproc
.LFE28:
	.size	to_string_tree_table.constprop.0, .-to_string_tree_table.constprop.0
	.p2align 4
	.globl	rdtsc
	.type	rdtsc, @function
rdtsc:
.LFB22:
	.cfi_startproc
#APP
# 15 "/home/Kannagi/Documents/Projet/AltairX/Git_AX/VM/altairx/test.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	orq	%rdx, %rax
	ret
	.cfi_endproc
.LFE22:
	.size	rdtsc, .-rdtsc
	.globl	test_loop
	.type	test_loop, @function
test_loop:
.LFB23:
	.cfi_startproc
	xorl	%eax, %eax
.L5:
	cmpl	%eax, %edi
	jle	.L7
	addl	$5, (%rsi,%rax,4)
	incq	%rax
	jmp	.L5
.L7:
	ret
	.cfi_endproc
.LFE23:
	.size	test_loop, .-test_loop
	.globl	test_loop2
	.type	test_loop2, @function
test_loop2:
.LFB24:
	.cfi_startproc
	movl	%edi, %edx
	xorl	%eax, %eax
.L9:
	cmpl	%eax, %edi
	jle	.L11
	addl	%edx, %edx
	movl	%edx, (%rsi,%rax,4)
	incq	%rax
	jmp	.L9
.L11:
	ret
	.cfi_endproc
.LFE24:
	.size	test_loop2, .-test_loop2
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"tab Helba   : "
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"%ld cycles (total cycles : %ld)\n"
	.text
	.p2align 4
	.globl	test_perf
	.type	test_perf, @function
test_perf:
.LFB25:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movl	$10000, %edi
	subq	$400000, %rsp
	.cfi_def_cfa_offset 400016
	movq	%rsp, %rsi
	call	test_loop
#APP
# 15 "/home/Kannagi/Documents/Projet/AltairX/Git_AX/VM/altairx/test.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movq	%rdx, %rcx
	orq	%rax, %rcx
	call	test_loop2
#APP
# 15 "/home/Kannagi/Documents/Projet/AltairX/Git_AX/VM/altairx/test.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	leaq	.LC1(%rip), %rdi
	orq	%rax, %rdx
	xorl	%eax, %eax
	subq	%rcx, %rdx
	movq	%rdx, %r12
	call	printf@PLT
	movq	%r12, %rax
	leaq	.LC2(%rip), %rdi
	movabsq	$3777893186295716171, %rsi
	mulq	%rsi
	xorl	%eax, %eax
	movq	%rdx, %rsi
	movq	%r12, %rdx
	shrq	$11, %rsi
	call	printf@PLT
	addq	$400000, %rsp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE25:
	.size	test_perf, .-test_perf
	.p2align 4
	.globl	to_string_tree_table
	.type	to_string_tree_table, @function
to_string_tree_table:
.LFB26:
	.cfi_startproc
	movq	%rsi, %r8
	movq	%rdi, %rax
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movabsq	$-6067343680855748867, %rsi
	movabsq	$3777893186295716171, %rcx
	mulq	%rsi
	movabsq	$2951479051793528259, %r11
	movq	%rdx, %rsi
	shrq	$26, %rsi
	imulq	$100000000, %rsi, %rax
	subq	%rax, %rdi
	movq	%rsi, %rax
	mulq	%rcx
	movq	%rdx, %rbx
	movq	%rdx, %r9
	shrq	$11, %rbx
	shrq	$13, %r9
	imulq	$10000, %rbx, %rax
	subq	%rax, %rsi
	movq	%rdi, %rax
	mulq	%rcx
	movq	%rdx, %r10
	movq	%rdx, %rcx
	shrq	$11, %r10
	shrq	$13, %rcx
	imulq	$10000, %r10, %rax
	subq	%rax, %rdi
	movq	%r9, %rax
	leaq	table.0(%rip), %r9
	mulq	%r11
	shrq	$2, %rdx
	movzwl	(%r9,%rdx,2), %eax
	movw	%ax, (%r8)
	leaq	(%rdx,%rdx,4), %rax
	movq	%rsi, %rdx
	leaq	(%rax,%rax,4), %rax
	shrq	$2, %rdx
	salq	$2, %rax
	subq	%rax, %rbx
	movzwl	(%r9,%rbx,2), %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	movw	%ax, 2(%r8)
	movq	%rdx, %rax
	mulq	%r11
	shrq	$2, %rdx
	movzwl	(%r9,%rdx,2), %eax
	movw	%ax, 4(%r8)
	leaq	(%rdx,%rdx,4), %rax
	leaq	(%rax,%rax,4), %rax
	salq	$2, %rax
	subq	%rax, %rsi
	movzwl	(%r9,%rsi,2), %eax
	movw	%ax, 6(%r8)
	movq	%rcx, %rax
	mulq	%r11
	shrq	$2, %rdx
	movzwl	(%r9,%rdx,2), %eax
	movw	%ax, 8(%r8)
	leaq	(%rdx,%rdx,4), %rax
	movq	%rdi, %rdx
	leaq	(%rax,%rax,4), %rax
	shrq	$2, %rdx
	salq	$2, %rax
	subq	%rax, %r10
	movzwl	(%r9,%r10,2), %eax
	movw	%ax, 10(%r8)
	movq	%rdx, %rax
	mulq	%r11
	shrq	$2, %rdx
	movzwl	(%r9,%rdx,2), %eax
	movw	%ax, 12(%r8)
	leaq	(%rdx,%rdx,4), %rax
	leaq	(%rax,%rax,4), %rax
	salq	$2, %rax
	subq	%rax, %rdi
	movzwl	(%r9,%rdi,2), %eax
	movw	%ax, 14(%r8)
	ret
	.cfi_endproc
.LFE26:
	.size	to_string_tree_table, .-to_string_tree_table
	.section	.rodata.str1.1
.LC3:
	.string	"string Helba  %s  : "
	.text
	.p2align 4
	.globl	test_perf2
	.type	test_perf2, @function
test_perf2:
.LFB27:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	subq	$100000, %rsp
	.cfi_def_cfa_offset 100016
	movq	%rsp, %rsi
	movq	%rsi, %rdi
	call	to_string_tree_table.constprop.0
#APP
# 15 "/home/Kannagi/Documents/Projet/AltairX/Git_AX/VM/altairx/test.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movq	%rdx, %rcx
	xorl	%edx, %edx
	orq	%rax, %rcx
	.p2align 4,,10
	.p2align 3
.L18:
	movq	%rsi, %rdi
	addl	$1, %edx
	call	to_string_tree_table.constprop.0
	cmpl	$10000, %edx
	jne	.L18
#APP
# 15 "/home/Kannagi/Documents/Projet/AltairX/Git_AX/VM/altairx/test.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	leaq	.LC3(%rip), %rdi
	orq	%rax, %rdx
	xorl	%eax, %eax
	subq	%rcx, %rdx
	movq	%rdx, %r12
	call	printf@PLT
	movq	%r12, %rax
	leaq	.LC2(%rip), %rdi
	movabsq	$3777893186295716171, %rsi
	mulq	%rsi
	xorl	%eax, %eax
	movq	%rdx, %rsi
	movq	%r12, %rdx
	shrq	$11, %rsi
	call	printf@PLT
	addq	$100000, %rsp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE27:
	.size	test_perf2, .-test_perf2
	.section	.rodata
	.align 32
	.type	table.0, @object
	.size	table.0, 200
table.0:
	.ascii	"000102030405060708091011121314151617181920212223242526272829"
	.ascii	"303132333435363738394041424344454647484950515253545556575859"
	.ascii	"606162636465666768697071727374757677787980818283848586878889"
	.ascii	"90919293949596979899"
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.value	12336
	.value	12336
	.value	12336
	.value	12336
	.value	12336
	.value	12592
	.value	12336
	.value	12336
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
