#ifndef __LMSTL_RB_TREE_H__
#define __LMSTL_RB_TREE_H__

#include "alloc.h"
#include "construct.h"
#include "exceptdef.h"
#include "iterator.h"
#include "utility.h"
#include <stddef.h>

namespace lmstl {

typedef bool rb_tree_color;

const rb_tree_color rb_red = false;
const rb_tree_color rb_black = true;

struct rb_tree_node_base{
	typedef rb_tree_node_base* base_ptr;
	base_ptr parent;
	base_ptr left;
	base_ptr right;
	rb_tree_color color;

	static base_ptr minimum(base_ptr x) {
		while (x->left)
			x = x->left;
		return x;
	}
	
	static base_ptr maximum(base_ptr x) {
		while (x->right)
			x = x->right;
		return x;
	}

	static base_ptr 
		imum(base_ptr x) {
		while (x->right)
			x = x->right;
		return x;
	}
};

template <typename T>
struct rb_tree_node : public rb_tree_node_base {
	typedef rb_tree_node* node_ptr;
	T value;
	rb_tree_node(const T& x):
		rb_tree_node_base(), value(x) {}
	rb_tree_node():
		rb_tree_node_base(), value() {}
};

struct rb_tree_iterator_base {
	typedef rb_tree_node_base::base_ptr base_ptr;
	typedef bidirectional_iterator_tag iterator_category;
	base_ptr node;
	rb_tree_iterator_base(base_ptr x = NULL):
		node(x) {}
};

template <typename T, typename Ref, typename Ptr>
struct rb_tree_iterator : public rb_tree_iterator_base {

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;

	typedef rb_tree_iterator<T, T&, T*> iterator;
	typedef rb_tree_iterator<T, const T&, const T*> const_iterator;
	typedef rb_tree_iterator self;

	typedef rb_tree_node_base::base_ptr base_ptr;
	typedef rb_tree_node<T>* node_ptr;

	bool operator!=(const self& rhs) const {
		return node != rhs.node;
	}

	bool operator==(const self& rhs) const {
		return node == rhs.node;
	}

	self& operator++() {
		if (node->right) {
			node = node->right;
			while (node->left)
				node = node->left;
		}
		else {
			base_ptr y = node->parent;
			while (node == y->right) {
				node = y;
				y = y->parent;
			}
			if (node->right != y)
				node = y;
		}
		return *this;
	}

	self operator++(int) {
		self tmp = *this;
		operator++();
		return tmp;
	}

	self& operator--() {
		if (node->color == rb_red && node->parent->parent == node)
			node = node->right;
		else if (node->left) {
			node = node->left;
			while (node->right)
				node = node->right;
		}
		else {
			base_ptr y = node->parent;
			while (node == y->left) {
				node = y;
				y = y->parent;
			}
			node = y;
		}
		return *this;
	}

	self operator--(int) {
		self tmp = *this;
		operator--();
		return tmp;
	}

	reference operator*() const { return ((node_ptr)node)->value; }
	pointer operator->() const { return &(operator*()); }

	rb_tree_iterator() {}
	rb_tree_iterator(node_ptr x):
		rb_tree_iterator_base(x) {}

