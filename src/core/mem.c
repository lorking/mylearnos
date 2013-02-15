#include <mem.h>
#include <system.h>
#include <vedio.h>
#include <mem_coredump_allock.h>
unsigned int current_address;//当先分配地址的位置
unsigned int *page_mem_dic;//页面目录的指针
//保存页表的一些信息
struct bit_map_info_struct{
	char *bit_map_ptr;//用来分配frame的bitmap的指针,内存的大小
	unsigned int bit_map_size;//分配指针的大小
	unsigned int frame_count;//frame的数量
} bit_mapinfo;
//保存bios的一些信息
struct mem_bios_info_struct bios_info;
//设置bit位
void set_bit_map(unsigned int index)
{
	if(index >= bit_mapinfo.frame_count)
	{
		return;
	}
	unsigned int a_index = index / 8;
	unsigned int bit_index = index % 8;
	char * tmpPtr = bit_mapinfo.bit_map_ptr + a_index;
	unsigned char tmpChar = 0x1;
	tmpChar <<=bit_index;
	(*tmpPtr) = (*tmpPtr) | tmpChar;
}
//清空bit位
void clear_bit_map(unsigned int index)
{
	if(index >= bit_mapinfo.frame_count)
	{
		return;
	}
	unsigned int a_index = index / 8;
	unsigned int bit_index = index % 8;
	char * tmpPtr = bit_mapinfo.bit_map_ptr + a_index;
	unsigned char tmpChar = 0x1;
	tmpChar <<=bit_index;
	tmpChar = ~tmpChar;
	(*tmpPtr) = (*tmpPtr) & tmpChar;
}
//获得第一个为0的frame
unsigned int get_bit_map_first0()
{
	int i;
	char * tmpPtr = bit_mapinfo.bit_map_ptr;
	for(i=0;i < bit_mapinfo.bit_map_size;i++)
	{
		unsigned char tmp = 0x1;
		int j;
		for(j=0;j < 8;j++)
		{
			if( (tmp & (*tmpPtr))==0 )//此位为0
			{
				unsigned int ret = i*8 + j;
				if(ret > bit_mapinfo.frame_count)
				{
					return -1;
				}else
				{
					return ret;
				}
			}
			tmp =tmp<<1;
		}
		tmpPtr ++;
	}
	return -1;
}
//清空所有bitmap,并把core所占的物理frame设置为非可应用的
void init_bit_map()
{
	memset(bit_mapinfo.bit_map_ptr,0,bit_mapinfo.bit_map_size);
	unsigned int coreSize = KERNERL_DUMP_MEMBEGIN/(4*1024);
	int i;
	for(i=0;i < coreSize;i++)
	{
		set_bit_map(i);
	}
	/*
	i = get_bit_map_first0();
	printk("wwwwwwwww:%d\n",i);
	unsigned int * p = pgalloc();
	i = get_bit_map_first0();
	printk("wwwwwwwww:%d\n",i);
	pgalloc();
	i = get_bit_map_first0();
	printk("wwwwwwwww:%d\n",i);
	pgfree(p);
	i = get_bit_map_first0();
	printk("wwwwwwwww:%d\n",i);
	*/
}
//分配物理页的操作
unsigned int * pgalloc()
{
	unsigned int tmp = get_bit_map_first0();
	if(tmp == -1)
	{
		return -1;
	}
	set_bit_map(tmp);//置位物理页的操作
	return tmp*4*1024;
}
//获得pagedic某一项的指针
unsigned int * get_dic_ptr(unsigned int *ptr,unsigned int address)
{
	address = address / (1024 * 4) / (1024 * 4);
	return (ptr + address);
}
//释放物理页的操作
void pgfree(unsigned int *address)
{
	unsigned int tmp = (unsigned int)address;
	tmp = tmp / (4 * 1024);
	clear_bit_map(tmp);
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
		printk("%x~%x,type:%d\n",(p+i)->baseAddr_low,((p+i)->baseAddr_low+(p+i)->regionLen_low),(p+i)->type);
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
	//计算实际物理内存的frame数
	unsigned int totalMemSize = obtain_total_memsize();
	bit_mapinfo.frame_count = totalMemSize / (4 * 1024);
	bit_mapinfo.bit_map_size = bit_mapinfo.frame_count / 8;
	if(bit_mapinfo.frame_count % 8 ==0)
	{
		bit_mapinfo.bit_map_size ++;
	}
	//分配指向bitmap的指针
	bit_mapinfo.bit_map_ptr = mem_malloc(bit_mapinfo.bit_map_size);
	//mem_malloc(4*1024*1024);
	init_bit_map();//初始化bitmap
	unsigned int pageSize = KERNERL_DUMP_MEMBEGIN/ (4*1024);
	coredump_init(4*1024,0,1024);
	/*
	void * ptr = coredump_malloc(10);
	void *ptr1 = coredump_malloc(12);
	void *ptr2 = coredump_malloc(5);
	coredump_free(ptr2);
	ptr2 = coredump_malloc(4);
	coredump_free(ptr);
	coredump_free(ptr1);
	ptr = coredump_malloc(23);
	*/
	//循环设置页目录
	/*
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
	*/
}
void * mem_malloc(unsigned int size)
{
	void * tmp = (void *)current_address;
	current_address += size;
	return tmp;
}
//给定地址是否在保留的地址内
int isAddressReserve(unsigned int address,struct mem_bios_info_struct *ptr)
{
	struct mem_bios_info *p = ptr->mem_bios_ptr;
	unsigned int size = ptr -> mem_bios_size;
	for(int i=0;i < size;i++)
	{
		//if(address > p->)
	}
	return -1;
}
