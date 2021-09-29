;syscall
;0x00
;kernel console ,read/write file

;0x01
;GIF

;0x02
;SIF

;0x03
;input (clavier/souris/joypad)

;0x04
;Net

;0x05
;GUI


	include "macro.asm"
	org $0
	
	nop.p
	fmovei v0,1.5

	fmovei v1,1.0

	move FR,r5

	nop
	addi r63,r63,5

	fadd.p v0,v0,v1
	nop
	
	
	syscall 0x00
	nop
	
lab2:
	movei r0,$00
	movei r1,$400
	
	movei r8,$00
	lddmai r0,r1,1
	
	ldml r1,$0	
	syscall 0x00
	
	
	call lfunc
	nop
	
	
	movei r0,$00	
	cmpi r0,0
	beq test
	nop
	
	syscall 0x00
	
	
test:
	
	endp
	nop
	
lfunc:
	nop

	ret
	nop


	org $400
	dc.b "Hello World",$A,0
	
	org $480
	
