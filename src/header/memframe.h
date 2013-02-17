#ifndef _MEMFRAME_H
#define	_MEMFRAME_H
	//计算所占内存的大小
	extern unsigned int mem_frame_max_size(unsigned int physicsize);
	//初始化内存
	extern void * init_mem_frame(unsigned int memsize);
	//分配一个物理页的操作
	extern unsigned int pgalloc();
	//释放一个物理页的操作
	extern void pgfree(unsigned int address);
	//内存页的管理
	struct mem_frame_struct{
		unsigned char status;//frame的状态0,未被用,1被用
		unsigned int address;//物理页的起始物理地址
		//struct mem_frame_struct * nxt;//指向下一个节点
	};
	//调试用
	extern void print_mem_frame_header();
	//打印结果
	extern void print_mem_frame_struct(unsigned int index);
#endif

