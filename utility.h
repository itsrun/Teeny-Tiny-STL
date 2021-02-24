#ifndef __LMSTL_UTILITY_H__
#define __LMSTL_UTILITY_H__

#include "type_traits.h"
#include <type_traits>

namespace lmstl {

template <typename T1, typename T2>
struct pair {
	typedef T1 first_type;
	typedef T2 second_type;

	first_type first;
	second_type second;

	template <typename = typename enable_if<
		std::is_default_constructible<T1>::value &&
		std::is_default_constructible<T2>::value , bool>::type>
	pair():
		first(), second() {}

	template <typename U1, typename U2, typename = typename enable_if<
		std::is_convertible<const U1&, T1>::value && 
		std::is_convertible<const U2&, T2>::value, bool>::type>
	pair(const U1& uf, const U2& us):
		first(uf), second(us) {}

	template <typename U1, typename U2, typename = typename enable_if<
		std::is_convertible<const U1&, T1>::value && 
		std::is_convertible<const U2&, T2>::value, bool>::type>
	pair(const pair<U1, U2>& x):
		first(x.first), second(x.second) {}

	pair(const pair&) = default;
	pair(pair&&) = default;

	inline pair& operator=(const pair& rhs)
	{
		if (this != &rhs)
		{
			first = rhs.first;
			second = rhs.second;
		}
		return *this;
	}

	template <typename U1, typename U2, typename = typename enable_if<
		std::is_convertible<const U1&, T1>::value && 
		std::is_convertible<const U2&, T2>::value, bool>::type>
	inline pair& operator=(const pair<U1, U2>& x) {
		first = x.first;
		second = x.second;
		return *this;
	}

};

}
#endif // !__LMSTL_UTILITY_H__
