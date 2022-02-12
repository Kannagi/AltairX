
	movei.q r0,$00
	movei.q r1,$20
	
	nop
	lddmar r0,r1,1

	movei.q r0,0
	movei.q r1,$20
	;--------------------
	
	call print
	nop
	
	nop
	nop


	endp
	nop
	
	nop
	nop
	
	org $100
print:

	Loop:

	ldm.b r1,$0[r0]
	nop
	
	cmpi r1,0
	nop
	
	beq Label
	addq r0,1
	
	outi.b 1,1 ;fputc = 1
	nop
	
	out.b 32,r1 ; data = r0
	nop
	
	bra Loop
	nop
	
	outi.b 0,1 ;control = 1
	nop

Label:
	;-------------------------------
	outi.b 32,$A
	nop
	
	ret
	nop
	
	outi.b 0,1
	nop


	org $400
	dc.b "Hello World",0
	
	org $480
	