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
//设置定时器的divisor
void enable_timer(unsigned int hz)
{
	int divisor = 1193180 / hz;
	outportb(0x43,0x36);//counter= 0,RW =3(LSB then MSB),Mode = 3(Square Wave Mode),BCD = 0(note use BCD)
	outportb(0x40,divisor & 0xff);
	outportb(0x40,divisor>>8);
}
//Send EOI
void send_EOI(unsigned short i_num)
{
	if(i_num >= 40)
	{
		outportb(0xA0, 0x20);
	}
	outportb(0x20, 0x20);
	//enable_int();
}
//重新设置中断门
void reset_int_gate()
{
	//设置icw1
	outportb(0x20,0x11);
	outportb(0xa0,0x11);
	//设置icw2
	outportb(0x21,0x20);
	outportb(0xa1,0x28);
	//设置icw3
	outportb(0x21,0x04);
	outportb(0xa1,0x02);
	//设置icw4
	outportb(0x21,0x01);
	outportb(0xa1,0x01);
	//设置ocw1,ocw2
	outportb(0x21,0x0);
	outportb(0xa1,0x0);
}
//允许中断
void enable_int()
{
	__asm__ __volatile__ ("sti");
}
//禁止中断
void disable_int()
{
	__asm__ __volatile__ ("cli");
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

	idt_set_trap_gate(32,(unsigned int)ir32,0x08,0x8b);
	idt_set_trap_gate(33,(unsigned int)ir33,0x08,0x8b);
	idt_set_trap_gate(34,(unsigned int)ir34,0x08,0x8b);
	idt_set_trap_gate(35,(unsigned int)ir35,0x08,0x8b);
	idt_set_trap_gate(36,(unsigned int)ir36,0x08,0x8b);
	idt_set_trap_gate(37,(unsigned int)ir37,0x08,0x8b);
	idt_set_trap_gate(38,(unsigned int)ir38,0x08,0x8b);
	idt_set_trap_gate(39,(unsigned int)ir39,0x08,0x8b);
	idt_set_trap_gate(40,(unsigned int)ir40,0x08,0x8b);
	idt_set_trap_gate(41,(unsigned int)ir41,0x08,0x8b);
	idt_set_trap_gate(42,(unsigned int)ir42,0x08,0x8b);
	idt_set_trap_gate(43,(unsigned int)ir43,0x08,0x8b);
	idt_set_trap_gate(44,(unsigned int)ir44,0x08,0x8b);
	idt_set_trap_gate(45,(unsigned int)ir45,0x08,0x8b);
	idt_set_trap_gate(46,(unsigned int)ir46,0x08,0x8b);
	idt_set_trap_gate(47,(unsigned int)ir47,0x08,0x8b);
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
	printk("The time intterupt!\n");
	send_EOI(32);
}
void ir33()
{		
	send_EOI(33);
}
void ir34()
{
	send_EOI(34);
}
void ir35()
{
	send_EOI(35);
}
void ir36()
{
	send_EOI(36);
}
void ir37()
{
	send_EOI(37);
}
void ir38()
{
	send_EOI(38);
}
void ir39()
{
	send_EOI(39);
}
void ir40()
{
	send_EOI(40);
}
void ir41()
{
	send_EOI(41);
}
void ir42()
{
	send_EOI(42);
}
void ir43()
{
	send_EOI(43);
}
void ir44()
{
	send_EOI(44);
}
void ir45()
{
	send_EOI(45);
}
void ir46()
{
	send_EOI(46);
}
void ir47()
{
	send_EOI(47);
}
