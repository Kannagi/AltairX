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
	cmpi r8,1
