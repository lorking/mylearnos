#include <vedio.h>
#include <io.h>
#include <system.h>
#define WIDTH 80
#define HEIGHT 25
int _x = 0,_y =0;
unsigned short color = 0x0f;//文字的颜色
unsigned short *textmemptr = (unsigned short *)0xB8000;
//清屏
void clear()
{
	unsigned char c = ' '; 
	color = 0x0f;
	textmemptr = (unsigned short *)0xB8000;

	for(_y=0;_y < HEIGHT;_y++)
	{
		for(_x=0;_x < WIDTH;_x++)
		{
			unsigned int _index = _y * WIDTH + _x;
			*(textmemptr+_index) = (color << 8 | c);
		}
	}
	_x = 0;
	_y = 0;	
	setFlash();
}
//设置文本的颜色
void setTextColor(unsigned char forecolor,unsigned char backcolor)
{
	color = backcolor << 4 | forecolor;
}
//向屏幕写字符
void putch(unsigned char c)
{
	//判断是否为回车
	if(c=='\n' || c=='\r')
	{
		_y ++;
		_x = 0;
	}else
	{
		writech(c,_x,_y);
		int tmp = _x +1;
		_x = tmp % WIDTH;
		_y = _y + tmp/WIDTH;	
	}
	//判断是否需要卷屏幕
	if(_y >= HEIGHT)
	{
		roll_up();
	}
	setFlash();
}
//向屏幕里写字符串
void puts(unsigned char * str) 
{
	int len = strlen(str);
	int i = 0;
	for(i=0;i < len;i++)
	{
		char c = *(str+i);
		putch(c);
	}
}
//向屏幕设置闪烁
void setFlash()
{
	unsigned int _index = _y * WIDTH + _x;
	outportb(0x3D4, 14);
	outportb(0x3D5, _index>>8);
	outportb(0x3D4, 15);
	outportb(0x3D5, _index);
}
//向指定位置写入字符
void writech(unsigned char c,int i_x,int i_y)
{
	unsigned short _index = i_y * WIDTH + i_x;
	*(textmemptr+_index) = (color << 8 | c); 
}
//卷屏幕
void roll_up()
{
	int begin = 0;
	int index = 0;
	char * starPtr = textmemptr;
	char * endPtr = NULL;
	for(index = 1;index <= HEIGHT - 1;index ++)
	{
		endPtr = starPtr + WIDTH*2;
		memcpy(starPtr,endPtr,WIDTH*2);
		starPtr = endPtr;
	}
	//把最后一行设为空行
	int index_x;
	short * tmp_ptr = (short *)endPtr;
	for(index_x = 0;index_x < WIDTH;index_x++)
	{
		*(tmp_ptr+index_x) = (color << 8 | ' ');
	}
	_y = HEIGHT - 1;
	if(_y < 0)
	{
		_y = 0;
	}
	setFlash();
}
//递归打印数字
void printn(unsigned int n,unsigned int b)
{
	static char *ntab = "0123456789ABCDEF";
	unsigned int a,m;
	if((a=n/b))
	{
		printn(a,b);
	}
	m = n % b;
	putch(ntab[m]);
}
//格式化参数
void printk(char * fmt,...)
{
	char c;
	int n;
	int *adx = (int *)(void *)&fmt + 1;
_loop:
	while((c = * fmt++) != '%')
	{
		if(c=='\0')
			return;
		putch(c);
	}
	c = * fmt ++;
	if(c == 'd' || c == 'l')
	{
		n = *adx;
		if(n < 0)
		{
			putch('-');
			n *= -1;
		}
		printn(n,10);
	}
	if(c=='o' || c=='x')
	{
		printn(*adx,c=='o'?8:16);
	}
	if(c=='c')
	{
		putch(*adx);
	}
	if(c=='s')
	{
		puts((char *)*adx);
	}
	adx ++;
	goto _loop;
}

