#ifndef __LMSTL_VECTOR_H__
#define __LMSTL_VECTOR_H__

#include "alloc.h"
#include "iterator.h"
#include "uninitialized.h"
#include "exceptdef.h"
#include "type_traits.h"
#include "algobase.h"
#include <stddef.h>

namespace lmstl {

template <typename T, typename Alloc = alloc>
class vector {

public:
	typedef T					value_type;
	typedef value_type*			pointer;
	typedef const value_type*	const_pointer;
	typedef value_type&			reference;
	typedef const value_type&	const_reference;
	typedef ptrdiff_t			difference_type;
	typedef size_t				size_type;

	typedef value_type*			iterator;
	typedef const value_type*	const_iterator;
	typedef reverse_iterator<const_iterator> const_reverse_iterator;
	typedef reverse_iterator<iterator> reverse_iterator;

public:
	vector():
		start(0), finish(0), end_of_storage(0) {}
	vector(size_type n, const T& val) {
		start = data_allocator::allocate(n);
		lmstl::uninitialized_fill_n(start, n, val);
		finish = end_of_storage = start + n;
	}
	explicit vector(size_type n) {
		start = data_allocator::allocate(n);
		lmstl::uninitialized_fill_n(start, n, T());
		finish = end_of_storage = start + n;
	}
	vector(const vector& x) {
		size_type cap = (size_type)(x.end_of_storage - x.start);
		size_type sz = (size_type)(x.finish - x.start);
		init(sz, cap);
		lmstl::uninitialized_copy(x.begin(), x.end(), start);
	}
	vector(vector&& x) noexcept :
		start(x.start), finish(x.finish), end_of_storage(x.end_of_storage) {
		x.start = 0;
		x.finish = 0;
		x.end_of_storage = 0;
	}

	template <typename InputIter, typename = typename enable_if<
		is_input_iterator_v<InputIter>>::type>
	vector(InputIter beg, InputIter end) {
		size_type sz = distance(beg, end);
		init(sz, sz);
		lmstl::uninitialized_copy(beg, end, start);
	}

	~vector() {
		destroy(start, finish);
		data_allocator::deallocate(start, capacity());
	}

protected:
	typedef simple_alloc<value_type, Alloc> data_allocator;
	iterator start;
	iterator finish;
	iterator end_of_storage;

private:
	void init(size_type sz, size_type cap) {
		try {
			start = data_allocator::allocate(cap);
			finish = start + sz;
			end_of_storage = start + cap;
		}
		catch (...) {
			start = 0;
			finish = 0;
			end_of_storage = 0;
			__THROW_RUNTIME_ERROR(1, "Error when initializing");
		}
	}

public:

	iterator begin() noexcept { return start; }
	const_iterator begin() const noexcept { return start; }
	const_iterator cbegin() const noexcept { return start; }
	reverse_iterator rbegin() noexcept { return reverse_iterator(finish); }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(finish); }

	iterator end() { return finish; }
	const_iterator end() const noexcept { return finish; }
	const_iterator cend() const noexcept { return finish; }
	reverse_iterator rend() noexcept { return reverse_iterator(start); }
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator(start); }

	size_type size() const noexcept { return size_type(finish - start); }
	size_type capacity() const noexcept { return size_type(end_of_storage - start); }
	bool empty() const noexcept { return (start == finish); }

	void swap(vector<T, Alloc>& x) {
		iterator tmp = start;
		start = x.start;
		x.start = tmp;
		tmp = finish;
		finish = x.finish;
		x.finish = tmp;
		tmp = end_of_storage;
		end_of_storage = x.end_of_storage;
		x.end_of_storage = tmp;
	}

	reference operator[](size_type n) {
		__THROW_OUT_OF_RANGE_ERROR(n >= size(), "Index out of range");
		return *(start + n);
	}
	const_reference operator[](size_type n) const {
		__THROW_OUT_OF_RANGE_ERROR(n >= size(), "Index out of range");
		return *(start + n);
	}

	reference at(size_type n) {
		__THROW_OUT_OF_RANGE_ERROR(n >= size(), "Index out of range");
		return *(start + n);
	}
	const_reference at(size_type n) const {
		__THROW_OUT_OF_RANGE_ERROR(n >= size(), "Index out of range");
		return *(start + n);
	}

	reference front() noexcept { return *start; }
	const_reference front() const noexcept { return *start; }
	reference back() noexcept { return *(finish - 1); }
	const_reference back() const noexcept { return *(finish - 1); }

