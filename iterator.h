#ifndef __LMSTL_ITERATOR_H__
#define __LMSTL_ITERATOR_H__

#include <stddef.h>

namespace lmstl {

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <typename T>
struct iterator_traits {
	typedef typename T::iterator_category	iterator_category;
	typedef typename T::value_type			value_type;
	typedef typename T::difference_type		difference_type;
	typedef typename T::pointer				pointer;
	typedef typename T::reference			reference;
};

template <typename T>
struct iterator_traits<T*> {
	typedef random_access_iterator_tag	iterator_category;
	typedef T							value_type;
	typedef ptrdiff_t					difference_type;
	typedef T*							pointer;
	typedef T&							reference;
};

template <typename T>
struct iterator_traits<const T*> {
	typedef random_access_iterator_tag	iterator_category;
	typedef T							value_type;
	typedef ptrdiff_t					difference_type;
	typedef const T*					pointer;
	typedef const T&					reference;
};

template <typename InputIterator, typename Distance>
inline void __advance(InputIterator& i, Distance d, input_iterator_tag) {
	while (d--)
		++i;
}

template <typename InputIterator, typename Distance>
inline void __advance(InputIterator& i, Distance d, bidirectional_iterator_tag) {
	if (d > 0) {
		while (d--)	++i;
	}
	else {
		while (d++) --i;
	}
}

template <typename InputIterator, typename Distance>
inline void __advance(InputIterator& i, Distance d, random_access_iterator_tag) {
	i += d;
}

template <typename InputIterator, typename Distance>
inline void advance(InputIterator& i, Distance d) {
	__advance(i, d, iterator_traits<InputIterator>::iterator_category());
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator& beg, InputIterator& end, input_iterator_tag) {
	typename iterator_traits<InputIterator>::difference_type cnt = 0;
	while (beg != end) {
		cnt++;
		beg++;
	}
	return cnt;
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator& beg, InputIterator& end, random_access_iterator_tag) {
	return (end - beg);
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator& beg, InputIterator& end) {
	return __distance(beg, end, iterator_traits<InputIterator>::iterator_category());
}

template <typename Category, typename T, typename Distance = ptrdiff_t, typename Pointer = T*, typename Reference = T&>
struct iterator {
	typedef Category	iterator_category;
	typedef T			value_type;
	typedef Distance	difference_type;
	typedef Pointer		pointer;
	typedef Reference	reference;
};

}

#endif // !__LMSTL_ITERATOR_H__
