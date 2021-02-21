#ifndef __LMSTL_TEST_FRAME_H__
#define __LMSTL_TEST_FRAME_H__

#include <iostream>
#include <iomanip>
#include <vector>
#include <time.h>
#include "utility.h"
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
#define WIDE2 22

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const pair<T1, T2>& p) {
	out << "(" << p.first << " " << p.second << ")";
	return out;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& p) {
	out << "(" << p.first << " " << p.second << ")";
	return out;
}

template <typename T1, typename T2>
bool operator!=(const pair<T1, T2>& l, const std::pair<T1, T2>& r) {
	return (l.first != r.first) && (l.second != r.second);
}

template <typename T1, typename T2>
bool operator==(const pair<T1, T2>& l, const std::pair<T1, T2>& r) {
	return (l.first == r.first) && (l.second == r.second);
}

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
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(arg);	\
	stdctn.func(arg);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST02(myctn, stdctn, func, arg1, arg2) do{	\
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(arg1, arg2);	\
	stdctn.func(arg1, arg2);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST03(myctn, stdctn, func, arg1, arg2, arg3) do{	\
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(arg1, arg2, arg3);	\
	stdctn.func(arg1, arg2, arg3);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST11(myctn, stdctn, func, funcarg, arg1, arg2) do{	\
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg(arg1) + arg2);	\
	stdctn.func(stdctn.funcarg(arg1) + arg2);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST12(myctn, stdctn, func, funcarg1, arg1, arg2, arg3) do{	\
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg1(arg1)+arg2, arg3);	\
	stdctn.func(stdctn.funcarg1(arg1)+arg2, arg3);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST13(myctn, stdctn, func, funcarg1, arg1, arg2, arg3, arg4) do{	\
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg1(arg1)+arg2, arg3, arg4);	\
	stdctn.func(stdctn.funcarg1(arg1)+arg2, arg3, arg4);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST22(myctn, stdctn, func, funcarg1, funcarg2, arg1, arg2, arg3, arg4) do{	\
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg1(arg1) + arg2, myctn.funcarg2(arg3) + arg4);	\
	stdctn.func(stdctn.funcarg1(arg1) + arg2, stdctn.funcarg2(arg3) + arg4);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST23(myctn, stdctn, func, funcarg1, funcarg2, arg1, arg2, arg3, arg4, arg5) do{	\
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg1(arg1) + arg2, myctn.funcarg2(arg3) + arg4, arg5);	\
	stdctn.func(stdctn.funcarg1(arg1) + arg2, stdctn.funcarg2(arg3) + arg4, arg5);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST33(myctn, stdctn, func, funcarg1, funcarg2, funcarg3, arg1, arg2, arg3, arg4, arg5, arg6) do{	\
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(myctn.funcarg1(arg1) + arg2, myctn.funcarg2(arg3) + arg4, myctn.funcarg3(arg5) + arg6);	\
	stdctn.func(stdctn.funcarg1(arg1) + arg2, stdctn.funcarg2(arg3) + arg4, stdctn.funcarg3(arg5) + arg6);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST_EACH1(myctn, stdctn, func, myarg, stdarg) do{	\
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(myarg);	\
	stdctn.func(stdarg);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST_EACH2(myctn, stdctn, func, myarg1, myarg2, stdarg1, stdarg2) do{	\
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(myarg1, myarg2);	\
	stdctn.func(stdarg1, stdarg2);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST_EACH3(myctn, stdctn, func, myarg1, myarg2, myarg3, stdarg1, stdarg2, stdarg3) do{	\
	cout<<"TESTING "<<#func<<": "<<endl;	\
	myctn.func(myarg1, myarg2, myarg3);	\
	stdctn.func(stdarg1, stdarg2, stdarg3);	\
	API_COMPARE(myctn, stdctn);	\
}while(0)

#define API_TEST_END() do{	\
	size_t total = API_TEST_PASS+API_TEST_FAIL;	\
	cout<<API_TEST_PASS<<" / "<<total<<" Cases passed"<<endl<<endl;	\
}while(0)

#define TIMING00(xctn, func, len) do{	\
	clock_t start, end;	\
	std::vector<int> tv;	\
	for (int i = 0; i < len; ++i) {	\
		tv.push_back(rand());	\
	}	\
	start = clock();	\
	for(size_t i=0;i<len;++i)	\
		xctn.func(tv[i]);	\
	end = clock();	\
	tv.~vector();	\
	int n = (int)(static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000);	\
	char outstring[20];	\
	std::snprintf(outstring, sizeof(outstring), "%d", n);	\
	std::string t = outstring;	\
	t += "ms    |";	\
	cout << std::setw(WIDE) << t;	\
}while(0)

#define TIMING11(xctn, func, funcarg, len) do{	\
	clock_t start, end;	\
	std::vector<int> tv;	\
	for (int i = 0; i < len; ++i) {	\
		tv.push_back(rand());	\
	}	\
	start = clock();	\
	for(size_t i=0;i<len;++i)	\
		xctn.func(xctn.funcarg(), tv[i]);	\
	end = clock();	\
	tv.~vector();	\
	int n = (int)(static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000);	\
	char outstring[20];	\
	std::snprintf(outstring, sizeof(outstring), "%d", n);	\
	std::string t = outstring;	\
	t += "ms    |";	\
	cout << std::setw(WIDE) << t;	\
}while(0)

#define PERF_TEST00(ctn, func, len1, len2, len3) do{	\
	cout << "|---------------------|-------------|-------------|-------------|\n";	\
	std::string name(#func);	\
	name += "      |";	\
	cout << "|"<<std::setw(WIDE2)<< name;	\
	std::string l1(#len1), l2(#len2), l3(#len3);	\
	l1+="   |";l2+="   |";l3+="   |";	\
	cout<<std::setw(WIDE)<<l1<<std::setw(WIDE)<<l2<<std::setw(WIDE)<<l3<<"\n|        lmstl        |";	\
	lmstl::ctn<int> myctn;	\
	TIMING00(myctn, func, len1);	\
	myctn.clear();\
	TIMING00(myctn, func, len2);	\
	myctn.clear();\
	TIMING00(myctn, func, len3);	\
	myctn.clear();\
	cout<<"\n|         std         |";	\
	std::ctn<int> stdctn;	\
	TIMING00(stdctn, func, len1);	\
	stdctn.clear();\
	TIMING00(stdctn, func, len2);	\
	stdctn.clear();\
	TIMING00(stdctn, func, len3);	\
	stdctn.clear();\
	cout << endl;\
}while(0)

#define MAPTIMING1(xctn, func, len, ns) do{	\
	clock_t start, end;	\
	std::vector<ns::pair<int, int>> tv;	\
	for (int i = 0; i < len; ++i) {	\
		tv.push_back(ns::pair<int, int>(i, rand()));	\
	}	\
	start = clock();	\
	for(size_t i=0;i<len;++i)	\
		xctn.func(tv[i]);	\
	end = clock();	\
	tv.~vector();	\
	int n = (int)(static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000);	\
	char outstring[20];	\
	std::snprintf(outstring, sizeof(outstring), "%d", n);	\
	std::string t = outstring;	\
	t += "ms    |";	\
	cout << std::setw(WIDE) << t;	\
}while(0)

#define MAPTIMING2(xctn, len) do{	\
	clock_t start, end;	\
	std::vector<int> tv;	\
	for (int i = 0; i < len; ++i) {	\
		tv.push_back(rand());	\
	}	\
	start = clock();	\
	for(size_t i=0;i<len;++i)	\
		xctn[tv[i]]=i;	\
	end = clock();	\
	tv.~vector();	\
	int n = (int)(static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000);	\
	char outstring[20];	\
	std::snprintf(outstring, sizeof(outstring), "%d", n);	\
	std::string t = outstring;	\
	t += "ms    |";	\
	cout << std::setw(WIDE) << t;	\
}while(0)

#define MAP_PERF_TEST2(ctn, func, len1, len2, len3) do{	\
	cout << "|---------------------|-------------|-------------|-------------|\n";	\
	std::string name(#func);	\
	name += "      |";	\
	cout << "|"<<std::setw(WIDE2)<< name;	\
	std::string l1(#len1), l2(#len2), l3(#len3);	\
	l1+="   |";l2+="   |";l3+="   |";	\
	cout<<std::setw(WIDE)<<l1<<std::setw(WIDE)<<l2<<std::setw(WIDE)<<l3<<"\n|        lmstl        |";	\
	lmstl::ctn<int, int> myctn;	\
	MAPTIMING2(myctn, len1);	\
	myctn.clear();\
	MAPTIMING2(myctn, len2);	\
	myctn.clear();\
	MAPTIMING2(myctn, len3);	\
	myctn.clear();\
	cout<<"\n|         std         |";	\
	std::ctn<int, int> stdctn;	\
	typedef std::ctn<int, int>::value_type vt2;\
	MAPTIMING2(stdctn, len1);	\
	stdctn.clear();\
	MAPTIMING2(stdctn, len2);	\
	stdctn.clear();\
	MAPTIMING2(stdctn, len3);	\
	stdctn.clear();\
	cout << endl;\
}while(0)

#define MAP_PERF_TEST1(ctn, func, len1, len2, len3) do{	\
	cout << "|---------------------|-------------|-------------|-------------|\n";	\
	std::string name(#func);	\
	name += "      |";	\
	cout << "|"<<std::setw(WIDE2)<< name;	\
	std::string l1(#len1), l2(#len2), l3(#len3);	\
	l1+="   |";l2+="   |";l3+="   |";	\
	cout<<std::setw(WIDE)<<l1<<std::setw(WIDE)<<l2<<std::setw(WIDE)<<l3<<"\n|        lmstl        |";	\
	lmstl::ctn<int, int> myctn;	\
	MAPTIMING1(myctn, func, len1, lmstl);	\
	myctn.clear();\
	MAPTIMING1(myctn, func, len2, lmstl);	\
	myctn.clear();\
	MAPTIMING1(myctn, func, len3, lmstl);	\
	myctn.clear();\
	cout<<"\n|         std         |";	\
	std::ctn<int, int> stdctn;	\
	typedef std::ctn<int, int>::value_type vt2;\
	MAPTIMING1(stdctn, func, len1, std);	\
	stdctn.clear();\
	MAPTIMING1(stdctn, func, len2, std);	\
	stdctn.clear();\
	MAPTIMING1(stdctn, func, len3, std);	\
	stdctn.clear();\
	cout << endl;\
}while(0)

#define PERF_TEST11(ctn, func, funcarg, len1, len2, len3) do{	\
	cout << "|---------------------|-------------|-------------|-------------|\n";	\
	std::string name(#func);	\
	name = "|       " + name;	\
	cout << std::left << std::setw(WIDE2) << name;	\
	std::string l1(#len1), l2(#len2), l3(#len3);	\
	l1 = "|    "+l1;l2 = "|    "+l2;l3 = "|    "+l3;	\
	cout<<std::setw(WIDE)<<l1<<std::setw(WIDE)<<l2<<std::setw(WIDE)<<l3<<std::right<<"|\n|        lmstl        |";	\
	lmstl::ctn<int> myctn;	\
	TIMING11(myctn, func, funcarg, len1);	\
	myctn.clear();\
	TIMING11(myctn, func, funcarg, len2);	\
	myctn.clear();\
	TIMING11(myctn, func, funcarg, len3);	\
	myctn.clear();\
	cout<<"\n|         std         |";	\
	std::ctn<int> stdctn;	\
	TIMING11(stdctn, func, funcarg, len1);	\
	stdctn.clear();\
	TIMING11(stdctn, func, funcarg, len2);	\
	stdctn.clear();\
	TIMING11(stdctn, func, funcarg, len3);	\
	stdctn.clear();\
	cout<<endl;	\
}while(0)

#define PERF_TEST_START() do{	\
	cout << "[--------------------- Performance Testing ---------------------]\n";	\
}while(0)

#define PERF_TEST_END() do{	\
	cout << "[---------------------- End container test----------------------]\n";	\
	cout << GREEN <<"PASS"<<CYAN<<endl;	\
}while(0)

}

#endif // !__LMSTL_TEST_FRAME_H__
