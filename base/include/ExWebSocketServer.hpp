//2019-01-06【Ex 系列hpp函数库】 by xjshi
/*
封装webdsocket服务，面向对象，可直接使用。
1）继承主动对象，双线程，监听线程使用xreactor。
2）基本的接收，关闭，和发信，故障问题排查。
3) 由于xReactor本身没有自己的线程，如果在主程序中调用xReactor.start() 会卡住，所以
   将xReaxtor封装在server里面，外部不可见。
4) 这里IO复用一个线程，socket 任务线程 ，还有一个主线程，3线程模型。 后期如果做到好，完全可以将前两个做成线程池的模型，单例线程池。
IO复用线程：对监听socketfd进行轮询，接收客户端的接入。
任务线程：在io收到读信号以后，读取数据，派发到任务线程，以免耗时操作会阻塞io复用的效率。
主线程：在创建完websocketserver以后，主线程继续往下执行。

5) 对连接过来的fd保存并记录状态。
*/
#include "xTcpServerBase.hpp"
#include "ExActiveObj.hpp"
#include "xReactorwithThread.h"
#include "ExWebsock.hpp"
#include "xbaseclass.hpp"
namespace SAEBASE{

enum{
	UNREALIZE,  //初始状态
	CONNAECTING, //连接中
	GETDATA,    //接收数据状态
	CLOSED      //关闭状态
};
//服务端接收client连接以后，在这进行数据的交换和关闭，这样
//每个客户端就对应一个Onclient实例，方便记状态和处理
class OnClient:public xEventHandler
{
public:
	OnClient(xReactor* reactor,int Acceptfd,ExActiveObj* serverlist):m_Eventfd(Acceptfd)
	{
		m_Reacotr=reactor;
		m_ServerTaskList=serverlist;
		m_status=UNREALIZE;
	}
	virtual handle_t GetHandler()const
	{
		return m_Eventfd;
	}
	virtual void HandleRead(int listentfd)
	{
		try
		{		
				char buf[MAXREADSIZE]={0};
				int len=MAXREADSIZE;
				int iret = ReadSocket(listentfd,buf,len);
				len=sizeof(buf);
				if(iret==0)
				{
					CloseSocket(listentfd);
					if(m_reactor)
						m_reactor->RemoveHandlerbyfd(listentfd);
					//ShutDownSocket(listentfd,0);
					this->Onclose(listentfd);
					//CloseSocket(listentfd);
				}
				else if(iret>=0)
					this->Ondata(listentfd,buf,iret);
				else
					return ;
		}
		catch (CException* e)
		{
		}
		return ;
	}

//暴露的接口
	virtual int Ondata(int socketfd,char*date,int len)
	{
		switch(m_status)
		{
		case UNREALIZE:

			break;
		case CONNAECTING:
			break;
		case GETDATA:
			break;
		case CLOSED:
			break;
		}
	}

private:
	xReactor* m_Reacotr;
	ExActiveObj* m_ServerTaskList;
	int m_status;
};

class ExWebSocketServer:public xTcpServerBase,public ExActiveObj<xMsgBlock>
{
public:
	ExWebSocketServer(){}
	virtual int Onrequest(xMsgBlock& block);
	virtual int Ondata(int socketfd,char*date,int len);
	int xTcpServerBase::Onaccept(int socketfd,char*date,int len,IN xEventHandler *clientHandle)
	{
		OnClient myEvent(&m_reactor,socketfd,this);
		clientHandle=&myEvent;
		return 0;
	}
	int StartSvr(const char* ip,int port);
	int StopSvr();

	void RespHandshake();
private:
	xReactorwithThread m_Reactor;
};
int ExWebSocketServer::StartSvr(const char* ip,int port)
{
	startTCPServer(&m_Reactor,ip,port);
	m_Reactor.startReactorWithThread(); //io复用的线程
	startObj();//任务线程。

}
//这里接收websocket 的客户端发过来的信息。
int ExWebSocketServer::Ondata(int socketfd,char*date,int len)
{
	xMsgBlock Msgblock={0};
	Msgblock.pdata=date;
	Msgblock.len=len;
	pushMsg(Msgblock);
}
int ExWebSocketServer::Onrequest(xMsgBlock& block)
{
	webSockReq reqmsg;
	reqmsg.Parse2Reg(block.pdata);

}
void ExWebSocketServer::RespHandshake()
{
	webSockResp resp;
}
}