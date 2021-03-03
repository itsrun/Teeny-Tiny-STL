#ifndef __LMSTL_FUNCTIONAL_H__
#define __LMSTL_FUNCTIONAL_H__

namespace lmstl {

template <typename Arg, typename Result>
struct unary_function {
	typedef Arg argument_type;
	typedef Result result_type;
};

template <typename Arg1, typename Arg2, typename Result>
struct binary_function {
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};

template <typename T>
struct plus : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x + y; }
};

template <typename T>
struct minus : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x - y; }
};

template <typename T>
struct multiplies : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x * y; }
};

template <typename T>
struct divides : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x / y; }
};

template <typename T>
struct modulus : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x % y; }
};

template <typename T>
struct negate : public unary_function<T, T> {
	T operator()(const T& x) const { return -x; }
};

template <typename T>
struct equal_to : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x == y; }
};

template <typename T>
struct not_equal_to : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x != y; }
};

template <typename T>
struct greater : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x > y; }
};

template <typename T>
struct less : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x < y; }
};

template <typename T>
struct greater_equal : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x >= y; }
};

template <typename T>
struct less_equal : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x <= y; }
};

template <typename T>
struct logical_and : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x && y; }
};

template <typename T>
struct logical_or : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x || y; }
};

template <typename T>
struct logical_not : public unary_function<T, bool> {
	bool operator()(const T& x) const { return !x; }
};

template <typename T>
struct identity : public unary_function<T, T> {
	const T& operator()(const T& x) const { return x; }
};

template <typename Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type> {
	const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

template <typename Pair>
struct select2nd : public unary_function<Pair, typename Pair::second_type> {
	const typename Pair::second_type& operator()(const Pair& x) const { return x.second; }
};

template <typename Arg1, typename Arg2>
struct project1st : public binary_function<Arg1, Arg2, Arg1> {
	Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
};

template <typename Arg1, typename Arg2>
struct project2nd : public binary_function<Arg1, Arg2, Arg2> {
	Arg2 operator()(const Arg1&, const Arg2& y) const { return y; }
};

template <typename Key>
struct hash {};

inline size_t hash_string(const char* s) {
	unsigned long h = 0;
	for (; *s; ++s)
		h = 5 * h + *s;
	return size_t(h);
}

template <>
struct hash<char*> {
	size_t operator()(const char* s) const { return hash_string(s); }
};
template <>
struct hash<const char*> {
	size_t operator()(const char* s) const { return hash_string(s); }
};
template <>
struct hash<char> {
	size_t operator()(char x) const { return x; }
};
template <>
struct hash<unsigned char> {
	size_t operator()(unsigned char x) const { return x; }
};
template <>
struct hash<signed char> {
	size_t operator()(signed char x) const { return x; }
};
template <>
struct hash<short> {
	size_t operator()(short x) const { return x; }
};
template <>
struct hash<unsigned short> {
	size_t operator()(unsigned short x) const { return x; }
};
template <>
struct hash<int> {
	size_t operator()(int x) const { return x; }
};
template <>
struct hash<unsigned int> {
	size_t operator()(unsigned int x) const { return x; }
};
template <>
struct hash<long> {
	size_t operator()(long x) const { return x; }
};
template <>
struct hash<unsigned long> {
	size_t operator()(unsigned long x) const { return x; }
};

template <typename Operation>
class binder1st : public unary_function<typename Operation::first_argument_type, typename Operation::result_type> {
protected:
	Operation op;
	typename Operation::first_argument_type value;

public:
	binder1st(const Operation& oper, const typename Operation::first_argument_type& x):
		op(oper), value(x) {}
	typename Operation::result_type operator()(const typename Operation::second_argument_type& y) const {
		return op(value, y);
	}
};

template <typename Operation, typename T>
inline binder1st<Operation> bind1st(const Operation& op, const T& x) {
	typedef typename Operation::first_argument_type arg1_type;
	return binder1st<Operation>(op, arg1_type(x));
}

template <typename Operation>
class binder2nd : public unary_function<typename Operation::second_argument_type, typename Operation::result_type> {
protected:
	Operation op;
	typename Operation::second_argument_type value;

public:
	binder2nd(const Operation& oper, const typename Operation::second_argument_type& y):
		op(oper), value(y) {}
	typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const {
		return op(x, value);
	}
};

template <typename Operation, typename T>
inline binder2nd<Operation> bind2nd(const Operation& op, const T& y) {
	typedef typename Operation::second_argument_type arg2_type;
	return binder2nd<Operation>(op, arg2_type(y));
}

template <typename Predicate>
class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
protected:
	Predicate pred;

public:
	unary_negate(const Predicate& upred):
		pred(upred) {}
	bool operator()(const typename Predicate::argument_type& x) const {
		return !pred(x);
	}
};

