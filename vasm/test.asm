

send	macro		
	movei.b r1,\1
	movei.b r2,\2
	
	stm.b r1,$0[r0]
	movei.b r3,\3
	
	stm.b r2,$1[r0]
	nop
	
	stm.b r3,$2[r0]
	addq r0,3
endm

	
	send 'H','e','l'
	send 'l','o',' '
	send 'W','o','r'
	send 'l','d',0
	
	
	movei.q r0,0
	movei.q r1,$10
	
	

	
	;--------------------
	
	call print
	nop
	
	nop
	nop


	endp
	nop
	
	nop
	nop
	
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
	
	nop ;outi.b 0,1 ;control = 1
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



