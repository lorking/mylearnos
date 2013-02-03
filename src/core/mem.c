#include <mem.h>
#include <system.h>
#include <vedio.h>
struct mem_bios_info *mem_bios_ptr;
unsigned int mem_bios_size;
//设置内存信息
void set_mem_bios_info(int size,struct mem_bios_info *p)
{
	mem_bios_ptr = p;
	mem_bios_size = size;
}
//获得内存的总量
extern unsigned int obtain_total_memsize()
{
	struct mem_bios_info * p = mem_bios_ptr;
	unsigned int total = 0;	
	int i = 0;
	for(i=0;i < mem_bios_size;i++)
	{
		//printk("mem1:%x\n",(p+i)->baseAddr_low);
		//printk("size1:%x\n",(p+i)->regionLen_low);
		//printk("type:%d\n",(p+i)->type);
		if((p+i)->type==MEM_TYPE_1)
		{
			if((p+i)->baseAddr_low + (p+i)->regionLen_low > total)
			{
				total = (p+i)->baseAddr_low + (p+i)->regionLen_low;
			}
		}
	}
	//printk("total ram:%d\n",total/1024/1024);
	return total;
}
