#ifndef __LMSTL_LIST_H__
#define __LMSTL_LIST_H__

#include "iterator.h"
#include <stddef.h>
#include "alloc.h"
#include "construct.h"
#include "type_traits.h"

namespace lmstl {

struct list_node_base{
	typedef list_node_base* base_ptr;
	base_ptr prev = NULL;
	base_ptr next = NULL;
};
template <typename T>
struct list_node : public list_node_base {
	T data;
	list_node(const T& val):
		list_node_base(), data(val) {}
};

struct list_iterator_base {
	typedef bidirectional_iterator_tag iterator_category;
	typedef list_node_base* base_ptr;
	base_ptr node;
	list_iterator_base(const base_ptr x):
		node(x) {}
};

template<typename T, typename Ref, typename Ptr>
struct list_iterator : public list_iterator_base {
	typedef T				value_type;
	typedef Ptr				pointer;
	typedef Ref				reference;
	typedef list_node<T>*	node_ptr;
	typedef list_node_base* base_ptr;
	typedef size_t			size_type;
	typedef ptrdiff_t		difference_type;

	typedef list_iterator<T, T&, T*> iterator;
	typedef list_iterator<T, const T&, const T*> const_iterator;
	typedef list_iterator self;

	list_iterator(const base_ptr x = NULL):
		list_iterator_base(x) {}
	list_iterator(const iterator& rhs) :
		list_iterator_base(rhs.node) {}
	list_iterator(const const_iterator& rhs):
		list_iterator_base(rhs.node) {}

	template <typename T, typename Ref, typename Ptr>
	friend bool operator==(const list_iterator<T, Ref, Ptr>&, const list_iterator<T, Ref, Ptr>&);
	template <typename T, typename Ref, typename Ptr>
	friend bool operator!=(const list_iterator<T, Ref, Ptr>&, const list_iterator<T, Ref, Ptr>&);
	
	reference operator*() { return ((node_ptr&)(node))->data; }
	pointer operator->() { return &operator*(); }

	self& operator++() {
		node = node->next;
		return *this;
	}
	self operator++(int) {
		self temp = *this;
		node = node->next;
		return temp;
	}
	self& operator--() {
		node = node->prev;
		return *this;
	}
	self operator--(int) {
		self temp = *this;
		node = node->prev;
		return temp;
	}

};

template <typename T, typename Ref, typename Ptr>
bool operator==(const list_iterator<T, Ref, Ptr>& l, const list_iterator<T, Ref, Ptr>& r) {
	return l.node == r.node;
}

template <typename T, typename Ref, typename Ptr>
bool operator!=(const list_iterator<T, Ref, Ptr>& l, const list_iterator<T, Ref, Ptr>& r) {
	return l.node != r.node;
}

template <typename T, typename Alloc = alloc>
class list {

public:
	typedef T										value_type;
	typedef list_node<T>*							node_ptr;
	typedef simple_alloc<list_node<T>, Alloc>		data_allocator;

	typedef size_t									size_type;
	typedef T*										pointer;
	typedef const T*								const_pointer;
	typedef T&										reference;
	typedef const T&								const_reference;
	typedef ptrdiff_t								difference_type;

