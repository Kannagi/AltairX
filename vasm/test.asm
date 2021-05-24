
	movei r0,-1
	movei r1,$20
	
	add r5,r4,r20 
	addi r1,r3,56

	xor r5,r4,r20 
	xori r1,r3,56

	move r5,r5
	ori r5,r5,0

	nop
	nop

	move LR,r0
	move BR,r1

	move r1,LR
	mulu P,r3,r1


	ldmi r0,50[r3]
	ldm r1,r3[r5]


	ldml r3,500
	stml r1,500

	fcmpi vf01,-1.5
	move r5,r6

	org $400
	dc.b "Hello World",0
	
	org $480
	