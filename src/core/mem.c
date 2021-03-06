#include <mem.h>
#include <system.h>
#include <vedio.h>
#include <mem_coredump_allock.h>
#include <memframe.h>
unsigned int current_address;//当先分配地址的位置
unsigned int *page_mem_dic;//页面目录的指针
//内存分隔的地址间隔
struct mem_split_address{
	unsigned int core_static_mem_address;//静态区域的地址
	unsigned int core_dump_begin_address;//堆内存的起始位置
	unsigned int user_address;
} split_address;
//保存bios的一些信息
struct mem_bios_info_struct bios_info;
//设置mem_split_address的一些信息
void set_mem_split_address_info(struct mem_split_address * ptr,unsigned short frameSize)
{
	ptr-> core_static_mem_address = KERNERL_PAGE_DIC_ADDRESS;
	ptr -> core_dump_begin_address = STATIC_MEM_LEN + ptr-> core_static_mem_address + frameSize;
	ptr -> user_address = USESR_PROGRAM_STARTADDRESS;
}
//获得pagedic某一项的指针
unsigned int * get_dic_ptr(unsigned int *ptr,unsigned int address)
{
	address = address / (1024 * 4) / (1024 * 4);
	return (ptr + address);
}
//设置内存信息
void set_mem_bios_info(int size,struct mem_bios_info *p)
{
	bios_info.mem_bios_ptr = p;
	bios_info.mem_bios_size = size;
}
//获得内存的总量
unsigned int obtain_total_memsize()
{
	struct mem_bios_info * p = bios_info.mem_bios_ptr;
	unsigned int total = 0;	
	int i = 0;
	for(i=0;i < bios_info.mem_bios_size;i++)
	{
		//printk("%x~%x,type:%d\n",(p+i)->baseAddr_low,((p+i)->baseAddr_low+(p+i)->regionLen_low),(p+i)->type);
		if((p+i)->type==MEM_TYPE_1)
		{
			if((p+i)->baseAddr_low + (p+i)->regionLen_low > total)
			{
				total = (p+i)->baseAddr_low + (p+i)->regionLen_low;
			}
		}
	}
	return total;
}
//初始化页面录的操作，具体操作为把物理地址按照4k对齐后，把后边连续1024可int填充为0
unsigned int* init_page_dic_array(unsigned int phy_address)
{
	unsigned int tmp = phy_address & 0xfffff000;
	if(tmp != phy_address)
	{
		tmp = tmp + 0x1000;
	}
	memset((void *)tmp,0,4*1024);
	return (unsigned int *)tmp;
}
//初始化页面录的某一项,page_dic_address页目录的物理地址,index页目录的选项,base_address页表项的物理地址,
//present页是否在缓存中,canWrite可写,userPro是否普通用户的权限,avail
void init_page_dic_index(unsigned int * page_dic_address,int index,unsigned int * base_address,
	int present,int canWrite,
	int userPro,unsigned short avail)
{
	unsigned int tmp = (unsigned int)base_address;
	tmp &=0xfffff000;
	if(present)
	{
		tmp = tmp | 0x1;
	}
	if(canWrite)
	{
		tmp = tmp | 0x2;
	}
	if(userPro)
	{
		tmp = tmp | 0x4;
	}
	if(avail)
	{
		tmp = tmp | (avail << 9);
	}
	page_dic_address+=index;
	(*page_dic_address)=tmp;
}
//设置页面表present
void set_page_present(unsigned int * address,int index)
{
	address += index;
	unsigned int tmp = *address;
	tmp |=0x1;
	(*address) = tmp;
}
//清除页面表的present
void clear_page_present(unsigned int * address,int index)
{
	address += index;
	unsigned int tmp = *address;
	tmp &= 0xfffffffe;
	(*address) = tmp;
}
//页面是否被访问过
int is_page_accessed(unsigned int * address,int index)
{
	address += index;
	unsigned int tmp = *address;
	if(tmp & 0x2f > 0)
	{
		return 1;
	}else
	{
		return 0;
	}
}
//清除页面被访问过的痕迹
void clearn_page_accessed(unsigned int *address,int index)
{
	address += index;
	unsigned int tmp = *address;
	tmp &= 0xffffffdf;
	(*address) = tmp;
}
//读取avail的值
unsigned char read_page_avail(unsigned int *address,int index)
{
	address += index;
	unsigned int tmp = *address;
	tmp >>=9;
	tmp &=0x7;
	return (unsigned char)tmp;
}
//设置avail的值
void set_page_avail(unsigned int * address,int index,unsigned short avail)
{
	address += index;
	unsigned int tmp = *address;
	tmp &=0xfffff1ff;
	tmp |=(avail << 9);
	(*address) = tmp;
}
//初始化页面表的操作
unsigned int * init_page_table_array(unsigned int phy_address)
{
	unsigned int tmp = phy_address & 0xfffff000;
	if(tmp != phy_address)
	{
		tmp = tmp + 0x1000;
	}
	memset((void *)tmp,0,4*1024);
	return (unsigned int *)tmp;
}
//初始化页面表的某一项
void init_page_table_index(unsigned int * page_table_address,int index,unsigned int * base_address,
		int present,int canWrite,int userPro,int global,unsigned short avail)
{
	unsigned int tmp = (unsigned int)base_address;
	tmp &=0xfffff000;
	if(present)
	{
		tmp = tmp | 0x1;
	}
	if(canWrite)
	{
		tmp = tmp | 0x2;
	}
	if(userPro)
	{
		tmp = tmp | 0x4;
	}
	if(global)
	{
		tmp = tmp | 0x0100;
	}
	if(avail)
	{
		tmp = tmp | (avail << 9);
	}
	page_table_address+=index;
	(*page_table_address)=tmp;
}
//设置dirty bit
void set_page_dirty(unsigned int * address,int index)
{
	address += index;
	unsigned int tmp = *address;
	tmp |= 0x40;
	(*address) = tmp;	
}
//清空dirty bit
void clear_page_dirty(unsigned int * address,int index)
{
	address += index;
	unsigned int tmp = *address;
	tmp &= 0xbf;
	(*address) = tmp;
}
//初始化分页内存的管理
void init_page_manage()
{
	current_address = split_address.core_static_mem_address;
	//初始化页目录
	page_mem_dic = (unsigned int*)mem_malloc(4*1024);
	//初始化frame管理的内存
	unsigned int totalMemSize = obtain_total_memsize();
	unsigned int frame_size = mem_frame_max_size(totalMemSize);
	set_mem_split_address_info(&split_address,frame_size);
	//设置页目录
	unsigned int pageSize = split_address.core_dump_begin_address/ (4*1024*1024);
	unsigned int page_start_address = (unsigned int)mem_malloc(4*1024*pageSize);
	init_page_dic_array(split_address.core_static_mem_address);
	//初始化frame管理的内存
	init_mem_frame(frame_size);
	//初始化堆内存的管理
	coredump_init(4*1024*1024,split_address.core_dump_begin_address,split_address.user_address);//初始化4m的空间用来进行内核的内存的管理
	//循环设置页目录
	unsigned int actual_phy_address = 0,pageSize_counter =0;
	for(int i=0;i < pageSize;i++)
	{
		unsigned int page_table_address = page_start_address + i*4*1024;//获得页目录的地址
		init_page_dic_index((unsigned int *)page_mem_dic,i,(unsigned int *)page_table_address,1,1,0,0);
		for(int j=0;j < 1024;j++)
		{
			actual_phy_address = pageSize_counter * 4*1024;
			//把相应的帧分配出去
			init_page_table_index((unsigned int *)page_table_address,j,(unsigned int *)actual_phy_address,1,1,0,0,0);
			pgalloc();
			pageSize_counter ++;
		}
	}
	page_flush();
	/*
	unsigned int a = coredump_malloc(1024*8);
	coredump_free((void *)a);
	a = coredump_mallocpage();
	*/
}
void * mem_malloc(unsigned int size)
{
	void * tmp = (void *)current_address;
	current_address += size;
	return tmp;
}
