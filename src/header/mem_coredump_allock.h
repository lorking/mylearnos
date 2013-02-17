#ifndef _MEM_COREDUMP_ALLOCK_H
#define	_MEM_COREDUMP_ALLOCK_H
	extern void * coredump_malloc(unsigned short size);//分配内存大小
	extern void * coredump_mallocpage();//分配一页的内存的大小
	extern void  coredump_free(void * ptr);//释放堆中的内存
	extern void  coredump_init(unsigned int size,unsigned int mem_begin_addr,unsigned int mem_end_addr);//初始化内存分配的大小
	extern void  coredump_printheader();//打印header的信息,调试用
	extern void  coredump_printnodes();//打印nodes的信息,调试用
#endif
