floppy.img:tmp/floppy.img
	dd if=tmp/floppy.img of=floppy.img bs=65535 count=1
tmp/floppy.img:tmp/boot.img tmp/boot2.img tmp/kernel.img
	cat tmp/boot.img tmp/boot2.img tmp/kernel.img > tmp/floppy.img
tmp/boot.img:boot/boot.asm
	nasm -f bin boot/boot.asm -o tmp/boot.img
tmp/boot2.img:boot/boot2.asm
	nasm -f bin boot/boot2.asm -o tmp/boot2.img
tmp/kernel.img:tmp/kernel.bin
	objcopy -R .pdr -R .comment -R .note -S -O binary tmp/kernel.bin tmp/kernel.img
tmp/kernel.bin:tmp/entry.o tmp/kernel.o tmp/vedio.o tmp/io.o ldfile/linker.ld tmp/system.o tmp/idt.o tmp/key.o tmp/mem.o tmp/mem_coredump_allock.o tmp/memframe.o
	ld -T ldfile/linker.ld -o tmp/kernel.bin tmp/entry.o tmp/kernel.o tmp/vedio.o tmp/io.o tmp/system.o tmp/idt.o tmp/key.o tmp/mem.o tmp/mem_coredump_allock.o \
	tmp/memframe.o
tmp/mem_coredump_allock.o:src/header/mem_coredump_allock.h src/core/mem_coredump_allock.c
	gcc -c -o tmp/mem_coredump_allock.o src/core/mem_coredump_allock.c  -std=c99 -nostdinc -std=c99 -fno-builtin -fno-stack-protector -I./src/header
tmp/mem.o:src/core/mem.c src/header/mem.h src/header/mem_coredump_allock.h src/header/memframe.h
	gcc -c -o tmp/mem.o src/core/mem.c -nostdinc -fno-builtin -std=c99 -fno-stack-protector -I./src/header
tmp/memframe.o:src/core/memframe.c src/header/memframe.h
	gcc  -c -o tmp/memframe.o src/core/memframe.c -nostdinc -fno-builtin -std=c99 -fno-stack-protector -I./src/header
tmp/vedio.o:src/core/vedio.c src/header/vedio.h src/header/io.h src/header/system.h
	gcc -c -o tmp/vedio.o src/core/vedio.c -nostdinc -fno-builtin -std=c99 -fno-stack-protector -I./src/header
tmp/entry.o:src/core/entry.asm
	nasm -f elf -o tmp/entry.o src/core/entry.asm
tmp/kernel.o:src/core/kernel.c src/header/system.h src/header/vedio.h src/header/system.h src/header/struct.h src/header/idt.h src/header/key.h src/header/mem.h
	gcc -c src/core/kernel.c -o tmp/kernel.o  -nostdinc -fno-builtin -std=c99 -fno-stack-protector -I./src/header
tmp/io.o:src/core/io.c src/header/io.h src/header/system.h
	gcc -c src/core/io.c -o tmp/io.o  -nostdinc -fno-builtin -std=c99 -fno-stack-protector -I./src/header
tmp/system.o:src/core/system.c src/header/system.h
	gcc -c src/core/system.c -o tmp/system.o -nostdinc -fno-builtin -std=c99 -fno-stack-protector -I./src/header
tmp/idt.o:src/core/idt.c src/header/idt.h src/header/key.h
	gcc -c src/core/idt.c -o tmp/idt.o -nostdinc -fno-builtin -std=c99 -fno-stack-protector -I./src/header
tmp/key.o:src/core/key.c src/header/key.h
	gcc -c src/core/key.c -o tmp/key.o -nostdinc -fno-builtin -std=c99 -fno-stack-protector -I./src/header
clean:
	rm tmp/*
	rm floppy.img
