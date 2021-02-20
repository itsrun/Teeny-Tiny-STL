#ifndef __LMSTL_CONSTRUCT_H__
#define __LMSTL_CONSTRUCT_H__

#include <new>
#include <type_traits>
#include "iterator.h"

namespace lmstl {

template <typename T1, typename T2>
inline void construct(T1* p, const T2& value) {
	new(p) T1(value);
}

template <typename T>
inline void destroy(T* pointer) {
	pointer->~T();//析构函数显式调用的表示法可用于任何标量类型名称，不必知道给定类型是否存在析构函数。
}

template <typename ForwardIterator>
inline void __destroy(ForwardIterator beg, ForwardIterator end, std::false_type) {
	for (; beg != end; beg++)
		destroy(&*beg);
}

template <typename ForwardIterator>
inline void __destroy(ForwardIterator beg, ForwardIterator end, std::true_type){}

template <typename ForwardIterator>
inline void destroy(ForwardIterator beg, ForwardIterator end) {
	typedef iterator_traits<ForwardIterator>::value_type value_type;
	__destroy(beg, end, std::is_trivially_destructible<value_type>());
}

template <>
inline void destroy(char *, char *){}

template <>
inline void destroy(wchar_t *, wchar_t *){}

}

#endif // !__LMSTL_CONSTRUCT_H__
