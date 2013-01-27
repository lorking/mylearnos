#include <key.h>
#include <io.h>
unsigned char key_map[128];
//读取键盘控制器
unsigned char read_key_controler()
{
	unsigned char ret = inportb(KEY_BOARD_CONTROLER);
	return ret;
}
//写键盘控制器
void write_key_controler(unsigned char key)
{
	outportb(KEY_BOARD_CONTROLER,key);
}
//读取键盘码
unsigned char read_key_encoder()
{
	unsigned char con_key = read_key_controler();
	//不可读的情况下，循环等待
	while(keybuffer_canread(con_key) != 1)
	{
		con_key = read_key_controler();
	}
	unsigned char scancode = inportb(KEY_BOARD_ENCODER);
	return scancode;
}
//写键盘码
void write_key_encoder(unsigned char key)
{
	unsigned char con_key = read_key_controler();
	while(keybuffer_canwrite(con_key) !=1)
	{
		con_key = read_key_controler();
	}
	outportb(KEY_BOARD_ENCODER,key);
}
//键盘缓冲区是否可读,0不可读,1可读
unsigned char keybuffer_canread(unsigned char status)
{
	unsigned char mark = 0x1;
	if(status & mark)
	{
		return 1;
	}
	return 0;
}
//键盘缓冲区是否可写,0不可写,1可写
unsigned char keybuffer_canwrite(unsigned char status)
{
	unsigned char mark = 0x02;
	if(status & mark)
	{
		return 0;
	}
	return 1;
}
//判断键盘是否被锁
unsigned char is_keyboard_locked(unsigned char status)
{
	unsigned char mark = 0x8;
	if(status & mark)
	{
		return 0;
	}
	return 1;
}
//设置键盘的状态
void set_led_lock(unsigned char lock)
{
	write_key_encoder(KEY_COMMAND_SET_LED);
	write_key_encoder(lock);
}
//设置键盘禁止相应
void disable_scanning()
{
	write_key_encoder(KEY_COMMAND_RESET_WAITENABLE);
}
//获得键盘类型
/*
void indentfy()
{
	disable_scanning();
	write_key_encoder(KEY_COMMAND_KEYBOARDID);
}*/
