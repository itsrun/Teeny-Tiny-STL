#include "vector_test.h"
#include "list_test.h"
#include "map_test.h"
#include "algobase.h"
#include "algo.h"
using namespace lmstl;

int main() {
	cout << CYAN;
	map_test();
	vector_test();
	list_test();
	system("pause");
	return 0;
}