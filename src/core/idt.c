#include <system.h>
#include <idt.h>
#include <vedio.h>
#include <io.h>
#include <key.h>
extern struct idt_entry idt[256];
extern struct idt_ptr	idtp;
extern void idt_flush();
//错误消息列表
char * error_msg [20] = {"Division By Zero Exception\n","Debug exception\n","Non maskable interrupt\n","Breakpoint exception\n",
	"Into detected overflow\n","Out of bounds exception\n","Invalid Opcode Exception\n",
	"No coprocessor exception\n","Double fault,error code:%d\n","Coprocessor segment overrun\n",
	"Bad TSS:%d\n","Segment not present:%d\n","Stack Fault Exception:%d\n",
	"General protection fault:%d\n","Page fault:%d\n","Unknown interrupt exception\n"
	,"Coprocessor fault\n","Alignment check exception\n","Machine check exception\n","Reserved\n"}; 
//中断函数指针数组
void (* func_array[15])();
//中断函数的处理函数
void timer_handler();
void ir33_handler();
void ir34_handler();
void ir35_handler();
void ir36_handler();
void ir37_handler();
void ir38_handler();
void ir39_handler();
void ir40_handler();
void ir41_handler();
void ir42_handler();
void ir43_handler();
void ir44_handler();
void ir45_handler();
void ir46_handler();

void extern is0();//中断为0的函数
void extern is1();
void extern is2();
void extern is3();
void extern is4();
void extern is5();
void extern is6();
void extern is7();
void extern is8();
void extern is9();
void extern is10();
void extern is11();
void extern is12();
void extern is13();
void extern is14();
void extern is15();
void extern is16();
void extern is17();
void extern is18();
void extern is19_31();
//中断服务函数
extern void ir32();
extern void ir33();
extern void ir34();
extern void ir35();
extern void ir36();
extern void ir37();
extern void ir38();
extern void ir39();
extern void ir40();
extern void ir41();
extern void ir42();
extern void ir43();
extern void ir44();
extern void ir45();
extern void ir46();
//错误处理函数
void interrupt_handler(struct regs_struct * reg)
{
	int index = reg->int_no;
	int err_code = reg->err_code;
	if(index == 8 || index == 10 ||index == 11 || index == 12 || index == 13 || index == 14)
	{
		printk(error_msg[index],err_code);
		for(;;);
	}else if(index >= 32 && index <=46)
	{
		void (*f) ();
		f = func_array[index - 32];
		f();
		send_EOI(index);
	}else
	{
		printk(error_msg[index]);
		for(;;);
	}
}
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
	//设置handler函数
	func_array[0] = timer_handler;
	func_array[1] = ir33_handler;
	func_array[2] = ir34_handler;
	func_array[3] = ir35_handler;
	func_array[4] = ir36_handler;
	func_array[5] = ir37_handler;
	func_array[6] = ir38_handler;
	func_array[7] = ir39_handler;
	func_array[8] = ir40_handler;
	func_array[9] = ir41_handler;
	func_array[10] = ir42_handler;
	func_array[11] = ir43_handler;
	func_array[12] = ir44_handler;
	func_array[13] = ir45_handler;
	func_array[14] = ir46_handler;
	idt_flush();
}
//中断函数的实现
//unsigned int counter = 0;
void timer_handler()
{
	/*
	counter++;
	if(counter % 100 ==0)
	{
		printk("one minitues spend%d\n",counter);
	}*/
}
void ir33_handler()
{
	unsigned char scancode;
	scancode = read_key_encoder();
	printk("The input code is:[%x]\n",scancode);
	if(scancode==1)
	{
		set_led_lock(1);
	}else if(scancode==2)
	{
		set_led_lock(0);
	}
}
void ir34_handler()
{
}
void ir35_handler()
{
}
void ir36_handler()
{
}
void ir37_handler()
{
}
void ir38_handler()
{
}
void ir39_handler()
{
}
void ir40_handler()
{
}
void ir41_handler()
{
}
void ir42_handler()
{
}
void ir43_handler()
{
}
void ir44_handler()
{
}
void ir45_handler()
{
}
void ir46_handler()
{
}
