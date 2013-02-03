[bits 16]
org  0x10000
jmp	loader2
;************************************************;
;obtain memory size
;************************************************;
obtainMemory:
	push	eax
	push	es
	push	di
	push	ebx
	push	edx
	push	ecx
	push	bp
	;jmp	obtainMemOver
	;pop the register
	;设置缓冲区的地址
	mov	ax,0x1000
	mov	es,ax
	mov	di,memcheckbuf
	;清空ebx
	xor	ebx,ebx
	;设置edx
	mov	edx,0x534d4150
obtainMemLoop:
	mov	eax,0xe820
	mov	ecx,20
	;调用中断
	int	0x15
	;cf被置位说调用失败
	jc	obtainMemFail
	;增加下一个位置,20
	add	di,20
	;增加调用的次数
	inc	dword	[memcheckNum]
	;检查是否调用完成
	cmp	ebx,0
	jne	obtainMemLoop
	jmp obtainMemOver
obtainMemFail:;调用失败,halt
	ret
obtainMemOver:
	pop	bp
	pop	ecx
	pop	edx
	pop	ebx
	pop	di
	pop	es
	pop	eax
	ret
;************************************************;
;read from disk
;************************************************;
readSect:
	push    cx
	push    bx
	mov     ax, si              ; disk sector number
	mov     bx, di              ; buffer address.
	; ch = cylinder = sn / 36
	mov     cl, 36              
	div     cl                  
	mov     ch, al              
	; dh = head = (sn%36)/18
	mov     al, ah
	mov     ah, 0
	mov     cl, 18
	div     cl
	mov     dh, al
	; cl = sector = (ln%36)%18+1
	mov     cl, ah 
	inc     cl
	; dl = drive = 0; 
	mov     dl, 0
	; raise int 13h 
	mov     ax, 201h
	int     13h
	pop     bx
	pop     cx
	ret
;********************************************;
;mem chek buf
;********************************************;
memcheckNum	dd	0;内存检查的个数
memcheckbuf:times 512 db 0
loader2:
	;读取内存的参数
	call	obtainMemory
	;执行逻辑
.Reset:
	mov ah,0	;reset floppy disk
	mov dl,0	;driver 0 is floppy disk
	int 0x13	;call BIOS
	jc .Reset
;
;load kernel to 0x20000(es:bx=0x1000:0000)
;
	mov ax,0x2000
	mov es,ax
	mov di,0
	mov si,5
	mov cx,128
readSectLoop:
	call	readSect
	inc	si	
	add 	di,0x200
	dec 	cx
	jnz 	readSectLoop
;第二阶段加载程序的操作,包括加载32位程序,打开地址线等操作
;
;prepare to enter protect mode
;

;Enable the a20
seta20.1:
	in 	al,0x64
	test	al,0x2
	jnz	seta20.1
	mov	al,0xd1
	out	byte 0x64,al
seta20.2:
	in	al,0x64
	test	al,0x2
	jnz	seta20.2
	mov	al,0xdf
	out	byte 0x60,al
;clear register
	mov	ax,0x1000
	mov	ds,ax
	xor	ax,ax
	mov	ss,ax
	mov	es,ax
;clear the intrrupt
	cli
	lgdt	[gdt_desc]
	mov	eax,cr0
	or	eax,1
	mov	cr0,eax
	jmp	dword 08h:_start_pm
;
;---------------------------------------
[bits 32]
_start_pm:
	mov	ax,	10h
	mov	ds,	ax
	mov	ss,	ax
	mov	es,	ax
	mov	gs,	ax
	mov	esp,	1000h
	cld
	mov	esi,	20000h
	mov	edi,	100000h
	mov	ecx,	10000h
	rep	movsb
	;jump to C!
	;never return it shoud be
	jmp	08h:100000h
_hang:
	jmp	_hang
;null descriptor
gdt:
gdt_null:
	dd	0
	dd	0
;code descriptor
gdt_code:
	dw	0ffffH
	dw	0
	db	0
	db	10011010b
	db	11001111b
	db	0
;data	descriptor
gdt_data:
	dw	0ffffh
	dw	0
	db	0
	db	10010010b
	db	11001111b
	db	0
gdt_end:
gdt_desc:
	dw	gdt_end-gdt-1
	dd	gdt

	times 2048 - ($-$$) db 0
