#ifndef _STRUCT_H
#define _STRUCT_H
	//gdt描述符
	struct gdt_entry
	{
		unsigned short limit_low;
		unsigned short base_low;
		unsigned char  base_middle;
		unsigned char  access;
		unsigned char  granularity;
		unsigned char  base_high;
	}__attribute__((packed));
	struct gdt_ptr
	{
		unsigned short limit;
		unsigned int   base;
	}__attribute__((packed));
	//tss段的描述符
	struct tss_descript
	{
		unsigned int	preTssSelector;//前一个tss的选择符
		unsigned int	esp0;//esp0
		unsigned int	ss0;
		unsigned int	esp1;
		unsigned int	ss1;
		unsigned int	esp2;
		unsigned int	ss2;
		unsigned int	cr3;
		unsigned int	eip;
		unsigned int	eflags;
		unsigned int	eax;
		unsigned int	ecx;
		unsigned int	edx;
		unsigned int	ebx;
		unsigned int	esp;
		unsigned int	ebp;
		unsigned int	esi;
		unsigned int	edi;
		unsigned int	es;
		unsigned int	cs;
		unsigned int	ss;
		unsigned int	ds;
		unsigned int	fs;
		unsigned int	gs;
		unsigned int	ldt;
		unsigned short	trap;
		unsigned short	iomap_base;	
	}__attribute__((packed));
#endif
