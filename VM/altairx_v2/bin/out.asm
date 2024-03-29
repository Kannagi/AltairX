.code  $80000000
.org 0
..begin main
main:
;asm
 
	nop
	reti
	nop
	reti
	
	
	add t0,t1,t2 

	boucle:
	sub t0,t1,47
	or t0,t1,t2
	xor t0,t1,t2


	movei a0,3
	syscall


	jump boucle
	

ret
..end
