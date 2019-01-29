// xlogtext.cpp : 定义控制台应用程序的入口点。
//
#pragma once
// #include "../base/include/xlog.hpp"
// #include "../base/include/xcrypt.hpp"
// #include "../base/include/singleton.hpp"
// #include "../base/include/xsmartPtr.hpp"
#include "../base/include/ExTcpClient.hpp"
#include <memory>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
using namespace std;
//两个问题
//1）数字，字符有乱码
//2）“%s”,"xxx" 后面是常量字符串会报错。

void createuser()
{

// 	for(int i=1;i<101;i++)
// 	{
// 		char tempbuff[1024+64]={0};
// 		sprintf(tempbuff,"CoremailUser%d@abc.cn\n",i);
// 		std::string filename="Username.txt";
// 		FILE* fp=NULL;
// 		if(access(filename.c_str(),0)==-1)
// 		{
// 			fp=fopen(filename.c_str(),"wb+");
// 		}
// 		else
// 			fp=fopen(filename.c_str(),"ab+");
// 		string tempstr=string(tempbuff);
// 		fwrite(tempbuff,tempstr.size(),1,fp);
// 		fclose(fp);
// 	}

};


using namespace SAEBASE;
class fine
{
public:
	fine()
	{
		id=2;
	}
public:
	int id;
};

int main(int argc, char* argv[])
{
	int i=7;

	//xlog*plog=xlogger::instance();
//	plog->SetFilePath("E:/");
// 	for(;i<10;i++)
// 	{
// 		XLOG_DEBUGOUT("this is a test %d",i);
// 		XLOG_ERROROUT("error log %d",i);
// 		XLOG_INFOOUT("info log %d",i);
// 	}
#define TCPCLIENTTEST
#ifdef BASE64TEST
	string src="iloveyou";
	int len = src.length();
	string enstr = encode_base64(src.c_str(),len);
	cout<<"enstr:"<<enstr<<endl;
	len=enstr.length();
	string destr = decode_base64(enstr.c_str(),&len);
	cout<<"destr:"<<destr<<endl;
#endif
#ifdef SINGLETON
	fine* mgfine1 = xsingleton<fine>::instance();
	mgfine1->id+=1;
	fine* myfine2=xsingleton<fine>::instance();
	cout<<"id:"<<myfine2->id<<endl;
#endif

#ifdef SMARTPRTTEST
	 xsmartRef<int >p = new int(6);
	 cout<<*p<<endl;
#endif

#ifdef TCPCLIENTTEST
	 ExTcpClient myclient;
	 myclient.connectTCP("192.168.200.80",6000);
	 myclient.sendMsg("iloveyou",strlen("ilovelen"));
#endif
#ifdef WIN32
	system("pause");
#endif
// 	auto_ptr<string> strptr(new string("zs"));
// 	cout<<*strptr<<endl;
	return 0;
}

