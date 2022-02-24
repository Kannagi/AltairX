 
if_eq	macro		
	bne L1\@!
endm

if_ne	macro		
	beq L1\@!
endm

if_le	macro		
	ble L1\@!
endm

	
endi	macro		
	L1\@@:
endm



do	macro		
	L1\@!:
endm

while	macro		
	bra	L1\@@
endm

while_eq	macro		
	bne L1\@@
endm

while_ne	macro		
	beq L1\@@
endm


L equ 59
B equ 60
ACC equ 61
P equ 62
Q equ 62
