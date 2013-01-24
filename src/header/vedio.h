#ifndef _VEDIO_H
#define _VEDIO_H
	extern void clear();//清除屏幕的操作
	extern void setTextColor(unsigned char forecolor,unsigned char backcolor);//设置文字的颜色
	extern void putch(unsigned char c);//向屏幕写文字
	extern void puts(unsigned char * str);//向屏幕写文字
	extern void writech(unsigned char c,int i_x,int i_y);//写入字符
	extern void setFlash();//设置闪烁的
	extern void roll_up();//卷屏幕
	extern void printk(char * fmt,...);//格式化int参数
#endif
