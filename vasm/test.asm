

	include "macro.asm"

	
	moveu r3,2
	moveu r4,5
	smove.w r4,$80
	;sti r3,0[r4]

	endp

	nop
	move r8,r9

	do
		moveu r59,5
		cmpu r3,0

		if_ne 
		nop
		    addi r3,r3,0xFF
		endi
		ldl r3,0
		addi r4,r5,0
	while _wh
	addi r4,r5,5
		
	move r8,r9
	moven r1,-5

	
	moveu r6,1

	loop:
	addi r4,r6,5

	mul P,r4,r3
	divs Q,r6,r6
	divu Q,r6,r6


	moveu r4,$01

	syscall
	nop

	endp
	nop