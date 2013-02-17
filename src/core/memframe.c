#include <memframe.h>
#include <system.h>
#include <mem.h>
#include <vedio.h>
//内存页的header类
struct mem_frame_struct_head{
	struct mem_frame_struct *header;//指向页面的指针
	unsigned short size;//列表的长度
	unsigned short usedFrames;//当前已经使用了的物理帧数 
	unsigned short frameSize;//物理frame的大小
};
struct mem_frame_struct_head mem_frame_header;
//在列表末尾创建一空白frame
struct mem_frame_struct * create_unused_frame()
{
	mem_frame_header.size ++;
	unsigned int address =  (mem_frame_header.size -1)*4*1024;
	struct mem_frame_struct * p = mem_frame_header.header + (mem_frame_header.size -1);
	p -> status = 0;
	p -> address = address;
	return p;
}
//在列表中查找没用的帧的操作
struct mem_frame_struct * find_unused_frame()
{
	struct mem_frame_struct * p = mem_frame_header.header;
	for(int i=0;i < mem_frame_header.size;i++)
	{
		if(p->status == 0)
		{
			return p;
		}
		p++;
	}
	return create_unused_frame();
}
//在列表中查找物理地址的frame
struct mem_frame_struct * find_frame_byaddress(unsigned int address)
{
	if(mem_frame_header.size == 0)
		return NULL;
	struct mem_frame_struct *p = mem_frame_header.header;
	address &= 0xfffff000;//对齐地址
	for(int i=0;i < mem_frame_header.size;i++)
	{
		if( p -> address == address)
		{
			return p;
		}
	}
	return NULL;
}
//获取frame可能的最大内存
unsigned int mem_frame_max_size(unsigned int physicsize)
{
	unsigned int frameSize = physicsize/(1024*4);
	return frameSize * sizeof(struct mem_frame_struct);
}
//初始化内存
void * init_mem_frame(unsigned int memsize)
{
	mem_frame_header.header = (struct mem_frame_struct *)mem_malloc(memsize);
	mem_frame_header.size = 0;
	mem_frame_header.usedFrames = 0;
	mem_frame_header.frameSize = memsize / sizeof(struct mem_frame_struct);
}
//分配一个物理页
unsigned int pgalloc()
{
	struct mem_frame_struct * p = find_unused_frame();
	p -> status = 1;
	mem_frame_header.usedFrames++;
	return p -> address;
}
//释放一个物理页
void pgfree(unsigned int address)
{
	address &= 0xfffff000;//对齐地址
	struct mem_frame_struct * p = find_frame_byaddress(address);
	if(p)
	{
		p->status = 0;
		mem_frame_header.usedFrames--;
	}
}
void print_mem_frame_header()
{
	printk("----------------------header begin------------------\n");
	printk("size:%d,usedFrames:%d,frameSize:%d\n",mem_frame_header.size,mem_frame_header.usedFrames,mem_frame_header.frameSize);
	printk("----------------------header end--------------------\n");
}
void print_mem_frame_struct(unsigned int index)
{
	printk("--------------------frame begin---------------------\n");
	printk("index:%d,status:%d,address:%x\n",index,(mem_frame_header.header+index)->status,(mem_frame_header.header+index)->address);
	printk("--------------------frame end-----------------------\n");
}
