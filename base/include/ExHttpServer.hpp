//2019-01-02【Ex 系列hpp函数库】 by xjshi
/*
封装httpserver
1) 基于对象模式，留下OnRequest接口。
2）基于reactor模式接收消息，默认使用单例的全局reactor,也可以传入xReactor参数
3）双线程，继承于线程类。工作线程进行轮询
4）目前支持post解析
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
	virtual void Onrequest(int sockfd,HttpRequest& req)   // 如果需要处理自定义业务，则需要继承然后实现Onrequest()
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
