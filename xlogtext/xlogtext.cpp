// xlogtext.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "../base/include/xlog.hpp"
#include <memory>
#include <stdlib.h>
#include <iostream>
using namespace std;
//��������
//1�����֣��ַ�������
//2����%s��,"xxx" �����ǳ����ַ����ᱨ��

void createuser()
{

	for(int i=1;i<101;i++)
	{
		char tempbuff[1024+64]={0};
		sprintf(tempbuff,"CoremailUser%d@abc.cn\n",i);
		std::string filename="Username.txt";
		FILE* fp=NULL;
		if(access(filename.c_str(),0)==-1)
		{
			fp=fopen(filename.c_str(),"wb+");
		}
		else
			fp=fopen(filename.c_str(),"ab+");
		string tempstr=string(tempbuff);
		fwrite(tempbuff,tempstr.size(),1,fp);
		fclose(fp);
	}

};

using namespace SAEBASE;
int main(int argc, char* argv[])
{
// 	int i=7;
// 	string str="lovelove";
// 	printf("zhang:%s","sdsf");
// 	xlog*plog=xlogger::instance();
// 	plog->SetFilePath("E:/");
// 	XLOG_DEBUGOUT("this is a test %d",i);
// 
// 	auto_ptr<string> strptr(new string("zs"));
// 	cout<<*strptr<<endl;
	createuser();
	return 0;
}

