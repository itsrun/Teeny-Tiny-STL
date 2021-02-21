#ifndef __LMSTL_FORWARD_LIST_H__
#define __LMSTL_FORWARD_LIST_H__

#include "alloc.h"
#include "iterator.h"
#include "construct.h"
#include <stddef.h>

namespace lmstl {

struct flist_node_base {
	typedef flist_node_base* base_ptr;
	base_ptr next;
	flist_node_base(const base_ptr& x = 0):
		next(x) {}
};

template <typename T>
struct flist_node : public flist_node_base {
	T data;
	flist_node(const T& x):
		flist_node_base(), data(x) {}
};

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
	typedef flist_node_base* base_ptr;
	base_ptr node;
	flist_iterator_base(const base_ptr& x):
		node(x) {}
	void incr() { node = node->next; }
	bool operator==(const flist_iterator_base& x) const {
		return node == x.node;
	}
	bool operator!=(const flist_iterator_base& x) const {
		return node != x.node;
	}
};

template<typename T, typename Ref, typename Ptr>
class flist_iterator : public flist_iterator_base {

public:
	typedef flist_iterator<T, T&, T*> iterator;
	typedef flist_iterator<T, const T&, const T*> const_iterator;
	typedef flist_iterator self;

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef flist_node<T> list_node;
	typedef flist_node_base* base_node;

	flist_iterator():
		flist_iterator_base(0) {}
	flist_iterator(const base_node& x) :
		flist_iterator_base(x) {}
	flist_iterator(const iterator &x):
		flist_iterator_base(x.node) {}
	flist_iterator(const const_iterator &x):
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
	typedef list_node* node_ptr;
	typedef list_node_base* base_ptr;
	typedef simple_alloc<list_node> node_allocator;

	static node_ptr create_node(const value_type& x) {
		node_ptr node = node_allocator::allocate(1);
		try {
			construct(&*node, x);
			node->next = 0;
		}
		catch (...) {
			node_allocator::deallocate(node, 1);
			__THROW_BAD_ALLOC__;
		}
		return node;
	}

	static void destroy_node(node_ptr node) {
		destroy(node);
		node_allocator::deallocate(node, 1);
	}

	flist_node_base head;

public:
	forward_list() { head.next = 0; }
	forward_list(const forward_list& x) {
		head.next = 0;
		base_ptr node = &head;
		iterator beg = x.begin(), last = x.end();
		for (; beg != last; ++beg)
			node = __insert_after(node, *beg);
	}
	forward_list(forward_list&& x) :
		head(x.head) {
		head = list_node_base();
	}
	forward_list(size_type n, const value_type& val) {
		node_ptr new_node;
		for (; n; --n) {
			new_node = create_node(val);
			new_node->next = head.next;
			head.next = new_node;
		}
	}
	~forward_list() { clear(); }

	iterator begin() { return iterator(head.next); }
	const_iterator begin() const { return const_iterator(head.next); }
	const_iterator cbegin() const { return const_iterator(head.next); }
	iterator before_begin() { return iterator(&head); }
	const_iterator before_begin() const { return const_iterator((base_ptr)&head); }
	const_iterator cbefore_begin() const { return const_iterator((base_ptr)&head); }
	iterator end() { return iterator(); }
	const_iterator end() const { return const_iterator(); }
	const_iterator cend() const { return const_iterator(); }
	size_type size() const { return flist_size(head.next); }
	bool empty() const { return head.next == 0; }

	void swap(forward_list& fl) {
		base_ptr tmp = head.next;
		head.next = fl.head.next;
		fl.head.next = tmp;
	}

	reference front() { return ((node_ptr&)head.next)->data; }
	
	void push_front(const value_type &val){
		node_ptr new_node = create_node(val);
		new_node->next = head.next;
		head.next = new_node;
	}

	void pop_front() {
		node_ptr node = (node_ptr&)head.next;
		head.next = head.next->next;
		destroy_node(node);
	}

	iterator insert_after(const iterator position, const value_type& val) {
		node_ptr new_node = create_node(val);
		new_node->next = position.node->next;
		position.node->next = new_node;
		return new_node;
	}
	
	const_iterator insert_after(const_iterator position, const value_type& val) {
		node_ptr new_node = create_node(val);
		new_node->next = position.node->next;
		position.node->next = new_node;
		return new_node;
	}

	iterator erase_after(iterator position) {
		base_ptr tmp = position.node->next;
		base_ptr ret = position.node->next = tmp->next;
		destroy_node((node_ptr&)tmp);
		return ret;
	}
	
	const_iterator erase_after(const_iterator position) {
		base_ptr tmp = position.node->next;
		base_ptr ret = position.node->next = tmp->next;
		destroy_node((node_ptr&)tmp);
		return ret;
	}

protected:
	node_ptr __insert_after(base_ptr pos, const value_type& val) {
		node_ptr new_node = create_node(val);
		new_node->next = pos->next;
		pos->next = new_node;
		return new_node;
	}
	base_ptr __erase_after(base_ptr pos) {
		base_ptr tmp = pos->next;
		base_ptr ret = pos->next = tmp->next;
		destroy_node((node_ptr&)tmp);
		return ret;
	}

public:
	template <typename InputIter, typename = typename enable_if<is_input_iterator_v<InputIter>>::type>
	void insert_after(iterator position, InputIter beg, InputIter end) {
		base_ptr node = position.node;
		for (; beg != end; ++ beg)
			node = __insert_after(node, *beg);
	}
	
	template <typename InputIter, typename = typename enable_if<is_input_iterator_v<InputIter>>::type>
	void insert_after(const_iterator position, InputIter beg, InputIter end) {
		base_ptr node = position.node;
		for (; beg != end; ++ beg)
			node = __insert_after(node, *beg);
	}

	template <typename InputIter, typename = typename enable_if<is_input_iterator_v<InputIter>>::type>
	void erase_after(iterator position, InputIter beg, InputIter end) {
		base_ptr node = position.node;
		for (; beg != end; ++beg)
			node = __erase_after(node);
	}

	void clear() {
		base_ptr p = head.next;
		base_ptr tmp;
		while (p) {
			tmp = p;
			p = p->next;
			destroy_node((node_ptr&)tmp);
		}
		head.next = 0;
	}

};

}
#endif // !__LMSTL_FORWARD_LIST_H__
