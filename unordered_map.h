#ifndef __LMSTL_UNORDERED_MAP_H__
#define __LMSTL_UNORDERED_MAP_H__

#include "hash_fun.h"
#include "alloc.h"
#include "hashtable.h"
#include "utility.h"
#include "functional.h"

namespace lmstl {

template <typename Key, typename T, typename HashFcn = hash<Key>, typename EqualKey = equal_to<Key>, typename Alloc = alloc>
class unordered_map {
public:
	typedef Key key_type;
	typedef T data_type;
	typedef T mapped_type;
	typedef pair<const key_type, data_type> value_type;

private:
	typedef hashtable<key_type, value_type, HashFcn, select1st<value_type>, EqualKey, Alloc> ht;
	ht rep_type;
public:
	typedef typename ht::size_type size_type;
	typedef typename ht::key_equal key_equal;
	typedef typename ht::difference_type difference_type;
	typedef typename ht::pointer pointer;
	typedef typename ht::const_pointer const_pointer;
	typedef typename ht::reference reference;
	typedef typename ht::const_reference const_reference;

	typedef typename ht::iterator iterator;
	typedef typename ht::const_iterator const_iterator;
	typedef reverse_iterator<const_iterator> const_reverse_iterator;
	typedef reverse_iterator<iterator> reverse_iterator;
	typedef typename ht::hasher hasher;
	hasher hash_funct() const { return rep_type.hash_funct(); }
	key_equal key_eq() const { return rep_type.key_eq; }

	unordered_map(size_type n = 100, const hasher& hf = hasher(), const key_equal &eql = key_equal()):
		rep_type(n, hf, eql) {}

	template <typename InputIterator>
	unordered_map(InputIterator beg, InputIterator end, size_type n = 100, const hasher& hf=hasher(), const key_equal& eql=key_equal()):
		rep_type(n, hf, eql) {
		rep_type.insert_unique(beg, end);
	}

	size_type size() const { return rep_type.size(); }
	bool empty() const { return rep_type.empty(); }
	void swap(unordered_map& x) { rep_type.swap(x.rep_type); }
	friend bool operator==(const unordered_map&, const unordered_map&);

	iterator begin() { return rep_type.begin(); }
	const_iterator begin() const { return rep_type.cbegin(); }
	const_iterator cbegin() const { return rep_type.cbegin(); }
	reverse_iterator rbegin() { return reverse_iterator(rep_type.end()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(rep_type.cend()); }

	iterator end() { return rep_type.end(); }
	iterator end() const { return rep_type.cend(); }
	iterator cend() const { return rep_type.cend(); }
	reverse_iterator rend() { return reverse_iterator(rep_type.begin()); }
	const_reverse_iterator rend() const { const_reverse_iterator(return rep_type.cbegin()); }

	pair<iterator, bool> insert(const value_type& obj) { return rep_type.insert_unique(obj); }
	
	template<typename InputIterator>
	void insert(InputIterator beg, InputIterator end) { rep_type.insert_unique(beg, end); }

	iterator find(const key_type& key) { return rep_type.find(key); }
	const_iterator find(const key_type& key) const { return rep_type.find(key); }
	
	T& operator[](const key_type& key) {
		iterator res = rep_type.find(key);
		if (res != rep_type.end())
			return (*res).second;
		return (*(insert(value_type(key, data_type())).first)).second;
	}

	const T& operator[](const key_type& key) const {
		iterator res = rep_type.find(key);
		if (res != rep_type.end())
			return *res;
		__THROW_OUT_OF_RANGE_ERROR(1, "Operator [] out of range");
	}

	size_type count(const key_type& key) const { return rep_type.count(key); }

	pair<iterator, iterator> equal_range(const key_type& key) { return rep_type.equal_range(key); }
	pair<const_iterator, const_iterator> equal_range(const key_type& key) const { return rep_type.equal_range(key); }

	size_type erase(const key_type& k) { return rep_type.erase(k); }
	void erase(iterator it) { rep_type.erase(it); }
	void erase(iterator beg, iterator end) { rep_type.erase(beg, end); }
	void clear() { rep_type.clear(); }

	void resize(size_type sz) { rep_type.resize(sz); }
	size_type bucket_count() const { return rep_type.bucket_count(); }

};

template <typename Key, typename T, typename HashFcn, typename EqualKey, typename Alloc>
bool operator==(const unordered_map<Key, T, HashFcn, EqualKey, Alloc>& l, unordered_map<Key, T, HashFcn, EqualKey, Alloc>& r) {
	return l.rep_type == r.rep_type;
}

template <typename Key, typename T, typename HashFcn = hash<Key>, typename EqualKey = equal_to<Key>, typename Alloc = alloc>
class unordered_multimap {
public:
	typedef Key key_type;
	typedef T data_type;
	typedef T mapped_type;
	typedef pair<const key_type, data_type> value_type;

private:
	typedef hashtable<key_type, value_type, HashFcn, select1st<value_type>, EqualKey, Alloc> ht;
	ht rep_type;
public:
	typedef typename ht::size_type size_type;
	typedef typename ht::key_equal key_equal;
	typedef typename ht::difference_type difference_type;
	typedef typename ht::pointer pointer;
	typedef typename ht::const_pointer const_pointer;
	typedef typename ht::reference reference;
	typedef typename ht::const_reference const_reference;
	typedef typename ht::iterator iterator;
	typedef typename ht::const_iterator const_iterator;
	typedef typename ht::hasher hasher;
	hasher hash_funct() const { return rep_type.hash_funct(); }
	key_equal key_eq() const { return rep_type.key_eq; }

