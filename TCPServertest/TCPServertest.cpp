//测试程序，
//测试tcpserver reactor select 
//2019/1/29 测试epoll结构的reactor tcp的压力。

#include "../base/include/xTcpServerBase.hpp"
#include "../base/include/xReactorwithThread.h"
#include <string>
#include <stdlib.h>
using namespace std;
using namespace SAEBASE;
static int clientfd=0;
class mytcpsvr:public xTcpServerBase
{
	virtual int Onaccept(int socketfd,char*date,int len,IN xEventHandler *clientHandle=NULL)
	{
		printf(" in Onaccept!\n");
		char ip[64];
		int port;
		this->GetPeerInfo(socketfd,ip,port);
		clientfd=socketfd;
		return 0;
	}
	virtual int Ondata(int socketfd,char*date,int len)
	{
		printf("in Ondata!：%s\n",date);
		return 0;
	}
	virtual int Onclose(int socketfd)
	{
		printf("in Onclose!\n");
		return 0;
	}
};

int main(int argc, char* argv[])
{
	if(argc<3)
	{
		printf("error。usage: ./xxx ip port\n");
	}
	string ip=argv[1];
	int port=atoi(argv[2]);
	mytcpsvr m_tcpsvr;
	xReactorwithThread m_reactor;
	m_tcpsvr.startTCPServer(&m_reactor,ip.c_str(),port);
	m_reactor.startReactorWithThread();
	for(;;)
	{
		char data[100];
		scanf("%s",data);
		if(clientfd!=0)
			m_tcpsvr.SendMsg(clientfd,data,strlen(data));
	}
	return 0;
}