private:
	iterator realloc_insert(iterator pos, const T& val);

public:

	void push_back(const T& val) {
		if (finish != end_of_storage) {
			construct(finish, val);
			++finish;
		}
		else 
			realloc_insert(finish, val);
	}

	void pop_back() {
		__THROW_RUNTIME_ERROR(empty(), "pop_back on empty vector");
		--finish;
		destroy(finish);
	}

	iterator erase(iterator beg, iterator end) {
		__THROW_OUT_OF_RANGE_ERROR(!(beg <= end && beg >= start && end <= finish), "Range Error");
		iterator p = lmstl::copy(end, finish, beg);
		destroy(p, finish);
		finish = finish - (end - beg);
		return beg;
	}
	iterator erase(iterator pos) {
		__THROW_OUT_OF_RANGE_ERROR((pos > finish || pos < start), "Range Error");
		if (pos + 1 != finish)
			lmstl::copy(pos + 1, finish, pos);
		--finish;
		destroy(finish);
		return pos;
	}

	void reserve(size_type n) {
		start = data_allocator::allocate(n);
		finish = start;
		end_of_storage = start + n;
	}

	void clear() {
		destroy(start, finish);
	}

	iterator insert(const iterator position, size_type n, const T& val);
	iterator insert(const iterator position, const value_type& val) {
		return realloc_insert(position, val);
	}
	
	template <typename InputIterator, typename = typename enable_if<is_input_iterator_v<InputIterator>>::type>
	void insert(const iterator position, InputIterator beg, InputIterator end) {
		iterator pos = const_cast<iterator>(position);
		const size_type after_nums = finish - pos;
		const size_type len = (size_type)lmstl::distance(beg, end);
		if ((size_type)(end_of_storage - finish) >= len) {
			if (after_nums > len) {
				lmstl::uninitialized_copy(finish - len, finish, finish);
				lmstl::copy_backward(pos, finish - len, finish);
				lmstl::copy(beg, end, pos);
			}
			else {
				InputIterator p = beg;
				lmstl::advance(p, after_nums);
				lmstl::uninitialized_copy(pos, finish, pos + len);
				lmstl::copy(beg, p, pos);
				lmstl::uninitialized_copy(p, end, finish);
			}
			finish += len;
		}
		else {
			size_type old_size = end_of_storage - start;
			size_type new_size = old_size + max(old_size, len);
			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;
			try {
				new_finish = lmstl::uninitialized_copy(start, pos, new_start);
				new_finish = lmstl::uninitialized_copy(beg, end, new_finish);
				new_finish = lmstl::uninitialized_copy(pos, finish, new_finish);
			}
			catch (...) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);
				__THROW_RUNTIME_ERROR(1, "Error when reallocating");
			}
			start = new_start;
			finish = new_finish;
			end_of_storage = start + new_size;
		}
	}		

	template <typename InputIterator, typename = typename enable_if<is_input_iterator_v<InputIterator>>::type>
	void insert(const const_iterator position, InputIterator beg, InputIterator end) {
		iterator pos = const_cast<iterator>(position);
		const size_type after_nums = finish - pos;
		const size_type len = (size_type)lmstl::distance(beg, end);
		if ((size_type)(end_of_storage - finish) >= len) {
			if (after_nums > len) {
				lmstl::uninitialized_copy(finish - len, finish, finish);
				lmstl::copy_backward(pos, finish - len, finish);
				lmstl::copy(beg, end, pos);
			}
			else {
				InputIterator p = beg;
				lmstl::advance(p, after_nums);
				lmstl::uninitialized_copy(pos, finish, pos + len);
				lmstl::copy(beg, p, pos);
				lmstl::uninitialized_copy(p, end, finish);
			}
			finish += len;
		}
		else {
			size_type old_size = end_of_storage - start;
			size_type new_size = old_size + max(old_size, len);
			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;
			try {
				new_finish = lmstl::uninitialized_copy(start, pos, new_start);
				new_finish = lmstl::uninitialized_copy(beg, end, new_finish);
				new_finish = lmstl::uninitialized_copy(pos, finish, new_finish);
			}
			catch (...) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);
				__THROW_RUNTIME_ERROR(1, "Error when reallocating");
			}
			start = new_start;
			finish = new_finish;
			end_of_storage = start + new_size;
		}
	}	
};

