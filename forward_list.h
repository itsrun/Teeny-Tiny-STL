#ifndef __LMSTL_FORWARD_LIST_H__
#define __LMSTL_FORWARD_LIST_H__

#include "alloc.h"
#include "iterator.h"
#include "construct.h"
#include <stddef.h>

namespace lmstl {

struct flist_node_base {
	flist_node_base* next;
};

template <typename T>
struct flist_node : public flist_node_base {
	T data;
};

inline flist_node_base* flist_make_link(flist_node_base* prev_node, flist_node_base* new_node) {
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	return new_node;
}

inline size_t flist_size(flist_node_base* node) {
	size_t ret = 0;
	for (; node; node = node->next)
		++ret;
	return ret;
}

struct flist_iterator_base {
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef forward_iterator_tag iterator_category;
	flist_node_base* node;
	flist_iterator_base(flist_node_base *x):
		node(x) {}
	void incr() { node = node->next; }
	bool operator==(const flist_iterator_base& x) const {
		return node == x.node;
	}
	bool operator==(const flist_iterator_base& x) const {
		return node != x.node;
	}
};

template<typename T, typename Ref, typename Ptr>
class flist_iterator : public flist_iterator_base{

public:
	typedef flist_iterator<T, T&, T*> iterator;
	typedef flist_iterator<T, const T&, const T*> const_iterator;
	typedef flist_iterator self;

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef flist_node<T> list_node;

	flist_iterator(list_node *x):
		flist_iterator_base(x) {}
	flist_iterator():
		flist_iterator_base(0) {}
	flist_iterator(const flist_iterator &x):
		flist_iterator_base(x.node) {}

	reference operator*() const { return ((list_node*)node)->data; }
	pointer operator->() const { return &(operator*()); }

	self& operator++() { incr(); return *this; }
	self operator++(int) { self temp = *this; incr(); return temp; } 
};

template <typename T, typename Alloc = alloc>
class forward_list {

public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef flist_iterator<T, T&, T*> iterator;
	typedef flist_iterator<T, const T&, const T*> const_iterator;

private:
	typedef flist_node<T> list_node;
	typedef flist_node_base list_node_base;
	typedef flist_iterator_base iterator_base;
	typedef simple_alloc<list_node> node_allocator;

	static list_node* create_node(const value_type& x) {
		list_node* node = node_allocator::allocate(1);
		try {
			construct(&*node, x);
			node->next = 0;
		}
		catch (...) {
			node_allocator::deallocate(node, 1);
		}
		return node;
	}

	static void destroy_node(list_node* node) {
		destroy(node);
		node_allocator::deallocate(node, 1);
	}

	list_node_base head;

public:
	forward_list() { head.next = 0; }
	~forward_list() { clear(); }

	iterator begin() { return iterator((list_node*)head.next); }
	iterator end() { return iterator(); }
	size_type size() const { return flist_size(head.next); }
	bool empty() const { return head.next == 0; }

	void swap(forward_list& fl) {
		list_node_base* tmp = head.next;
		head.next = fl.head.next;
		fl.head.next = tmp;
	}

	reference front() { return ((list_node*)head.next)->data; }
	
	void push_front(const value_type &val){
		list_node* new_node = create_node(val);
		new_node->next = head.next;
		head.next = new_node;
	}

	void pop_front() {
		list_node* node = (list_node *)head.next;
		head.next = head.next->next;
		destroy_node(node);
	}

	void clear() {
		list_node* p = (list_node*)head.next;
		list_node* tmp;
		while (p) {
			tmp = p;
			p = p->next;
			destroy_node(tmp);
		}
		head.next = 0;
	}

};

}
#endif // !__LMSTL_FORWARD_LIST_H__
