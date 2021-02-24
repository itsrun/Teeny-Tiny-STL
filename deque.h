#ifndef __LMSTL_DEQUE_H__
#define __LMSTL_DEQUE_H__

#include "alloc.h"
#include "iterator.h"
#include "uninitialized.h"
#include "exceptdef.h"
#include "type_traits.h"
#include <iostream>
#include <stddef.h>

namespace lmstl {

const size_t Init_Map_Size = 8;

inline size_t __deque_buf_size(size_t n, size_t sz) {
	return n ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

template <typename T, typename Ref, typename Ptr, size_t Buff_size>	//Ref和Ptr大概是为了区分const_iterator吧（
struct deque_iterator : public iterator<random_access_iterator_tag, T> {
	typedef T			value_type;
	typedef Ptr			pointer;
	typedef Ref			reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

	typedef T** map_pointer;
	typedef deque_iterator<T, T&, T*, Buff_size> iterator;
	typedef deque_iterator<T, const T&, const T*, Buff_size> const_iterator;
	typedef deque_iterator self;

	T* cur;
	T* first;
	T* last;
	map_pointer node;

	deque_iterator():
		cur(0), first(0), last(0), node(0){}

	deque_iterator(const iterator& x):
		cur(x.cur), first(x.first), last(x.last), node(x.node) {}
	
	deque_iterator(const const_iterator& x):
		cur(x.cur), first(x.first), last(x.last), node(x.node) {}

	static size_type buffer_size() { return __deque_buf_size(Buff_size, sizeof(T)); }

	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}

	reference operator*() { return *cur; }
	pointer operator->() { return &(operator*()); }

	difference_type operator-(const self& x) const {
		return buffer_size() * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
	}

	self& operator++() {
		++cur;
		if (cur == last) {
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}

	self& operator++(int) {
		self temp = *this;
		++(*this);
		return temp;
	}

	self& operator--() {
		if (cur == first) {
			set_node(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}

	self& operator--(int) {
		self temp = *this;
		--(*this);
		return temp;
	}

	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size()))
			cur += n;
		else {
			difference_type node_offset = offset > 0 ? (difference_type(offset / buffer_size())) : (-difference_type((-offset - 1) / buffer_size()) - 1);
			set_node(node + node_offset);
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}

	self operator+(difference_type n) const {
		self tmp = *this;
		return tmp += n;
	}

	self& operator-=(difference_type n) {
		return *this += -n;
	}

	self operator-(difference_type n) const {
		self tmp = *this;
		return tmp += (-n);
	}

	reference operator[](size_type n) const {
		self tmp = *this;
		return *(tmp += n);
	}
	
	bool operator==(const iterator& x) const { return cur == x.cur; }
	bool operator==(const const_iterator& x) const { return cur == x.cur; }
	bool operator!=(const iterator& x) const { return cur != x.cur; }
	bool operator!=(const const_iterator& x) const { return cur != x.cur; }
	bool operator<(const iterator& x) const { return (node == x.node) ? (cur < x.cur) : (node < x.node); }
	bool operator<(const const_iterator& x) const { return (node == x.node) ? (cur < x.cur) : (node < x.node); }
	bool operator<=(const iterator& x) const { return (node == x.node) ? (cur <= x.cur) : (node <= x.node); }
	bool operator<=(const const_iterator& x) const { return (node == x.node) ? (cur <= x.cur) : (node <= x.node); }

};

template <typename T, typename Alloc = alloc, size_t Buff_size = 0>
class deque {
public:
	typedef T			value_type;
	typedef T*			pointer;
	typedef T&			reference;
	typedef const T&	const_reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

	typedef deque_iterator<T, T&, T*, Buff_size> iterator;
	typedef deque_iterator<T, const T&, const T*, Buff_size> const_iterator;
	typedef reverse_iterator<const_iterator> const_reverse_iterator;
	typedef reverse_iterator<iterator> reverse_iterator;

protected:
	typedef pointer* map_pointer;

	iterator start;
	iterator finish;
	map_pointer map;
	size_type map_size;

public:
	iterator begin() noexcept { return start; }
	const_iterator begin() const noexcept { return start; }
	const_iterator cbegin() const noexcept { return start; }
	reverse_iterator rbegin() noexcept { return reverse_iterator(finish); }
	const_reverse_iterator rbegin() const noexcept { return reverse_iterator(cend()); }

	iterator end() noexcept { return finish; }
	const_iterator end() const noexcept { return finish; }
	const_iterator cend() const noexcept { return finish; }
	reverse_iterator rend() noexcept { return reverse_iterator(start); }
	const_reverse_iterator rend() const noexcept { return reverse_iterator(cbegin()); }

	reference operator[](size_type n) { return start[difference_type(n)]; }
	const_reference operator[](size_type n) const { return start[difference_type(n)]; }

	reference front() { return *start; }
	const_reference front() const { return *start; }
	reference back() { return *(finish - 1); }
	const_reference back() const { return *(finish - 1); }

	size_type size() const { return finish - start; }
	bool empty() const { return finish == start; }

protected:
	typedef simple_alloc<value_type> data_allocator;
	typedef simple_alloc<pointer> map_allocator;

	static size_type buffer_size() { return __deque_buf_size(Buff_size, sizeof(T)); }

	void create_map_and_nodes(size_type num_elements) {
		size_type num_nodes = num_elements / buffer_size() + 1;
		map_size = lmstl::
			
			(Init_Map_Size, num_nodes + 2);
		map = map_allocator::allocate(map_size);

		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;
		map_pointer cur_node;

		for (cur_node = nstart; cur_node <= nfinish; ++cur_node)
			*cur_node = data_allocator::allocate(buffer_size());

		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + num_elements % buffer_size();
	}

	void fill_initialize(size_type n, const value_type& val) {
		create_map_and_nodes(n);
		map_pointer cur_node;
		for (cur_node = start.node; cur_node < finish.node; ++cur_node)
			uninitialized_fill(*cur_node, *cur_node + buffer_size(), val);
		uninitialized_fill(finish.first, finish.cur, val);
	}

	void reallocate_map(size_type nodes_to_add, bool add_at_front) {
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;

		map_pointer new_nstart;
		if (map_size > (new_num_nodes << 1)) {
			new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			if (new_nstart < start.node)
				lmstl::copy(start.node, finish.node + 1, new_nstart);
			else
				lmstl::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
		}
		else {
			size_type new_map_size = map_size + lmstl::max(map_size, nodes_to_add) + 2;
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			lmstl::copy(start.node, finish.node + 1, new_nstart);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}

	void reserve_map_back(size_type nodes_to_add = 1) {
		if (nodes_to_add + 1 > size_type(map_size - (finish.node - map)))
			reallocate_map(nodes_to_add, false);
	}

	void reserve_map_front(size_type nodes_to_add = 1) {
		if (nodes_to_add + 1 > size_type(start.node - map))
			reallocate_map(nodes_to_add, true);
	}

	void push_back_aux(const value_type& val) {
		value_type val_copy = val;
		reserve_map_back();
		*(finish.node + 1) = data_allocator::allocate(buffer_size());
		construct(finish.cur, val_copy);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}

	void push_front_aux(const value_type& val) {
		value_type val_copy = val;
		reserve_map_front();
		*(start.node - 1) = data_allocator::allocate(buffer_size());
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		construct(start.cur, val_copy);
	}

	void pop_back_aux() {
		data_allocator::deallocate(finish.first, buffer_size());
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}

	void pop_front_aux() {
		destroy(start.cur);
		data_allocator::deallocate(start.first, buffer_size());
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

	iterator insert_aux(iterator pos, const value_type& val) {
		size_type index = pos - start;
		value_type val_copy = val;
		if (index < (size()/2)) {
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			lmstl::copy(front2, pos1, front1);
		}
		else {
			push_back(back());
			iterator last1 = finish;
			--last1;
			iterator last2 = last1;
			--last2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			lmstl::copy_backward(pos1, last2, last1);
		}
		*pos = val_copy;
		return pos;
	}

public:
	deque(size_type n, const value_type& val) {
		fill_initialize(n, val);
	}

	deque(){
		create_map_and_nodes(0);
	}

	void push_back(const value_type& val) {
		if (finish.cur != finish.last - 1) {
			construct(finish.cur, val);
			++finish.cur;
		}
		else
			push_back_aux(val);
	}
	void push_front(const value_type& val) {
		if (start.cur != start.first) {
			--start.cur;
			construct(start.cur, val);
		}
		else
			push_front_aux(val);
	}

	void pop_back() {
		if (finish.cur != finish.first) {
			--finish.cur;
			destroy(finish.cur);
		}
		else
			pop_back_aux();
	}

	void pop_front() {
		if (start.cur != start.last - 1) {
			destroy(start.cur);
			++start.cur;
		}
		else
			pop_front_aux();
	}

	void clear() {
		for (map_pointer node = start.node + 1; node < finish.node; ++node) {
			destroy(*node, *node + buffer_size());
			data_allocator::deallocate(*node, buffer_size());
		}
		if (start.node != finish.node) {
			destroy(start.first, start.last);
			destroy(finish.first, finish.cur);
			data_allocator::deallocate(*finish.node, buffer_size());
		}
		else
			destroy(start.cur, start.last);
		finish = start;
	}

	iterator erase(iterator pos) {
		difference_type index = pos - start;
		iterator next = pos;
		++next;
		if (index < (difference_type)(size() >> 1)) {
			lmstl::copy_backward(start, pos, next);
			pop_front();
		}
		else {
			lmstl::copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}

	iterator erase(iterator beg, iterator end) {
		if (beg == start && end == finish) {
			clear();
			return start;
		}
		difference_type n = end - beg;
		difference_type elems_before = beg - start;
		if (elems_before < ((size() - n) >> 1)) {
			lmstl::copy_backward(start, beg, end);
			iterator new_start = start + n;
			destroy(start, new_start);
			for (map_pointer p = start.node; p < new_start.node; p++)
				data_allocator::deallocate(*p, buffer_size());
			start = new_start;
		}
		else {
			lmstl::copy(end, finish, beg);
			iterator new_finish = finish - n;
			destroy(new_finish, finish);
			for (map_pointer p = new_finish.node + 1; p <= finish.node; p++)
				data_allocator::deallocate(*p, buffer_size());
			finish = new_finish;
		}
		return start + elems_before;
	}

	iterator insert(iterator position, const value_type& val) {
		if (position == start) {
			push_front(val);
			return start;
		}
		else if (position == finish) {
			push_back(val);
			return finish - 1;
		}
		else
			return insert_aux(position, val);
	}
	
	const_iterator insert(const_iterator position, const value_type& val) {
		if (position == start) {
			push_front(val);
			return start;
		}
		else if (position == finish) {
			push_back(val);
			return finish - 1;
		}
		else
			return insert_aux(position, val);
	}

};

template <typename T, typename Alloc = alloc, size_t Buff_size>
inline bool operator==(const deque<T, Alloc, Buff_size>& lhs, const deque<T, Alloc, Buff_size>& rhs) {
	return (lhs.start == rhs.start && lhs.finish == rhs.finish && lhs.map == rhs.map);
}

template <typename T, typename Alloc = alloc, size_t Buff_size>
inline bool operator!=(const deque<T, Alloc, Buff_size>& lhs, const deque<T, Alloc, Buff_size>& rhs) {
	return !(lhs == rhs);
}

template <typename T, typename Alloc, size_t Buff_size>
inline bool operator<(const deque<T, Alloc, Buff_size>& lhs, const deque<T, Alloc, Buff_size>& rhs) {
	return lmstl::lexicographical_compare(lhs.start, lhs.finish, rhs.start, rhs.finish);
}

template <typename T, typename Alloc, size_t Buff_size>
inline bool operator<=(const deque<T, Alloc, Buff_size>& lhs, const deque<T, Alloc, Buff_size>& rhs) {
	return (lhs < rhs || lhs == rhs);
}

template <typename T, typename Alloc, size_t Buff_size>
inline bool operator>(const deque<T, Alloc, Buff_size>& lhs, const deque<T, Alloc, Buff_size>& rhs) {
	return rhs < lhs;
}

template <typename T, typename Alloc, size_t Buff_size>
inline bool operator>=(const deque<T, Alloc, Buff_size>& lhs, const deque<T, Alloc, Buff_size>& rhs) {
	return (rhs < lhs || rhs == lhs);
}

}
#endif // !__LMSTL_DEQUE_H__