#ifndef _MEM_COREDUMP_ALLOCK_H
#define	_MEM_COREDUMP_ALLOCK_H
	extern void * coredump_malloc(unsigned short size);//分配内存大小
	extern void * coredump_free(void * ptr);//释放堆中的内存
	extern void  coredump_init(unsigned int size,unsigned int mem_begin_addr);//初始化内存分配的大小
#endif
