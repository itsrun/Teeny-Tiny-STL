#ifndef __LMSTL_UNINITIALIZED_H__
#define __LMSTL_UNINITIALIZED_H__

#include "iterator.h"
#include "construct.h"
#include <type_traits>
#include <cstring>

namespace lmstl {

template <typename ForwardIterator, typename size, typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator beg, size n, const T& x) {
	typedef iterator_traits<T>::value_type value_type;
	return __uninitialized_fill_n(beg, n, x, std::is_pod<value_type>());
}

template <typename ForwardIterator, typename size, typename T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator beg, size n, const T& x, std::true_type) {
	return fill_n(beg, n, x);//未完成
}

template <typename ForwardIterator, typename size, typename T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator beg, size n, const T& x, std::false_type) {
	ForwardIterator p = beg;
	for (; n > 0; n--, p++)
		construct(&*p, x);
	return p;
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_n(InputIterator beg, InputIterator end, ForwardIterator dest, std::true_type) {
	return copy(beg, end, dest);//未完成
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_n(InputIterator beg, InputIterator end, ForwardIterator dest, std::false_type) {
	ForwardIterator p = dest;
	for (; beg != end; beg++, p++) 
		construct(&*p, *beg);
	return p;
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator beg, InputIterator end, ForwardIterator dest) {
	typedef iterator_traits<InputIterator>::value_type value_type;
	return __uninitialized_copy_nf(beg, end, dest, std::is_pod<value_type>());
}

template<>
inline char* uninitialized_copy(const char* beg, const char* end, char* dest) {
	std::memmove(dest, beg, end - beg);
	return dest + (end - beg);
}

template<>
inline wchar_t* uninitialized_copy(const wchar_t* beg, const wchar_t* end, wchar_t* dest) {
	std::memmove(dest, beg, sizeof(wchar_t) * (end - beg));
	return dest + (end - beg);
}

template <typename ForwardIterator, typename T>
inline void __uninitialized_fill(ForwardIterator beg, ForwardIterator end, const T& val, std::false_type) {
	ForwardIterator p = beg;
	for (; p != end; p++)
		construct(&*p, val);
}

template <typename ForwardIterator, typename T>
inline void __uninitialized_fill(ForwardIterator beg, ForwardIterator end, const T& val, std::true_type) {
	fill(beg, end, x);//未完成
}

template <typename ForwardIterator, typename T>
inline void uninitialized_fill(ForwardIterator beg, ForwardIterator end, const T& val) {
	typedef iterator_traits<ForwardIterator>::value_type value_type;
	__uninitialized_fill(beg, end, val, std::is_pod<value_type>());
}

}
#endif // !__LMSTL_UNINITIALIZED_H__
