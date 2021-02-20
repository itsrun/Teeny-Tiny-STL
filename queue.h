#ifndef __LMSTL_QUEUE_H__
#define __LMSTL_QUEUE_H__

#include "deque.h"
#include "vector.h"
#include "heap_algo.h"

namespace lmstl {

template <typename T, typename Sequence = deque<T>>
class queue {
	friend bool operator==(const queue&, const queue&);
	friend bool operator!=(const queue&, const queue&);
	friend bool operator<(const queue&, const queue&);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;
public:
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }
	reference back() { return c.back(); }
	const_reference back() const { return c.back(); }
	void push(const value_type& val) { c.push_back(val); }
	void pop() { c.pop_front(); }
};

template <typename T, typename Sequence>
bool operator==(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
	return lhs.c == rhs.c;
}

template <typename T, typename Sequence>
bool operator!=(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
	return !(lhs.c == rhs.c);
}

template <typename T, typename Sequence>
bool operator<(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs) {
	return lhs.c < rhs.c;
}
/*
template <typename T, typename Sequence = vector<T>, typename Compare = std::less<typename Sequence::value_type>>
class priority_queue {
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;
	Compare comp;
public:
	priority_queue():
		c() {}
	explicit priority_queue(const Compare& x) :
		c(), comp(x) {}
};
*/
}

#endif // !__LMSTL_QUEUE_H__
