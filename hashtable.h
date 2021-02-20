#ifndef __LMSTL_HASHTABLE_H__
#define __LMSTL_HASHTABLE_H__

#include "alloc.h"
#include "construct.h"
#include "exceptdef.h"
#include "iterator.h"
#include "type_traits.h"
#include "utility.h"
#include "vector.h"
#include <stddef.h>

using std::lower_bound;

namespace lmstl {

static const int _primes_num = 28;
static const unsigned long _primes_list[_primes_num] = {
	53,97,193,389,769,
	1543,3079,6151,12289,24593,
	49157,98317,196613,393241,786433,
	1572869,3145739,6291469,12582917,25165843,
	50331653,100663319,201326611,402653189,805306457,
	1610612741,3221225473ul,4294967291ul
};

template <typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc = alloc>
class hashtable;

struct hashtable_node_base {
	typedef hashtable_node_base* base_ptr;
	base_ptr next;
};

template <typename T>
struct hashtable_node :public hashtable_node_base {
	T val;
	hashtable_node():
		hashtable_node_base(), val(T()) {}
	hashtable_node(const T &v):
		hashtable_node_base(), val(v) {}
};

struct hashtable_iterator_base {
	typedef hashtable_node_base::base_ptr base_ptr;
	typedef forward_iterator_tag iterator_category;
	base_ptr cur;
	hashtable_iterator_base(base_ptr x=0):
		cur(x) {}
};

template <typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc = alloc>
struct hashtable_iterator : public hashtable_iterator_base {
	
	typedef Value value_type;
	typedef Key key_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef hashtable_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
	typedef hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;

	typedef hashtable_node<Value> node;
	typedef hashtable_node<Value>* node_ptr;
	typedef hashtable_node_base* base_ptr;

	hashtable* ht;

	hashtable_iterator():
		hashtable_iterator_base(), ht(0) {}
	hashtable_iterator(base_ptr n, hashtable* tab):
		hashtable_iterator_base(n), ht(tab) {}
	hashtable_iterator(const hashtable_iterator& x):
		hashtable_iterator_base(x.cur), ht(x.ht) {}

	reference operator*() const { return ((node_ptr&)cur)->val; }
	pointer operator->() const { return &(operator*()); }
	iterator& operator++();
	iterator operator++(int);

	bool operator==(const iterator& it) const { return cur == it.cur; }
	bool operator!=(const iterator& it) const { return cur != it.cur; }

};

template <typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc = alloc>
struct hashtable_const_iterator : public hashtable_iterator_base {

	typedef Value value_type;
	typedef Key key_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef const value_type* pointer;
	typedef const value_type* const_pointer;
	typedef const value_type& reference;
	typedef const value_type& const_reference;

	typedef hashtable_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
	typedef hashtable_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
	typedef hashtable<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;

	typedef hashtable_node<Value> node;
	typedef hashtable_node<Value>* node_ptr;
	typedef hashtable_node_base* base_ptr;

	const hashtable* ht;

	hashtable_const_iterator() {}
	hashtable_const_iterator(base_ptr n, const hashtable* tab) :
		hashtable_iterator_base(n), ht(tab) {}
	hashtable_const_iterator(const const_iterator& x):
		hashtable_iterator_base(x.cur), ht(x.ht) {}
	hashtable_const_iterator(const iterator& x) :
		hashtable_iterator_base(x.cur), ht(x.ht) {}

	reference operator*() const { return ((node_ptr&)cur)->val; }
	pointer operator->() const { return &(operator*()); }
	const_iterator operator++();
	const_iterator operator++(int);

	bool operator==(const const_iterator& it) const { return cur == it.cur; }
	bool operator!=(const const_iterator& it) const { return cur != it.cur; }

};

template <typename Key, typename Value, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
class hashtable {
	friend hashtable_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>;
	friend hashtable_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc>;
public:
	typedef HashFcn hasher;
	typedef EqualKey key_equal;

	typedef size_t size_type;
	typedef Key key_type;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;

	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;

	typedef hashtable_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
	typedef hashtable_const_iterator<Key, Value, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;

