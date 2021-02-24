#ifndef __LMSTL_ALGO_H__
#define __LMSTL_ALGO_H__

#include "algobase.h"
#include "iterator.h"
#include "heap_algo.h"

#define __lmstl_threshold 16

namespace lmstl {

template <typename ForwardIter>
ForwardIter adjacent_find(ForwardIter beg, ForwardIter end) {
	while (beg != end) {
		if (*beg == *end)
			return beg;
		++beg;
	}
	return end;
}

template <typename ForwardIter, typename BinaryPredicate>
ForwardIter adjacent_find(ForwardIter beg, ForwardIter end, BinaryPredicate binary_pred) {
	while (beg != end) {
		if (binary_pred(*beg, *end))
			return beg;
		++beg;
	}
	return end;
}

template <typename InputIter, typename T>
typename iterator_traits<InputIter>::difference_type count(InputIter beg, InputIter end, const T& val) {
	typename iterator_traits<InputIter>::difference_type n = 0;
	for (; beg != end; ++beg)
		if (*beg == val)
			++n;
	return n;
}


template <typename InputIter, typename Predicate>
typename iterator_traits<InputIter>::difference_type count_if(InputIter beg, InputIter end, Predicate pred) {
	typename iterator_traits<InputIter>::difference_type n = 0;
	for (; beg != end; ++beg)
		if (pred(*beg))
			++n;
	return n;
}

template <typename InputIter, typename T>
InputIter find(InputIter beg, InputIter end, const T& val) {
	while (beg != end && *beg != val)
		++beg;
	return beg;
}

template <typename InputIter, typename Predicate>
InputIter find_if(InputIter beg, InputIter end, Predicate pred) {
	while (beg != end && !pred(*beg))
		++beg;
	return beg;
}
/*
template <typename ForwardIter1, typename ForwardIter2, typename = typename enable_if<
	is_forward_iterator_v<ForwardIter1> && is_forward_iterator_v<ForwardIter2>>::type>
inline ForwardIter1 find_end(ForwardIter1 beg1, ForwardIter1 end1, ForwardIter2 beg2, ForwardIter2 end2) {
	if (beg2 == end2)
		return end1;
	ForwardIter1 result = end1;
}
*/
template <typename InputIter, typename ForwardIter>
InputIter find_first_of(InputIter beg1, InputIter end1, ForwardIter beg2, ForwardIter end2) {
	for (; beg1 != end1; ++beg1)
		for (ForwardIter iter = beg2; iter != end2; ++iter)
			if (*beg1 == *iter)
				return beg1;
	return end1;
}

template <typename InputIter, typename ForwardIter, typename BinaryPredicate>
InputIter find_first_of(InputIter beg1, InputIter end1, ForwardIter beg2, ForwardIter end2, BinaryPredicate comp) {
	for (; beg1 != end1; ++beg1)
		for (ForwardIter iter = beg2; iter != end2; ++iter)
			if (comp(*beg1, *iter))
				return beg1;
	return end1;
}

template <typename InputIter, typename Function>
Function for_each(InputIter beg, InputIter end, Function f) {
	for (; beg != end; ++beg)
		f(*beg);
	return f;
}

template <typename ForwardIter, typename Generator>
void generate(ForwardIter beg, ForwardIter end, Generator gen) {
	for (; beg != end; ++beg)
		*beg = gen();
}

template <typename OutputIter, typename Size, typename Generator>
OutputIter generate(OutputIter beg, Size n, Generator gen) {
	for (; n; --n, ++beg)
		*beg = gen();
	return beg;
}

template <typename InputIter1, typename InputIter2>
bool includes(InputIter1 beg1, InputIter1 end1, InputIter2 beg2, InputIter2 end2) {
	while (beg1 != end1 && beg2 != end2) {
		if (*beg2 < *beg1)
			return false;
		else if (*beg1 < *beg2)
			++beg1;
		else {
			++beg1;
			++beg2;
		}
	}
	return beg2 == end2;
}

template <typename InputIter1, typename InputIter2, typename Compare>
bool includes(InputIter1 beg1, InputIter1 end1, InputIter2 beg2, InputIter2 end2, Compare comp) {
	while (beg1 != end1 && beg2 != end2) {
		if (comp(*beg2, *beg1))
			return false;
		else if (comp(*beg1, *beg2))
			++beg1;
		else {
			++beg1;
			++beg2;
		}
	}
	return beg2 == end2;
}
/*
template <typename BidirectIter, typename Predicate>
BidirectIter partition(BidirectIter beg, BidirectIter end, Predicate pred) {
	while (beg != end) {
		while (pred(*beg))
			++beg;
		--end;
		while (!pred(*end))
			--end;
		iter_swap(beg, end);
		++beg;
	}
	return beg;
}
*/
template <typename ForwardIter, typename T>
ForwardIter remove(ForwardIter beg, ForwardIter end, const T& val) {
	ForwardIter result;
	for (result = beg; beg != end; ++beg) {
		if (*beg == val) {
			*result = *beg;
			++result;
		}
	}
	return result;
}

template <typename RandomAccessIter>
inline void partial_sort(RandomAccessIter beg, RandomAccessIter mid, RandomAccessIter end) {
	typename iterator_traits<RandomAccessIter>::value_type value_type;
	make_heap(beg, mid);
	for (RandomAccessIter p = mid; p != end; ++p) {
		if (*p < *beg) {
			lmstl::iter_swap(p, beg);
			lmstl::__adjust_heap(beg, 0, mid - beg, *beg);
		}
	}
	heap_sort(beg, mid);
}

template <typename RandomAccessIter, typename Compare>
inline void partial_sort(RandomAccessIter beg, RandomAccessIter mid, RandomAccessIter end, Compare comp) {
	typename iterator_traits<RandomAccessIter>::value_type value_type;
	make_heap(beg, mid);
	for (RandomAccessIter p = mid; p != end; ++p) {
		if (comp(*p, *beg)) {
			iter_swap(p, beg);
			lmstl::__adjust_heap(beg, 0, mid - beg, *beg);
		}
	}
	heap_sort(beg, mid);
}

template <typename RandomAccessIter, typename T>
void __unguarded_insert(RandomAccessIter end, const T& val) {
	RandomAccessIter next = end;
	--next;
	while (val < *next) {
		*end = *next;
		end = next;
		--next;
	}
	*end = val;
}

template <typename RandomAccessIter>
void __insert_sort(RandomAccessIter beg, RandomAccessIter end) {
	typedef iterator_traits<RandomAccessIter>::value_type value_type;
	if (beg == end) return;
	value_type val;
	for (RandomAccessIter i = beg + 1; i != end; ++i) {
		val = *i;
		if (val < *beg) {
			lmstl::copy_backward(beg, i, i + 1);
			*beg = val;
		}
		else
			__unguarded_insert(i, val);
	}
}

template <typename T>
inline const T& __median(const T& a, const T& b, const T& c) {
	if (a < b)
		if (b < c)
			return b;
		else if (a < c)
			return c;
		else
			return a;
	else if (a < c)
		return a;
	else if (b < c)
		return c;
	else
		return b;
}

template <typename RandomAccessIter>
RandomAccessIter __partition(RandomAccessIter beg, RandomAccessIter end) {
	typename iterator_traits<RandomAccessIter>::value_type pivot = __median(*beg, *(beg + (end - beg) / 2), *(end - 1));
	while (1) {
		while (*beg < pivot)
			++beg;
		--end;
		while (pivot < *end)
			--end;
		if (!(beg < end))
			return beg;
		lmstl::iter_swap(beg, end);
		++beg;
	}
}

template <typename Size>
inline Size __lg(Size n) {
	Size k;
	for (k = 0; n > 1; n >>= 1)
		++k;
	return k;
}

template <typename RandomAccessIter, typename Size>
void __introsort_loop(RandomAccessIter beg, RandomAccessIter end, Size depth_limit) {
	typedef typename iterator_traits<RandomAccessIter>::value_type value_type;
	RandomAccessIter mid;
	while ((end - beg) > __lmstl_threshold) {
		if (!depth_limit) {
			lmstl::make_heap(beg, end);
			lmstl::heap_sort(beg, end);
			return;
		}
		mid = __partition(beg, end);
		--depth_limit;
		__introsort_loop(mid, end, depth_limit);
		end = mid;
	}
}

template <typename RandomAccessIter>
void __unguarded_insert_sort(RandomAccessIter beg, RandomAccessIter end) {
	typedef typename iterator_traits<RandomAccessIter>::value_type T;
	for (; beg != end; ++beg)
		__unguarded_insert(beg, T(*beg));
}

template <typename RandomAccessIter>
void __final_insert_sort(RandomAccessIter beg, RandomAccessIter end) {
	if (end - beg > __lmstl_threshold) {
		__insert_sort(beg, beg + __lmstl_threshold);
		__unguarded_insert_sort(beg + __lmstl_threshold, end);
	}
	else
		__insert_sort(beg, end);
}

template <typename RandomAccessIter>
void sort(RandomAccessIter beg, RandomAccessIter end) {
	size_t n = end - beg;
	__introsort_loop(beg, end, __lg(n)*2);
	__final_insert_sort(beg, end);
}

}

#endif // !__LMSTL_ALGO_H__
