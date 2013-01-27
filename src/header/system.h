#ifndef _SYSTEM_H
#define _SYSTEM_H
#define NULL 0
	extern int strlen(char *s);//获得字符串的长度
	//extern char * strncat(char * dest,char * src,int n);//拼接字符串
	//extern char * strncpy(char * dest,char * src,int n);//拷贝字符串
	extern char *memcpy(void *dest,const void *src,int len);//内存拷贝
	extern void *memset(void *s,int ch,int size);//内存设置函数
#endif
