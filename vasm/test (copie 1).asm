
	;LSU
	ldm.b r01,$FFF[r0+]
 	stm.q r31,$FFF[r2+] 

 	ldc.b r01,$FFF[r0+]
 	stc.q r31,$FFF[r2+]

	ldmx.b r0,$3FFF[r63]
	stmx.w r3,$0FFF[r62]

	in.b 1,r7
	out.w 0,r2
	outi.w 4,$F0

	nop
	xchg

	;ALU
	movei r63,4

	add.b r7,r1,r3    ;8 bits
	sub.w r7,r1,r3    ;16 bits
	muls.l r7,r1,r3   ;32 bits
	mulu.q r7,r1,r3   ;64 bits

	divs r7,r1,r3
	divu r7,r1,r3

	and r7,r1,r3
	or r7,r1,r3
	xor r7,r1,r3


	addi.b r7,r1,3
	subi.w r7,r1,3
	mulsi.l r7,r1,3
	mului.q r7,r1,3

	divsi r7,r1,3
	divui r7,r1,3

	andi r7,r1,3
	ori r7,r1,3
	xori r7,r1,3

 
	asl r7,r1,r3
	lsl r7,r1,r3
	asr r7,r1,r3
	lsr r7,r1,r3

	asli r7,r1,3
	lsli r7,r1,3
	asri r7,r1,3
	lsri r7,r1,3

	aslq r7,3
	lslq r7,3
	asrq r7,3
	lsrq r7,3

	addq.b r7,3
	subq.w r7,3
	mulsq.l r7,3
	muluq.q r7,3

	divsq r7,3
	divuq r7,3

	andq r7,3
	orq r7,3
	xorq r7,$FFFF

	addi.b r7,r1,0
	move.b r7,r1

	;BRU

	cmp r1,r3
	cmpi.w r1,$FFFF

	bne label
	beq label

label:

	call label
	jmp label
	callr label
	jmpr label

	ret