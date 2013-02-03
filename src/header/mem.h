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
#endif
