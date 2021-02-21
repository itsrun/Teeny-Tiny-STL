#ifndef __LMSTL_VECTOR_TEST_H__
#define __LMSTL_VECTOR_TEST_H__

#include "test_frame.h"
#include "vector.h"
#include <vector>
#include <string>

namespace lmstl {

void vector_test() {
	vector<int> mv2(2, 7);
	std::vector<int> sv2(2, 7);
	vector<int> mv(mv2.cbegin(), mv2.cend());
	std::vector<int> sv(sv2.cbegin(), sv2.cend());
	mv2.push_back(1);
	sv2.push_back(1);
	mv2.insert(mv2.begin(), 3);
	sv2.insert(sv2.begin(), 3);
	API_TEST_START();
	cout << "[------------------- Container test : vector -------------------]\n";
	API_TEST01(mv, sv, push_back, 6);
	API_TEST13(mv, sv, insert, begin, , 0, 3, 5);
	API_TEST12(mv, sv, insert, begin, , 1, 7);
	API_TEST33(mv, sv, insert, begin, rbegin, rend, , 2, , 0, , 0);
	API_TEST22(mv, sv, erase, begin, begin, , 1, , 3);
	API_TEST_EACH3(mv, sv, insert, mv.cbegin(), mv2.rbegin(), mv2.rend(), sv.cbegin(), sv2.rbegin(), sv2.rend());
	API_TEST11(mv, sv, erase, begin, , 2);
	API_TEST01(mv, sv, pop_back, );
	vector<int> mv3(mv2);
	std::vector<int> sv3(sv2);
	API_TEST_EACH1(mv, sv, swap, mv3, sv3);
	vector<int> mv4(std::move(mv3));
	std::vector<int> sv4(std::move(sv3));
	API_TEST_EACH1(mv, sv, swap, mv4, sv4);
	mv.clear();
	sv.clear();
	vector<std::string> msv2(3, "A");
	std::vector<std::string> ssv2(3, "A");
	vector<std::string> msv(msv2);
	std::vector<std::string> ssv(ssv2);
	API_TEST01(msv, ssv, push_back, "hi");
	API_TEST13(msv, ssv, insert, begin, , 0, 2, "ins");
	API_TEST12(msv, ssv, insert, begin, , 1, "ins");
	API_TEST33(msv, ssv, insert, begin, rbegin, rend, , 2, , 0, , 0);
	API_TEST22(msv, ssv, erase, begin, begin, , 1, , 3);
	API_TEST_EACH3(msv, ssv, insert, msv.cbegin(), msv2.rbegin(), msv2.rend(), ssv.cbegin(), ssv2.rbegin(), ssv2.rend());
	API_TEST11(msv, ssv, erase, begin, , 2);
	API_TEST01(msv, ssv, pop_back, );
	msv.clear();
	ssv.clear();
	API_TEST_END();
	PERF_TEST_START();
	cout << "[------------------- Container test : vector -------------------]\n";
	PERF_TEST00(vector, push_back, 500000, 5000000, 50000000);
	PERF_TEST11(vector, insert, begin, 20000, 100000, 150000);
	PERF_TEST_END();
}

}

#endif // !__LMSTL_VECTOR_TEST_H__