	hasher hash_funct() const { return hash; }
	key_equal key_eq() const { return equals; }

private:
	typedef hashtable_node<Value> node;
	typedef hashtable_node<Value>* node_ptr;
	typedef hashtable_node_base* base_ptr;
	typedef simple_alloc<node, Alloc> node_allocator;

	vector<node_ptr> buckets;
	size_type num_elements;
	hasher hash;
	key_equal equals;
	ExtractKey get_key;

	unsigned long _next_prime(unsigned long n) {
		const unsigned long* first = _primes_list;
		const unsigned long* last = _primes_list + _primes_num;
		const unsigned long* pos = lower_bound(first, last, n);
		return pos == last ? *(last - 1) : *pos;
	}

	node_ptr new_node(const value_type& obj) {
		node_ptr ret = node_allocator::allocate(1);
		ret->next = 0;
		try {
			construct(ret, obj);
		}
		catch (...) {
			node_allocator::deallocate(ret, 1);
			__THROW_RUNTIME_ERROR(1, "Error when constructing");
		}
		return ret;
	}

	void delete_node(node_ptr n) {
		destroy(n);
		node_allocator::deallocate(n, 1);
	}

public:
	size_type bucket_count() const { return buckets.size(); }

	hashtable(size_type n, const HashFcn& hf, const EqualKey& eqk) :
		hash(hf), equals(eqk), get_key(ExtractKey()), num_elements(0) {
		init_buckets(n);
	}

	hashtable(const hashtable& ht) :
		hash(ht.hash), equals(ht.equals), get_key(ht.get_key), num_elements(ht.num_elements) {
		buckets.reserve(ht.buckets.size());
		buckets.insert(buckets.begin(), ht.buckets.size(), (node_ptr)0);
		try {
			size_type len = ht.buckets.size();
			node_ptr cur, copy;
			for (size_type i = 0; i < len; ++i) {
				if (cur = ht.buckets[i]) {
					copy = new_node(cur->val);
					buckets[i] = copy;
					for (cur = (node_ptr&)cur->next; cur; cur = (node_ptr&)cur->next) {
						copy->next = new_node(cur->val);
						copy = (node_ptr&)copy->next;
					}
					copy->next = 0;
				}
			}
			num_elements = ht.num_elements;
		}
		catch (...) {
			clear();
			__THROW_RUNTIME_ERROR(1, "Error when copying");
		}
	}

	hashtable& operator=(const hashtable& ht) {
		copy_from(ht);
		return *this;
	}

	~hashtable() {
		clear();
	}

	iterator begin() noexcept {
		size_type len = buckets.size();
		for (size_type i = 0; i < len; ++i) {
			if (buckets[i])
				return iterator(buckets[i], this);
		}
		return iterator((base_ptr)0, this);
	}
	iterator end() noexcept { return iterator((base_ptr)0, this); }
	const_iterator begin() const noexcept {
		size_type len = buckets.size();
		for (size_type i = 0; i < len; ++i) {
			if (buckets[i])
				return const_iterator(buckets[i], this);
		}
		return const_iterator((base_ptr)0, this);
	}
	const_iterator end() const noexcept { return const_iterator((base_ptr)0, this); }
	size_type size() const noexcept { return num_elements; }
	bool empty() const noexcept { return !num_elements; }

	void swap(hashtable& x) {
		buckets.swap(x.buckets);
		std::swap(num_elements, x.num_elements);
		std::swap(hash, x.hash);
		std::swap(equals, x.equals);
		std::swap(get_key, x.get_key);
	}

	iterator find(const key_type& k) {
		const size_type bk = bkt_num_key(k);
		node_ptr optr = buckets[bk];
		while (optr) {
			if (equals(k, get_key(optr->val)))
				return iterator(optr, this);
			optr = (node_ptr&)optr->next;
		}
		return iterator();
	}

	const_iterator find(const key_type& k) const {
		const size_type bk = bkt_num_key(k);
		node_ptr optr = buckets[bk];
		while (optr) {
			if (equals(k, get_key(optr->val)))
				return const_iterator(optr, this);
			optr = (node_ptr&)optr->next;
		}
		return const_iterator();
	}

	size_type count(const key_type& k) const {
		const size_type bk = bkt_num_key(k);
		size_type ret = 0;
		node_ptr optr = buckets[bk];
		while (optr) {
			if (equals(k, get_key(optr->val)))
				++ret;
		}
		return ret;
	}

