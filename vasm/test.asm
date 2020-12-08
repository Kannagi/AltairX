
	
	movei r0,4
	movei r1,1500


	movei r2,140
	movei r3,2700


	add r4,r0,r1
	add r5,r2,r3


	cmpi r4,0
	nop


	bne label
	nop

	add r4,r0,r1
	add r5,r2,r3

	label:


	;----------------------------
	addi r4,r0,4
	addq r5,10
	
	mulu r6,r0,r4
	muluq r5,10

	
	
	;---------------------------
	nop.e
	nop
	
	nop
	nop
