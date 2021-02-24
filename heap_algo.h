#ifndef __LMSTL_HEAP_ALGO_H__
#define __LMSTL_HEAP_ALGO_H__

#include "iterator.h"

namespace lmstl {

	template <typename RandomAccessIterator>
	inline void push_heap(RandomAccessIterator beg, RandomAccessIterator end) {
		typedef iterator_traits<RandomAccessIterator>::value_type value_type;
		typedef iterator_traits<RandomAccessIterator>::difference_type difference_type;
		value_type temp = *(end - 1);
		difference_type i = (end - beg) - 1, j = (i - 1) / 2;
		while (i && *(beg + j) < temp) {
			*(end - 1) = *(beg + j);
			i = j;
			j = (j - 1) / 2;
		}
		*(beg + i) = temp;
	}

	template <typename RandomAccessIterator, typename difference_type, typename value_type>
	inline void __adjust_heap(RandomAccessIterator beg, difference_type top, difference_type len, value_type val) {
		difference_type i = top, j = 2 * i + 1;
		while (j < len) {
			if (j < (len - 1) && *(beg + j) < *(beg + (j + 1)))
				++j;
			if (*(beg + j) < val)
				break;
			*(beg + i) = *(beg + j);
			i = j;
			j = 2 * j + 1;
		}
		*(beg + i) = val;
	}
	
	template <typename RandomAccessIterator, typename difference_type, typename value_type, typename Compare>
	inline void __adjust_heap(RandomAccessIterator beg, difference_type top, 
		difference_type len, value_type val, Compare comp) {
		difference_type i = top, j = 2 * i + 1;
		while (j < len) {
			if (j < (len - 1) && comp(*(beg + j), *(beg + (j + 1))))
				++j;
			if (comp(*(beg + j), val))
				break;
			*(beg + i) = *(beg + j);
			i = j;
			j = 2 * j + 1;
		}
		*(beg + i) = val;
	}

	template <typename RandomAccessIterator>
	inline void pop_heap(RandomAccessIterator beg, RandomAccessIterator end) {
		typedef iterator_traits<RandomAccessIterator>::value_type value_type;
		typedef iterator_traits<RandomAccessIterator>::difference_type difference_type;
		value_type temp = *(end - 1);
		*(end - 1) = *beg;
		difference_type len = (end - beg) - 1;
		__adjust_heap(beg, 0, len, temp);
	}
	
	template <typename RandomAccessIterator, typename Compare>
	inline void pop_heap(RandomAccessIterator beg, RandomAccessIterator end, Compare comp) {
		typedef iterator_traits<RandomAccessIterator>::value_type value_type;
		typedef iterator_traits<RandomAccessIterator>::difference_type difference_type;
		value_type temp = *(end - 1);
		*(end - 1) = *beg;
		difference_type len = (end - beg) - 1;
		__adjust_heap(beg, 0, len, temp, comp);
	}

	template <typename RandomAccessIterator>
	inline void heap_sort(RandomAccessIterator beg, RandomAccessIterator end) {
		while (end - beg > 1) {
			lmstl::pop_heap(beg, end);
			--end;
		}
	}
	

	template <typename RandomAccessIterator, typename Compare>
	inline void heap_sort(RandomAccessIterator beg, RandomAccessIterator end, Compare comp) {
		while (end - beg > 1) {
			lmstl::pop_heap(beg, end, comp);
			--end;
		}
	}

	template <typename RandomAccessIterator>
	inline void make_heap(RandomAccessIterator beg, RandomAccessIterator end) {
		typedef iterator_traits<RandomAccessIterator>::value_type value_type;
		typedef iterator_traits<RandomAccessIterator>::difference_type difference_type;
		difference_type len = end - beg;
		if (len < 2)
			return;
		difference_type parent = (len - 2) / 2;
		while (parent >= 0) {
			__adjust_heap(beg, parent, len, *(beg + parent));
			--parent;
		}
	}
	
	template <typename RandomAccessIterator, typename Compare>
	inline void make_heap(RandomAccessIterator beg, RandomAccessIterator end, Compare comp) {
		typedef iterator_traits<RandomAccessIterator>::value_type value_type;
		typedef iterator_traits<RandomAccessIterator>::difference_type difference_type;
		difference_type len = end - beg;
		if (len < 2)
			return;
		difference_type parent = (len - 2) / 2;
		while (parent >= 0) {
			__adjust_heap(beg, parent, len, *(beg + parent), comp);
			--parent;
		}
	}

}

#endif // !__LMSTL__HEAP_ALGO_H__
