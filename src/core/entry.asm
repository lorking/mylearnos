[bits 32]
global loader
global gdt_flush
global idt_flush
global is0
global is1
global is2
global is3
global is4
global is5
global is6
global is7
global is8
global is9
global is10
global is11
global is12
global is13
global is14
global is15
global is16
global is17
global is18
global is19_31

global	ir32
global	ir33
global	ir34
global	ir35
global	ir36
global	ir37
global	ir38
global	ir39
global	ir40
global	ir41
global	ir42
global	ir43
global	ir44
global	ir45
global	ir46	
extern gp
extern idtp
extern interrupt_handler;中断处理函数
[extern kmain]
loader:
	jmp kmain
gdt_flush:;刷新gdt的操作
	lgdt [gp]
	mov	ax,0x10
	mov	ds,ax
	mov	es,ax
	mov	fs,ax
	mov	gs,ax
	mov	ss,ax
	jmp	0x08:flush2
flush2:
	ret;跳回c函数	
idt_flush:;刷新idt的操作
	lidt [idtp]
	ret
ir32:
	push	byte 0
	push	byte 32
	jmp	isq_common_stub
ir33:
	push	byte 0
	push	byte 33
	jmp	isq_common_stub
ir34:
	push	byte 0
	push	byte 34
	jmp	isq_common_stub
ir35:
	push	byte 0
	push	byte 35
	jmp	isq_common_stub
ir36:
	push	byte 0
	push	byte 36
	jmp	isq_common_stub
ir37:
	push	byte 0
	push	byte 37
	jmp	isq_common_stub
ir38:
	push	byte 0
	push	byte 38
	jmp	isq_common_stub
ir39:
	push	byte 0
	push	byte 39
	jmp	isq_common_stub
ir40:
	push	byte 0
	push	byte 40
	jmp	isq_common_stub
ir41:
	push	byte 0
	push	byte 41
	jmp	isq_common_stub
ir42:
	push	byte 0
	push	byte 42
	jmp	isq_common_stub
ir43:
	push	byte 0
	push	byte 43
	jmp	isq_common_stub
ir44:
	push	byte 0
	push	byte 44
	jmp	isq_common_stub
ir45:
	push	byte 0
	push	byte 45
	jmp	isq_common_stub
ir46:
	push	byte 0
	push	byte 46
	jmp	isq_common_stub
is0:
	push	byte 0
	push	byte 0
	jmp	isq_common_stub
is1:
	push	byte 0
	push	byte 1
	jmp	isq_common_stub
is2:
	push	byte 0
	push	byte 2
	jmp	isq_common_stub
is3:
	push	byte 0
	push	byte 3
	jmp	isq_common_stub
is4:
	push	byte 0
	push	byte 4
	jmp	isq_common_stub
is5:
	push	byte 0
	push	byte 5
	jmp	isq_common_stub
is6:
	push	byte 0
	push	byte 6
	jmp	isq_common_stub
is7:
	push	byte 0
	push	byte 7
	jmp	isq_common_stub
is8:
	push	byte 8
	jmp	isq_common_stub
is9:
	push	byte 0
	push	byte 9
	jmp	isq_common_stub
is10:
	push	byte 10
	jmp	isq_common_stub
is11:
	push	byte 11
	jmp	isq_common_stub
is12:
	push	byte 12
	jmp	isq_common_stub
is13:
	push	byte 13
	jmp	isq_common_stub
is14:
	push	byte 14
	jmp	isq_common_stub
is15:
	push	byte 0
	push	byte 15
	jmp	isq_common_stub
is16:
	push	byte 0
	push	byte 16
	jmp	isq_common_stub
is17:
	push	byte 0
	push	byte 17
	jmp	isq_common_stub
is18:
	push	byte 0
	push	byte 18
	jmp	isq_common_stub
is19_31:
	push	byte 0
	push	byte 19
	jmp	isq_common_stub
isq_common_stub:;所有错误函数的统一入口,用来保存各个段寄存器的值
	pusha
	push	ds
	push	es
	push	fs
	push	gs
	mov	ax,0x10
	mov	ds,ax
	mov	es,ax
	mov	fs,ax
	mov	gs,ax
	mov	eax,esp
	push	eax
	mov	eax,interrupt_handler
	call	eax		
	pop	eax
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popa
	add	esp,8
	iret
	
	
