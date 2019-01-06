//2019-01-06��Ex ϵ��hpp�����⡿ by xjshi
/*
��װwebdsocket����������󣬿�ֱ��ʹ�á�
1���̳���������˫�̣߳������߳�ʹ��xreactor��
2�������Ľ��գ��رգ��ͷ��ţ����������Ų顣
3) ����xReactor����û���Լ����̣߳�������������е���xReactor.start() �Ῠס������
   ��xReaxtor��װ��server���棬�ⲿ���ɼ���
4) ����IO����һ���̣߳�socket �����߳� ������һ�����̣߳�3�߳�ģ�͡� ������������ã���ȫ���Խ�ǰ���������̳߳ص�ģ�ͣ������̳߳ء�
IO�����̣߳��Լ���socketfd������ѯ�����տͻ��˵Ľ��롣
�����̣߳���io�յ����ź��Ժ󣬶�ȡ���ݣ��ɷ��������̣߳������ʱ����������io���õ�Ч�ʡ�
���̣߳��ڴ�����websocketserver�Ժ����̼߳�������ִ�С�

5) �����ӹ�����fd���沢��¼״̬��
*/
#include "xTcpServerBase.hpp"
#include "ExActiveObj.hpp"
#include "xReactorwithThread.h"
#include "ExWebsock.hpp"
#include "xbaseclass.hpp"
namespace SAEBASE{

enum{
	UNREALIZE,  //��ʼ״̬
	CONNAECTING, //������
	GETDATA,    //��������״̬
	CLOSED      //�ر�״̬
};
//����˽���client�����Ժ�����������ݵĽ����͹رգ�����
//ÿ���ͻ��˾Ͷ�Ӧһ��Onclientʵ���������״̬�ʹ���
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

//��¶�Ľӿ�
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
	m_Reactor.startReactorWithThread(); //io���õ��߳�
	startObj();//�����̡߳�

}
//�������websocket �Ŀͻ��˷���������Ϣ��
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