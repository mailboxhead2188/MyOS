	bits 32

	section .text
	;; multiboot spec
	align 4
	dd 0x1BADB002 		; Magic number
	dd 0x00
	dd - (0X1BADB002 + 0x00)

	global start
	global keyboard_handler
	global read_port
	global write_port
	global load_idt
	global gdt_flush
	global enableA20
	global tss_flush
	
	global isr0
	global isr1
	global isr2
	global isr3
	global isr4
	global isr5
	global isr6
	global isr7
	global isr8
	global isr9
	global isr10
	global isr11
	global isr12
	global isr13
	global isr14
	global isr15
	global isr16
	global isr17
	global isr18
	global isr19
	global isr20
	global isr21
	global isr22
	global isr23
	global isr24
	global isr25
	global isr26
	global isr27
	global isr28
	global isr29
	global isr30
	global isr31

	global irq0
	global irq1
	global irq2
	global irq3
	global irq4
	global irq5
	global irq6
	global irq7
	global irq8
	global irq9
	global irq10
	global irq11
	global irq12
	global irq13
	global irq14
	global irq15

        global EnterProtectedMode
	
	extern kmain 		; c main
	extern keyboard_handler_main ; hands control of the keyboard interrupt back to c
	extern gp
	extern fault_handler
	extern irq_handler

	
isr0:
	cli
	push byte 0
	push byte 0
	jmp isr_common_stub

isr1:
	cli
	push byte 0
	push byte 1
	jmp isr_common_stub

isr2:
	cli
	push byte 0
	push byte 2
	jmp isr_common_stub

isr3:
	cli
	push byte 0
	push byte 3
	jmp isr_common_stub

isr4:
	cli
	push byte 0
	push byte 4
	jmp isr_common_stub

isr5:
	cli
	push byte 0
	push byte 5
	jmp isr_common_stub

isr6:
	cli
	push byte 0
	push byte 6
	jmp isr_common_stub

isr7:
	cli
	push byte 0
	push byte 7
	jmp isr_common_stub

isr8:
	cli
	push byte 8
	jmp isr_common_stub

isr9:
	cli
	push byte 0
	push byte 9
	jmp isr_common_stub

isr10:
	cli
	push byte 0
	push byte 10
	jmp isr_common_stub

isr11:
	cli
	push byte 0
	push byte 11
	jmp isr_common_stub

isr12:
	cli
	push byte 0
	push byte 12
	jmp isr_common_stub

isr13:
	cli
	push byte 0
	push byte 13
	jmp isr_common_stub

isr14:
	cli
	push byte 0
	push byte 14
	jmp isr_common_stub

isr15:
	cli
	push byte 0
	push byte 15
	jmp isr_common_stub

isr16:
	cli
	push byte 0
	push byte 16
	jmp isr_common_stub

isr17:
	cli
	push byte 0
	push byte 17
	jmp isr_common_stub

isr18:
	cli
	push byte 0
	push byte 18
	jmp isr_common_stub

isr19:
	cli
	push byte 0
	push byte 19
	jmp isr_common_stub

isr20:
	cli
	push byte 0
	push byte 20
	jmp isr_common_stub

isr21:
	cli
	push byte 0
	push byte 21
	jmp isr_common_stub

isr22:
	cli
	push byte 0
	push byte 22
	jmp isr_common_stub

isr23:
	cli
	push byte 0
	push byte 23
	jmp isr_common_stub

isr24:
	cli
	push byte 0
	push byte 24
	jmp isr_common_stub

isr25:
	cli
	push byte 0
	push byte 25
	jmp isr_common_stub

isr26:
	cli
	push byte 0
	push byte 26
	jmp isr_common_stub

isr27:
	cli
	push byte 0
	push byte 27
	jmp isr_common_stub

isr28:
	cli
	push byte 0
	push byte 28
	jmp isr_common_stub

isr29:
	cli
	push byte 0
	push byte 29
	jmp isr_common_stub

isr30:
	cli
	push byte 0
	push byte 30
	jmp isr_common_stub

