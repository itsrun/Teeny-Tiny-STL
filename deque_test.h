#ifndef __LMSTL_DEQUE_TEST_H__
#define __LMSTL_DEQUE_TEST_H__

#include "deque.h"
#include <iostream>
using std::cout;
using std::endl;

namespace lmstl {

void deque_test() {
	deque<int> d1;
	deque<int> d2(5, 8);
	for (auto i = d2.begin(); i != d2.cend(); ++i)
		cout << *i << " ";
	cout << endl;
	auto i = d2.cend();
	cout << (i == d2.begin()) << endl;
	d1.push_back(*d2.cbegin());
	auto p = d2.begin();
	++p;
	++p;
	d1.push_front(*p);
	d1.push_back(1);
	d1.push_back(6);
	d1.insert(d1.cbegin(), 9);
	for(auto i = d1.rbegin();i!=d1.rend();++i)
		cout << *i << " ";
	cout << endl;
}
}
#endif // !__LMSTL_DEQUE_TEST_H__
