#ifndef __LMSTL_ALLOC_H__
#define __LMSTL_ALLOC_H__

/*
描述：
当分配空间小于512字节时，使用内存池pool_alloc
当分配空间超过512字节时，使用malloc_alloc

调用deallocate后内存只是回收到free list中，并没有真正释放，即pool_size只会增大不会减小...但侯捷的书中并没有讲deallocate后内存池的内存应该如何释放，
因此这里也只实现了书上讲到的部分（pool_alloc的reallocate同理）

性能：
在我的电脑上pool_alloc比标准库allocator快一倍
*/

#include <new>
#include <iostream>

#define __THROW_BAD_ALLOC__ std::cerr<<"Out of Memory"<<std::endl; exit(1)

namespace lmstl {

	class malloc_alloc {
	public:
		static void* allocate(size_t);
		static void* reallocate(void*, size_t);
		static void deallocate(void*, size_t);
		static void (*set_new_handler(void (*)()))();
	private:
		static void (*oom_handler)();
		static void* oom_alloc(size_t);
		static void* oom_realloc(void*, size_t);
	};

	void (*malloc_alloc::oom_handler)() = 0;

	void* malloc_alloc::allocate(size_t n) {
		void* ret = malloc(n);
		if (!ret) 
			ret = oom_alloc(n);
		return ret;
	}

	void* malloc_alloc::reallocate(void* p, size_t n) {
		void* ret = realloc(p, n);
		if (!ret)
			ret = oom_realloc(p, n);
		return ret;
	}

	void malloc_alloc::deallocate(void* p, size_t n) {
		free(p);
	}

	void (*malloc_alloc::set_new_handler(void (*f)()))() {
		void (*old)() = oom_handler;
		oom_handler = f;
		return old;
	}

	void* malloc_alloc::oom_alloc(size_t n) {
		void* ret;
		for (;;) {
			if (!oom_handler)
				__THROW_BAD_ALLOC__;
			(*oom_handler)();
			ret = malloc(n);
			if (ret)
				return ret;
		}
	}

	void* malloc_alloc::oom_realloc(void* p, size_t n) {
		void* ret;
		for (;;) {
			if (!oom_handler)
				__THROW_BAD_ALLOC__;
			(*oom_handler)();
			ret = realloc(p, n);
			if (ret)
				return ret;
		}
	}

	static const size_t __ALIGN = 8;
	static const size_t __MAX_BYTES = 512;
	static const size_t __FREELISTS_NUMS = __MAX_BYTES / __ALIGN;

	union obj {
		union obj* next_free_list;	//下一个freelist
		char client_data[1];	//用户能看到的
	};

	class pool_alloc {
	public:
		static void* allocate(size_t);

		//void* reallocate(void*, size_t);

		static void deallocate(void*, size_t);

	private:
		static size_t ROUND_UP(size_t);
		static size_t FREELIST_INDEX(size_t);

	public:
		static obj* free_list[__FREELISTS_NUMS];
		static char* start_pos;
		static char* end_pos;
		static size_t pool_size;

	private:
		static void* refill(size_t);
		static char* chunk_alloc(size_t, int&);
	};

	size_t pool_alloc::pool_size = 0;

	char* pool_alloc::start_pos = 0;

	char* pool_alloc::end_pos = 0;

	obj* pool_alloc::free_list[__FREELISTS_NUMS] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
	};

	size_t pool_alloc::ROUND_UP(size_t n) {
		return (((n)+__ALIGN - 1) & ~(__ALIGN - 1));
	}

	size_t pool_alloc::FREELIST_INDEX(size_t n) {
		return (((n)+__ALIGN - 1) / __ALIGN - 1);
	}

	void* pool_alloc::allocate(size_t n) {
		if (n > __MAX_BYTES) 
			return malloc_alloc::allocate(n);

		obj** ptr_free_list;
		obj* result;
		ptr_free_list = free_list + FREELIST_INDEX(n);
		result = *ptr_free_list;

		if (!result) {
			void* r = refill(ROUND_UP(n));
			return r;
		}
		*ptr_free_list = result->next_free_list;

		return (result);
	}

	void pool_alloc::deallocate(void* p, size_t n) {
		if (n > __MAX_BYTES) {
			malloc_alloc::deallocate(p, n);
			return;
		}

		obj* q = (obj*)p;
		obj** ptr_free_list = free_list + FREELIST_INDEX(n);
		q->next_free_list = *ptr_free_list;
		*ptr_free_list = q;
	}

	void* pool_alloc::refill(size_t n) {
		int nobj = 10;
		char* chunk = chunk_alloc(n, nobj);

		if (nobj == 1)
			return (chunk);

		obj* curr;
		obj** ptr_free_list = free_list + FREELIST_INDEX(n);

		*ptr_free_list = curr = (obj*)(chunk + n);
		for (int i = 1; i < nobj - 1; i++) {
			curr->next_free_list = (obj*)((char*)curr + n);
			curr = curr->next_free_list;
		}
		curr->next_free_list = 0;
		return (chunk);
	}

	char* pool_alloc::chunk_alloc(size_t n, int& nobj) {
		size_t pool_left = end_pos - start_pos;
		char* ret;
		if (pool_left >= n * nobj) {
			ret = start_pos;
			start_pos += n * nobj;
			return ret;
		}
		else if (pool_left >= n) {
			nobj = pool_left / n;
			ret = start_pos;
			start_pos += n * nobj;
			return ret;
		}
		else {
			size_t bytes_needed = 2 * n * nobj + ROUND_UP(pool_size >> 4);
			if (pool_left > 0) {
				obj** ptr_free_list = free_list + FREELIST_INDEX(pool_left);
				((obj*)start_pos)->next_free_list = *ptr_free_list;
				*ptr_free_list = (obj*)start_pos;
			}
			start_pos = (char*)malloc(bytes_needed);
			if (!start_pos) {
				obj** ptr_free_list;
				obj* p;
				for (int i = n; i < __MAX_BYTES; i += __ALIGN){//注意此处是从n开始，这样才能把所有空余块整合到一起；否则陷入死循环
					ptr_free_list = free_list + FREELIST_INDEX(i);
					p = *ptr_free_list;
					if (p) {
						*ptr_free_list = p->next_free_list;
						start_pos = (char*)p;
						end_pos = start_pos + i;
						return (chunk_alloc(n, nobj));
					}
				}
				end_pos = 0;
				__THROW_BAD_ALLOC__;
			}
			pool_size += bytes_needed;
			end_pos = start_pos + bytes_needed;
			return chunk_alloc(n, nobj);
		}
	}

	typedef pool_alloc alloc;

	template<typename T, typename Alloc = alloc>
	class simple_alloc {
	public:
		static T* allocate(size_t n) {
			return n ? (T*)Alloc::allocate(n * sizeof(T)) : 0;
		}
		static void deallocate(T* p, size_t n) {
			Alloc::deallocate(p, n * sizeof(T));
		}
	};

}
#endif // !__LMSTL_ALLOC_H__
