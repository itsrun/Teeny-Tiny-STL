#ifndef __LMSTL_STACK_H__
#define __LMSTL_STACK_H__

#include "deque.h"

namespace lmstl {

template <typename T, typename Sequence = deque<T>>
class stack {
	friend bool operator==(const stack&, const stack&);
	friend bool operator!=(const stack&, const stack&);
	friend bool operator<(const stack&, const stack&);

public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;
public:
	stack():
		c(){}
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference top() { return c.back(); }
	const_reference top() const { return c.back(); }
	void push(const value_type& val) { c.push_back(val); }
	void pop() { c.pop_back(); }
};

template <typename T, typename Sequence>
bool operator==(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
	return lhs.c == rhs.c;
}

template <typename T, typename Sequence>
bool operator!=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
	return !(lhs.c == rhs.c);
}

template <typename T, typename Sequence>
bool operator<(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
	return lhs.c < rhs.c;
}

}
#endif // !__LMSTL_STACK_H__
