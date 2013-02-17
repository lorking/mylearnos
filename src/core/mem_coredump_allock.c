#include <mem.h>
#include <system.h>
#include <mem_coredump_allock.h>
#include <vedio.h>
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
	unsigned int mem_end_addr;//内存终止的位置
	unsigned int lenth;
}  header;
//打印header消息
void  coredump_printheader()
{
	printk("--------------header info begin------------------------\n");
	printk("the head:%d,tail:%d,begin_addr:%d,end_addr:%d,lenth:%d\n",header.head, \
		(header.tail-header.head),header.mem_begin_addr,header.mem_end_addr,header.lenth);
	printk("--------------header info end--------------------------\n");
}
//打印nodes消息
void  coredump_printnodes()
{
	printk("--------------nodes info begin--------------------------\n");
	struct coredump_struct *p = header.head;
	for(int i=0;i < header.lenth;i++)
	{
		printk("the nodes [%d] info:flag=%d,free=%d,nxt=%d,size=%d,address=%d\n",i,p->flag,p->free,p->nxt,p->size,p->address);
		p++;
	}
	printk("--------------nodes info end----------------------------\n");
}
//查找相应地址的节点
struct coredump_struct * findAddrNode(void * addr)
{
	unsigned int n_addr = (unsigned int) addr;
	if(header.lenth==0)
		return NULL;
	struct coredump_struct * ptr = header.head;
	int n = 0;
	do{
		ptr = header.head + n;
		if(ptr -> address == n_addr)
		{
			return ptr;
		}
		n++;
	}while(ptr->nxt !=-1);
	return NULL;
}
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
	tmpPtr = header.head + n;
	if(tmpPtr->free==0)
		return;
	//把两项合并成一项
	p->nxt = tmpPtr -> nxt;
	p->size = p->size + tmpPtr->size;
	if(header.tail == tmpPtr)//设置尾部
		header.tail = p;
	//把二节点设为不可用
	tmpPtr -> flag = 0;
	//递归调用本节点
	combination(p);
}
//初始化内存区域
void  coredump_init(unsigned int size,unsigned int mem_begin_addr,unsigned int mem_end_addr)
{
	header.head = (struct coredump_struct *)mem_malloc(size);
	header.lenth = 0;
	header.mem_begin_addr = mem_begin_addr;
	header.mem_end_addr = mem_end_addr;
	header.tail = NULL;
	//memset(header.head,0,size);
}
//查找第一个复合内存大小的节点块
int findFirstSuitIndex(unsigned int size)
{
	//当长度为0时直接返回不存在
	if(header.lenth == 0)
	{
		return -1;
	}
	//循环查找比需要分配的内存大的节点,并且合并相邻都是没有used的节点
	struct coredump_struct * ptr = header.head;
	int nxt = 0;
	do{
		ptr = header.head + nxt;
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
//查找第一个复合frame大小的节点块
int findFisrtSuitFrame()
{
	struct coredump_struct * ptr = header.head;
	int nxt = 0;
	do{
		ptr = header.head + nxt;
		combination(ptr);//合并相邻的节点
		if(ptr -> free ==1)
		{
			unsigned tmp = ptr -> address & 0xfffff000;
			if(ptr -> size >= ptr->address - tmp +4*1024)
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
		if(ptr->flag==0)
		{
			return i;
		}
		ptr ++;
	}
	creatNewNode();//创建个新的节点
	return header.lenth-1;
}
//按page对齐分配方式分配内存
void * coredump_mallocpage()
{
	//先判断是否为刚开始分配,刚开始分配的话就把起始地址取整
	if(header.lenth==0)
	{
		unsigned int tmp = header.mem_begin_addr & 0xfffff000;
		if(tmp != header.mem_begin_addr)
		{
			header.mem_begin_addr = tmp + 0x1000; 
		}
		return coredump_malloc(4*1024);
	}
	struct coredump_struct * ptr = header.head;
	//查找第一个符合内存的节点
	int index = findFisrtSuitFrame();
	//节点不存在,直接创建一个新的节点
	if(index == -1)
	{
		//查找最高位置的内存
		unsigned int highAddress = findTheHighAddress();
		//判断要分配的内存是否已经超过地址上线,如果是的话分配失败
		if(highAddress + 4*1024 > header.mem_end_addr)
		{
			return NULL;
		}
		//找到一个空白的节点,赋上相应的值
		int in = findFirstUnUsedIndex();
		ptr = ptr + in;
		ptr -> flag = 1;
		ptr -> free = 0;
		ptr -> nxt = -1;
		ptr -> size = 4*1024;
		ptr -> address = highAddress;
		//设置tail及tail的下个节点
		if(header.tail == NULL)
		{
			header.tail = ptr;
		}else
		{
			header.tail->nxt = in;
			header.tail = ptr;
		}
		return (void *)highAddress;
	}
	//复用新的节点
	ptr = ptr + index;
	//如果不恰好的话,先分裂出去一个小部分
	unsigned int tmp  = ptr->address;
	unsigned int highAddress = tmp;
	if(tmp!=(tmp & 0xfffff000))
	{
		tmp += 0x1000;
		tmp = tmp - ptr->address;
		int unusedIndex = findFirstUnUsedIndex();
		struct coredump_struct * n_ptr = (header.head + unusedIndex);
		ptr -> flag = 1;
		ptr -> free = 1;
		n_ptr -> nxt = ptr-> nxt;
		ptr -> nxt = unusedIndex;
		n_ptr -> size = ptr -> size - tmp;
		ptr -> size = tmp;
		ptr -> address = highAddress;
		n_ptr -> address = (highAddress & 0xfffff000) + 0x1000;
		n_ptr -> flag = 1;
		n_ptr -> free = 1;
		if(header.tail == ptr)
		{
			header.tail = n_ptr;
		}
		ptr = n_ptr;
	}
	highAddress = ptr->address;
	if(ptr->size == 1024 * 4)
	{
		ptr -> free = 0;
		return (void *)highAddress;
	}
	//从根部开始分裂
	int unusedIndex = findFirstUnUsedIndex();
	struct coredump_struct * n_ptr = (header.head + unusedIndex);
	ptr -> flag = 1;
	ptr -> free = 0;
	n_ptr -> nxt = ptr -> nxt;
	ptr -> nxt = unusedIndex;
	n_ptr -> size = ptr -> size - 4*1024;
	ptr -> size = 4*1024;
	ptr -> address = highAddress;
	n_ptr -> address = ptr -> address + 4*1024;
	n_ptr -> flag = 1;
	n_ptr -> free = 1;
	if(header.tail == ptr)
	{
		header.tail = n_ptr;
	}
	return (void *)highAddress;
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
		//判断要分配的内存是否已经超过地址上线,如果是的话分配失败
		if(highAddress + size > header.mem_end_addr)
		{
			return NULL;
		}
		//找到一个空白的节点,赋上相应的值
		int in = findFirstUnUsedIndex();
		ptr = ptr + in;
		ptr -> flag = 1;
		ptr -> free = 0;
		ptr -> nxt = -1;
		ptr -> size = size;
		ptr -> address = highAddress;
		//设置tail及tail的下个节点
		if(header.tail == NULL)
		{
			header.tail = ptr;
		}else
		{
			header.tail->nxt = in;
			header.tail = ptr;
		}
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
//释放堆中的内存
void  coredump_free(void * ptr)
{
	struct coredump_struct * p = findAddrNode(ptr);
	if(p==NULL)
		return;
	p-> free = 1;
}