template<typename T, typename Alloc>
inline typename vector<T, Alloc>::iterator vector<T, Alloc>::realloc_insert(iterator pos, const T& val) {
	if(finish!=end_of_storage){
		++finish;
		T val_copy = val;
		lmstl::move_backward(pos, finish - 1, finish);
		*pos = val_copy;
		return pos;
	}
	const size_type old_size = end_of_storage - start;
	const size_type new_size = old_size ? (old_size << 1) : 10;
	iterator ret;
	iterator new_start = data_allocator::allocate(new_size);
	iterator new_finish = new_start;
	try {
		ret = new_finish = lmstl::uninitialized_move(start, pos, new_start);
		construct(new_finish, val);
		++new_finish;
		new_finish = lmstl::uninitialized_move(pos, finish, new_finish);
	}
	catch (...) {
		destroy(new_start, new_finish);
		data_allocator::deallocate(new_start, new_size);
		__THROW_RUNTIME_ERROR(1, "Error when reallocating");
	}
	if (start) {
		destroy(start, finish);
		data_allocator::deallocate(start, old_size);
	}
	start = new_start;
	finish = new_finish;
	end_of_storage = start + new_size;
	return ret;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const iterator position, size_type n, const T& val) {
	iterator pos = const_cast<iterator>(position);
	if (!n) return pos;
	if (size_type(end_of_storage - finish) >= n) {
		const size_type after_nums = finish - pos;
		T val_copy = val;
		if (after_nums > n) {
			lmstl::uninitialized_move(finish - n, finish, finish);
			lmstl::copy_backward(pos, finish - n, finish);
			lmstl::fill_n(pos, n, val_copy);
		}
		else {
			lmstl::uninitialized_copy(pos, finish, pos + n);
			lmstl::fill_n(pos, after_nums, val_copy);
			lmstl::uninitialized_fill_n(finish, n - after_nums, val_copy);
		}
		finish += n;
		return pos;
	}
	const size_type old_size = end_of_storage - start;
	const size_type new_size = old_size + max(old_size, n);
	iterator ret;
	iterator new_start = data_allocator::allocate(new_size);
	iterator new_finish = new_start;
	try {
		ret = new_finish = lmstl::uninitialized_copy(start, pos, new_start);
		new_finish = lmstl::uninitialized_fill_n(new_finish, n, val);
		new_finish = lmstl::uninitialized_copy(pos, finish, new_finish);
	}
	catch (...) {
		destroy(new_start, new_finish);
		data_allocator::deallocate(new_start, new_size);
		__THROW_RUNTIME_ERROR(1, "Error when reallocating");
	}
	if (start) {
		destroy(start, finish);
		data_allocator::deallocate(start, old_size);
	}
	start = new_start;
	finish = new_finish;
	end_of_storage = start + new_size;
	return ret;
}

}
#endif // !__LMSTL_VECTOR_H__
