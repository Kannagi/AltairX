


	movei r1,$0FFFF
	movei r2,42

	add r1,r1,r2
	nop

	addi r1,r1,5
	nop

	addq r1,5
	nop

	sub r1,r1,r2
	nop

	subi r1,r1,5
	nop

	subq r1,5
	nop

	cmp r1,r1   ;if
	nop


	nop         ;id
	movei r3,42

	nop         ;rr
	nop

	nop         ;exe
	nop

	bne test ;wb /if
	nop

	nop
	nop

	nop         ;-- /id-exe
	nop
test:
	movei r3,4200
	nop


	nend
	nop





	









