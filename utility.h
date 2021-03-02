#ifndef __LMSTL_UTILITY_H__
#define __LMSTL_UTILITY_H__

#include "type_traits.h"
#include <type_traits>

namespace lmstl {

template <typename T>
constexpr std::remove_reference_t<T>&& move(T&& arg) noexcept {
	return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

template <typename T>//利用引用坍缩
constexpr T&& forward(std::remove_reference_t<T>& t) noexcept {
	return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& forward(std::remove_reference_t<T>&& t) noexcept {
	return static_cast<T&&>(t);
}

template <typename T>
inline void swap(T& a, T& b) {
	T tmp(lmstl::move(a));
	a = lmstl::move(b);
	b = lmstl::move(tmp);
}

template <typename T1, typename T2>
struct pair {
	typedef T1 first_type;
	typedef T2 second_type;

	first_type first;
	second_type second;

	template <typename U1 = T1, typename U2 = T2, typename = typename enable_if <
		std::is_default_constructible_v<U1> && std::is_default_constructible_v<U2>>::type>
	constexpr pair():
		first(), second() {}
	
	template <typename U1 = T1, typename U2 = T2, typename = typename enable_if<
		std::is_constructible_v<T1, const U1&> && std::is_constructible_v<T2, const U2&>>::type>
	constexpr pair(const U1& uf, const U2& us):
		first(uf), second(us) {}
	
	template <typename U1 = T1, typename U2 = T2, typename = typename enable_if<
		std::is_constructible_v<T1, U1&&> && std::is_constructible_v<T2, U2&&>>::type>
	constexpr pair(U1&& uf, U2&& us):
		first(lmstl::forward<U1>(uf)), second(lmstl::forward<U2>(us)) {}
	
	template <typename U1 = T1, typename U2 = T2, typename = typename enable_if<
		std::is_constructible_v<T1, const U1&> && std::is_constructible_v<T2, const U2&>>::type>
	constexpr pair(const pair<U1, U2>& x):
		first(x.first), second(x.second) {}
	
	template <typename U1 = T1, typename U2 = T2, typename = typename enable_if<
		std::is_constructible_v<T1, U1&&> && std::is_constructible_v<T2, U2&&>>::type>
	constexpr pair(pair<U1, U2>&& x):
		first(lmstl::forward<U1>(x.first)), second(lmstl::forward<U2>(x.second)) {}

	template <typename U1 = T1, typename U2 = T2, typename = typename enable_if<
		std::is_convertible_v<const U1&, T1> && std::is_convertible_v<const U2&, T2>>::type>
	pair& operator=(const pair<U1, U2>& x) {
		first = x.first;
		second = x.second;
		return *this;
	}

	template <typename U1 = T1, typename U2 = T2, typename = typename enable_if<
		std::is_convertible_v<U1&&, T1>&& std::is_convertible_v<U2&&, T2>>::type>
	pair& operator=(pair<U1, U2>&& x) {
		first = lmstl::forward<U1>(x.first);
		second = lmstl::forward<U2>(x.second);
		return *this;
	}

	~pair() = default;

	void swap(pair& x) {
		if (&x != this) {
			lmstl::swap(first, x.first);
			lmstl::swap(second, x.second);
		}
	}

	void swap(pair&& x) {
		first = lmstl::move(x.first);
		second = lmstl::move(x.second);
	}
};

}
#endif // !__LMSTL_UTILITY_H__
