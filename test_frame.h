#ifndef __LMSTL_TEST_FRAME_H__
#define __LMSTL_TEST_FRAME_H__

#include <iostream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <Windows.h>
using std::cout;
using std::endl;

namespace lmstl {

struct COUT_COLOR {
	size_t _color;
	COUT_COLOR(size_t x):
		_color(x) {}
	friend std::ostream& operator<<(std::ostream&, COUT_COLOR&);
};

std::ostream& operator<<(std::ostream& out, const COUT_COLOR& uc) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | uc._color);
	return out;
}

#define RED COUT_COLOR(FOREGROUND_RED)
#define GREEN COUT_COLOR(FOREGROUND_GREEN)
#define CYAN COUT_COLOR(FOREGROUND_GREEN | FOREGROUND_BLUE)
#define YELLOW COUT_COLOR(FOREGROUND_GREEN | FOREGROUND_RED)
#define WIDE 14

#define SHOW(ctn) do{			\
	cout<<#ctn<<":";			\
	for(auto i : ctn)			\
		cout << i << " ";		\
	cout<<endl;					\
}while(0)

static size_t API_TEST_PASS;
static size_t API_TEST_FAIL;

#define API_TEST_START() do{	\
	cout << "[===============================================================]\n";	\
	cout << "[------------------- Container test : vector -------------------]\n";	\
	cout << "[-------------------------- API test ---------------------------]\n";	\
	API_TEST_PASS = 0;	\
	API_TEST_FAIL = 0;	\
}while(0)

#define API_COMPARE(myctn, stdctn) do{	\
	auto beg1 = myctn.begin(), end1 = myctn.end();	\
	auto beg2 = stdctn.begin(), end2 = stdctn.end();	\
	bool flag=1;	\
	for(;beg1!=end1;++beg1, ++beg2){	\
		cout << *beg1 << " ";	\
		if(*beg1 != *beg2){	\
			flag = 0;\
			break;	\
		}	\
	}	\
	if(flag){	\
		cout<<endl<<GREEN<<"PASS"<<endl;	\
		API_TEST_PASS++;}	\
	else{	\
		cout<<endl<<RED<<"FAIL"<<endl;	\
		SHOW(myctn);	\
		SHOW(stdctn);	\
		API_TEST_FAIL++;}	\
	cout<<CYAN<<"---------------------------------------------------------\n";	\
}while(0)

