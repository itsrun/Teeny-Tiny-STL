#ifndef __LMSTL_ALLOC_H__
#define __LMSTL_ALLOC_H__

#include <new>
#include <stdlib.h>
#include <iostream>

#define __THROW_BAD_ALLOC__ std::cerr<<"Out of Memory"<<std::endl; exit(1)

namespace lmstl {



template<int inst>		//inst后续不会用到，作用是定义多个类型相同但内容不同的类。eg.<0>和<1>是不同的类，它们的static成员不同
class malloc_alloc {	//第一类配置器
public:
	static void* allocate(size_t);

	static void* reallocate(void*, size_t);

	static void deallocate(void *, size_t);

private:
	typedef void(*voidFuncPtr)();
	static void* oom_alloc(size_t);		//out of memory时执行
	static void* oom_realloc(void *, size_t);
	static voidFuncPtr oom_handler;
	static voidFuncPtr set_new_handler(voidFuncPtr);
};



//不管元素类型，直接分配n大小的空间；确定n的大小是simple_alloc接口包装类的任务
template<int inst>
void* malloc_alloc<inst>::allocate(size_t n) {
	void* ret = malloc(n);
	if (!ret)
		ret = oom_alloc(size_t);
	return ret;
}

template<int inst>
void* malloc_alloc<inst>::reallocate(void* p, size_t n) {
	void* ret = realloc(p, n);
	if (!ret)
		ret = oom_realloc(p, n);
	return ret;
}

//直接用free删除p，不用指定分配的大小（因为是malloc分配的）
template<int inst>
void malloc_alloc<inst>::deallocate(void *p, size_t) {
	free(p);
}

//设置新的内存不足时的处理函数，返回旧的处理函数指针
template<int inst>
malloc_alloc<inst>::voidFuncPtr malloc_alloc<inst>::set_new_handler(voidFuncPtr new_handler) {
	voidFuncPtr old = oom_handler;
	oom_handler = new_handler;
	return old;
}

template<int inst>
malloc_alloc<inst>::voidFuncPtr malloc_alloc<inst>::oom_handler = 0;

template<int inst>
void* malloc_alloc<inst>::oom_alloc(size_t n) {
	void* ret = 0;
	for(;;) {
		(*oom_handler)();
		ret = malloc(n);
		if (ret)
			return ret;
	}
	__THROW_BAD_ALLOC__;
}

template<int inst>
void* malloc_alloc<inst>::oom_realloc(void* p, size_t n) {
	void* ret = 0;
	for(;;) {
		(*oom_handler)();
		ret = realloc(p, n);
		if (ret)
			return ret;
	}
	__THROW_BAD_ALLOC__;
}

static const size_t __ALIGN = 8;
static const size_t __MAX_BYTES = 512;
static const size_t __FREELISTS_NUMS = __MAX_BYTES / __ALIGN;



template<bool threads, int inst>
class pool_alloc {
public:
	void* allocate(size_t);
	
	void* reallocate(void*, size_t);

	void deallocate(void*, size_t);

private:
	static size_t ROUND_UP(size_t);
	static size_t FREELIST_INDEX(size_t);

private:
	union obj {
		union obj* free_list_link;	//下一个freelist
		char client_data[1];	//用户能看到的
	};
	static obj* volatile free_list[__FREELISTS_NUMS];

private:
	static obj* refill(size_t);
	static void* chunk_alloc(size_t, int&);
};

template<bool threads, int inst>
size_t pool_alloc<threads, inst>::ROUND_UP(size_t n) {
	return (((n)+__ALIGN - 1) & ~(__ALIGN - 1));
}

template<bool threads, int inst>
size_t pool_alloc<threads, inst>::FREELIST_INDEX(size_t n) {
	return (((n)+__ALIGN - 1) / __ALIGN - 1);
}

template<bool threads, int inst>
void* pool_alloc<threads, inst>::allocate(size_t n) {
	if (n > __MAX_BYTES)
		return malloc_alloc<inst>().allocate(n);
	obj* volatile* ptr_free_list;
	obj* result;
	ptr_free_list = free_list + FREELIST_INDEX(n);
	result = *ptr_free_list;
	if (!result) {
		void* r = refill(n);
		return r;
	}
	*ptr_free_list = result->free_list_link;
	return (result);
}

template<bool threads, int inst>
void pool_alloc<threads, inst>::deallocate(void *p, size_t n) {
	if (n > __MAX_BYTES) {
		malloc_alloc<inst>().deallocate(p, n);
		return;
	}
	obj* volatile* ptr_free_list;
	ptr_free_list = free_list + FREELIST_INDEX(n);
	p->free_list_link = *ptr_free_list;
	*ptr_free_list = p;
}

template<bool threads, int inst>
pool_alloc<threads, inst>::obj* pool_alloc<threads, inst>::refill(size_t n) {
	int nobj = 20;
	void* chunk = chunk_alloc(n, nobj);
	
	if (nobj == 1)
		return (chunk);

	obj* curr;
	obj* volatile* ptr_free_list = free_list + FREELIST_INDEX(n);

	*ptr_free_list = curr = (obj*)(chunk + n);
	for (int i = 1; i < nobj; i++) {
		curr->free_list_link = (obj*)(curr + n);
		curr = curr->free_list_link;
	}

	return (chunk);
}

template<bool threads, int inst>
void* pool_alloc<threads, inst>::reallocate(void *p, size_t n) {
}



}
#endif // !__LMSTL_ALLOC_H__
