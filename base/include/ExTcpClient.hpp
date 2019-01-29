//2018-12-20
/*
1)面向对象封装
2)单线程调用
---------
12-28修改： 一般客户端不需要绑定端口，因为没有谁需要直到client的端口，随机绑定就好。
*/
#pragma once
#include<string>
#include "basesock.hpp"
using namespace std;
namespace SAEBASE{
class ExTcpClient
{
public:
	ExTcpClient()
	{
		InitSocket();
		m_sockfd=CreateSocket(SOCK_STREAM);
	}

	//ExTcpClient(char* clientip,int clientport);
	int connectTCP(char*serverip,int serverport);
	int recieve(char* buf,int len);
	int sendMsg(char*buf,int len);
	int close();
	int getSockfd()
	{
		return m_sockfd;
	}
private:
	char m_clientip[32];
	char m_serverip[32];
	int  m_clientport;
	int  m_serverport;
	int  m_sockfd;
};
// ExTcpClient::ExTcpClient(char* clientip,int clientport)
// {
// 	strcpy(m_clientip,clientip);
// 	//strcpy(m_serverip,serverip);
// 	m_clientport=clientport;
// 	//m_serverport=serverport;
// 	InitSocket();
// 	m_sockfd=CreateSocket(SOCK_STREAM);
// 	struct sockaddr_in tcpddr;
// 	tcpddr.sin_family=AF_INET;
// 	tcpddr.sin_addr.s_addr=inet_addr(m_clientip);
// 	tcpddr.sin_port=htons(m_clientport);
// 	BindSocket(m_sockfd,(sockaddr*)&tcpddr,sizeof(struct sockaddr_in));
// 
// }
int ExTcpClient::connectTCP(char*serverip,int serverport)
{
	strcpy(m_serverip,serverip);
	m_serverport=serverport;
	struct sockaddr_in tcpddr;
	tcpddr.sin_family=AF_INET;
	tcpddr.sin_addr.s_addr=inet_addr(m_serverip);
	tcpddr.sin_port=htons(m_serverport);
	return ConnectSocket(m_sockfd,(sockaddr*)&tcpddr,sizeof(struct sockaddr_in));
}
int ExTcpClient::recieve(char* buf,int len)
{
	return ReadSocket(m_sockfd,buf,len,0);

}
int ExTcpClient::sendMsg(char*buf,int len)
{
	return SendSocket(m_sockfd,buf,len);
}
int ExTcpClient::close()
{
	return CloseSocket(m_sockfd);
}
}

