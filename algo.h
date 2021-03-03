#ifndef __LMSTL_ALGO_H__
#define __LMSTL_ALGO_H__

#include "algobase.h"
#include "iterator.h"
#include "heap_algo.h"
#include "memory.h"

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

template <typename InputIter, typename OutputIter, typename UnaryOp>
OutputIter transform(InputIter beg, InputIter end, OutputIter result, UnaryOp op) {
	for (; beg != end; ++beg, ++result)
		*result = op(*beg);
	return result;
}

template <typename InputIter1, typename InputIter2, typename OutputIter, typename BinaryOp>
OutputIter transform(InputIter1 beg1, InputIter1 end1, InputIter2 beg2, OutputIter result, BinaryOp op) {
	for (; beg1 != end1; ++beg1, ++beg2, ++result)
		*result = op(*beg1, *beg2);
	return result;
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

template <typename BidirectIter, typename Predicate>
BidirectIter partition(BidirectIter beg, BidirectIter end, Predicate pred) {
	while (1) {
		while (1) {
			if (beg == end)
				return beg;
			if (pred(*beg))
				++beg;
			else
				break;
		}
		--end;
		while (1) {
			if (beg == end)
				return beg;
			if (!pred(*end))
				--end;
			else
				break;
		}
		iter_swap(beg, end);
		++beg;
	}
}

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
			lmstl::iter_swap(p, beg);
			lmstl::__adjust_heap(beg, 0, mid - beg, *beg, comp);
		}
	}
	heap_sort(beg, mid, comp);
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

