#include <system.h>
int strlen(char *s)
{
	int i = -1;
	char c = 0;
	do{
		i++;
		c = *(s+i);
	}while(c!=NULL);
	return i;
}
char *memcpy(void *dest,const void *src,int len)
{
	int i;
	char * dest_ = (char *)dest;
	char * src_ = (char *)src;
	for(i=0;i < len;i++)
	{
		*(dest_ + i)  = *(src_ + i);
	}
	return dest;
}
//ÄÚ´æÉèÖÃº¯Êý
void *memset(void *s,int ch,int size)
{
	char * cs = (char*)s;
	int i;
	for(i = 0;i < size;i++)
	{
		*(cs+i) = (char)ch;
	}
	return s;
}
