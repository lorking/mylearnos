#ifndef _MEM_H
#define	_MEM_H
	#define MEM_TYPE_1	1//Usable (normal) RAM
	#define	MEM_TYPE_2	2//Reserved - unusable
	#define	MEM_TYPE_3	3//ACPI reclaimable memory
	#define	MEM_TYPE_4	4//ACPI NVS memory
	#define	MEM_TYPE_5	5//Area containing bad memory
	struct mem_bios_info
	{
		unsigned int baseAddr_low;//基础地址low
		unsigned int baseAddr_high;//基础地址high
		unsigned int regionLen_low;//长度低地址
		unsigned int regionLen_high;//长度的高地址
		unsigned int type;//类型
	};
	extern struct mem_bios_info *mem_bios_ptr;
	extern unsigned int mem_bios_size;
	//extern int total_size;//总内存的总数
	extern void set_mem_bios_info(int size,struct mem_bios_info *p);
	//获得内存的总数
	extern unsigned int obtain_total_memsize();
	//初始化页面录的操作
	extern unsigned int* init_page_dic_array(unsigned int phy_address);
	//初始化页面录的某一项,page_dic_address页目录的物理地址,index页目录的选项,base_address页表项的物理地址,
	//present页是否在缓存中,canWrite可写,userPro是否普通用户的权限,avail
	extern void init_page_dic_index(unsigned int * page_dic_address,int index,unsigned int * base_address,
		int present,int canWrite,
		int userPro,unsigned short avail);
	//设置页面表present
	extern void set_page_present(unsigned int * address,int index);
	//清除页面表的present
	extern void clear_page_present(unsigned int * address,int index);
	//页面是否被访问过
	extern int is_page_accessed(unsigned int * address,int index);
	//清除页面被访问过的痕迹
	extern void clearn_page_accessed(unsigned int *address,int index);
	//读取avail的值
	extern unsigned char read_page_avail(unsigned int *address,int index);
	//设置avail的值
	extern void set_page_avail(unsigned int * address,int index,unsigned short avail);
	//初始化页面表的操作
	extern unsigned int * init_page_table_array(unsigned int phy_address);
	//初始化页面表的某一项
	extern void init_page_table_index(unsigned int * page_table_address,int index,unsigned int * base_address,
		int present,int canWrite,int userPro,int global,unsigned short avil);
	//设置dirty bit
	extern void set_page_dirty(unsigned int * address,int index);
	//查看dirty bit是否被设置
	extern void clear_page_dirty(unsigned int * address,int index);
	//初始化分页内存的管理
	extern void init_page_manage();
	extern void page_flush();
#endif
