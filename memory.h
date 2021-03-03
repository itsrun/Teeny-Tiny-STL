#ifndef __LMSTL_MEMORY_H__
#define __LMSTL_MEMORY_H__

#include "alloc.h"
#include "iterator.h"
#include "exceptdef.h"
#include "uninitialized.h"
#include <stddef.h>

namespace lmstl {

template <typename ForwardIter, typename T, typename Alloc = alloc>
class temporary_buffer {
private:
	typedef T* buf_iterator;
	ptrdiff_t len;
	buf_iterator start;

	void allocate_buffer() {
		while (len) {
			start = static_cast<T*>(malloc(len * sizeof(T)));
			if (start)
				return;
			len >>= 1;
		}
	}

	void init_buffer(std::true_type) {}
	void init_buffer(std::false_type) {
		lmstl::uninitialized_fill_n(start, len, T());
	}

public:
	temporary_buffer(ForwardIter beg, ForwardIter end) {
		len = lmstl::distance(beg, end);
		try {
			allocate_buffer();
			init_buffer(std::is_trivially_default_constructible<T>());
		}
		catch (...) {
			free(start);
			start = 0;
			len = 0;
			__THROW_BAD_ALLOC__;
		}
	}

	ptrdiff_t size() const { return len; }
	buf_iterator begin() const { return start; }
	buf_iterator end() const { return start + len; }

};
}

#endif // !__LMSTL_MEMORY_H__
