// smartptrTest.cpp : �������̨Ӧ�ó������ڵ㡣
// ����ָ�����

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

