#ifndef _IDT_H
#define _IDT_H
	//IDT的定义
	struct idt_entry
	{
		unsigned short offset_low;//偏移量的低16位
		unsigned short seg_selector;//段选择器
		unsigned char  reseved;//保留的字段
		unsigned char  control;//控制的字段
		unsigned short offset_high;//偏移量的前16位
	}__attribute__((packed));
	struct idt_ptr
	{
		unsigned short limit;
		unsigned int base;
	}__attribute__((packed));
	//设置中断门的操作
	extern void idt_set_int_gate(int index,unsigned int offset,unsigned short selected,unsigned char control);
	//设置trap门的操作
	extern void idt_set_trap_gate(int index,unsigned int offset,unsigned short selected,unsigned char control);
	//初始化门的操作
	extern void idt_init();
	//重新设置interrup gate
	extern void reset_int_gate();
	//允许中断
	extern void enable_int();
	//禁止中断
	extern void disable_int();
	//重置中断
	extern void send_EOI(unsigned short i_num);
	//enable timer
	extern void enable_timer(unsigned int hz);
#endif
