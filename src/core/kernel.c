#include <system.h>
#include <vedio.h>
#include <struct.h>
#include <idt.h>
extern void gdt_flush();//刷新gdt的操作
//gdt表
struct gdt_entry gdt[5];
struct gdt_ptr gp;
//ldt表
struct idt_entry idt[256];
struct idt_ptr	idtp;
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
//gdt初始化的操作
void gdt_init()
{
	gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
	gp.base = &gdt;
	gdt_set_table(0,0,0,0,0);//NULL segment
	gdt_set_table(1,0,0xFFFFFFFF,0x9A,0xCF);//Code segment
	gdt_set_table(2,0,0xFFFFFFFF,0x92,0xCF);//Data segment
	gdt_set_table(3,0,0xFFFFFFFF,0xFA,0xCF);//user code segment
	gdt_set_table(4,0,0xFFFFFFFF,0xF2,0xCF);//user data segment
}
void kmain()
{
	disable_int();
	gdt_init();//初始化gdt
	gdt_flush();//刷新gdt
	clear();
	reset_int_gate();//设置中断门
	idt_init();//创建中断向量表
	enable_timer(1000);//允许定时器中断
	enable_int();//允许中断
	for(;;)
	{
	}
}
