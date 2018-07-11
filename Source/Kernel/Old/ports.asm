extern keyboard_handler_main
	global read_port
	global write_port
	global load_idt
	global keyboard_handler
	
read_port:
	mov edx, [esp + 4]
	in al, dx
	ret

write_port:
	mov edx, [esp + 4]
	mov al, [esp + 4 + 4]
	out dx, al
	ret

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret

keyboard_handler:
	call keyboard_handler_main
	iretd
