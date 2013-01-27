#ifndef _KEY_H
#define _KEY_H
	#define KEY_BOARD_ENCODER 0X60
	#define KEY_BOARD_CONTROLER 0x64
	#define KEY_COMMAND_SET_LED 0xed//Set LEDs
	#define KEY_COMMAND_ECHO 0xee//Echo command. Returns 0xEE to port 0x60 as a diagnostic test
	#define KEY_COMMAND_ALTERSCANCODE 0xf0//Set alternate scan code set
	#define KEY_COMMAND_KEYBOARDID 0xf2//Send 2 byte keyboard ID code as the next two bytes to be read from port 0x60
	#define KEY_COMMAND_AUTOREPEAT 0XF3//Set autrepeat delay and repeat rate
	#define KEY_COMMAND_ENABLEKEYBOARD 0xf4//Enable keyboard
	#define KEY_COMMAND_RESET_WAITENABLE 0xf5//Reset to power on condition and wait for enable command
	#define KEY_COMMAND_RESET_WAITESCAN 0xf6//Reset to power on condition and begin scanning keyboard
	#define KEY_COMMAND_ALLKEY_AUTOREPEAT 0x0xf7//Set all keys to autorepeat (PS/2 only)
	#define KEY_COMMAND_MAKEANDBREAKCODE 0xf8//Set all keys to send make code and break code (PS/2 only)
	#define KEY_COMMAND_OANLYMAKECODE 0xf9//Set all keys to generate only make codes
	#define KEY_COMMAND_AUTOREPEAT_MBCODE 0xfa//Set all keys to autorepeat and generate make/break codes
	#define KEY_COMMAND_SINGLEKEY_AUTOREPEAT 0xfb//Set a single key to autorepeat
	#define KEY_COMMAND_SINGLEKEY_MBCODE 0xfc//Set a single key to generate make and break codes
	#define KEY_COMMAND_SINGLEKEY_ONLYBCODE 0xfd//Set a single key to generate only break codes
	#define KEY_COMMAND_RESEND 0xfe//Resend last result
	#define KEY_COMMAND_RESET_POWER 0xff//Reset keyboard to power on state and start self test
	#define KEY_LED_SCROLL_LOCK 0x1//scroll lock
	#define KEY_LED_NUM_LOCK 0x2//num lock
	#define KEY_LED_CAPS_LOCK 0x4//caps lock
	extern unsigned char key_map[128];
	extern unsigned char read_key_controler();//读取键盘控制器的状态
	extern void write_key_controler(unsigned char key);//写键盘控制器的状态
	extern unsigned char read_key_encoder();//读取键盘码
	extern void write_key_encoder(unsigned char key);//写键盘码
	extern unsigned char keybuffer_canread(unsigned char status);//键盘缓冲区是否可读,0不可读,1可读
	extern unsigned char keybuffer_canwrite(unsigned char status);//键盘缓冲区是否可写,0不可写,1可写
	extern unsigned char is_keyboard_locked(unsigned char status);//判断键盘是否被锁,1被锁，0未被锁
	extern void set_led_lock(unsigned char lock);//设置键盘lock
	extern void disable_scanning();//设置键盘禁止相应
	//extern void indentfy();//获得键盘的类型
	//extern void enable_scanning();//设置键盘可以相应
#endif
