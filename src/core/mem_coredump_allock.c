#include <mem.h>
#include <system.h>
#include <mem_coredump_allock.h>
struct coredump_struct{
	unsigned char	flag;//标志位,用来表明是否被使用
	unsigned char	free;//标志位，用来表明这块内存是否被使用
	int	nxt;//下一个链接的图标
	unsigned short size;//分配内存的大小
	unsigned int address;//指向的地址
};
//用来保存相关信息
struct coredump_struct_header{
	struct coredump_struct *head;//表头
	struct coredump_struct *tail;//表尾
	unsigned int mem_begin_addr;//内存起始的位置
	unsigned int lenth;
}  header;
//组合相邻的节点
void combination(struct coredump_struct *p)
{
	struct coredump_struct *tmpPtr = NULL;
	if(p->flag == 0)
		return;
	if(p->free == 0)//正在被使用
		return;
	int n = p->nxt;
	if(n==-1)
		return;
	if(tmpPtr->free==0)
		return;
	//把两项合并成一项
	p->nxt = tmpPtr -> nxt;
	p->size = p->size + tmpPtr->nxt;
	if(header.tail = tmpPtr)//设置尾部
		header.tail = p;
	//把二节点设为不可用
	tmpPtr -> flag = 0;
	//递归调用本节点
	combination(p);
}
//初始化内存区域
void  coredump_init(unsigned int size,unsigned int mem_begin_addr)
{
	header.head = (struct coredump_struct *)mem_malloc(size);
	header.lenth = 0;
	header.mem_begin_addr = mem_begin_addr;
	header.tail = NULL;
	memset(header.head,0,size);
}
//查找第一个复合内存大小的节点块
int findFirstSuitIndex(unsigned int size)
{
	//当长度为0时直接返回不存在
	if(header.lenth = 0)
	{
		return -1;
	}
	//循环查找比需要分配的内存大的节点,并且合并相邻都是没有used的节点
	struct coredump_struct * ptr = header.head;
	int nxt = 0;
	do{
		ptr = ptr + nxt;
		combination(ptr);//合并相邻的节点
		if(ptr -> free ==1)
		{
			if(ptr -> size >= size)
			{
				return ptr - header.head;
			}
		}
		nxt = ptr -> nxt;
	}while(nxt != -1);
	return -1;
}
//查找最高的内存的地址
unsigned int findTheHighAddress()
{
	if(header.lenth==0)
	{
		return header.mem_begin_addr;
	}
	struct coredump_struct * ptr = header.tail;
	return ptr->address + ptr ->size;
}
//创建一个新的节点
void creatNewNode()
{
	struct coredump_struct * ptr = header.head;
	ptr += header.lenth;
	ptr -> flag = 0;
	header.lenth ++;
}
//查找第一个没有用的下标
unsigned int findFirstUnUsedIndex()
{
	struct coredump_struct * ptr = header.head;
	for(int i = 0;i < header.lenth;i++)
	{
		if(ptr->flag==1)
		{
			return i;
		}
		ptr ++;
	}
	creatNewNode();//创建个新的节点
	return header.lenth-1;
}
//分配内存
void * coredump_malloc(unsigned short size)
{
	struct coredump_struct * ptr = header.head;
	//查找第一个复合内存的节点
	int index = findFirstSuitIndex(size);
	//节点不存在,直接创建一个新的节点
	if(index == -1)
	{
		//查找最高位置的内存
		unsigned int highAddress = findTheHighAddress();
		//找到一个空白的节点,赋上相应的值
		int in = findFirstUnUsedIndex();
		ptr = ptr + in;
		ptr -> flag = 1;
		ptr -> free = 0;
		ptr -> nxt = -1;
		ptr -> size = size;
		header.tail = ptr;
		return (void *)highAddress;
	}else//节点存在复用新的节点
	{
		ptr = ptr + index;
		unsigned int highAddress = ptr -> address;
		if(ptr->size == size)//内存恰好相等,直接返回
		{
			ptr -> free = 0;
			return (void *)highAddress;
		}
		//节点不相等，进行分裂的操作
		int unusedIndex = findFirstUnUsedIndex();
		struct coredump_struct * n_ptr = (header.head + unusedIndex);
		ptr -> flag = 1;
		ptr -> free = 0;
		n_ptr -> nxt = ptr -> nxt;
		ptr -> nxt = unusedIndex;
		n_ptr -> size = ptr -> size - size;
		ptr -> size = size;
		ptr -> address = highAddress;
		n_ptr -> address = ptr -> address + size;
		n_ptr -> flag = 1;
		n_ptr -> free = 1;
		if(header.tail == ptr)
		{
			header.tail = n_ptr;
		}
		return (void *)highAddress;
	}
}

