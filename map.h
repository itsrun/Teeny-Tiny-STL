#ifndef __LMSTL_MAP_H__
#define __LMSTL_MAP_H__

#include "alloc.h"
#include "utility.h"
#include "iterator.h"
#include "functional.h"
#include "rb_tree.h"

namespace lmstl {

template <typename Key, typename T, typename Compare = less<Key>, typename Alloc = alloc>
class map {
public:
	typedef Key key_type;
	typedef T data_type;
	typedef T mapped_type;
	typedef pair<const key_type, data_type> value_type;
	typedef Compare key_compare;

private:
	typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;
	rep_type t;

public:
	typedef typename rep_type::pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::reference reference;
	typedef typename rep_type::const_reference const_reference;

	typedef typename rep_type::iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef reverse_iterator<const_iterator> const_reverse_iterator;
	typedef reverse_iterator<iterator> reverse_iterator;

	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	map():
		t(Compare()) {}

	explicit map(const Compare& comp):
		t(comp) {}

	template <typename InputIterator>
	map(InputIterator beg, InputIterator end) :
		t(Compare()) {
		t.insert_unique(beg, end);
	}

	template <typename InputIterator>
	map(InputIterator beg, InputIterator end, const Compare& comp) :
		t(comp) {
		t.insert_unique(beg, end);
	}

	map(const map<Key, T, Compare, Alloc>& x):
		t(x.t) {}
	
	map(map<Key, T, Compare, Alloc>&& x):
		t(lmstl::move(x.t)) {}

	map<Key, T, Compare, Alloc>& operator=(const map<Key, T, Compare, Alloc>& x) {
		t = x.t;
		return *this;
	}
	
	map<Key, T, Compare, Alloc>& operator=(map<Key, T, Compare, Alloc>&& x) {
		t = lmstl::move(x.t);
		return *this;
	}

	key_compare key_comp() const { return t.key_comp(); }

	iterator begin() noexcept { return t.begin(); }
	const_iterator begin() const noexcept { return t.begin(); }
	const_iterator cbegin() const noexcept { return t.begin(); }
	reverse_iterator rbegin() noexcept { return reverse_iterator(t.end()); }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(t.cend()); }

	iterator end() noexcept { return t.end(); }
	const_iterator end() const noexcept { return t.end(); }
	const_iterator cend() const noexcept { return t.end(); }
	reverse_iterator rend() noexcept { return reverse_iterator(t.begin()); }
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator(t.cbegin()); }

	bool empty() const { return t.empty(); }
	size_type size() const { return t.size(); }

	pair<iterator, bool> insert(const value_type& x) {
		return t.insert_unique(x);
	}

	T& operator[](const key_type& k) {
		return (*((t.insert_unique(value_type(k, T())).first))).second;
	}

	template <typename InputIterator>
	void insert(InputIterator beg, InputIterator end) {
		t.insert_unique(beg, end);
	}

	void erase(iterator position) {
		t.erase(position);
	}

	size_type erase(const key_type& x) {
		return t.erase(x);
	}

	void erase(iterator beg, iterator end) {
		t.erase(beg, end);
	}

	~map() {
		t.clear();
	}

	void clear() {
		t.clear();
	}

	iterator find(const key_type& x) {
		return t.find(x);
	}
	const_iterator find(const key_type& x) const {
		return t.find(x);
	}
	size_type count(const key_type& x) const { return t.count(x); }

	iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
	const_iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
	iterator upper_bound(const key_type& x) { return t.upper_bound(x); }
	const_iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }

	pair<iterator, iterator> equal_range(const key_type& x) {
		return t.equal_range(x);
	}
	pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
		return t.equal_range(x);
	}
	friend bool operator==(const map&, const map&);
	friend bool operator<(const map&, const map&);
};

template <typename Key, typename T, typename Compare, typename Alloc>
inline bool operator==(const map<Key, T, Compare, Alloc>& l, const map<Key, T, Compare, Alloc>& r) {
	return l.t == r.t;
}

template <typename Key, typename T, typename Compare, typename Alloc>
inline bool operator<(const map<Key, T, Compare, Alloc>& l, const map<Key, T, Compare, Alloc>& r) {
	return l.t < r.t;
}

template <typename Key, typename T, typename Compare = less<Key>, typename Alloc = alloc>
class multimap {
public:
	typedef Key key_type;
	typedef T data_type;
	typedef T mapped_type;
	typedef pair<const key_type, data_type> value_type;
	typedef Compare key_compare;

private:
	typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;
	rep_type t;

public:
	typedef typename rep_type::pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;

	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	multimap() :
		t(Compare()) {}

	explicit multimap(const Compare& comp) :
		t(comp) {}

	template <typename InputIterator>
	multimap(InputIterator beg, InputIterator end) :
		t(Compare()) {
		t.insert_equal(beg, end);
	}

	template <typename InputIterator>
	explicit multimap(InputIterator beg, InputIterator end, const Compare& comp) :
		t(comp) {
		t.insert_equal(beg, end);
	}

	multimap(const multimap<Key, T, Compare, Alloc>& x) :
		t(x.t) {}

	multimap<Key, T, Compare, Alloc>& operator=(const multimap<Key, T, Compare, Alloc>& x) {
		t = x.t;
		return *this;
	}

	key_compare key_comp() const { return t.key_comp(); }

	iterator begin() { return t.begin(); }
	const_iterator begin() const { return t.begin(); }
	iterator end() { return t.end(); }
	const_iterator end() const { return t.end(); }
	bool empty() const { return t.empty(); }
	size_type size() const { return t.size(); }

	iterator insert(const value_type& x) {
		return t.insert_equal(x);
	}

	template <typename InputIterator>
	void insert(InputIterator beg, InputIterator end) {
		t.insert_equal(beg, end);
	}

	void erase(iterator position) {
		t.erase(position);
	}

	size_type erase(const key_type& x) {
		return t.erase(x);
	}

	void erase(iterator beg, iterator end) {
		t.erase(beg, end);
	}

	void clear() {
		t.clear();
	}

	iterator find(const key_type& x) {
		return t.find(x);
	}
	const_iterator find(const key_type& x) const {
		return t.find(x);
	}

	size_type count(const key_type& x) const { return t.count(x); }

	iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
	const_iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
	iterator upper_bound(const key_type& x) { return t.upper_bound(x); }
	const_iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }

	pair<iterator, iterator> equal_range(const key_type& x) {
		return t.equal_range(x);
	}
	pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
		return t.equal_range(x);
	}
	friend bool operator==(const multimap&, const multimap&);
	friend bool operator<(const multimap&, const multimap&);
};

template <typename Key, typename T, typename Compare, typename Alloc>
inline bool operator==(const multimap<Key, T, Compare, Alloc>& l, const multimap<Key, T, Compare, Alloc>& r) {
	return l.t == r.t;
}

template <typename Key, typename T, typename Compare, typename Alloc>
inline bool operator<(const multimap<Key, T, Compare, Alloc>& l, const multimap<Key, T, Compare, Alloc>& r) {
	return l.t < r.t;
}

}
#endif // !__LMSTL_MAP_H__