	void clear() {
		size_type len = buckets.size();
		node_ptr cur, next;
		for (size_type i = 0; i < len; ++i) {
			cur = buckets[i];
			while (cur) {
				next = (node_ptr&)cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = 0;
		}
		num_elements = 0;
	}

	void copy_from(const hashtable& ht) {
		buckets.clear();
		buckets.reserve(ht.buckets.size());
		buckets.insert(buckets.begin(), ht.buckets.size(), (node_ptr)0);
		try {
			size_type len = ht.buckets.size();
			node_ptr cur, copy;
			for (size_type i = 0; i < len; ++i) {
				if (cur = ht.buckets[i]) {
					copy = new_node(cur->val);
					buckets[i] = copy;
					for (cur = (node_ptr&)cur->next; cur; cur = (node_ptr&)cur->next) {
						copy->next = new_node(cur->val);
						copy = (node_ptr&)copy->next;
					}
					copy->next = 0;
				}
			}
			num_elements = ht.num_elements;
		}
		catch (...) {
			clear();
			__THROW_RUNTIME_ERROR(1, "Error when copying");
		}
	}

	pair<iterator, bool> insert_unique(const value_type& obj) {
		resize(num_elements + 1);
		const size_type bk = bkt_num(obj);
		node_ptr first = buckets[bk];

		for (node_ptr cur = first; cur; cur = (node_ptr&)cur->next)
			if (equals(get_key(cur->val), get_key(obj))) {
				return pair<iterator, bool>(iterator(cur, this), false);
			}
		node_ptr tmp = new_node(obj);
		tmp->next = first;
		buckets[bk] = tmp;
		++num_elements;
		return pair<iterator, bool>(iterator(tmp, this), true);
	}

	template <typename InputIterator>
	void insert_unique(InputIterator beg, InputIterator end) {
		for (; beg != end; ++beg)
			insert_unique(*beg);
	}

	iterator insert_equal(const value_type& obj) {
		resize(num_elements + 1);
		const size_type n = bkt_num(obj);
		node_ptr first = buckets[n], tmp;

		for (node_ptr cur = first; cur; cur = (node_ptr&)cur->next)
			if (equals(get_key(cur->val), get_key(obj))) {
				tmp = new_node(obj);
				tmp->next = cur->next;
				cur->next = tmp;
				++num_elements;
				return iterator(tmp, this);
			}
		tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return iterator(tmp, this);
	}

	pair<iterator, iterator> equal_range(const key_type& key) {
		const size_type bk = bkt_num_key(key);
		iterator iter1(buckets[bk], this);
		while (iter1.cur) {
			if (equals(((node_ptr&)iter1.cur)->val, key))
				break;
			++iter1;
		}
		iterator iter2(iter1);
		while (iter2.cur) {
			if (!equals(((node_ptr&)iter2.cur)->val, key))
				break;
			++iter2;
		}
		return pair<iterator, iterator>(iter1, iter2);
	}

	pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
		const size_type bk = bkt_num_key(key);
		iterator iter1(buckets[bk], this);
		while (iter1.cur) {
			if (equals(((node_ptr&)iter1.cur)->val, key))
				break;
			++iter1;
		}
		iterator iter2(iter1);
		while (iter2.cur) {
			if (!equals(((node_ptr&)iter2.cur)->val, key))
				break;
			++iter2;
		}
		return pair<const_iterator, const_iterator>(iter1, iter2);
	}

	template <typename InputIterator, typename _BOOL = typename enable_if<is_input_iterator<InputIterator>::value>::type>
	void insert_equal(InputIterator beg, InputIterator end) {
		for (; beg != end; ++beg)
			insert_equal(*beg);
	}

	void erase(iterator position) {
		if (position.cur == 0)
			__THROW_OUT_OF_RANGE_ERROR(1, "Invalid iterator");
		size_type bk = bkt_num(((node_ptr)position.cur)->val);
		node_ptr prev = 0;
		node_ptr ptr = buckets[bk];
		while (ptr && ptr != position.cur) {
			prev = ptr;
			ptr = (node_ptr&)ptr->next;
		}
		if (!ptr)
			return;
		if (prev)
			prev->next = (node_ptr&)ptr->next;
		else
			buckets[bk] = (node_ptr&)ptr->next;
		delete_node(ptr);
	}