	typedef list_iterator<T, T&, T*>				iterator;
	typedef list_iterator<T, const T&, const T*>	const_iterator;
	typedef reverse_iterator<const_iterator>		const_reverse_iterator;
	typedef reverse_iterator<iterator>				reverse_iterator;

protected:
	node_ptr node;
	typedef list_node_base* base_ptr;

public:
	iterator begin() {
		return node->next;
	}
	const_iterator begin() const {
		return node->next;
	}
	const_iterator cbegin() const {
		return node->next;
	}
	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(cend());
	}
	iterator end() {
		return node;
	}
	const_iterator end() const {
		return node;
	}
	const_iterator cend() const {
		return node;
	}
	reverse_iterator rend() {
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const {
		return reverse_iterator(cbegin());
	}
	bool empty() const {
		return (node == node->next);
	}
	size_type size() const {
		return distance(begin(), end());
	}
	reference front() {
		return ((node_ptr&)(node->next))->data;
	}
	const_reference front() const {
		return ((node_ptr&)(node->next))->data;
	}
	reference back() {
		iterator tmp = node;
		--tmp;
		return *tmp;
	}
	const_reference back() const {
		iterator tmp = node;
		--tmp;
		return *tmp;
	}

protected:
	node_ptr get_node() { return data_allocator::allocate(1); }
	void put_node(node_ptr x) { data_allocator::deallocate(x, 1); }

	node_ptr create_node(const T& x) {
		node_ptr p = get_node();
		construct(&p->data, x);
		return p;
	}
	void destroy_node(node_ptr p) {
		destroy(&p->data);
		put_node(p);
	}
	void init() {
		node = get_node();
		node->next = node;
		node->prev = node;
	}

public:
	list() {
		init();
	}
	list(size_type n, const T& val) {
		init();
		for (; n; --n)
			__insert(node, val);
	}
	list(const list& x) {
		init();
		size_type n = x.size();
		copy_insert(node, x.begin(), n);
	}
	list(list&& x) noexcept :
		node(x.node) {
		x.node = 0;
	}
	iterator insert(iterator position, const T& x) {
		node_ptr p = create_node(x);
		p->next = position.node;
		p->prev = position.node->prev;
		position.node->prev->next = p;
		position.node->prev = p;
		return p;
	}
	
	iterator insert(const_iterator position, const T& x) {
		node_ptr p = create_node(x);
		p->next = position.node;
		p->prev = position.node->prev;
		position.node->prev->next = p;
		position.node->prev = p;
		return p;
	}

	template <typename InputIter, typename = typename enable_if<is_input_iterator_v<InputIter>>::type>
	void insert(iterator position, InputIter beg, InputIter end){
		if (beg == end)
			return;
		size_type n = distance(beg, end);
		copy_insert((node_ptr&)position.node, beg, n);
	}

protected:
	base_ptr __insert(base_ptr pos, const T& x) {
		base_ptr p = create_node(x);
		p->next = pos;
		p->prev = pos->prev;
		pos->prev->next = p;
		pos->prev = p;
		return p;
	}
	
	template <typename InputIter>
	void copy_insert(node_ptr pos, InputIter beg, size_type n) {
		node_ptr p;
		for (; n; --n) {
			p = create_node(*beg);
			pos->prev->next = p;
			p->prev = pos->prev;
			pos->prev = p;
			p->next = pos;
			++beg;
		}
	}

public:
	void push_back(const T& x) {
		__insert(node, x);
	}
	void push_front(const T& x) {
		__insert(node->next, x);
	}
	iterator erase(iterator position) {
		base_ptr next_node = position.node->next;
		position.node->prev->next = next_node;
		next_node->prev = position.node->prev;
		destroy_node((node_ptr&)position.node);
		return next_node;
	}
	iterator erase(const_iterator position) {
		base_ptr next_node = (node_ptr&)(position.node->next);
		position.node->prev->next = next_node;
		next_node->prev = position.node->prev;
		destroy_node((node_ptr&)position.node);
		return next_node;
	}

protected:
	base_ptr __erase(base_ptr pos) {
		base_ptr next_node = pos->next;
		pos->prev->next = next_node;
		next_node->prev = pos->prev;
		destroy_node((node_ptr&)pos);
		return next_node;
	}

public:
	void pop_front() {
		__erase(node->next);
	}
	void pop_back() {
		base_ptr p = node->prev;
		__erase(p);
	}
	void clear() {
		base_ptr p = node->next;
		base_ptr tmp;
		while (p != node) {
			tmp = p;
			p = p->next;
			destroy_node((node_ptr&)tmp);
		}
		node->next = node;
		node->prev = node;
	}
	void remove(const T& val) {
		base_ptr p = node->next;
		while (p != node) {
			if (((node_ptr&)p)->data == val)
				p = __erase(p);
			else
				p = p->next;
		}
	}
	void unique() {
		base_ptr p = node->next, next = p->next;
		while (p != node) {
			if (((node_ptr&)p)->data == ((node_ptr&)next)->data)
				next = __erase(next);
			else {
				p = next;
				next = next->next;
			}
		}
	}

protected:
	void transfer(iterator position, iterator beg, iterator end) {
		beg.node->prev->next = end.node;
		base_ptr tmp = end.node->prev;
		end.node->prev = beg.node->prev;
		position.node->prev->next = beg.node;
		beg.node->prev = position.node->prev;
		position.node->prev = tmp;
		tmp->next = position.node;
	}

public:
	void splice(iterator position, list& x) {
		if (!x.empty())
			transfer(position, x.begin(), x.end());
	}
	void splice(iterator position, iterator i) {
		iterator j = i;
		j++;
		if (position == i || position == j) return;
		transfer(position, i, j);
	}
	void splice(iterator position, iterator beg, iterator end) {
		if (beg != end)
			transfer(position, beg, end);
	}
	void swap(list& x) {
		node_ptr tmp = node;
		node = x.node;
		x.node = tmp;
	}

	void merge(list<T, Alloc>& x) {
		iterator beg1 = begin();
		iterator end1 = end();
		iterator beg2 = x.begin();
		iterator end2 = x.end();
		while (beg1 != end1 && beg2 != end2) {
			if (*beg2 < *beg1) {
				iterator next = beg2;
				++next;
				transfer(beg1, beg2, next);
				beg2 = next;
			}
			else ++beg1;
		}
		if (beg2 != end2)
			transfer(end1, beg2, end2);
	}

	void reverse() {
		if (node->next == node || node->next->next == node)
			return;
		iterator beg = begin(), old;
		++beg;
		iterator last = end();
		while (beg != last) {
			old = beg;
			++beg;
			transfer(begin(), old, beg);
		}
	}

	void sort() {
		if (node->next == node || node->next->next == node)
			return;
		list<T, Alloc> carry;
		list<T, Alloc> counter[64];
		int fill = 0, i;
		while (!empty()) {
			carry.splice(carry.begin(), begin());
			i = 0;
			while (i < fill && !counter[i].empty()) {
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill) ++fill;
		}
		for (i = 1; i < fill; ++i)
			counter[i].merge(counter[i - 1]);
		swap(counter[fill - 1]);
	}
};

}
#endif // !__LMSTL_LIST_H__