	rb_tree_iterator(const iterator &it):
		rb_tree_iterator_base(it.node) {}
	rb_tree_iterator(const const_iterator& it) :
		rb_tree_iterator_base(it.node) {}
};

inline void rb_tree_rotate_left(rb_tree_node_base* x, rb_tree_node_base*& root) {
	rb_tree_node_base* y = x->right;
	x->right = y->left;
	if (y->left)
		y->left->parent = x;
	y->parent = x->parent;
	if (x == root)
		root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

inline void rb_tree_rotate_right(rb_tree_node_base* x, rb_tree_node_base*& root) {
	rb_tree_node_base* y = x->left;
	x->left = y->right;
	if (y->right)
		y->right->parent = x;
	y->parent = x->parent;
	if (x == root)
		root = y;
	else if (x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;
	y->right = x;
	x->parent = y;
}

inline void rb_tree_rebalance(rb_tree_node_base* x, rb_tree_node_base*& root) {
	x->color = rb_red;
	while (x != root && x->parent->color == rb_red) {
		if (x->parent == x->parent->parent->left) {
			rb_tree_node_base* tmp = x->parent->parent->right;
			if (tmp && tmp->color == rb_red) {
				tmp->color = x->parent->color = rb_black;
				x->parent->parent->color = rb_red;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->right) {
					x = x->parent;
					rb_tree_rotate_left(x, root);
				}
				x->parent->color = rb_black;
				x->parent->parent->color = rb_red;
				rb_tree_rotate_right(x->parent->parent, root);
			}
		}
		else {
			rb_tree_node_base* tmp = x->parent->parent->left;
			if (tmp && tmp->color == rb_red) {
				tmp->color = x->parent->color = rb_black;
				x->parent->parent->color = rb_red;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->left) {
					x = x->parent;
					rb_tree_rotate_right(x, root);
				}
				x->parent->color = rb_black;
				x->parent->parent->color = rb_red;
				rb_tree_rotate_left(x->parent->parent, root);
			}
		}
	}
	root->color = rb_black;
}

inline rb_tree_node_base *rb_tree_erase_and_rebalance(rb_tree_node_base* x, rb_tree_node_base*& root, rb_tree_node_base *&leftmost, rb_tree_node_base*& rightmost) {
	rb_tree_node_base* y = x;
	rb_tree_node_base* z, * zp = NULL;
	if (x->left && x->right) {
		y = x->right;
		while (y->left)
			y = y->left;
	}
	z = y->left ? y->left : y->right;
	if (y != x) {
		x->left->parent = y;
		y->left = x->left;

		if (y != x->right) {
			zp = y->parent;
			if (z)
				z->parent = y->parent;
			y->parent->left = z;
			y->right = x->right;
			x->right->parent = y;
		}
		else
			zp = y;

		if (root == x)
			root = y;
		else if (x->parent->left == x)
			x->parent->left = y;
		else
			x->parent->right = y;

		y->parent = x->parent;
		lmstl::swap(y->color, x->color);
		y = x;
	}
	else {
		zp = y->parent;
		if (z)
			z->parent = y->parent;

		if (root == x)
			root = y;
		else if (x->parent->left == x)
			x->parent->left = z;
		else
			x->parent->right = z;

		if (leftmost == x)
			leftmost = (z == 0) ? zp : rb_tree_node_base::minimum(zp);
		if (rightmost == x)
			rightmost = (z == 0) ? zp : rb_tree_node_base::maximum(zp);
	}

	if (y->color == rb_black) {
		while (z != root && (!z || z->color == rb_black)) {
			if (z == zp->left) {
				auto sib = zp->right;
				if (sib->color == rb_red) {
					sib->color = rb_black;
					zp->color = rb_red;
					rb_tree_rotate_left(zp, root);
					sib = zp->right;
				}

				if ((!zp->left || zp->left->color == rb_black) && (!zp->right || zp->right->color == rb_black)) {
					sib->color = rb_red;
					z = zp;
					zp = zp->parent;
				}
				else {
					if (!sib->right || sib->right->color == rb_black) {
						if (sib->left)
							sib->left->color = rb_black;
						sib->color = rb_red;
						rb_tree_rotate_right(sib, root);
						sib = zp->right;
					}
					sib->color = zp->color;
					zp->color = rb_black;
					if (sib->right)
						sib->right->color = rb_black;
					rb_tree_rotate_left(zp, root);
					break;
				}
			}
			else {
				auto sib = zp->left;
				if (sib->color == rb_red) {
					sib->color = rb_black;
					zp->color = rb_red;
					rb_tree_rotate_right(zp, root);
					sib = zp->left;
				}
				if ((!sib->left || sib->left->color == rb_black) && (!sib->right || sib->right->color == rb_black)) {
					sib->color = rb_red;
					z = zp;
					zp = zp->parent;
				}
				else {
					if (!sib->left || sib->left->color == rb_red) {
						if (sib->right)
							sib->right->color = rb_black;
						sib->color = rb_red;
						rb_tree_rotate_left(sib, root);
						sib = zp->left;
					}
					sib->color = zp->color;
					zp->color = rb_black;
					if (sib->left)
						sib->left->color = rb_black;
					rb_tree_rotate_right(zp, root);
					break;
				}
			}
		}
		if (z)
			z->color = rb_black;
	}
	return y;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
class rb_tree {
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef rb_tree_iterator<value_type, reference, pointer> iterator;
	typedef rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

protected:
	typedef rb_tree_node_base* base_ptr;
	typedef rb_tree_node<value_type>* node_ptr;
	typedef rb_tree_node<value_type> rb_tree_node;
	typedef simple_alloc<rb_tree_node, Alloc> node_allocator;
	
	node_ptr get_node() { return node_allocator::allocate(1); }
	void put_node(node_ptr p) { return node_allocator::deallocate(p, 1); }

	node_ptr create_node(const value_type& val) {
		node_ptr ret = get_node();
		try {
			construct(ret, val);
		}
		catch (...) {
			node_allocator::deallocate(ret, 1);
			__THROW_RUNTIME_ERROR(1, "Error when constructing");
		}
		return ret;
	}

	node_ptr clone_node(node_ptr x) {
		node_ptr ret = create_node(x->value);
		ret->left = ret->right = 0;
		ret->color = x->color;
		return ret;
	}

	void destroy_node(node_ptr x) {
		destroy(x);
		put_node(x);
	}

	node_ptr _copy(node_ptr x, node_ptr p) {
		node_ptr top = clone_node(x);
		top->parent = p;
		try {
			if (x->left)
				top->left = _copy((node_ptr&)x->left, top);
			if (x->right)
				top->right = _copy((node_ptr&)x->right, top);
		}
		catch (...) {
			erase_since(p);
			__THROW_RUNTIME_ERROR(1, "Error when copying");
		}
		return top;
	}

protected:
	size_type node_count;
	node_ptr header;
	Compare key_compare;

	node_ptr& root() const { return (node_ptr&)header->parent; }
	node_ptr& leftmost() const { return (node_ptr&)header->left; }
	node_ptr& rightmost() const { return (node_ptr&)header->right; }

	static const key_type& key(base_ptr x) { return KeyOfValue()(((node_ptr&)x)->value); }
	static const key_type& key(node_ptr x) { return KeyOfValue()(x->value); }

	static node_ptr minimum(node_ptr x) {
		return (node_ptr)rb_tree_node_base::minimum(x);
	}
	static node_ptr maximum(node_ptr x) {
		return (node_ptr)rb_tree_node_base::maximum(x);
	}

	void init() {
		header = create_node(value_type());
		header->left = header->right = header;
		header->parent = NULL;
		header->color = rb_red;
	}

	void erase_since(base_ptr x) {
		while (x) {
			erase_since(x->right);
			base_ptr y = x->left;
			destroy_node((node_ptr&)x);
			x = y;
			--node_count;
		}
	}

public:
	rb_tree(const Compare &comp = Compare()):
		node_count(0), key_compare(comp) {
		init();
	}

	~rb_tree() {
		erase_since(root());
		destroy_node(header);
	}

	rb_tree& operator=(const rb_tree& x) {
		erase_since(root());
		node_ptr root = _copy(x.root(), header);
		node_count = x.node_count;
		key_compare = x.key_compare;
		header->parent = root;
		header->left = rb_tree_node_base::minimum(root);
		header->right = rb_tree_node_base::maximum(root);
		return *this;
	}

	void clear() {
		erase_since(root());
		header->parent = 0;
		header->left = header->right = header;
	}

	iterator begin() { return leftmost(); }
	const_iterator begin() const { return leftmost(); }
	const_iterator cbegin() const { return leftmost(); }
	iterator end() { return header; }
	const_iterator end() const { return header; }
	const_iterator cend() const { return header; }

	size_type size() const { return node_count; }
	bool empty() const { return !node_count; }
	Compare key_comp() const { return key_compare; }
	size_type count(const Key& x) const {
		pair<const_iterator, const_iterator> p = equal_range(x);
		return distance(p.first, p.second);
	}

	void erase(iterator x) {
		base_ptr del = rb_tree_erase_and_rebalance(x.node, header->parent, header->left, header->right);
		destroy_node((node_ptr&)del);
		--node_count;
	}

	void erase(iterator beg, iterator end) {
		while (beg != end)
			erase(beg++);
	}

	size_type erase(const Key& k) {
		pair<iterator, iterator> p = equal_range(k);
		size_type n = 0;
		n = distance(p.first, p.second);
		while (p.first != p.second)
			erase(p.first++);
		return n;
	}

	iterator find(const Key& k) {
		base_ptr prev = header;
		base_ptr curr = root();

		while (curr) {
			if (!key_compare(key(curr), k)) {
				prev = curr;
				curr = curr->left;
			}
			else
				curr = curr->right;
		}
		iterator j = iterator((node_ptr&)prev);
		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}

	const_iterator find(const Key& k) const {
		base_ptr prev = header;
		base_ptr curr = root();

		while (curr) {
			if (!key_compare(key(curr), k)) {
				prev = curr;
				curr = curr->left;
			}
			else
				curr = curr->right;
		}
		const_iterator j = const_iterator((node_ptr&)prev);
		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}

	iterator lower_bound(const Key& k) {
		base_ptr prev = header;
		base_ptr curr = root();
		
		while (curr) {
			if (!key_compare(key(curr), k)) {
				prev = curr;
				curr = curr->left;
			}
			else
				curr = curr->right;
		}
		return (node_ptr&)prev;
	}

	const_iterator lower_bound(const Key& k) const {
		base_ptr prev = header;
		base_ptr curr = root();

		while (curr) {
			if (!key_compare(key(curr), k)) {
				prev = curr;
				curr = curr->left;
			}
			else
				curr = curr->right;
		}
		return (node_ptr&)prev;
	}

	iterator upper_bound(const Key& k) {
		base_ptr prev = header;
		base_ptr curr = root();

		while (curr) {
			if (key_compare(k, key(curr))) {
				prev = curr;
				curr = curr->left;
			}
			else
				curr = curr->right;
		}
		return (node_ptr&)prev;
	}

	const_iterator upper_bound(const Key& k) const {
		base_ptr prev = header;
		base_ptr curr = root();

		while (curr) {
			if (key_compare(k, key(curr))) {
				prev = curr;
				curr = curr->left;
			}
			else
				curr = curr->right;
		}
		return (node_ptr&)prev;
	}

	pair<iterator, iterator> equal_range(const Key& k) {
		return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
	}

	pair<const_iterator, const_iterator> equal_range(const Key& k) const {
		return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
	}

	iterator insert_equal(const value_type& val) {
		node_ptr prev = header;
		node_ptr curr = root();
		while (curr) {
			prev = curr;
			curr = (node_ptr&)(key_compare(KeyOfValue()(val), key(curr)) ? curr->left : curr->right);
		}
		return __insert(prev, val);
	}

	template <typename InputIterator>
	void insert_equal(InputIterator beg, InputIterator end) {
		for (; beg != end; ++beg)
			insert_equal(*beg);
	}

	pair<iterator, bool> insert_unique(const value_type& val) {
		node_ptr prev = header;
		node_ptr curr = root();
		bool comp = true;
		while (curr) {
			prev = curr;
			comp = key_compare(KeyOfValue()(val), key(curr));
			curr = (node_ptr&)(comp ? curr->left : curr->right);
		}
		iterator tmp = iterator(prev);
		if (comp) {
			if(prev == leftmost())
				return pair<iterator, bool>(__insert(prev, val), true);
			--tmp;
		}
		if (key_compare(key(tmp.node), KeyOfValue()(val)))
			return pair<iterator, bool>(__insert(prev, val), true);
		return pair<iterator, bool>(tmp, false);
	}

	template <typename InputIterator>
	void insert_unique(InputIterator beg, InputIterator end) {
		for (; beg != end; ++beg)
			insert_unique(*beg);
	}

private:
	iterator __insert(base_ptr pos_, const value_type& val) {
		node_ptr pos = (node_ptr&)pos_;
		node_ptr tar = create_node(val);
		if (pos == header) {
			pos->left = tar;
			root() = tar;
			rightmost() = tar;
		}
		else if (key_compare(KeyOfValue()(val), key(pos))) {
			pos->left = tar;
			if(pos == leftmost())
				leftmost() = tar;
		}
		else {
			pos->right = tar;
			if (pos == rightmost())
				rightmost() = tar;
		}
		tar->parent = pos;
		tar->left = tar->right = 0;

		rb_tree_rebalance(tar, header->parent);
		++node_count;
		return iterator(tar);
	}
};

}
#endif // !__LMSTL_RB_TREE_H__
