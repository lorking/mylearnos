ENTRY(loader)
SECTIONS {
    .text 0x100000 : {
        tmp/entry.o(.text) *(.text);
        . = ALIGN(4096);
    }
    .data : {
        *(.rodata);
        *(.data);
        . = ALIGN(4096);
    }
    .bss : {
        . = ALIGN(4096);
    }
}
