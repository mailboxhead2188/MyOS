	;; entry.asm
	bits 32
	section .text
	;;  multiboot spec
	align 4
	dd 0x1BADB002 		; magic number
	dd 0x00 		; Flags
	dd - (0x1BADB002 + 0x00) ; Checksum

	
	global start
	extern kmain 		; C-defined main function

start:
	cli 			; Clears interrupts so we can continue

	mov esp, stack_space 	; Sets the stack pointer
	call kmain
	hlt 			; Halts

	section .bss
	resb 8192 		; 8KB for the stack
stack_space:	