#define API_TEST01(myctn, stdctn, func, arg) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(arg);	\
	stdctn.func(arg);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST02(myctn, stdctn, func, arg1, arg2) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(arg1, arg2);	\
	stdctn.func(arg1, arg2);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST03(myctn, stdctn, func, arg1, arg2, arg3) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(arg1, arg2, arg3);	\
	stdctn.func(arg1, arg2, arg3);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST11(myctn, stdctn, func, funcarg, arg1, arg2) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg(arg1) + arg2);	\
	stdctn.func(stdctn.funcarg(arg1) + arg2);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST12(myctn, stdctn, func, funcarg1, arg1, arg2, arg3) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg1(arg1)+arg2, arg3);	\
	stdctn.func(stdctn.funcarg1(arg1)+arg2, arg3);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST13(myctn, stdctn, func, funcarg1, arg1, arg2, arg3, arg4) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg1(arg1)+arg2, arg3, arg4);	\
	stdctn.func(stdctn.funcarg1(arg1)+arg2, arg3, arg4);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST22(myctn, stdctn, func, funcarg1, funcarg2, arg1, arg2, arg3, arg4) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg1(arg1) + arg2, myctn.funcarg2(arg3) + arg4);	\
	stdctn.func(stdctn.funcarg1(arg1) + arg2, stdctn.funcarg2(arg3) + arg4);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST23(myctn, stdctn, func, funcarg1, funcarg2, arg1, arg2, arg3, arg4, arg5) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg1(arg1) + arg2, myctn.funcarg2(arg3) + arg4, arg5);	\
	stdctn.func(stdctn.funcarg1(arg1) + arg2, stdctn.funcarg2(arg3) + arg4, arg5);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST33(myctn, stdctn, func, funcarg1, funcarg2, funcarg3, arg1, arg2, arg3, arg4, arg5, arg6) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg1(arg1) + arg2, myctn.funcarg2(arg3) + arg4, myctn.funcarg3(arg5) + arg6);	\
	stdctn.func(stdctn.funcarg1(arg1) + arg2, stdctn.funcarg2(arg3) + arg4, stdctn.funcarg3(arg5) + arg6);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST_EACH01(myctn, stdctn, func, myarg, stdarg) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(myarg);	\
	stdctn.func(stdarg);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST_EACH02(myctn, stdctn, func, myarg1, myarg2, stdarg1, stdarg2) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(myarg1, myarg2);	\
	stdctn.func(stdarg1, stdarg2);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST_EACH03(myctn, stdctn, func, myarg1, myarg2, myarg3, stdarg1, stdarg2, stdarg3) do{	\
	cout<<"TESTING: "<<#func<<": "<<endl;	\
	myctn.func(myarg1, myarg2, myarg3);	\
	stdctn.func(stdarg1, stdarg2, stdarg3);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST_CUSTOM(myctn, stdctn, func1, func2) do{	\
	cout<<"Testing: "<<#func1<<": "<<endl;	\
	func1;	\
	func2;	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST_END() do{	\
	size_t total = API_TEST_PASS+API_TEST_FAIL;	\
	cout<<API_TEST_PASS<<" / "<<total<<" Cases passed"<<endl<<endl;	\
}while(0)

#define PERF_TEST00(xctn, func, len) do{	\
	clock_t start, end;	\
	std::vector<int> tv;	\
	for (int i = 0; i < len; ++i) {	\
		tv.push_back(rand());	\
	}	\
	start = clock();	\
	for(size_t i=0;i<len;++i)	\
		xctn.func(tv[i]);	\
	end = clock();	\
	int n = (int)(static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000);	\
	char outstring[20];	\
	std::snprintf(outstring, sizeof(outstring), "%d", n);	\
	std::string t = outstring;	\
	t += "ms    |";	\
	cout << std::setw(WIDE) << t;	\
}while(0)

#define PERF_TEST11(xctn, func, funcarg, len) do{	\
	clock_t start, end;	\
	std::vector<int> tv;	\
	for (int i = 0; i < len; ++i) {	\
		tv.push_back(rand());	\
	}	\
	start = clock();	\
	for(size_t i=0;i<len;++i)	\
		xctn.func(xctn.funcarg(), tv[i]);	\
	end = clock();	\
	int n = (int)(static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000);	\
	char outstring[20];	\
	std::snprintf(outstring, sizeof(outstring), "%d", n);	\
	std::string t = outstring;	\
	t += "ms    |";	\
	cout << std::setw(WIDE) << t;	\
}while(0)

#define PUSH_BACK_PERF(ctn) do{	\
	cout << "|      push_back      |    500000   |   5000000   |   10000000  |\n|        lmstl        |";	\
	lmstl::ctn<int> myctn;	\
	PERF_TEST00(myctn, push_back, 500000);	\
	PERF_TEST00(myctn, push_back, 5000000);	\
	PERF_TEST00(myctn, push_back, 10000000);	\
	cout<<"\n|         std         |";	\
	std::ctn<int> stdctn;	\
	PERF_TEST00(stdctn, push_back, 500000);	\
	PERF_TEST00(stdctn, push_back, 5000000);	\
	PERF_TEST00(stdctn, push_back, 10000000);	\
	cout<<endl;	\
}while(0)

#define INSERT_PERF(ctn) do{	\
	cout << "|        insert       |    10000    |    50000    |   200000    |\n|        lmstl        |";	\
	lmstl::ctn<int> myctn;	\
	PERF_TEST11(myctn, insert, begin, 10000);	\
	PERF_TEST11(myctn, insert, begin, 50000);	\
	PERF_TEST11(myctn, insert, begin, 200000);	\
	cout<<"\n|         std         |";	\
	std::ctn<int> stdctn;	\
	PERF_TEST11(stdctn, insert, begin, 10000);	\
	PERF_TEST11(stdctn, insert, begin, 50000);	\
	PERF_TEST11(stdctn, insert, begin, 200000);	\
	cout<<endl;	\
}while(0)

#define PERF_TEST(ctn) do{	\
	cout << "[--------------------- Performance Testing ---------------------]\n";	\
	cout << "|---------------------|-------------|-------------|-------------|\n";	\
	PUSH_BACK_PERF(ctn);	\
	cout << "|---------------------|-------------|-------------|-------------|\n";	\
	INSERT_PERF(ctn);	\
	cout << "[---------------------- End container test----------------------]\n";	\
}while(0)

}

#endif // !__LMSTL_TEST_FRAME_H__
