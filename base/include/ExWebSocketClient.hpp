//2019-01-05��Ex ϵ��hpp�����⡿ by xjshi
/*��װ�򵥵�webclient
1) ����websocket ��ͬ��http��Ϊȫ˫����ͨ��ģʽ����ExTcpClient�ʹ��̵߳�reactor���з�װ
2����װ���̵߳��÷������ݡ������߳̽������ݡ�
*/
#include "ExWebsock.hpp"
#include "ExTcpClient.hpp"
#include "xthreadbase.hpp"
#include "Exonevent.hpp"
#include "xReactorwithThread.h"
#include "xAutoLock.hpp"
namespace SAEBASE
{
	enum{
		m_status_handshake,
		m_status_datafram,
		m_close,
		m_text,
		m_errortype
	};
	class ExWebSocketClient:public Onevent
	{
	public:
		ExWebSocketClient()
		{
			m_Eventfd=m_client.getSockfd();
			int on=1; // ����nagle�㷨�رգ��ڲ���С����ʱ�����ײ���ճ�����������޷����ⲻ�������������
		#if WIN32
			SetSockOpt(m_Eventfd,IPPROTO_TCP,TCP_NODELAY,(const char*)&on,sizeof(on));
		#else
			SetSockOpt(m_Eventfd,IPPROTO_TCP,TCP_NODELAY,(void*)&on,sizeof(on));
		#endif
			m_reactor.RegisterHandler(this,xReadEvent);
			m_reactor.startReactorWithThread();
			m_status=m_status_handshake;
		}
		int Handshark(char* wsurl);
		int SendSockdata(char* data,int len); //���׽ӿڣ����ְ�������mask,����С��65535
		int SendSockdata(char*data,int len,int fin,int mask,int pcode=0x1);
		int SendPing();
		int SendPong();
		int HeartBeat(char*heatcontext,int heartlen);
		int Closewebsocket();
		void onReceivedata(char*data,int len);
	public:
		virtual int Ondata(int socketfd,char*date,int len);
		virtual handle_t GetHandler()const ;
	private:
		ExTcpClient m_client;
		xReactorwithThread m_reactor; 
		int m_status;
		xMutex m_lock;
	};
	int ExWebSocketClient::Ondata(int socketfd,char*date,int len)
	{
		printf("client get:%s\n",date);
		switch(m_status)
		{
		case m_status_handshake:
			m_status=m_status_datafram;
			break;
		case m_status_datafram:
			onReceivedata(date,len);
			break;
		}
		return 0;
	}
	handle_t ExWebSocketClient::GetHandler() const
	{
		return m_Eventfd;
	}
	int ExWebSocketClient::Handshark(char* wsurl)
	{	//ws://121.40.165.18:8800
		xAutoLock L(m_lock);
		string url=wsurl;
		url.erase(0,url.find_first_of("//")+2);
		string ip = url.substr(0,url.find_first_of(":"));
		url.erase(0,url.find_first_of(":")+1);
		string port =url.substr(0, url.find_first_of("/"));
		if(m_client.connectTCP((char*)ip.c_str(),atoi(port.c_str()))==-1 )
			return -1;
		webSockReq handleshakereq;
		string reqstr = handleshakereq.EasyHandshake(wsurl);
		m_client.sendMsg((char*)reqstr.c_str(),reqstr.length());
		return 0;
	}
	void ExWebSocketClient::onReceivedata(char*data,int len)
	{
		Dataframe msgframe;
		msgframe.FrameParse(data);
		switch(msgframe.m_code)
		{
		case 0x1://�ı�
			{
				cout<<"frambody:"<<msgframe.m_MsgBody<<endl;
			}
			break;
		case 0x9: //ping
			SendPong();
			break;
		case 0xA:
			SendPing();
			break;
		case 0x8: //close
 			Closewebsocket();
			break;
		default:
			break;
		}
		
	}
	int ExWebSocketClient::SendSockdata(char* data,int len)
	{
		xAutoLock L(m_lock);
		if(m_status!=m_status_datafram)
			return -1;
		Dataframe frame;
		int framelen=0;
		char* strframe=frame.Encode2s(1,0x01,1,(unsigned int)len,data,framelen);
		return m_client.sendMsg(strframe,framelen);
	}
	int ExWebSocketClient::SendSockdata(char*data,int len,int fin,int mask,int pcode)//Ĭ�ϴ����ı���Ϣ
	{
		xAutoLock L(m_lock);
		if(m_status!=m_status_datafram)
			return -1;
		Dataframe frame;
		char*  strframe=NULL;
		int framelen=0;
		strframe=frame.Encode2s(fin,pcode,mask,(unsigned int)len,data,framelen);
		return m_client.sendMsg(strframe,framelen);
		return 0;

	}
	int ExWebSocketClient::SendPing()
	{
		return SendSockdata("",1,1,1,0x9);
	}
	int ExWebSocketClient::SendPong()
	{
		return SendSockdata("",1,1,1,0xA);
	}
	int ExWebSocketClient::HeartBeat(char*heatcontext,int heartlen)
	{
		return SendSockdata("@heart",6,1,1);
	}
	int ExWebSocketClient::Closewebsocket()
	{
		return SendSockdata("",1,1,1,0x8);
	}
}