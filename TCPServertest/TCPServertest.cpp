//²âÊÔ³ÌÐò£¬
//²âÊÔtcpserver reactor select 

#include "../base/include/xTcpServerBase.hpp"
using namespace SAEBASE;

class mytcpsvr:public xTcpServerBase
{
	virtual int Onaccept(int socketfd,char*date,int len,IN xEventHandler *clientHandle=NULL)
	{
		printf(" in Onaccept!\n");
		char ip[64];
		int port;
		this->GetPeerInfo(socketfd,ip,port);
		return 0;
	}
	virtual int Ondata(int socketfd,char*date,int len)
	{
		printf("in Ondata!\n");
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
	mytcpsvr m_tcpsvr;
	xReactor m_reactor;
	m_tcpsvr.startTCPServer(&m_reactor,"192.168.200.80",6000);
	m_reactor.start();
	return 0;
}

