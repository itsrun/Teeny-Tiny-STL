#include "vector_test.h"
#include "list_test.h"
#include "map_test.h"
#include "algo.h"

using namespace lmstl;

int main() {
	/*
	cout << CYAN;
	map_test();
	vector_test();
	list_test();
	*/
	vector<int> v;
	for (int i = 0; i < 20; ++i)
		v.emplace_back(rand());
	auto p = v.begin() + 9;
	SHOW(v);
	cout << *p << endl;
	nth_element(v.begin(), p, v.end());
	SHOW(v);
	cout << *p << endl;
	sort(v.begin(), v.end());
	SHOW(v);
	system("pause");
	return 0;
}