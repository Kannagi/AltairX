send	macro		
	movei.b r1,\1
	movei.b r2,\2
	
	stm.b r1,$0[r0]
	movei.b r3,\3
	
	stm.b r2,$1[r0]
	nop
	
	stm.b r3,$2[r0]
	addq r56,3
endm

	lddmar r1,r3,$FF
	nop

	send 'H','e','l'
	send 'l','o',' '
	send 'W','o','r'
	send 'l','d',0

	movei.q r56,0
	nop
	

	;--------------------

Loop:
	ldm.b r1,$0[r0]
	nop

	cmpi r1,0
	nop

	beq Label
	addq r56,1
	
	outi.b 1,1 ;fputc = 1
	nop
	
	out.b 32,r1 ; data = r0
	nop
	
	outi.b 0,1 ;control = 1
	nop
	
	bra Loop
	nop
	
	nop
	nop
Label:	
	
	;-------------------------------
	outi.b 32,$A
	nop
	
	outi.b 0,1
	nop


	endp
	nop
	
	nop
	nop