isr31:
	cli
	push byte 0
	push byte 31
	jmp isr_common_stub

irq0:
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub

irq1:
	cli
	push byte 0
	push byte 33
	jmp irq_common_stub

irq2:
	cli
	push byte 0
	push byte 34
	jmp irq_common_stub

irq3:
	cli
	push byte 0
	push byte 35
	jmp irq_common_stub

irq4:
	cli
	push byte 0
	push byte 36
	jmp irq_common_stub

irq5:
	cli
	push byte 0
	push byte 37
	jmp irq_common_stub

irq6:
	cli
	push byte 0
	push byte 38
	jmp irq_common_stub

irq7:
	cli
	push byte 0
	push byte 39
	jmp irq_common_stub

irq8:
	cli
	push byte 0
	push byte 40
	jmp irq_common_stub

irq9:
	cli
	push byte 0
	push byte 41
	jmp irq_common_stub

irq10:
	cli
	push byte 0
	push byte 42
	jmp irq_common_stub

irq11:
	cli
	push byte 0
	push byte 43
	jmp irq_common_stub

irq12:
	cli
	push byte 0
	push byte 44
	jmp irq_common_stub

irq13:
	cli
	push byte 0
	push byte 45
	jmp irq_common_stub

irq14:
	cli
	push byte 0
	push byte 46
	jmp irq_common_stub

irq15:
	cli
	push byte 0
	push byte 47
	jmp irq_common_stub

irq_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, irq_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret
	
isr_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, fault_handler
	call eax
	pop eax
	pop gs
	pop gs
	pop es
	pop ds
	popa
	add esp, 8
	iret
	
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

gdt_flush:
	lgdt [gp]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2

flush2:
	ret

tss_flush:
	mov ax, 0x2B
	ltr ax
	ret

enableA20:
	pusha

	cli

	mov cx, 5
.startAttempt1:
.commandWait1:
	xor ax, ax
	in al, 64h
	bt ax, 1
	jc .commandWait1

	mov al, 0d0h
	out 64h, al

.dataWait1:
	xor ax, ax
	in al, 64h
	bt ax, 0
	jnc .dataWait1

	xor ax, ax
	in al, 60h

	push eax

.commandWait2:
	in al, 64h
	bt ax, 1
	jc .commandWait2

	mov al, 0d1h
	out 64h, al

.commandWait3:
	xor ax, ax
	in al, 64h
	bt ax, 1
	jc .commandWait3

	pop eax

	or al, 00000010b
	out 60h, al

.commandWait4:
	xor ax, ax
	in al, 64h
	bt ax, 1
	jc .commandWait4

	mov al, 0d0h
	out 64h, al

.dataWait2:
	xor ax, ax
	in al, 64h
	bt ax, 0
	jnc .dataWait2

	xor ax, ax
	in al, 60h

	bt ax, 1

	jc .success

	loop .startAttempt1

	mov cx, 5

.startAttempt2:
.commandWait6:
	xor ax, ax
	in al, 64h
	bt ax, 1
	jc .commandWait6

	mov al, 0dfh
	out 64h, al

.commandWait7:
	xor ax, ax
	in al, 64h
	bt ax, 1
	jc .commandWait7

	mov al, 0d0h
	out 64h, al

.dataWait3:
	xor ax, ax
	in al, 64h
	bt ax, 0
	jnc .dataWait3

	xor ax, ax
	in al, 60h

	bt ax, 1

	jc .warn

	loop .startAttempt2

	jmp .fail

.warn:

.success:
	
	
	sti
	popa
	xor eax, eax
	
	ret

.fail:
	sti
	popa
	mov eax, -1
	ret

EnterProtectedMode:
	cli
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp 08h:PMode

PMode:
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov esp, 9000h
	iret
	
start:
	cli
	mov esp, stack_space
	push esp
	push eax 		; Header magic
	push ebx 		; Header pointer
	call kmain
	hlt

	section .bss
	resb 8192
	
stack_space:	
