#ifndef _KEY_H
#define _KEY_H
	#define KEY_BOARD_ENCODER 0X60
	#define KEY_BOARD_CONTROLER 0x64
	#define ESC_KEY 0x1
	extern unsigned char key_map[128];
	extern unsigned char read_key_controler();//读取键盘控制器的状态
	extern void write_key_controler(unsigned char key);//写键盘控制器的状态
	extern unsigned char read_key_encoder();//读取键盘码
	extern void write_key_encoder(unsigned char key);//写键盘码
	extern unsigned char keybuffer_canread(unsigned char status);//键盘缓冲区是否可读,0不可读,1可读
	extern unsigned char keybuffer_canwrite(unsigned char status);//键盘缓冲区是否可写,0不可写,1可写
	
#endif
