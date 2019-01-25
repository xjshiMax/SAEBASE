// webclient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdio.h"
#include "../base/include/ExWebSocketClient.hpp"
using namespace SAEBASE;
int main(int argc, char* argv[])
{
	ExWebSocketClient client;
	//client.start();
	client.Handshark("ws://123.207.167.163:9010/ajaxchattest");
	while(1)
	{
		int command=0;
		char data[1024]={0};
		printf("input command,0:end,1:send data\n");
		scanf("%d",&command);
		printf("input data:%s\n",data);
		scanf("%s",data);
		string strdata=data;
		switch(command)
		{
		case 0:
			break;
		case 1:
			client.SendSockdata(data,strdata.length());
			break;
		default:
			break;
		}
	}
	system("pause");
	return 0;
}

