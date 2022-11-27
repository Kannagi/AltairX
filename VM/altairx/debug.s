	.file	"debug.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"NOP"
.LC1:
	.string	"SEXT R%d,R%d"
.LC2:
	.string	"BOOL R%d,R%d"
.LC3:
	.string	"SMOVE R%d,%d"
.LC4:
	.string	"SLTS R%d,R%d,R%d"
.LC5:
	.string	"SLTU R%d,R%d,R%d"
.LC6:
	.string	"SLTSI R%d,R%d,%d"
.LC7:
	.string	"SLTUI R%d,R%d,%d"
.LC8:
	.string	"MOVEN R%d,%d"
.LC9:
	.string	"MOVEU R%d,%d"
.LC10:
	.string	"ADD R%d,R%d,R%d (%d)"
.LC11:
	.string	"SUB R%d,R%d,R%d (%d)"
.LC12:
	.string	"XOR R%d,R%d,R%d (%d)"
.LC13:
	.string	"OR R%d,R%d,R%d (%d)"
.LC14:
	.string	"AND R%d,R%d,R%d (%d)"
.LC15:
	.string	"LSL R%d,R%d,R%d (%d)"
.LC16:
	.string	"ASR R%d,R%d,R%d (%d)"
.LC17:
	.string	"LSR R%d,R%d,R%d (%d)"
.LC18:
	.string	"LDL R%d,%d"
.LC19:
	.string	"STL R%d,%d"
.LC20:
	.string	"LDVL R%d,%d"
.LC21:
	.string	"STVL R%d,%d"
.LC22:
	.string	"LDL1 R%d,%d"
.LC23:
	.string	"STL1 R%d,%d"
.LC24:
	.string	"LDVL1 R%d,%d"
.LC25:
	.string	"STVL1 R%d,%d"
.LC26:
	.string	"LD R%d,R%d[R%d]"
.LC27:
	.string	"ST R%d,R%d[R%d]"
.LC28:
	.string	"LDV R%d,R%d[R%d]"
.LC29:
	.string	"STV R%d,R%d[R%d]"
.LC30:
	.string	"LDI R%d,%d[R%d]"
.LC31:
	.string	"STI R%d,%d[R%d]"
.LC32:
	.string	"LDVI R%d,%d[R%d]"
.LC33:
	.string	"STVI R%d,%d[R%d]"
.LC34:
	.string	"FDIV V%d,V%d,V%d"
.LC35:
	.string	"DDIV V%d,V%d,V%d"
.LC36:
	.string	"FSQRT V%d,V%d"
.LC37:
	.string	"DSQRT V%d,V%d"
.LC38:
	.string	"FSIN V%d,V%d"
.LC39:
	.string	"DSIN V%d,V%d"
.LC40:
	.string	"FSUM V%d,V%d"
.LC41:
	.string	"FEXP V%d,V%d"
.LC42:
	.string	"FATAN2 V%d,V%d,V%d"
.LC43:
	.string	"FATAN V%d,V%d,V%d"
.LC44:
	.string	"DMOVEI V%d,V%d"
.LC45:
	.string	"DADD V%d,V%d,V%d"
.LC46:
	.string	"DSUB V%d,V%d,V%d"
.LC47:
	.string	"DMUL V%d,V%d,V%d"
.LC48:
	.string	"DABS V%d,V%d"
.LC49:
	.string	"DNEG V%d,V%d"
.LC50:
	.string	"DMIN V%d,V%d,V%d"
.LC51:
	.string	"DMAX V%d,V%d,V%d"
.LC52:
	.string	"VDTOF V%d,V%d,V%d"
.LC53:
	.string	"VFTOD V%d,V%d,V%d"
	.text
	.p2align 4
	.globl	AX_debug_core1
	.type	AX_debug_core1, @function
