#ifndef _SYSTEM_H
#define _SYSTEM_H
#define NULL 0
	extern int strlen(char *s);//����ַ����ĳ���
	//extern char * strncat(char * dest,char * src,int n);//ƴ���ַ���
	//extern char * strncpy(char * dest,char * src,int n);//�����ַ���
	extern char *memcpy(void *dest,const void *src,int len);//�ڴ濽��
	extern void *memset(void *s,int ch,int size);//�ڴ����ú���
#endif
