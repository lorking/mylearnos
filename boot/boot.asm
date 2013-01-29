[bits 16]
org  0x7c00
start: jmp loader
msg db "Welcome to lorking's Operating system!",0
;*******************************************;
;prints a string
;*******************************************;
Print:
	lodsb
	or	al,al
	jz	PrintDone
	mov	ah,	0eh
	int	10h
	jmp	Print
PrintDone:
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
;*******************************************;
;BootLoader entry point
;*******************************************;
loader:
	xor ax,ax
	mov ds,ax
	mov ss,ax
	mov sp,0x2000
.Reset:
	mov ah,0	;reset floppy disk
	mov dl,0	;driver 0 is floppy disk
	int 0x13	;call BIOS
	jc .Reset
;
;load kernel to 0x10000(es:bx=0x1000:0000)
;
	mov ax,0x1000
	mov es,ax
	mov di,0
	mov si,1
	mov cx,4
readSectLoop:
	call	readSect
	inc	si	
	add 	di,0x200
	dec 	cx
	jnz 	readSectLoop
	jmp dword 0x1000:0

	times 510 - ($-$$) db 0
	dw 0xAA55
