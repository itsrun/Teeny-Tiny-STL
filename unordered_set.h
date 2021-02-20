#ifndef __LMSTL_UNORDERED_SET_H__
#define __LMSTL_UNORDERED_SET_H__

#include "alloc.h"
#include "utility.h"
#include "hash_fun.h"
#include "functional.h"
#include "hashtable.h"

using std::equal_to;

namespace lmstl {

template <typename Value, typename HashFcn = hash<Value>, typename EqualKey = equal_to<Value>, typename Alloc = alloc>
class unordered_set {
private:
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
	ht rep_type;
public:
	typedef typename ht::key_type key_type;
	typedef typename ht::value_type value_type;
	typedef typename ht::hasher hasher;
	typedef typename ht::key_equal key_equal;

	typedef typename ht::size_type size_type;
	typedef typename ht::difference_type difference_type;
	typedef typename ht::const_pointer pointer;
	typedef typename ht::const_pointer const_pointer;
	typedef typename ht::const_reference reference;
	typedef typename ht::const_reference const_reference;

	typedef typename ht::const_iterator iterator;
	typedef typename ht::const_iterator iterator;

	unordered_set() :
		rep_type(100, hasher(), key_equal()) {}

	explicit unordered_set(size_type n) :
		rep_type(n, hasher(), key_equal()) {}

	unordered_set(size_type n, const hasher& hf) :
		rep_type(n, hf, key_equal()) {}

	unordered_set(size_type n, const hasher& hf, const key_equal& eql) :
		rep_type(n, hf, eql) {}

	template <typename InputIterator>
	unordered_set(InputIterator beg, InputIterator end) :
		rep_type(100, hasher(), key_equal()) {
		rep_type.insert_unique(beg, end);
	}

	template <typename InputIterator>
	unordered_set(InputIterator beg, InputIterator end, size_type n) :
		rep_type(n, hasher(), key_equal()) {
		rep_type.insert_unique(beg, end);
	}

	template <typename InputIterator>
	unordered_set(InputIterator beg, InputIterator end, size_type n, const hasher& hf, const key_equal& kel) :
		rep_type(n, hf, kel) {
		rep_type.insert_unique(beg, end);
	}

	size_type size() const { return rep_type.size(); }
	bool empty() const { return rep_type.empty(); }
	void size(unordered_set& us) { rep_type.swap(us.rep_type); }

	iterator begin() const { return rep_type.begin(); }
	iterator end() const { return rep_type.end(); }
	friend bool operator==(const unordered_set&, const unordered_set&);

	pair<iterator, bool > insert(const value_type& x) {
		pair<typename ht::iterator, bool> p = rep_type.insert_unique(x);
		return pair<iterator, bool>(p.first, p.second);
	}

	template <typename InputIterator>
	void insert(InputIterator beg, InputIterator end) {
		for (; beg != end; ++beg)
			rep_type->insert_unique(*beg);
	}

	iterator find(const key_type& key) const { return rep_type.find(key); }

	size_type count(const key_type& key) const { return rep_type.count(key); }

	pair<iterator, iterator> equal_range(const key_type& key) const {
		return rep_type.equal_range(key);
	}

	size_type erase(const key_type& key) {
		return rep_type.erase(key);
	}

	void erase(iterator x) {
		rep_type.erase(x);
	}

	void erase(iterator beg, iterator end) {
		rep_type.erase(beg, end);
	}

	void clear() {
		rep_type.clear();
	}

	void resize(size_type sz) {
		rep_type.resize(sz);
	}

	size_type bucket_count() const { return rep_type.bucket_count(); }

};

template <typename Value, typename HashFcn, typename EqualKey, typename Alloc>
inline bool operator==(const unordered_set<Value, HashFcn, EqualKey, Alloc>& l, unordered_set<Value, HashFcn, EqualKey, Alloc>& r) {
	return l.rep_type == r.rep_type;
}
}

#endif // !__LMSTL_UNORDERED_SET_H__