AX_debug_core1:
.LFB23:
	.cfi_startproc
	movl	172(%rdi), %eax
	movl	%eax, %edx
	movl	%eax, %esi
	movl	%eax, %r9d
	movl	%eax, %ecx
	shrl	$10, %edx
	movl	%eax, %r8d
	movl	%eax, %edi
	shrl	%eax
	shrl	$20, %r9d
	movl	%edx, %r10d
	shrl	$14, %ecx
	andl	$127, %eax
	shrl	$11, %r8d
	shrl	$8, %edi
	andl	$63, %r9d
	andl	$63, %ecx
	shrl	$26, %esi
	andl	$511, %r8d
	andl	$1023, %r10d
	movzwl	%dx, %edx
	andl	$262143, %edi
	cmpl	$121, %eax
	ja	.L1
	leaq	.L4(%rip), %r11
	movslq	(%r11,%rax,4), %rax
	addq	%r11, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L4:
	.long	.L57-.L4
	.long	.L56-.L4
	.long	.L55-.L4
	.long	.L54-.L4
	.long	.L53-.L4
	.long	.L52-.L4
	.long	.L51-.L4
	.long	.L50-.L4
	.long	.L49-.L4
	.long	.L48-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L47-.L4
	.long	.L46-.L4
	.long	.L45-.L4
	.long	.L44-.L4
	.long	.L43-.L4
	.long	.L42-.L4
	.long	.L41-.L4
	.long	.L40-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L39-.L4
	.long	.L38-.L4
	.long	.L37-.L4
	.long	.L36-.L4
	.long	.L35-.L4
	.long	.L34-.L4
	.long	.L33-.L4
	.long	.L32-.L4
	.long	.L31-.L4
	.long	.L30-.L4
	.long	.L29-.L4
	.long	.L28-.L4
	.long	.L27-.L4
	.long	.L26-.L4
	.long	.L25-.L4
	.long	.L24-.L4
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
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L23-.L4
	.long	.L22-.L4
	.long	.L21-.L4
	.long	.L20-.L4
	.long	.L16-.L4
	.long	.L19-.L4
	.long	.L18-.L4
	.long	.L1-.L4
	.long	.L17-.L4
	.long	.L16-.L4
	.long	.L15-.L4
	.long	.L14-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L1-.L4
	.long	.L13-.L4
	.long	.L12-.L4
	.long	.L11-.L4
	.long	.L10-.L4
	.long	.L9-.L4
	.long	.L8-.L4
	.long	.L7-.L4
	.long	.L6-.L4
	.long	.L5-.L4
	.long	.L3-.L4
	.text
	.p2align 4,,10
	.p2align 3
.L1:
	ret
	.p2align 4,,10
	.p2align 3
