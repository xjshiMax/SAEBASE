// testjsocpp.cpp : 定义控制台应用程序的入口点。
//
#pragma once
#include "stdafx.h"
#include<string>
#include<iostream>
#include <fstream>
#include "../base/jsoncpp/json/json.h"
using namespace std;
#pragma comment(lib,"../base/jsoncpp/lib/lib_jsond.lib")
#include "../base/include/xmd5.hpp"
void testreadfile()
{
	Json::Reader reader;
	Json::Value valueroot;
	fstream ifs;
	ifs.open("t.json",ios::in);
		if(reader.parse(ifs,valueroot))
		{
			string retcode=valueroot["retCode"].asString();
			string retMsg=valueroot["retMsg"].asString();
		}
		const Json::Value arrayobj = valueroot["adviceOpt"];
		for(int i=0;i<arrayobj.size();i++)
		{
			string iobj=arrayobj[i].asString();
		}
}
int main(int argc, char* argv[])
{
	//testreadfile();
	xmd5crpty mymd5;
	xmd5ctx ctxobj;
	mymd5.cs_md5_init(&ctxobj);
	mymd5.cs_md5_update(&ctxobj,(const unsigned char*)"iloveu",strlen("iloveu"));
	unsigned char encode[16]={0};
	mymd5.cs_md5_final(encode,&ctxobj);
	//string contect=encode+"0";
	return 0;
}

