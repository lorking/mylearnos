#include <system.h>
#include <vedio.h>
#include <struct.h>
#include <idt.h>
extern void gdt_flush();//ˢ��gdt�Ĳ���
//gdt��
struct gdt_entry gdt[5];
struct gdt_ptr gp;
//ldt��
struct idt_entry idt[256];
struct idt_ptr	idtp;
//����gdt��Ĳ���
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
//gdt��ʼ���Ĳ���
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
	gdt_init();//��ʼ��gdt
	gdt_flush();//ˢ��gdt
	clear();
	reset_int_gate();//�����ж���
	idt_init();//�����ж�������
	enable_timer(1000);//����ʱ���ж�
	enable_int();//�����ж�
	for(;;)
	{
	}
}
