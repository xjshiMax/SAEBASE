//2019-01-02��Ex ϵ��hpp�����⡿ by xjshi
/*
��װhttpserver
1) ���ڶ���ģʽ������OnRequest�ӿڡ�
2������reactorģʽ������Ϣ��Ĭ��ʹ�õ�����ȫ��reactor,Ҳ���Դ���xReactor����
3��˫�̣߳��̳����߳��ࡣ�����߳̽�����ѯ
4��Ŀǰ֧��post����
*/
#include "ExHttpbase.hpp"
#include "xthreadbase.hpp"

#define REACTORINSTANCE 1
#define SELFDEFINEREACTOR 2
using namespace SAEBASE{
class ExHttpServer :public Threadbase,xTcpServerBase
{
public:
	ExHttpServer()
	{
		m_reactor=ReactorInstance::GetInstance();
		m_reactorType=REACTORINSTANCE;
	}
	ExHttpServer(xReactor* xreacotr)
	{
		m_reactor=xreacotr;
		m_reactorType=SELFDEFINEREACTOR;
	}
	virtual void Onrequest(int sockfd,HttpRequest& req)   // �����Ҫ�����Զ���ҵ������Ҫ�̳�Ȼ��ʵ��Onrequest()
	{
		printf("i get a http request!\n");
	}
	virtual int starthttp(const char* ip,int port)
	{
		startTCPServer(m_reactor,ip,port);
		//m_reactor.start();
		start();

	}
	virtual int SendResponse(HttpResponse& resp);
public:
	virtual void run()
	{
		if(m_reactorType==REACTORINSTANCE)
			m_reactor.start();
	}
	virtual int Ondata(int socketfd,char*date,int len)
	{
		HttpRequest req;
		req=req.Parsestr(date);
		Onrequest(socketfd,req);
		return 0;
	}
	virtual int Onclose(int socketfd)
	{
		CloseSocket(socketfd);
	}
private:
	xReactor*m_reactor;
	int m_reactorType; 
};
};
