//2018-12-19【Ex 系列hpp函数库】 by xjshi
//简单的udp通信，简易封装basesock
/*
1)供单线程程序调用
2）获取handle sendto recfrom
3) 组播接口
4）广播接口,广播消息必须指定接收者的端口号，而不必指明ip.
5）关于多播  -----------xxxx------------
	局部多播地址：在224.0.0.0～224.0.0.255之间，这是为路由协议和其他用途保留的地址，路由器并不转发属于此范围的IP包.
	预留多播地址：在224.0.1.0～238.255.255.255之间，可用于全球范围（如Internet）或网络协议。
	管理权限多播地址：在239.0.0.0～239.255.255.255之间，可供组织内部使用，类似于私有IP地址，不能用于Internet，可限制多播范围
6) 做成面向对象模式，定义server或者client 直接使用。

*/
#include <string>
#include "basesock.hpp"
using namespace std;
namespace SAEBASE{

	enum{
		xUDP_SERVER,		//绑定本地端口
		xUDP_CLIENT,		//不绑定本地端口
		xUDP_BROADCAST,		//广播 （服务端默认不保定本地端口）
		xUDP_BROADCAST_CLIENT, //广播的客户端必须要绑定端口，
		xUDP_MUTILCAST		//多播---xxxxxx-----
	};
class ExudpPoint
{
public:
	//作为udp 客户端，不进行本地地址绑定。随机分配端口。如果需要接收，必须先发送消息。
	ExudpPoint(int udppart)
	{
		InitSocket();
		m_part=udppart;
		m_serverport=-1;
		m_sockfd = CreateSocket(SOCK_DGRAM);
	}
	//作为udp服务端
	ExudpPoint(int udppart,int serverport)
	{
		InitSocket();
		m_part=udppart;
		m_serverport=serverport;
		m_sockfd=CreateSocket(SOCK_DGRAM);
		struct sockaddr_in udp_addr;
		udp_addr.sin_family=AF_INET;
		udp_addr.sin_port=htons(m_serverport);
		//这里服务端ip可以再设置一下，下一期获取真实ip
		udp_addr.sin_addr.s_addr=INADDR_ANY;
		if(m_part==xUDP_BROADCAST_CLIENT) //如果是广播的客户端
		{
			BroadCastInit();
		}
		BindSocket(m_sockfd,(sockaddr*)&udp_addr,sizeof(struct sockaddr_in));
	}
	public:
		handle_t getHandle();
		int SendMsg(char*peerip,int peerport,const char*data,int len);
		//接收数据，只需要根据自己的sockfd接收，不需要对方的ip， 同时可以获取对方的地址作为输出参数传出。
		//接收广播消息，仍用这两个。因为只需要从绑定的端口收取信息，不需要跟过东西。
		int RecvMsg(OUT char*peerip,OUT int& peerport,OUT char*data,OUT int len);
		int RecvMsg(struct sockaddr *peeraddr,int* addrlen,char*data,int len);
private:
	void BroadCastInit()
	{
		const int opt=-1; //opt不为0，则允许广播。
		SetSockOpt(m_sockfd,SOL_SOCKET,SO_BROADCAST,(char*)opt,sizeof(opt));
	}
	//发送广播消息。指定接收端的port.
	int BroadCastMsg(int bcastport,char*msg,int len);
private:
	int m_sockfd;
	int m_serverport;
	int m_part; // 该对象扮演的角色
};
handle_t ExudpPoint::getHandle()
{
	return 0;
}

int ExudpPoint::SendMsg(char*peerip,int peerport,const char*data,int len)
{
	struct sockaddr_in peerAddr;
	memset(&peerAddr,0,sizeof(peerAddr));
	peerAddr.sin_family=AF_INET;
	peerAddr.sin_addr.s_addr=inet_addr(peerip);
	peerAddr.sin_port=htons(peerport);
	int addrlen=sizeof(struct sockaddr_in);
	return Sendto(m_sockfd,data,len,0,(sockaddr*)&peerAddr,addrlen);
}
int ExudpPoint::RecvMsg(struct sockaddr *peeraddr,int* addrlen,char*data,int len)
{
	return Recvfrom(m_sockfd,data,len,0,peeraddr,addrlen);
}
int ExudpPoint::RecvMsg(OUT char*peerip,OUT int& peerport,OUT char*data,OUT int len)
{
	struct sockaddr_in peerAddr;
	memset(&peerAddr,0,sizeof(peerAddr));
// 	peerAddr.sin_family=AF_INET;
// 	peerAddr.sin_addr.s_addr=inet_addr(peerip);
//	peerAddr.sin_port=htons(peerport);
	int addrlen=sizeof(struct sockaddr_in);
	int ret =  Recvfrom(m_sockfd,data,len,0,(sockaddr*)&peerAddr,&addrlen);
	strcpy(peerip,inet_ntoa(peerAddr.sin_addr));
	peerport=ntohs(peerAddr.sin_port);
	return ret;
}
int ExudpPoint::BroadCastMsg(int bcastport,char*msg,int len)
{
	u_long broadip=INADDR_BROADCAST;
	return SendMsg((char*)&broadip,bcastport,msg,len);
}

}