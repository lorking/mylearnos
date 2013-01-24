floppy.img:tmp/floppy.img
	dd if=tmp/floppy.img of=floppy.img bs=65535 count=1
tmp/floppy.img:tmp/boot.img tmp/kernel.img
	cat tmp/boot.img tmp/kernel.img > tmp/floppy.img
tmp/boot.img:boot/boot.asm
	nasm -f bin boot/boot.asm -o tmp/boot.img
tmp/kernel.img:tmp/kernel.bin
	objcopy -R .pdr -R .comment -R .note -S -O binary tmp/kernel.bin tmp/kernel.img
tmp/kernel.bin:tmp/entry.o tmp/kernel.o tmp/vedio.o tmp/io.o ldfile/linker.ld tmp/system.o tmp/idt.o
	ld -T ldfile/linker.ld -o tmp/kernel.bin tmp/entry.o tmp/kernel.o tmp/vedio.o tmp/io.o tmp/system.o tmp/idt.o
tmp/vedio.o:src/core/vedio.c src/header/vedio.h src/header/io.h src/header/system.h
	gcc -c -o tmp/vedio.o src/core/vedio.c -nostdinc -fno-builtin -fno-stack-protector -I./src/header
tmp/entry.o:src/core/entry.asm
	nasm -f elf -o tmp/entry.o src/core/entry.asm
tmp/kernel.o:src/core/kernel.c src/header/system.h src/header/vedio.h src/header/system.h src/header/struct.h src/header/idt.h
	gcc -c src/core/kernel.c -o tmp/kernel.o  -nostdinc -fno-builtin -fno-stack-protector -I./src/header
tmp/io.o:src/core/io.c src/header/io.h src/header/system.h
	gcc -c src/core/io.c -o tmp/io.o  -nostdinc -fno-builtin -fno-stack-protector -I./src/header
tmp/system.o:src/core/system.c src/header/system.h
	gcc -c src/core/system.c -o tmp/system.o -nostdinc -fno-builtin -fno-stack-protector -I./src/header
tmp/idt.o:src/core/idt.c src/header/idt.h
	gcc -c src/core/idt.c -o tmp/idt.o -nostdinc -fno-builtin -fno-stack-protector -I./src/header
clean:
	rm tmp/*
	rm floppy.img