template <typename RandomAccessIter, typename T, typename Compare>
void __unguarded_insert(RandomAccessIter end, const T& val, Compare comp) {
	RandomAccessIter next = end;
	--next;
	while (comp(val, *next)) {
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

template <typename RandomAccessIter, typename Compare>
void __insert_sort(RandomAccessIter beg, RandomAccessIter end, Compare comp) {
	typedef iterator_traits<RandomAccessIter>::value_type value_type;
	if (beg == end) return;
	value_type val;
	for (RandomAccessIter i = beg + 1; i != end; ++i) {
		val = *i;
		if (comp(val, *beg)) {
			lmstl::copy_backward(beg, i, i + 1);
			*beg = val;
		}
		else
			__unguarded_insert(i, val, comp);
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

template <typename RandomAccessIter, typename Compare>
RandomAccessIter __partition(RandomAccessIter beg, RandomAccessIter end, Compare comp) {
	typename iterator_traits<RandomAccessIter>::value_type pivot = __median(*beg, *(beg + (end - beg) / 2), *(end - 1));
	while (1) {
		while (comp(*beg, pivot))
			++beg;
		--end;
		while (comp(pivot, *end))
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

template <typename RandomAccessIter, typename Size, typename Compare>
void __introsort_loop(RandomAccessIter beg, RandomAccessIter end, Size depth_limit, Compare comp) {
	typedef typename iterator_traits<RandomAccessIter>::value_type value_type;
	RandomAccessIter mid;
	while ((end - beg) > __lmstl_threshold) {
		if (!depth_limit) {
			lmstl::make_heap(beg, end, comp);
			lmstl::heap_sort(beg, end, comp);
			return;
		}
		mid = __partition(beg, end, comp);
		--depth_limit;
		__introsort_loop(mid, end, depth_limit, comp);
		end = mid;
	}
}

template <typename RandomAccessIter>
void __unguarded_insert_sort(RandomAccessIter beg, RandomAccessIter end) {
	typedef typename iterator_traits<RandomAccessIter>::value_type T;
	for (; beg != end; ++beg)
		__unguarded_insert(beg, T(*beg));
}

template <typename RandomAccessIter, typename Compare>
void __unguarded_insert_sort(RandomAccessIter beg, RandomAccessIter end, Compare comp) {
	typedef typename iterator_traits<RandomAccessIter>::value_type T;
	for (; beg != end; ++beg)
		__unguarded_insert(beg, T(*beg), comp);
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

template <typename RandomAccessIter, typename Compare>
void __final_insert_sort(RandomAccessIter beg, RandomAccessIter end, Compare comp) {
	if (end - beg > __lmstl_threshold) {
		__insert_sort(beg, beg + __lmstl_threshold, comp);
		__unguarded_insert_sort(beg + __lmstl_threshold, end, comp);
	}
	else
		__insert_sort(beg, end, comp);
}

template <typename RandomAccessIter>
void sort(RandomAccessIter beg, RandomAccessIter end) {
	size_t n = end - beg;
	__introsort_loop(beg, end, __lg(n)*2);
	__final_insert_sort(beg, end);
}

template <typename RandomAccessIter, typename Compare>
void sort(RandomAccessIter beg, RandomAccessIter end, Compare comp) {
	size_t n = end - beg;
	__introsort_loop(beg, end, __lg(n)*2, comp);
	__final_insert_sort(beg, end, comp);
}

template <typename ForwardIter, typename T>
inline ForwardIter __lower_bound(ForwardIter beg, ForwardIter end, const T& val, forward_iterator_tag) {
	typedef typename iterator_traits<ForwardIter>::difference_type difference_type;
	difference_type len = lmstl::distance(beg, end);
	difference_type half;
	ForwardIter mid;
	while (len>0) {
		half = len >> 1;
		mid = beg;
		advance(mid, half);
		if (*mid < val) {
			beg = mid;
			len = len - half - 1;
			++beg;
		}
		else
			len = half;
	}
	return beg;
}

template <typename RandomAccessIter, typename T>
inline RandomAccessIter __lower_bound(RandomAccessIter beg, RandomAccessIter end, const T& val, random_access_iterator_tag) {
	typedef typename iterator_traits<RandomAccessIter>::difference_type difference_type;
	difference_type len = end - beg;
	difference_type half;
	RandomAccessIter mid;
	while (len>0) {
		half = len >> 1;
		mid = beg + half;
		if (*mid < val) {
			beg = mid + 1;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return beg;
}

template <typename ForwardIter, typename T>
inline ForwardIter lower_bound(ForwardIter beg, ForwardIter end, const T& val) {
	typedef typename iterator_traits<ForwardIter>::iterator_category iterator_category;
	return __lower_bound(beg, end, iterator_category());
}

template <typename ForwardIter, typename T, typename Compare>
inline ForwardIter __lower_bound(ForwardIter beg, ForwardIter end, const T& val, Compare comp, forward_iterator_tag) {
	typedef typename iterator_traits<ForwardIter>::difference_type difference_type;
	difference_type len = lmstl::distance(beg, end);
	difference_type half;
	ForwardIter mid;
	while (len>0) {
		half = len >> 1;
		mid = beg;
		advance(mid, half);
		if (comp(*mid, val)) {
			beg = mid;
			len = len - half - 1;
			++beg;
		}
		else
			len = half;
	}
	return beg;
}

template <typename RandomAccessIter, typename T, typename Compare>
inline RandomAccessIter __lower_bound(RandomAccessIter beg, RandomAccessIter end, const T& val, Compare comp, random_access_iterator_tag) {
	typedef typename iterator_traits<RandomAccessIter>::difference_type difference_type;
	difference_type len = end - beg;
	difference_type half;
	RandomAccessIter mid;
	while (len>0) {
		half = len >> 1;
		mid = beg + half;
		if (comp(*mid, val)) {
			beg = mid + 1;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return beg;
}

template <typename ForwardIter, typename T, typename Compare>
inline ForwardIter lower_bound(ForwardIter beg, ForwardIter end, const T& val, Compare comp) {
	typedef typename iterator_traits<ForwardIter>::iterator_category iterator_category;
	return __lower_bound(beg, end, comp, iterator_category());
}

template <typename ForwardIter, typename T, typename Compare>
inline ForwardIter __upper_bound(ForwardIter beg, ForwardIter end, const T& val, forward_iterator_tag) {
	typedef typename iterator_traits<ForwardIter>::difference_type difference_type;
	difference_type len = lmstl::distance(beg, end);
	difference_type half;
	ForwardIter mid;
	while (len > 0) {
		half = len >> 1;
		mid = beg;
		advance(mid, half);
		if (!(val < *mid)) {
			beg = mid;
			len = len - half - 1;
			++beg;
		}
		else
			len = half;
	}
	return beg;
}

template <typename RandomAccessIter, typename T, typename Compare>
inline RandomAccessIter __upper_bound(RandomAccessIter beg, RandomAccessIter end, const T& val, random_access_iterator_tag) {
	typedef typename iterator_traits<RandomAccessIter>::difference_type difference_type;
	difference_type len = end - beg;
	difference_type half;
	RandomAccessIter mid;
	while (len > 0) {
		half = len >> 1;
		mid = beg + half;
		if (!(val < *mid)) {
			beg = mid + 1;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return beg;
}

template <typename ForwardIter, typename T, typename Compare>
inline ForwardIter upper_bound(ForwardIter beg, ForwardIter end, const T& val) {
	typedef typename iterator_traits<ForwardIter>::iterator_category iterator_category;
	return __upper_bound(beg, end, iterator_category());
}

template <typename ForwardIter, typename T, typename Compare>
inline ForwardIter __upper_bound(ForwardIter beg, ForwardIter end, const T& val, Compare comp, forward_iterator_tag) {
	typedef typename iterator_traits<ForwardIter>::difference_type difference_type;
	difference_type len = lmstl::distance(beg, end);
	difference_type half;
	ForwardIter mid;
	while (len > 0) {
		half = len >> 1;
		mid = beg;
		advance(mid, half);
		if (!comp(val, *mid)) {
			beg = mid;
			len = len - half - 1;
			++beg;
		}
		else
			len = half;
	}
	return beg;
}

template <typename RandomAccessIter, typename T, typename Compare>
inline RandomAccessIter __upper_bound(RandomAccessIter beg, RandomAccessIter end, const T& val, Compare comp, random_access_iterator_tag) {
	typedef typename iterator_traits<RandomAccessIter>::difference_type difference_type;
	difference_type len = end - beg;
	difference_type half;
	RandomAccessIter mid;
	while (len > 0) {
		half = len >> 1;
		mid = beg + half;
		if (!comp(val, *mid)) {
			beg = mid + 1;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return beg;
}

template <typename ForwardIter, typename T, typename Compare>
inline ForwardIter upper_bound(ForwardIter beg, ForwardIter end, const T& val, Compare comp) {
	typedef typename iterator_traits<ForwardIter>::iterator_category iterator_category;
	return __upper_bound(beg, end, comp, iterator_category());
}

template <typename BidirectIter>
void __reverse(BidirectIter beg, BidirectIter end, bidirectional_iterator_tag) {
	for (;;) {
		if (beg == end || beg == --end)
			return;
		else {
			iter_swap(beg, end);
			++beg;
		}
	}
}

template <typename RandomAccessIter>
void __reverse(RandomAccessIter beg, RandomAccessIter end, random_access_iterator_tag) {
	for (;beg<end; ++beg)
		iter_swap(beg, --end);
}

template <typename BidirectIter>
void reverse(BidirectIter beg, BidirectIter end) {
	typedef typename iterator_traits<BidirectIter>::iterator_category iterator_category;
	__reverse(beg, end, iterator_category());
}

template <typename BidirectIter>
bool next_permutation(BidirectIter beg, BidirectIter end) {
	if (beg == end)
		return false;
	BidirectIter i = beg, ii;
	++i;
	if (i == end)
		return false;
	i = end;
	--i;
	for (;;) {
		ii = i;
		--i;
		if (*i < *ii) {
			BidirectIter j = end;
			while (!(*i < *--j));
			iter_swap(i, j);
			reverse(ii, end);
			return true;
		}
		if (i == beg) {
			reverse(beg, end);
			return false;
		}
	}
}

template <typename BidirectIter>
bool prev_permutation(BidirectIter beg, BidirectIter end) {
	if (beg == end)
		return false;
	BidirectIter i = beg, ii;
	++i;
	if (i == end)
		return false;
	i = end;
	--i;
	for (;;) {
		ii = i;
		--i;
		if (*ii < *i) {
			BidirectIter j = end;
			while (!(*--j < *i));
			iter_swap(i, j);
			reverse(ii, end);
			return true;
		}
		if (i == beg) {
			reverse(beg, end);
			return false;
		}
	}
}

template <typename RandomAccessIterator>
inline void nth_element(RandomAccessIterator beg, RandomAccessIterator nth, RandomAccessIterator end) {
	while (end - beg > 4) {
		RandomAccessIterator piv = __partition(beg, end);
		if (piv <= nth)
			beg = piv;
		else
			end = piv;
	}
	__insert_sort(beg, end);
}

template <typename BidirectIter>
inline void inplace_merge(BidirectIter beg, BidirectIter mid, BidirectIter end) {
	if (beg == mid || mid == end)
		return;
}

}

#endif // !__LMSTL_ALGO_H__