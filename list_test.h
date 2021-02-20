#ifndef __LMSTL_LIST_TEST_H__
#define __LMSTL_LIST_TEST_H__

#include "list.h"
#include "test_frame.h"
#include <list>

namespace lmstl {

void list_test(){
	list<int> ml2(2, 7);
	std::list<int> sl2(2, 7);
	ml2.push_back(1);
	sl2.push_back(1);
	list<int> ml(ml2);
	std::list<int> sl(sl2);
	ml2.insert(ml2.begin(), 3);
	sl2.insert(sl2.begin(), 3);
	API_TEST_START();
	API_TEST01(ml, sl, push_back, 6);
	API_TEST01(ml, sl, push_front, 5);
	API_TEST_EACH03(ml, sl, insert, ml.cbegin(), ml2.rbegin(), ml2.rend(), sl.cbegin(), sl2.rbegin(), sl2.rend());
	API_TEST_EACH01(ml, sl, erase, ml.cbegin(), sl.cbegin());
	API_TEST01(ml, sl, pop_back, );
	API_TEST01(ml, sl, unique, );
	API_TEST01(ml, sl, remove, 7);
	list<int> ml3(std::move(ml2));
	std::list<int> sl3(std::move(sl2));
	auto mp = ml.begin();
	auto sp = sl.begin();
	++mp; ++sp;
	API_TEST_EACH02(ml, sl, splice, mp, ml3, sp, sl3);
	API_TEST01(ml, sl, reverse, );
	API_TEST01(ml, sl, sort, );
	list<int> ml4(2, 5);
	std::list<int> sl4(2, 5);
	API_TEST_EACH01(ml, sl, merge, ml4, sl4);
	API_TEST_END();
	PERF_TEST(list);
}
}

#endif // !__LMSTL_LIST_TEST_H__
