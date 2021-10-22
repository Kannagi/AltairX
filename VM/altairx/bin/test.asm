
	include "macro.asm"
	org $0
	movei r0,$00

	
	;--------------
	movei r4,$00
	syscall
	nop
	;--------------
	movei r9,'a'
	stml r9,0
	stml r0,1


	moveiu r9,$400
	nop.p
	lddmai r0,r9,1
	
	movei r4,$01
	movei r5,0 ;ADDR SPM
	syscall
	nop
	;--------------
	movei r4,$0E
	;syscall
	nop



	;--------------
	endp
	nop
	;--------------





	fmovei v3,-1.11
	fneg v3,v3
	
	fmovei v4,1.0
	
	fadd v3,v3,v4
	fmul v3,v3,v3
	fmul v3,v3,v3

	smove.b r1,0
	smove.w r1,$8000

	moveiu rQ,8
	move r2,rQ



	stcl r2,0
	ldcl r3,0

	syscall
	nop
	
	nop.p
	moveiu r3,0
	
	cmpi r3,0
	if_ne
	nop
	
		addi r3,r3,5

	endi

	moveiu RL,0x3FFFF
	

	
		moveiu r59,0x3FFFF
		cmpi r3,0

		if_ne 
		nop
		
			addi r3,r3,5

		endi
		ldcl r3,0
		addi r4,r5,60
	
	addi r4,r5,5
	
	
	call lfunc2
	addi r3,r3,5
	
	
	syscall
	nop
	
	endp
	nop
	
	
	

lfunc:
	addi r3,r3,17
	
	ret
	nop


lfunc2:
	addi r3,r3,172
	
	ret
	nop
	
	org $400
	dc.b "Hello World",$A,0
	
	org $480
	