	void erase(iterator beg, iterator end) {
		for (; beg != end; ++beg)
			erase(beg);
	}

	size_type erase(const key_type& k) {
		size_type bk = bkt_num_key(k);
		node_ptr prev = 0;
		node_ptr ptr = buckets[bk];
		node_ptr tmp;
		size_type ret = 0;
		while (ptr) {
			if (equals(k, get_key(ptr->val))) {
				tmp = ptr;
				if (prev) {
					prev->next = (node_ptr&)ptr->next;
					prev = ptr;
				}
				else
					buckets[bk] = (node_ptr&)ptr->next;
				ptr = (node_ptr&)ptr->next;
				delete_node(tmp);
				++ret;
			}
			else {
				prev = ptr;
				ptr = (node_ptr&)ptr->next;
			}
		}
		return ret;
	}
private:
	void init_buckets(size_type n) {
		const size_type n_buckets = _next_prime(n);
		buckets.reserve(n_buckets);
		buckets.insert(buckets.begin(), n_buckets, (node_ptr)0);
		num_elements = 0;
	}

	void resize(size_type num) {
		const size_type old = buckets.size();
		if (num > old) {
			const size_type nlen = _next_prime(num);
			vector<node_ptr> tmp(nlen, (node*)0);
			try {
				node_ptr optr;
				size_type new_bk;
				for (size_type bk = 0; bk < old; ++bk) {
					optr = buckets[bk];
					while (optr) {
						new_bk = bkt_num(optr->val, nlen);
						buckets[bk] = (node_ptr&)optr->next;
						optr->next = tmp[new_bk];
						tmp[new_bk] = optr;
						optr = buckets[bk];
					}
				}
				buckets.swap(tmp);
			}
			catch (...) {
				__THROW_RUNTIME_ERROR(1, "Error when trying to resize the bucket");
			}
		}
	}

	size_type bkt_num(const value_type& obj) const {
		return hash(get_key(obj)) % (buckets.size());
	}

	size_type bkt_num(const value_type& obj, size_type n ) const {
		return hash(get_key(obj)) % n;
	}

	size_type bkt_num_key(const key_type& k) const {
		return hash(k) % (buckets.size());
	}

	size_type bkt_num_key(const key_type& k, size_type n ) const {
		return hash(k) % n;
	}

};

template <typename K, typename V, typename HF, typename ExK, typename EqK, typename A>
inline hashtable_iterator<K, V, HF, ExK, EqK, A>& hashtable_iterator<K, V, HF, ExK, EqK, A>::operator++() {
	const base_ptr old = cur;
	cur = cur->next;
	if (!cur) {
		size_type bucket = ht->bkt_num(((node_ptr&)old)->val);
		size_type len = ht->buckets.size();
		while (!cur && (++bucket < len))
			cur = ht->buckets[bucket];
	}
	return *this;
}

template <typename K, typename V, typename HF, typename ExK, typename EqK, typename A>
inline hashtable_iterator<K, V, HF, ExK, EqK, A> hashtable_iterator<K, V, HF, ExK, EqK, A>::operator++(int) {
	iterator tmp = *this;
	++* this;
	return tmp;
}

template <typename K, typename V, typename HF, typename ExK, typename EqK, typename A>
inline hashtable_const_iterator<K, V, HF, ExK, EqK, A> hashtable_const_iterator<K, V, HF, ExK, EqK, A>::operator++() {
	const base_ptr old = cur;
	cur = cur->next;
	if (!cur) {
		size_type bucket = ht->bkt_num(((node_ptr&)old)->val);
		size_type len = ht->buckets.size();
		while (!cur && (++bucket < len))
			cur = ht->buckets[bucket];
	}
	return *this;
}

template <typename K, typename V, typename HF, typename ExK, typename EqK, typename A>
inline hashtable_const_iterator<K, V, HF, ExK, EqK, A> hashtable_const_iterator<K, V, HF, ExK, EqK, A>::operator++(int) {
	const_iterator tmp = *this;
	++* this;
	return tmp;
}

}

#endif // !__LMSTL_HASHTABLE_H__
