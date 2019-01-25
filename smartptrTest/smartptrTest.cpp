// smartptrTest.cpp : 定义控制台应用程序的入口点。
// 智能指针测试

#include "../base/include/xsmartPtr.hpp"

using namespace SAEBASE;
int main(int argc, char* argv[])
{
	int * i=1;
	xsmartRef<int> ptr(i);
	const int* p = ptr.getConstPtr();
	printf("xsmart:%d",*ptr);
	return 0;
}

