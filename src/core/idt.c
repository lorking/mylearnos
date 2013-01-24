#include <system.h>
#include <idt.h>
#include <vedio.h>
#include <io.h>
extern struct idt_entry idt[256];
extern struct idt_ptr	idtp;
extern void idt_flush();
//中断函数
void is0();//中断为0的函数
void is1();
void is2();
void is3();
void is4();
void is5();
void is6();
void is7();
void is8(unsigned char c);
void is9();
void is10(unsigned char c);
void is11(unsigned char c);
void is12(unsigned char c);
void is13(unsigned char c);
void is14(unsigned char c);
void is15();
void is16();
void is17();
void is18();
void is19_31();
//中断服务函数
void ir32();
void ir33();
void ir34();
void ir35();
void ir36();
void ir37();
void ir38();
void ir39();
void ir40();
void ir41();
void ir42();
void ir43();
void ir44();
void ir45();
void ir46();
void ir47();
//重置中断号
void reset_int(unsigned short i_num)
{
	if(i_num >= 40)
	{
		outportb(0xA0, 0x20);
	}
	outportb(0x20, 0x20);
}
//重新设置中断门
void reset_int_gate()
{
	unsigned char pri_icw1,sec_icw1,pri_icw2,sec_icw2,pri_icw3,sec_icw3,pri_icw4,sec_icw4;
	//设置icw1
	pri_icw1 = 0x11;
	sec_icw1 = 0x11;
	outportb(0x20,pri_icw1);
	outportb(0xa0,sec_icw1);
	//设置icw2
	pri_icw2 = 0x20;
	sec_icw2 = 0x28;
	outportb(0x21,pri_icw2);
	outportb(0xa1,pri_icw2);
	//设置icw3
	pri_icw3 = 0x4;
	sec_icw3 = 0x2;
	outportb(0x21,pri_icw3);
	outportb(0xa1,sec_icw3);
	//设置icw4
	pri_icw4 = 0x1;
	sec_icw4 = 0x1;
	outportb(0x21,pri_icw4);
	outportb(0xa1,pri_icw4);
	//设置ocw1,ocw2
	outportb(0x21,0);
	outportb(0xa1,0);
}
//允许中断
void enable_int()
{
	__asm__ __volatile__ ("sti");
}
//设置中断门的操作
void idt_set_int_gate(int index,unsigned int offset,unsigned short selected,unsigned char control)
{
	idt[index].offset_low = (short)(0xffff & offset);
	idt[index].offset_high = (short)((offset >> 16) & 0xffff);
	idt[index].reseved = 0;
	idt[index].seg_selector = selected;
	//设置control字段
	idt[index].control = (control & 0xe8) | 0x6;
}
//设置trap门的操作
void idt_set_trap_gate(int index,unsigned int offset,unsigned short selected,unsigned char control)
{
	idt[index].offset_low = (short)(0xffff & offset);
	idt[index].offset_high = (short)((offset >> 16) & 0xffff);
	idt[index].reseved = 0;
	idt[index].seg_selector = selected;
	//设置control字段
	idt[index].control = (control & 0xe8) | 0x7;
}
//idt的初始化操作
void idt_init()
{
	idtp.limit = sizeof(struct idt_entry)*256 - 1;
	idtp.base = &idt;
	memset(idt,0,sizeof(struct idt_entry)*256);
	idt_set_int_gate(0,(unsigned int)is0,0x08,0x8b);
	idt_set_int_gate(1,(unsigned int)is1,0x08,0x8b);
	idt_set_int_gate(2,(unsigned int)is2,0x08,0x8b);
	idt_set_int_gate(3,(unsigned int)is3,0x08,0x8b);
	idt_set_int_gate(4,(unsigned int)is4,0x08,0x8b);
	idt_set_int_gate(5,(unsigned int)is5,0x08,0x8b);
	idt_set_int_gate(6,(unsigned int)is6,0x08,0x8b);
	idt_set_int_gate(7,(unsigned int)is7,0x08,0x8b);
	idt_set_int_gate(8,(unsigned int)is8,0x08,0x8b);
	idt_set_int_gate(9,(unsigned int)is9,0x08,0x8b);
	idt_set_int_gate(10,(unsigned int)is10,0x08,0x8b);
	idt_set_int_gate(11,(unsigned int)is11,0x08,0x8b);
	idt_set_int_gate(12,(unsigned int)is12,0x08,0x8b);
	idt_set_int_gate(13,(unsigned int)is13,0x08,0x8b);
	idt_set_int_gate(14,(unsigned int)is14,0x08,0x8b);
	idt_set_int_gate(15,(unsigned int)is15,0x08,0x8b);
	idt_set_int_gate(16,(unsigned int)is16,0x08,0x8b);
	idt_set_int_gate(17,(unsigned int)is17,0x08,0x8b);
	idt_set_int_gate(18,(unsigned int)is18,0x08,0x8b);
	idt_set_int_gate(19,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(20,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(21,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(22,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(23,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(24,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(25,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(26,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(27,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(28,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(29,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(30,(unsigned int)is19_31,0x08,0x8b);
	idt_set_int_gate(31,(unsigned int)is19_31,0x08,0x8b);

	idt_set_int_gate(32,(unsigned int)ir32,0x08,0x8b);
	idt_set_int_gate(33,(unsigned int)ir33,0x08,0x8b);
	idt_set_int_gate(34,(unsigned int)ir34,0x08,0x8b);
	idt_set_int_gate(35,(unsigned int)ir35,0x08,0x8b);
	idt_set_int_gate(36,(unsigned int)ir36,0x08,0x8b);
	idt_set_int_gate(37,(unsigned int)ir37,0x08,0x8b);
	idt_set_int_gate(38,(unsigned int)ir38,0x08,0x8b);
	idt_set_int_gate(39,(unsigned int)ir39,0x08,0x8b);
	idt_set_int_gate(40,(unsigned int)ir40,0x08,0x8b);
	idt_set_int_gate(41,(unsigned int)ir41,0x08,0x8b);
	idt_set_int_gate(42,(unsigned int)ir42,0x08,0x8b);
	idt_set_int_gate(43,(unsigned int)ir43,0x08,0x8b);
	idt_set_int_gate(44,(unsigned int)ir44,0x08,0x8b);
	idt_set_int_gate(45,(unsigned int)ir45,0x08,0x8b);
	idt_set_int_gate(46,(unsigned int)ir46,0x08,0x8b);
	idt_set_int_gate(47,(unsigned int)ir47,0x08,0x8b);
	idt_flush();
}
void is0()
{
	printk("Division By Zero Exception\n");
	for(;;);
}
void is1()
{
	printk("Debug exception\n");
	for(;;);
}
void is2()
{
	printk("Non maskable interrupt\n");
	for(;;);
}
void is3()
{
	printk("Breakpoint exception\n");
	for(;;);
}
void is4()
{
	printk("Into detected overflow\n");
	for(;;);
}
void is5()
{
	printk("Out of bounds exception\n");
	for(;;);
}
void is6()
{
	printk("Invalid Opcode Exception\n");
	for(;;);
}
void is7()
{
	printk("No coprocessor exception\n");
	for(;;);
}
void is8(unsigned char c)
{
	printk("Double fault,error code:%d",c);
	for(;;);
}
void is9()
{
	printk("Coprocessor segment overrun");
	for(;;);
}
void is10(unsigned char c)
{
	printk("Bad TSS:%d",c);
	for(;;);
}
void is11(unsigned char c)
{
	printk("Segment not present:%d",c);
	for(;;);
}
void is12(unsigned char c)
{
	printk("Stack Fault Exception:%d\n",c);
	for(;;);
}
void is13(unsigned char c)
{
	printk("General protection fault:%d\n",c);
	for(;;);
}
void is14(unsigned char c)
{
	printk("Page fault:%d\n",c);
	for(;;);
}
void is15()
{
	printk("Unknown interrupt exception\n");
	for(;;);
}
void is16()
{
	printk("Coprocessor fault\n");
	for(;;);
}
void is17()
{
	printk("Alignment check exception\n");
	for(;;);
}
void is18()
{
	printk("Machine check exception\n");
	for(;;);
}
void is19_31()
{
	printk("Reserved\n");
	for(;;);
}
void ir32()
{
	printk("zzzzzzzzzzzzzzzzzzzzzz");
	reset_int(32);
}
void ir33()
{
	reset_int(33);
}
void ir34()
{
	reset_int(34);
}
void ir35()
{
	reset_int(35);
}
void ir36()
{
	reset_int(36);
}
void ir37()
{
	reset_int(37);
}
void ir38()
{
	reset_int(38);
}
void ir39()
{
	reset_int(39);
}
void ir40()
{
	reset_int(40);
}
void ir41()
{
	reset_int(41);
}
void ir42()
{
	reset_int(42);
}
void ir43()
{
	reset_int(43);
}
void ir44()
{
	reset_int(44);
}
void ir45()
{
	reset_int(45);
}
void ir46()
{
	reset_int(46);
}
void ir47()
{
	reset_int(47);
}
