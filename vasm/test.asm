
	include "macro.asm"
	org $0
	
	movei r0,$00
	movei r1,$400
	
	
	movei r8,$00
	lddmai r0,r1,1
	
	ldml r1,$0
	nop
	
	
	
	call lfunc
	nop

	int r1,0
	nop
	
	
	movei r0,$00
	nop
	
	cmpi r0,0
	nop
	
	beq test
	nop
	
	nop
	nop
	
	int r1,0
	nop
	
	
test:
	

	endp
	nop
	
	nop
	nop
	
lfunc:
	nop
	nop

	ret
	nop
	
	nop
	nop

	org $400
	dc.b "Hello World",$A,0
	
	org $480
	
