#ifndef _MEMFRAME_H
#define	_MEMFRAME_H
	//内存页的管理
	struct mem_page_struct{
		unsigned char isValid;//是否有效的标志位
		unsigned char status;//fram的状态
		unsigned int address;//物理页的起始物理地址
		struct mem_page_struct * nxt;//指向下一个节点
	};
	//内存页的header类
	struct mem_page_struct_head{
		struct mem_page_struct *header;//指向页面的指针
		unsigned short size;//列表的长度
	};
	//计算所占内存的大小
	unsigned short mem_page_max_size(unsigned int physicsize);
	//初始化内存
	void * init_mem_page(unsigned short memsize);
	//分配一个物理页的操作
	extern unsigned int pgalloc();
	//释放一个物理页的操作
#endif

