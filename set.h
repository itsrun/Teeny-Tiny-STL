#ifndef __LMSTL_SET_H__
#define __LMSTL_SET_H__

#include "alloc.h"
#include "rb_tree.h"
#include "functional.h"

namespace lmstl {
	//后期要修改！

template <typename Key, typename Compare = std::less<Key>, typename Alloc = alloc>
class set {
public:
	typedef Key key_type;
	typedef Key value_type;
	typedef Compare key_compare;
	typedef Compare value_compare;

private:
	typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
	rep_type t;

public:
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	set():
		t(Compare()) {}

	template <typename InputIterator>
	set(InputIterator beg, InputIterator end):
		t(Compare()) {
		t.insert_unique(beg, end);
	}

	template <typename InputIterator>
	set(InputIterator beg, InputIterator end, const Compare &comp):
		t(comp) {
		t.insert_unique(beg, end);
	}

	set(const set<Key, Compare, Alloc>&x):
		t(x.t) {}

	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) {
		t = x.t;
		return *this;
	}

	key_compare key_comp() const { return t.key_comp(); }
	value_compare value_comp() const { return t.key_comp(); }
	iterator begin() const { return t.begin(); }
	iterator end() const { return t.end(); }
	bool empty() const { return t.empty(); }
	size_type size() const { return t.size(); }
	
	pair<iterator, bool> insert(const value_type& x) {
		return t.insert_unique(x);
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

	void clear() { t.clear(); }

	iterator find(const key_type& x) const { return t.find(x); }
	size_type count(const key_type& x) const { return t.count(x); }
	iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
	iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
	pair<iterator, iterator> equal_range(const key_type& x) const { return t.equal_range(x); }

	friend bool operator==(const set&, const set&);
	friend bool operator<(const set&, const set&);
};

template <typename Key, typename Compare, typename Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& l, const set<Key, Compare, Alloc>& r) {
	return l.t == r.t;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& l, const set<Key, Compare, Alloc>& r) {
	return l.t < r.t;
}

template <typename Key, typename Compare = std::less<Key>, typename Alloc = alloc>
class multiset {
public:
	typedef Key key_type;
	typedef Key value_type;
	typedef Compare key_compare;
	typedef Compare value_compare;

private:
	typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
	rep_type t;

public:
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	multiset() :
		t(Compare()) {}

	template <typename InputIterator>
	multiset(InputIterator beg, InputIterator end) :
		t(Compare()) {
		t.insert_unique(beg, end);
	}

	template <typename InputIterator>
	multiset(InputIterator beg, InputIterator end, const Compare& comp) :
		t(comp) {
		t.insert_unique(beg, end);
	}

	multiset(const multiset<Key, Compare, Alloc>& x) :
		t(x.t) {}

	multiset<Key, Compare, Alloc>& operator=(const multiset<Key, Compare, Alloc>& x) {
		t = x.t;
		return *this;
	}

	key_compare key_comp() const { return t.key_comp(); }
	value_compare value_comp() const { return t.key_comp(); }
	iterator begin() const { return t.begin(); }
	iterator end() const { return t.end(); }
	bool empty() const { return t.empty(); }
	size_type size() const { return t.size(); }

	pair<iterator, bool> insert(const value_type& x) {
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

	void clear() { t.clear(); }

	iterator find(const key_type& x) const { return t.find(x); }
	size_type count(const key_type& x) const { return t.count(x); }
	iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
	iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
	pair<iterator, iterator> equal_range(const key_type& x) const { return t.equal_range(x); }

	friend bool operator==(const multiset&, const multiset&);
	friend bool operator<(const multiset&, const multiset&);
};

template <typename Key, typename Compare, typename Alloc>
inline bool operator==(const multiset<Key, Compare, Alloc>& l, const multiset<Key, Compare, Alloc>& r) {
	return l.t == r.t;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator<(const multiset<Key, Compare, Alloc>& l, const multiset<Key, Compare, Alloc>& r) {
	return l.t < r.t;
}

}

#endif // !__LMSTL_SET_H__
