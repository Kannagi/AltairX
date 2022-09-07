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
	include "vector.asm"
	
	jmp infiniloop
	addi r60,r60,5
	add r60,r62,r60
	add r60,r62,r60

	fmovei v0,1.5
	fadd v0,v0,v1

	mulu P,r2,r0
	move r63,r62
	;endp
	nop

	smove.w r0,$8000
	smove.b r0,$0400

	;jmp stall
	movei r60,10000


loopt0:
	nop
	nop

	ldl2.p r5,0
	addi r1,r1,1

	loop.p loopt0
	addi r5,r5,5

	stl.p r5,0
	addi r0,r0,1

	endp
	nop

second:
	movei r60,2500

loopt:
	nop
	nop

	;

	

	ldl.p r4,0
	ldl r5,1

	ldl.p r6,2
	ldl r7,3 

	stl.p r8,0
	stl r9,1

	stl.p r10,2
	stl r11,3

	addi.p r8,r4,5 
	addi r0,r0,4 

	loop.p loopt
	addi r9,r5,5
	
	
	addi.p r10,r6,5 
	addi r11,r7,5  
	


	endp
	nop


stall:
	addi r60,r60,5
	add r60,r62,r60
	add r60,r62,r60

	fmovei v0,1.5
	fadd v0,v0,v1

	mulu P,r2,r0
	move r63,r62
	endp
	nop


	fmovei v0,1.5


	fmovei v1,1.0


	addi r63,r63,5



	fadd.p v0,v0,v1
	nop
	
	syscall 0x00
	nop
	
lab2:
	smove.w r5,$8000
	smove.b r5,$0400



	movei r8,1


	ldi r6,0[r5]
	movei r7,'0'

	;addi r6,r6,1
	add r6,r7,r8
	sti r6,0[r5]
	
	movei r4,$01

	syscall 0x00
	nop
	
	call lfunc
	nop
	
	
	movei r10,$1	
	cmpi r10,0
	beq test
	nop
	movei r4,$00
	syscall 0x00
	nop
	
test:
	
	endp
	nop

	
lfunc:
	nop

	ret

infiniloop:
	nop


		do
        moveiu r59,0x3FFFF
        cmpi r3,0

        if_ne 
        nop
        
            addi r3,r3,5

        endi
        ldl r3,0
        addi r4,r5,60
    while _wh
    addi r4,r5,5


	org $800
testj:
	nop
	endp
	dc.b "Hello World",$A,0
	
	
