#ifndef _IO_H
#define _IO_H
	extern unsigned char inportb(unsigned short port);//从端口读取数据
	extern void outportb(unsigned short port, unsigned char data);//向端口写数据
#endif
