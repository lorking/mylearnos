#include <system.h>
#include <vedio.h>
#include <struct.h>
#include <idt.h>
#include <key.h>
extern void gdt_flush();//刷新gdt的操作
extern void tss_flush();//刷新tss的操作
//gdt表
struct gdt_entry gdt[6];
struct gdt_ptr gp;
//ldt表
struct idt_entry idt[256];
struct idt_ptr	idtp;
//tss 字段的入口地址
struct tss_descript tss_entry;
//memory link address
unsigned int meminfo_dress;
unsigned int meminfo_size;
//设置gdt表的操作
void gdt_set_table(int index,unsigned int base,unsigned int limit,unsigned char access,unsigned char gran)
{
	gdt[index].base_low = base & 0xffff;
	gdt[index].base_middle = (base >> 16)&0xFF;
	gdt[index].base_high = (base >> 24)&0xff;
	gdt[index].access = access;
	gdt[index].limit_low = limit & 0xffff;
	gdt[index].granularity = ((limit >> 16) & 0x0F);
	gdt[index].granularity |= (gran & 0xF0);
	gdt[index].granularity = gran;
}
//设置tss的操作
void write_tss(int index,unsigned short ss0,unsigned int esp0)
{
	unsigned int base = (unsigned int)&tss_entry;
	unsigned int limit = base + sizeof(tss_entry);
	gdt_set_table(index,base,limit,0xe9,0x00);
	memset(&tss_entry,0,sizeof(tss_entry));
	tss_entry.ss0=ss0;
	tss_entry.esp0=esp0;
	tss_entry.cs   = 0x0b;
	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}
//gdt初始化的操作
void gdt_init()
{
	gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
	gp.base = &gdt;
	gdt_set_table(0,0,0,0,0);//NULL segment
	gdt_set_table(1,0,0xFFFFFFFF,0x9A,0xCF);//Code segment
	gdt_set_table(2,0,0xFFFFFFFF,0x92,0xCF);//Data segment
	gdt_set_table(3,0,0xFFFFFFFF,0xFA,0xCF);//user code segment
	gdt_set_table(4,0,0xFFFFFFFF,0xF2,0xCF);//user data segment
}
void kmain()
{
	gdt_init();//初始化gdt
	gdt_flush();//刷新gdt
	clear();
	reset_int_gate();//设置中断门
	idt_init();//创建中断向量表
	enable_timer(100);//允许定时器中断
	enable_int();//允许中断
	set_mem_bios_info(meminfo_size,(struct mem_bios_info *) meminfo_dress);
	init_page_manage();
	int i = 0;
	int * a = 0x20000;
	//__asm__ __volatile__("int $0x1");
	//int a = 1 /0;
	for(;;)
	{
	}
}
