#ifndef __LMSTL_LIST_TEST_H__
#define __LMSTL_LIST_TEST_H__

#include "list.h"
#include "forward_list.h"
#include <forward_list>
#include "test_frame.h"
#include <list>

namespace lmstl {

void list_test(){
	/*
	list<int> ml2(2, 7);
	std::list<int> sl2(2, 7);
	ml2.push_back(1);
	sl2.push_back(1);
	list<int> ml(ml2);
	std::list<int> sl(sl2);
	ml2.insert(ml2.begin(), 3);
	sl2.insert(sl2.begin(), 3);
	API_TEST_START();
	cout << "[-------------------- Container test : list --------------------]\n";
	API_TEST01(ml, sl, push_back, 6);
	API_TEST01(ml, sl, push_front, 5);
	API_TEST_EACH3(ml, sl, insert, ml.cbegin(), ml2.rbegin(), ml2.rend(), sl.cbegin(), sl2.rbegin(), sl2.rend());
	API_TEST_EACH1(ml, sl, erase, ml.cbegin(), sl.cbegin());
	API_TEST01(ml, sl, pop_back, );
	API_TEST01(ml, sl, unique, );
	API_TEST01(ml, sl, remove, 7);
	list<int> ml3(std::move(ml2));
	std::list<int> sl3(std::move(sl2));
	auto mp = ml.begin();
	auto sp = sl.begin();
	++mp; ++sp;
	API_TEST_EACH2(ml, sl, splice, mp, ml3, sp, sl3);
	API_TEST01(ml, sl, reverse, );
	API_TEST01(ml, sl, sort, );
	list<int> ml4(2, 5);
	std::list<int> sl4(2, 5);
	API_TEST_EACH1(ml, sl, swap, ml4, sl4);
	API_TEST_EACH1(ml, sl, swap, ml4, sl4);
	API_TEST_EACH1(ml, sl, merge, ml4, sl4);
	API_TEST_END();
	*/
	PERF_TEST_START();
	cout << "[-------------------- Container test : list --------------------]\n";
	PERF_TEST00(list, push_back, 500000, 5000000, 10000000);
	PERF_TEST11(list, insert, begin, 500000, 5000000, 10000000);
	PERF_TEST00(list, push_front, 500000, 5000000, 10000000);
	PERF_TEST_END();

	forward_list<int> mfl2(2, 11);
	std::forward_list<int> sfl2(2, 11);
	forward_list<int> mfl(mfl2);
	std::forward_list<int> sfl(sfl2);
	API_TEST_START();
	cout << "[---------------- Container test : forward_list ----------------]\n";
	API_TEST01(mfl, sfl, push_front, 5);
	//API_TEST_EACH3(mfl, sfl, insert_after, mfl.cbegin(), ml.rbegin(), ml.rend(), sfl.cbegin(), sl.rbegin(), sl.rend());
	API_TEST_EACH1(mfl, sfl, erase_after, mfl.cbegin(), sfl.cbegin());
	API_TEST01(mfl, sfl, pop_front, );
	API_TEST_EACH1(mfl, sfl, swap, mfl2, sfl2);
	API_TEST_EACH1(mfl, sfl, swap, mfl2, sfl2);
	API_TEST_END();
	PERF_TEST_START();
	cout << "[---------------- Container test : forward_list ----------------]\n";
	PERF_TEST00(forward_list, push_front, 500000, 5000000, 10000000);
	PERF_TEST11(forward_list, insert_after, cbefore_begin, 100000, 1000000, 5000000);
	PERF_TEST_END();
}
}

#endif // !__LMSTL_LIST_TEST_H__
