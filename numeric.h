#ifndef __LMSTL_NUMERIC_H__
#define __LMSTL_NUMERIC_H__

#include "iterator.h"

namespace lmstl {

template <typename InputIterator, typename T>
T accumulate(InputIterator beg, InputIterator end, T init) {
	for (; beg != end; ++beg)
		init = init + *beg;
	return init;
}
/*
template <typename InputIterator, typename T>
T accumulate(InputIterator beg, InputIterator end, T init, BinaryOperation bi_op) {
	for (; beg != end; ++beg)
		init = bi_op(init, *beg);
	return init;
}
*/
template <typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator beg, InputIterator end, OutputIterator result) {
	if (beg == end)
		return result;
	*result = *beg;
	typedef typename iterator_traits<InputIterator>::value_type value_type;
	value_type cur = *beg, prev;
	while (++beg != end) {
		prev = *beg;
		*(++result) = prev - value;
		cur = prev;
	}
	return ++result;
}

template <typename InputIterator1, typename InputIterator2, typename T>
T inner_product(InputIterator1 beg1, InputIterator1 end1, InputIterator2 beg2, T init) {
	for (; beg1 != end1; ++beg1, ++beg2)
		init = init + ((*beg1) * (*beg2));
	return init;
}

template <typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator beg, InputIterator end, OutputIterator result) {
	if (beg != end)
		return result;
	*result = *beg;
	typedef iterator_traits<InputIterator>::value_type sum = *beg;
	while ((++beg) != end) {
		sum = sum + *beg;
		*(++result) = sum;
	}
	return ++result;
}

}

#endif // !__LMSTL_NUMERIC_H__