template <typename Predicate>
inline unary_negate<Predicate> not1(const Predicate& pred) {
	return unary_negate<Predicate>(pred);
}

template <typename Predicate>
class binary_negate : public binary_function<
	typename Predicate::first_argument_type, typename Predicate::second_argument_type, bool> {
protected:
	Predicate pred;

public:
	binary_negate(const Predicate& upred):
		pred(upred) {}
	bool operator()(const typename Predicate::first_argument_type& x, const typename Predicate::second_argument_type& y) const {
		return !pred(x, y);
	}
};

template <typename Operation1, typename Operation2>
class unary_compose : public unary_function<typename Operation2::argument_type, typename Operation1::result_type> {
protected:
	Operation1 op1;
	Operation2 op2;

public:
	unary_compose(const Operation1& x, const Operation2& y):
		op1(x), op2(y) {}
	typename Operation1::result_type operator()(const typename Operation2::argument_type& x) const {
		return op1(op2(x));
	}
};

template <typename Operation1, typename Operation2>
inline unary_compose<Operation1, Operation2> compose1(const Operation1& op1, const Operation2& op2) {
	return unary_compose<Operation1, Operation2>(op1, op2);
}

template <typename Operation1, typename Operation2, typename Operation3>
class binary_compose : public unary_function<typename Operation2::argument_type, typename Operation1::result_type> {
protected:
	Operation1 op1;
	Operation2 op2;
	Operation3 op3;

public:
	binary_compose(const Operation1&x, const Operation2&y, const Operation3&z):
		op1(x), op2(y), op3(z) {}

	typename Operation1::result_type operator()(const typename Operation2::argument_type& x) const {
		return op1(op2(x), op3(x));
	}
};

template <typename Operation1, typename Operation2, typename Operation3>
inline binary_compose<Operation1, Operation2, Operation3> compose2(const Operation1& op1, const Operation2& op2, const Operation3& op3) {
	return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
}

template <typename Arg, typename Result>
class pointer_to_unary_function : public unary_function<Arg, Result> {
protected:
	Result(*ptr)(Arg);

public:
	pointer_to_unary_function(){}
	explicit pointer_to_unary_function(Result (*x)(Arg)):
		ptr(x) {}
	Result operator()(Arg x) const { return ptr(x); }
};

template <typename Arg, typename Result>
inline pointer_to_unary_function<Arg, Result> ptr_fun(Result(*x)(Arg)) {
	return pointer_to_unary_function<Arg, Result>(x);
}

template <typename Arg1, typename Arg2, typename Result>
class pointer_to_binary_function : public binary_function<Arg1, Arg2, Result> {
protected:
	Result(*ptr)(Arg1, Arg2);

public:
	pointer_to_binary_function(){}
	explicit pointer_to_binary_function(Result (*x)(Arg1, Arg2)):
		ptr(x) {}
	Result operator()(Arg1 x, Arg2 y) const { return ptr(x, y); }
};

template <typename Arg1, typename Arg2, typename Result>
inline pointer_to_binary_function<Arg1, Arg2, Result> ptr_fun(Result(*x)(Arg1, Arg2)) {
	return pointer_to_binary_function<Arg1, Arg2, Result>(x);
}

}

#endif // !__LMSTL_FUNCTIONAL_H__