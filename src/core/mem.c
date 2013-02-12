#include <mem.h>
#include <system.h>
#include <vedio.h>
#define KERNERL_PAGE_DIC_ADDRESS	0x20000//定义内核内存表dic起始的物理地址,2m开始
#define KERNERL_MEMSIZE                 0x200000//定义内核的内存大小,2m大小
unsigned int current_address;//当先分配地址的位置
void * mem_malloc(unsigned int size);//用来动态管理地址的类
struct mem_bios_info *mem_bios_ptr;
unsigned int mem_bios_size;
unsigned int *page_mem_dic;//页面目录的指针
//设置内存信息
void set_mem_bios_info(int size,struct mem_bios_info *p)
{
	mem_bios_ptr = p;
	mem_bios_size = size;
}
//获得内存的总量
unsigned int obtain_total_memsize()
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
//初始化页面录的操作，具体操作为把物理地址按照4k对齐后，把后边连续1024可int填充为0
unsigned int* init_page_dic_array(unsigned int phy_address)
{
	unsigned int tmp = phy_address & 0xfffff000;
	if(tmp != phy_address)
	{
		tmp = tmp + 0x1000;
	}
	memset(tmp,0,4*1024);
	return tmp;
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
	memset(tmp,0,4*1024);
	return (int *)tmp;
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
	current_address = KERNERL_PAGE_DIC_ADDRESS;
	//初始化页目录
	page_mem_dic = (unsigned int)mem_malloc(4*1024);
	init_page_dic_array(KERNERL_PAGE_DIC_ADDRESS);
	//计算需要的页表数
	unsigned int totalMemSize = obtain_total_memsize();
	unsigned int pageSize = KERNERL_MEMSIZE / (4*1024);
	//循环设置页目录
	unsigned int page_start_address = mem_malloc(4*1024*1024);
	unsigned int i,j,pageSize_counter =0;
	unsigned int actual_phy_address = 0;
	char flag = 0;
	for(i=0;i < 1024;i++)
	{
		unsigned int page_table_address = page_start_address + i*4*1024;//获得页目录的地址
		init_page_dic_index(page_mem_dic,i,page_table_address,1,1,0,0);
		//初始化page table
		init_page_table_array(page_table_address);
		for(j=0;j < 1024;j++)//循环设置页表项
		{
			//判断初始化条件
			if(pageSize_counter >= pageSize)
			{
				flag = 1;
				break;//初始化完毕，跳出循环
			}
			//初始化表项
			actual_phy_address = pageSize_counter * 4 * 1024;
			init_page_table_index(page_table_address,j,actual_phy_address,1,1,0,0,0);
			pageSize_counter ++;
		}
		if(flag)
		{
			break;
		}
	}
	//启用分页
	page_flush();
}
void * mem_malloc(unsigned int size)
{
	void * tmp = current_address;
	current_address += size;
	return tmp;
}
//分配物理页的操作
unsigned int pgalloc()
{
}
//释放物理页的操作
void pgfree(unsigned int address)
{
}
