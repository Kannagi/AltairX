	.file	"main.c"
	.option pic
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC0:
	.string	"-ncore"
	.align	3
.LC1:
	.string	"-wram"
	.align	3
.LC2:
	.string	"-vram"
	.align	3
.LC3:
	.string	"-spmram"
	.align	3
.LC4:
	.string	"-spmt"
	.align	3
.LC5:
	.string	"-spm2"
	.align	3
.LC6:
	.string	"-mode"
	.align	3
.LC7:
	.string	"prog.bin"
	.align	3
.LC8:
	.string	"%d\n"
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-1248
	sd	s3,1208(sp)
	addi	s3,sp,528
	sd	a0,0(sp)
	mv	a0,s3
	sd	ra,1240(sp)
	sd	s0,1232(sp)
	sd	s1,1224(sp)
	sd	s4,1200(sp)
	sd	s5,1192(sp)
	sd	s6,1184(sp)
	sd	s7,1176(sp)
	sd	s8,1168(sp)
	sd	s9,1160(sp)
	sd	s10,1152(sp)
	sd	s11,1144(sp)
	sd	a1,8(sp)
	sd	s2,1216(sp)
	sb	zero,24(sp)
	call	AX_init_proc@plt
	li	a5,512
	li	a4,8
	li	a3,1
	li	a2,1
	li	a1,16
	mv	a0,s3
	li	s1,0
	call	AX_init_mem@plt
	li	s4,1
	li	s0,0
	li	s6,45
	li	s7,5
	lla	s5,.L14
	addi	s8,sp,24
	lla	s9,.LC0
	lla	s10,.LC1
	lla	s11,.LC2
.L2:
	ld	a4,0(sp)
	sext.w	a5,s4
	bgt	a4,a5,.L20
	mv	a0,s3
	call	AX_init_proc_mem@plt
	addi	a1,sp,1040
	lla	a0,.LC7
	call	AX_load_prog@plt
	mv	a0,s3
	call	AX_add_core@plt
	lw	a2,1096(sp)
	ld	a1,1040(sp)
	ld	a0,528(sp)
	call	AX_boot_rom@plt
	lw	a5,1128(sp)
	li	a4,2
	beq	a5,a4,.L21
	bgt	a5,a4,.L22
	li	a4,1
	bleu	a5,a4,.L27
.L23:
	li	a5,1
	beq	s1,a5,.L26
	mv	a1,s1
	lla	a0,.LC8
	call	printf@plt
.L26:
	ld	ra,1240(sp)
	ld	s0,1232(sp)
	ld	s1,1224(sp)
	ld	s2,1216(sp)
	ld	s3,1208(sp)
	ld	s4,1200(sp)
	ld	s5,1192(sp)
	ld	s6,1184(sp)
	ld	s7,1176(sp)
	ld	s8,1168(sp)
	ld	s9,1160(sp)
	ld	s10,1152(sp)
	ld	s11,1144(sp)
	li	a0,0
	addi	sp,sp,1248
	jr	ra
.L20:
	ld	a4,8(sp)
	slli	a5,s4,3
	add	a5,a4,a5
	ld	s2,0(a5)
	lbu	a5,0(s2)
	bne	a5,s6,.L3
	mv	a1,s9
	mv	a0,s2
	call	strcmp@plt
	bne	a0,zero,.L4
	li	s0,1
.L4:
	mv	a1,s10
	mv	a0,s2
	call	strcmp@plt
	bne	a0,zero,.L5
	li	s0,2
.L5:
	mv	a1,s11
	mv	a0,s2
	call	strcmp@plt
	bne	a0,zero,.L6
	li	s0,3
.L6:
	lla	a1,.LC3
	mv	a0,s2
	call	strcmp@plt
	bne	a0,zero,.L7
	li	s0,4
.L7:
	lla	a1,.LC4
	mv	a0,s2
	call	strcmp@plt
	bne	a0,zero,.L8
	li	s0,5
.L8:
	lla	a1,.LC5
	mv	a0,s2
	call	strcmp@plt
	bne	a0,zero,.L9
	li	s0,6
.L9:
	lla	a1,.LC6
	mv	a0,s2
	call	strcmp@plt
	bne	a0,zero,.L10
	li	s0,7
.L10:
	addi	s4,s4,1
	j	.L2
.L3:
	bne	s0,zero,.L11
	mv	a1,s2
	mv	a0,s8
	call	strcpy@plt
	j	.L10
.L11:
	mv	a0,s2
	call	atoi@plt
	addiw	s0,s0,-2
	sext.w	a5,s0
	bgtu	a5,s7,.L12
	slli	s0,s0,32
	srli	s0,s0,30
	add	s0,s0,s5
	lw	a5,0(s0)
	add	a5,a5,s5
	jr	a5
	.section	.rodata
	.align	2
	.align	2
.L14:
	.word	.L19-.L14
	.word	.L18-.L14
	.word	.L17-.L14
	.word	.L16-.L14
	.word	.L15-.L14
	.word	.L13-.L14
	.section	.text.startup
.L12:
	sw	a0,1120(sp)
.L29:
	li	s0,0
	j	.L10
.L19:
	sw	a0,1116(sp)
	j	.L29
.L18:
	sw	a0,1112(sp)
	j	.L29
.L17:
	sw	a0,1108(sp)
	j	.L29
.L16:
	sw	a0,1100(sp)
	j	.L29
.L15:
	sw	a0,1104(sp)
	j	.L29
.L13:
	sw	a0,1128(sp)
	j	.L29
.L22:
	li	a4,3
	bne	a5,a4,.L23
	ld	a0,528(sp)
	call	AX_exe_core_mode3@plt
	j	.L30
.L27:
	ld	a0,528(sp)
	call	AX_exe_core_mode1@plt
.L30:
	mv	s1,a0
	j	.L23
.L21:
	ld	a0,528(sp)
	call	AX_exe_core_mode2@plt
	j	.L30
	.size	main, .-main
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
