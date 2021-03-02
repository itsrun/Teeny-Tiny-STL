#ifndef __LMSTL_MAP_TEST_H__
#define __LMSTL_MAP_TEST_H__

#include "map.h"
#include "vector.h"
#include "iterator.h"
#include <map>

namespace lmstl {

void map_test(){
	map<int, int> mm;
	pair<int, int> tp(pair<int, int>(1, 2));
	std::map<int, int> sm;
	mm.insert(tp);
	sm.insert(std::pair<int, int>(1, 2));
	vector<pair<int, int>> mv;
	vector<std::pair<int, int>> sv;
	for (int i = 0; i < 5; ++i) {
		mv.push_back(pair<int, int>(i, i+1));
		sv.push_back(std::pair<int, int>(i, i+1));
	}
	API_TEST_START();
	cout << "[-------------------- Container test : map ---------------------]\n";
	API_TEST_EACH2(mm, sm, insert, mv.begin(), mv.end(), sv.begin(), sv.end());
	pair<int, int> mp1(9, 99);
	std::pair<int, int> sp1(9, 99);
	API_TEST_EACH1(mm, sm, insert, mp1, sp1);
	API_TEST_EACH1(mm, sm, emplace, mp1, sp1);
	API_TEST_EACH1(mm, sm, erase, mm.begin(), sm.begin());
	API_TEST_EACH1(mm, sm, insert, mp1, sp1);
	API_TEST_EACH1(mm, sm, emplace, mp1, sp1);
	for (int i = 0; i < 5; ++i) {
		mv.push_back(pair<int, int>(i+3, i + 1));
		sv.push_back(std::pair<int, int>(i+3, i + 1));
	}
	map<int, int> mm2(mv.cbegin(), mv.cend());
	std::map<int, int> sm2(sv.cbegin(), sv.cend());
	API_TEST_EACH2(mm, sm, insert, mm2.cbegin(), mm2.cend(), sm2.cbegin(), sm2.cend());
	API_TEST01(mm, sm, erase, 3);
	auto mi = mm.begin(); ++mi; ++mi;
	auto si = sm.begin(); ++si; ++si;
	API_TEST_EACH1(mm, sm, erase, mi, si);
	API_TEST_END();
	PERF_TEST_START();
	cout << "[-------------------- Container test : map ---------------------]\n";
	MAP_PERF_TEST1(map, insert, 100000, 500000, 1000000);
	MAP_PERF_TEST1(map, emplace, 100000, 500000, 1000000);
	MAP_PERF_TEST2(map, operator[], 100000, 500000, 1000000);
	PERF_TEST_END();
	multimap<int, int> mmm;
}

}
#endif // !__LMSTL_MAP_TEST_H__