.L16:
	movq	%r9, %rdx
	leaq	.LC43(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L19:
	movq	%r9, %rdx
	leaq	.LC42(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L14:
	movq	%r9, %rdx
	leaq	.LC39(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L15:
	movq	%r9, %rdx
	leaq	.LC38(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L17:
	movq	%r9, %rdx
	leaq	.LC40(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L18:
	movq	%r9, %rdx
	leaq	.LC41(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L26:
	movq	%r9, %rcx
	movq	%r10, %rdx
	leaq	.LC31(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L27:
	movq	%r9, %rcx
	movq	%r10, %rdx
	leaq	.LC30(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L28:
	movq	%r9, %rdx
	leaq	.LC29(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L29:
	movq	%r9, %rdx
	leaq	.LC28(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L30:
	movq	%r9, %rdx
	leaq	.LC27(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L31:
	movq	%r9, %rdx
	leaq	.LC26(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L32:
	leaq	.LC25(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L33:
	leaq	.LC24(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L34:
	leaq	.LC23(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L35:
	leaq	.LC22(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L20:
	movq	%r9, %rdx
	leaq	.LC37(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L21:
	movq	%r9, %rdx
	leaq	.LC35(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L22:
	movq	%r9, %rdx
	leaq	.LC36(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L23:
	movq	%r9, %rdx
	leaq	.LC34(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L24:
	movq	%r9, %rcx
	movq	%r10, %rdx
	leaq	.LC33(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L25:
	movq	%r9, %rcx
	movq	%r10, %rdx
	leaq	.LC32(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L12:
	movq	%r9, %rdx
	leaq	.LC45(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L13:
	movq	%r9, %rdx
	leaq	.LC44(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L54:
	leaq	.LC3(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L55:
	movq	%r9, %rdx
	leaq	.LC2(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L50:
	movq	%r10, %rcx
	movq	%r9, %rdx
	leaq	.LC7(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L51:
	movq	%r10, %rcx
	movq	%r9, %rdx
	leaq	.LC6(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L52:
	movq	%r9, %rdx
	leaq	.LC5(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L53:
	movq	%r9, %rdx
	leaq	.LC4(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L42:
	movq	%r9, %rdx
	leaq	.LC15(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L43:
	movq	%r9, %rdx
	leaq	.LC14(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L44:
	movq	%r9, %rdx
	leaq	.LC13(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L45:
	movq	%r9, %rdx
	leaq	.LC12(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L46:
	movq	%r9, %rdx
	leaq	.LC11(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L47:
	movq	%r9, %rdx
	leaq	.LC10(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L48:
	movq	%rdi, %rdx
	xorl	%eax, %eax
	leaq	.LC9(%rip), %rdi
	jmp	printf@PLT
.L49:
	movq	%rdi, %rdx
	xorl	%eax, %eax
	leaq	.LC8(%rip), %rdi
	jmp	printf@PLT
.L10:
	movq	%r9, %rdx
	leaq	.LC47(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L11:
	movq	%r9, %rdx
	leaq	.LC46(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L8:
	movq	%r9, %rdx
	leaq	.LC49(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L9:
	movq	%r9, %rdx
	leaq	.LC48(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L3:
	movq	%r9, %rdx
	leaq	.LC52(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L5:
	movq	%r9, %rdx
	leaq	.LC53(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L6:
	movq	%r9, %rdx
	leaq	.LC51(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L7:
	movq	%r9, %rdx
	leaq	.LC50(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L56:
	movq	%r9, %rdx
	leaq	.LC1(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L57:
	leaq	.LC0(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L36:
	leaq	.LC21(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L37:
	leaq	.LC20(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L38:
	leaq	.LC19(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L39:
	leaq	.LC18(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L40:
	movq	%r9, %rdx
	leaq	.LC17(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
.L41:
	movq	%r9, %rdx
	leaq	.LC16(%rip), %rdi
	xorl	%eax, %eax
	jmp	printf@PLT
	.cfi_endproc
.LFE23:
	.size	AX_debug_core1, .-AX_debug_core1
	.section	.rodata.str1.1
.LC54:
	.string	"MUL R%d,R%d,R%d (%d)"
.LC55:
	.string	"DIVS R%d,R%d,R%d (%d)"
.LC56:
	.string	"DIVU R%d,R%d,R%d (%d)"
.LC57:
	.string	"REMS R%d,R%d,R%d (%d)"
.LC58:
	.string	"REMU R%d,R%d,R%d (%d)"
.LC59:
	.string	"CMP R%d,R%d"
.LC60:
	.string	"CMPN R%d,%d"
.LC61:
	.string	"CMPU R%d,%d"
.LC62:
	.string	"CMPFR R%d,%d"
.LC63:
	.string	"BEQ %d"
.LC64:
	.string	"BNE %d"
.LC65:
	.string	"BLE %d"
.LC66:
	.string	"BGE %d"
.LC67:
	.string	"BL %d"
.LC68:
	.string	"BG %d"
.LC69:
	.string	"BLES %d"
.LC70:
	.string	"BGES %d"
.LC71:
	.string	"BLS %d"
.LC72:
	.string	"BGS %d"
.LC73:
	.string	"BRA %d"
.LC74:
	.string	"LOOP %d"
.LC75:
	.string	"JUMPBR %d"
.LC76:
	.string	"CALLBR %d"
.LC77:
	.string	"JUMP %d"
.LC78:
	.string	"CALL %d"
.LC79:
	.string	"SYSCALL"
.LC80:
	.string	"INT"
.LC81:
	.string	"RET"
.LC82:
	.string	"RETI"
.LC83:
	.string	"MOVEINS R%d"
.LC84:
	.string	"MOVECYCLE R%d"
.LC85:
	.string	"MOVERI R%d,R%d"
.LC86:
	.string	"MOVEIR R%d,R%d"
.LC87:
	.string	"FCMP V%d,V%d"
.LC88:
	.string	"DCMP V%d,V%d"
.LC89:
	.string	"FCMPI V%d,%f"
.LC90:
	.string	"DCMPI V%d,%f"
.LC91:
	.string	"FADD V%d,V%d,V%d"
.LC92:
	.string	"FSUB V%d,V%d,V%d"
.LC93:
	.string	"FMUL V%d,V%d,V%d"
.LC94:
	.string	"FMULADD V%d,V%d,V%d"
.LC95:
	.string	"FMULSUB V%d,V%d,V%d"
.LC96:
	.string	"VFADD V%d,V%d,V%d"
.LC97:
	.string	"VFSUB V%d,V%d,V%d"
.LC98:
	.string	"VFMUL V%d,V%d,V%d"
.LC99:
	.string	"VFMULADD V%d,V%d,V%d"
.LC100:
	.string	"VFMULSUB V%d,V%d,V%d"
.LC101:
	.string	"VFADDS V%d,V%d,V%d"
.LC102:
	.string	"VFSUBS V%d,V%d,V%d"
.LC103:
	.string	"VFMULS V%d,V%d,V%d"
.LC104:
	.string	"VFMULADDS V%d,V%d,V%d"
.LC105:
	.string	"VFMULSUBS V%d,V%d,V%d"
.LC106:
	.string	"VFMULSUBS V%d,V%d,%d"
.LC107:
	.string	"FMOVE V%d,V%d"
.LC108:
	.string	"VFMOVE V%d,V%d"
.LC109:
	.string	"FMOVEI V%d,%f"
.LC110:
	.string	"VFMOVEI V%d,%f"
.LC111:
	.string	"FNEG V%d,V%d"
.LC112:
	.string	"FABS V%d,V%d"
.LC113:
	.string	"VFNEG V%d,V%d"
.LC114:
	.string	"VFABS V%d,V%d"
.LC115:
	.string	"VFTOI V%d,V%d,V%d"
.LC116:
	.string	"VITOF V%d,V%d,V%d"
.LC117:
	.string	"FMAX V%d,V%d"
.LC118:
	.string	"FMIN V%d,V%d"
.LC119:
	.string	"WAIT"
.LC120:
	.string	"EXE %a"
.LC121:
	.string	"ENDP"
	.text
	.p2align 4
	.globl	AX_debug_core0
	.type	AX_debug_core0, @function
AX_debug_core0:
.LFB24:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	172(%rdi), %edx
	movl	%edx, %ebx
	movl	%edx, %esi
	movl	%edx, %r10d
	movl	%edx, %r11d
	shrl	$10, %ebx
	movl	%edx, %r8d
	movl	%edx, %r9d
	shrl	%esi
	movzwl	%bx, %r13d
	shrl	$20, %r10d
	movl	%ebx, %r12d
	andl	$127, %esi
	movl	%r13d, %ecx
	shrl	$14, %r11d
	andl	$63, %r10d
	movq	%r13, %rax
	shrl	$11, %r8d
	shrl	$8, %edx
	andl	$63, %r11d
	andl	$1023, %r12d
	shrl	$3, %ecx
	shrl	$26, %r9d
	andl	$511, %r8d
	andl	$262143, %edx
	andl	$1920, %ecx
	andb	$64, %bh
	je	.L60
	sall	$16, %ecx
	orl	$1073741824, %ecx
.L61:
	movl	%eax, %ebx
	sall	$13, %eax
	sall	$16, %ebx
	andl	$8380416, %eax
	andl	$-2147483648, %ebx
	orl	%ebx, %eax
	leal	(%rax,%rcx), %ebx
	leaq	.L64(%rip), %rcx
	movslq	(%rcx,%rsi,4), %rax
	addq	%rcx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L64:
	.long	.L165-.L64
	.long	.L164-.L64
	.long	.L163-.L64
	.long	.L162-.L64
	.long	.L161-.L64
	.long	.L160-.L64
	.long	.L159-.L64
	.long	.L158-.L64
	.long	.L157-.L64
	.long	.L156-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L155-.L64
	.long	.L154-.L64
	.long	.L153-.L64
	.long	.L152-.L64
	.long	.L151-.L64
	.long	.L150-.L64
	.long	.L149-.L64
	.long	.L148-.L64
	.long	.L147-.L64
	.long	.L146-.L64
	.long	.L145-.L64
	.long	.L144-.L64
	.long	.L143-.L64
	.long	.L142-.L64
	.long	.L141-.L64
	.long	.L140-.L64
	.long	.L139-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L138-.L64
	.long	.L137-.L64
	.long	.L136-.L64
	.long	.L135-.L64
	.long	.L134-.L64
	.long	.L133-.L64
	.long	.L132-.L64
	.long	.L131-.L64
	.long	.L130-.L64
	.long	.L129-.L64
	.long	.L128-.L64
	.long	.L127-.L64
	.long	.L126-.L64
	.long	.L125-.L64
	.long	.L124-.L64
	.long	.L123-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L122-.L64
	.long	.L121-.L64
	.long	.L120-.L64
	.long	.L119-.L64
	.long	.L118-.L64
	.long	.L117-.L64
	.long	.L116-.L64
	.long	.L115-.L64
	.long	.L114-.L64
	.long	.L113-.L64
	.long	.L112-.L64
	.long	.L59-.L64
	.long	.L111-.L64
	.long	.L110-.L64
	.long	.L109-.L64
	.long	.L108-.L64
	.long	.L107-.L64
	.long	.L106-.L64
	.long	.L105-.L64
	.long	.L104-.L64
	.long	.L103-.L64
	.long	.L102-.L64
	.long	.L101-.L64
	.long	.L100-.L64
	.long	.L99-.L64
	.long	.L98-.L64
	.long	.L97-.L64
	.long	.L96-.L64
	.long	.L95-.L64
	.long	.L94-.L64
	.long	.L93-.L64
	.long	.L92-.L64
	.long	.L91-.L64
	.long	.L90-.L64
	.long	.L89-.L64
	.long	.L88-.L64
	.long	.L87-.L64
	.long	.L86-.L64
	.long	.L85-.L64
	.long	.L84-.L64
	.long	.L83-.L64
	.long	.L82-.L64
	.long	.L81-.L64
	.long	.L80-.L64
	.long	.L79-.L64
	.long	.L78-.L64
	.long	.L77-.L64
	.long	.L76-.L64
	.long	.L75-.L64
	.long	.L74-.L64
	.long	.L73-.L64
	.long	.L72-.L64
	.long	.L71-.L64
	.long	.L70-.L64
	.long	.L69-.L64
	.long	.L68-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L67-.L64
	.long	.L66-.L64
	.long	.L59-.L64
	.long	.L59-.L64
	.long	.L65-.L64
	.long	.L63-.L64
	.text
	.p2align 4,,10
	.p2align 3
.L60:
	movl	%ecx, %ebx
	sall	$16, %ebx
	orl	$939524096, %ebx
	testl	%ecx, %ecx
	cmovne	%ebx, %ecx
	jmp	.L61
	.p2align 4,,10
	.p2align 3
.L59:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L63:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC117(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L65:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC118(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L66:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC116(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L67:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC115(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L68:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC114(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L69:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC113(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L70:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC112(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L71:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC111(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L72:
	.cfi_restore_state
	movd	%ebx, %xmm4
	pxor	%xmm0, %xmm0
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r9, %rsi
	popq	%r12
	.cfi_def_cfa_offset 16
	movl	$1, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	cvtss2sd	%xmm4, %xmm0
	leaq	.LC110(%rip), %rdi
	jmp	printf@PLT
.L73:
	.cfi_restore_state
	movd	%ebx, %xmm3
	pxor	%xmm0, %xmm0
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r9, %rsi
	popq	%r12
	.cfi_def_cfa_offset 16
	movl	$1, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	cvtss2sd	%xmm3, %xmm0
	leaq	.LC109(%rip), %rdi
	jmp	printf@PLT
.L74:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC108(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L75:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC107(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L76:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r12, %rcx
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC106(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L77:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC105(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L78:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC100(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L79:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC95(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L80:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC104(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L81:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC103(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L82:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC102(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L83:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC101(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L84:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC99(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L85:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC98(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L86:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC97(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L87:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC96(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L88:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC94(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L89:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC93(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L90:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC92(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L91:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC91(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L92:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC76(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L93:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC78(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L94:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC75(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L95:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC77(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L96:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC74(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L97:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC73(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L98:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC70(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L99:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC72(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L100:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC69(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L101:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC71(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L102:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC66(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L103:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC68(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L104:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC65(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L105:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC67(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L106:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC63(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L107:
	.cfi_restore_state
	movl	160(%rdi), %esi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC64(%rip), %rdi
	xorl	%eax, %eax
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L108:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC80(%rip), %rdi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L109:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC79(%rip), %rdi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L110:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC82(%rip), %rdi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L111:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC81(%rip), %rdi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L112:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC119(%rip), %rdi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L113:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r9, %rsi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	leaq	.LC120(%rip), %rdi
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L114:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	leaq	.LC121(%rip), %rdi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L115:
	.cfi_restore_state
	movd	%ebx, %xmm2
	pxor	%xmm0, %xmm0
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r9, %rsi
	popq	%r12
	.cfi_def_cfa_offset 16
	movl	$1, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	cvtss2sd	%xmm2, %xmm0
	leaq	.LC90(%rip), %rdi
	jmp	printf@PLT
.L116:
	.cfi_restore_state
	movd	%ebx, %xmm1
	pxor	%xmm0, %xmm0
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r9, %rsi
	popq	%r12
	.cfi_def_cfa_offset 16
	movl	$1, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	cvtss2sd	%xmm1, %xmm0
	leaq	.LC89(%rip), %rdi
	jmp	printf@PLT
.L117:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC88(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L118:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC87(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L119:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r9, %rsi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	leaq	.LC61(%rip), %rdi
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L120:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r9, %rsi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	leaq	.LC60(%rip), %rdi
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L121:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r13, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC62(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L122:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC59(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L123:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r12, %rdx
	movq	%r10, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC33(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L124:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r12, %rdx
	movq	%r10, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC32(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L125:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r12, %rdx
	movq	%r10, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC31(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L126:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r12, %rdx
	movq	%r10, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC30(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L127:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC29(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L128:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC28(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L129:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC27(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L130:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC26(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L131:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r13, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC25(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L132:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r13, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC24(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L133:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r13, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC23(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L134:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r13, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC22(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L135:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r13, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC21(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L136:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r13, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC20(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L137:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r13, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC19(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L138:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r13, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC18(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L139:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC54(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L140:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC58(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L141:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC57(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L142:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC56(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L143:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC55(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L144:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC17(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L145:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC16(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L146:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC15(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L147:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC14(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L148:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC13(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L149:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC12(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L150:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC11(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L151:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC10(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L152:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC86(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L153:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC85(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L154:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r9, %rsi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	leaq	.LC84(%rip), %rdi
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L155:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r9, %rsi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	leaq	.LC83(%rip), %rdi
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L156:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r9, %rsi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	leaq	.LC9(%rip), %rdi
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L157:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r9, %rsi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	leaq	.LC8(%rip), %rdi
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L158:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r12, %rcx
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC7(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L159:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r12, %rcx
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC6(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L160:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC5(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L161:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r11, %rcx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r10, %rdx
	movq	%r9, %rsi
	leaq	.LC4(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L162:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r13, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC3(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L163:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC2(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L164:
	.cfi_restore_state
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%r10, %rdx
	popq	%r12
	.cfi_def_cfa_offset 16
	movq	%r9, %rsi
	leaq	.LC1(%rip), %rdi
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.L165:
	.cfi_restore_state
	popq	%rbx
	.cfi_def_cfa_offset 24
	leaq	.LC0(%rip), %rdi
	popq	%r12
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
	.cfi_endproc
.LFE24:
	.size	AX_debug_core0, .-AX_debug_core0
	.section	.rodata.str1.1
.LC122:
	.string	"PC %d: "
.LC123:
	.string	"    "
	.text
	.p2align 4
	.globl	AX_debug
	.type	AX_debug, @function
AX_debug:
.LFB25:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	144(%rdi), %eax
	movq	%rdi, %rbp
	leaq	.LC122(%rip), %rdi
	leal	-1(%rax), %esi
	xorl	%eax, %eax
	call	printf@PLT
	movq	%rbp, %rdi
	call	AX_debug_core0
	xorl	%eax, %eax
	leaq	.LC123(%rip), %rdi
	call	printf@PLT
	testb	$1, 172(%rbp)
	jne	.L175
	movl	$10, %edi
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	jmp	putchar@PLT
	.p2align 4,,10
	.p2align 3
.L175:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	AX_debug_core1
	movl	$10, %edi
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	putchar@PLT
	.cfi_endproc
.LFE25:
	.size	AX_debug, .-AX_debug
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
