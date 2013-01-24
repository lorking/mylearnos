[bits 32]
global loader
global gdt_flush
global idt_flush
extern gp
extern idtp
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
	