	unordered_multimap(size_type n = 100, const hasher& hf = hasher(), const key_equal& eql = key_equal()) :
		rep_type(n, hf, eql) {}

	template <typename InputIterator>
	unordered_multimap(InputIterator beg, InputIterator end, size_type n = 100, const hasher& hf = hasher(), const key_equal& eql = key_equal()) :
		rep_type(n, hf, eql) {
		rep_type.insert_equal(beg, end);
	}

	size_type size() const { return rep_type.size(); }
	bool empty() const { return rep_type.empty(); }
	void swap(unordered_multimap& x) { rep_type.swap(x.rep_type); }
	friend bool operator==(const unordered_multimap&, const unordered_multimap&);

	iterator begin() { return rep_type.begin(); }
	const_iterator begin() const { return rep_type.cbegin(); }
	const_iterator cbegin() const { return rep_type.cbegin(); }
	iterator end() { return rep_type.end(); }
	iterator end() const { return rep_type.cend(); }
	iterator cend() const { return rep_type.cend(); }

	iterator insert(const value_type& obj) { return rep_type.insert_equal(obj); }

	template<typename InputIterator>
	void insert(InputIterator beg, InputIterator end) { rep_type.insert_equal(beg, end); }

	iterator find(const key_type& key) { return rep_type.find(key); }
	const_iterator find(const key_type& key) const { return rep_type.find(key); }

	size_type count(const key_type& key) const { return rep_type.count(key); }

	pair<iterator, iterator> equal_range(const key_type& key) { return rep_type.equal_range(key); }
	pair<const_iterator, const_iterator> equal_range(const key_type& key) const { return rep_type.equal_range(key); }

	size_type erase(const key_type& k) { return rep_type.erase(k); }
	void erase(iterator it) { rep_type.erase(it); }
	void erase(iterator beg, iterator end) { rep_type.erase(beg, end); }
	void clear() { rep_type.clear(); }

	void resize(size_type sz) { rep_type.resize(sz); }
	size_type bucket_count() const { return rep_type.bucket_count(); }

};

template <typename Key, typename T, typename HashFcn, typename EqualKey, typename Alloc>
bool operator==(const unordered_multimap<Key, T, HashFcn, EqualKey, Alloc>& l, unordered_multimap<Key, T, HashFcn, EqualKey, Alloc>& r) {
	return l.rep_type == r.rep_type;
}

}
#endif // !__LMSTL_UNORDERED_MAP_H__
