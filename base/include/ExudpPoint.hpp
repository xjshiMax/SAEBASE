//2018-12-19��Ex ϵ��hpp�����⡿ by xjshi
//�򵥵�udpͨ�ţ����׷�װbasesock
/*
1)�����̳߳������
2����ȡhandle sendto recfrom
3) �鲥�ӿ�
4���㲥�ӿ�,�㲥��Ϣ����ָ�������ߵĶ˿ںţ�������ָ��ip.
5�����ڶಥ  -----------xxxx------------
	�ֲ��ಥ��ַ����224.0.0.0��224.0.0.255֮�䣬����Ϊ·��Э���������;�����ĵ�ַ��·��������ת�����ڴ˷�Χ��IP��.
	Ԥ���ಥ��ַ����224.0.1.0��238.255.255.255֮�䣬������ȫ��Χ����Internet��������Э�顣
	����Ȩ�޶ಥ��ַ����239.0.0.0��239.255.255.255֮�䣬�ɹ���֯�ڲ�ʹ�ã�������˽��IP��ַ����������Internet�������ƶಥ��Χ
6) �����������ģʽ������server����client ֱ��ʹ�á�

*/
#include <string>
#include "basesock.hpp"
using namespace std;
namespace SAEBASE{

	enum{
		xUDP_SERVER,		//�󶨱��ض˿�
		xUDP_CLIENT,		//���󶨱��ض˿�
		xUDP_BROADCAST,		//�㲥 �������Ĭ�ϲ��������ض˿ڣ�
		xUDP_BROADCAST_CLIENT, //�㲥�Ŀͻ��˱���Ҫ�󶨶˿ڣ�
		xUDP_MUTILCAST		//�ಥ---xxxxxx-----
	};
class ExudpPoint
{
public:
	//��Ϊudp �ͻ��ˣ������б��ص�ַ�󶨡��������˿ڡ������Ҫ���գ������ȷ�����Ϣ��
	ExudpPoint(int udppart)
	{
		InitSocket();
		m_part=udppart;
		m_serverport=-1;
		m_sockfd = CreateSocket(SOCK_DGRAM);
	}
	//��Ϊudp�����
	ExudpPoint(int udppart,int serverport)
	{
		InitSocket();
		m_part=udppart;
		m_serverport=serverport;
		m_sockfd=CreateSocket(SOCK_DGRAM);
		struct sockaddr_in udp_addr;
		udp_addr.sin_family=AF_INET;
		udp_addr.sin_port=htons(m_serverport);
		//��������ip����������һ�£���һ�ڻ�ȡ��ʵip
		udp_addr.sin_addr.s_addr=INADDR_ANY;
		if(m_part==xUDP_BROADCAST_CLIENT) //����ǹ㲥�Ŀͻ���
		{
			BroadCastInit();
		}
		BindSocket(m_sockfd,(sockaddr*)&udp_addr,sizeof(struct sockaddr_in));
	}
	public:
		handle_t getHandle();
		int SendMsg(char*peerip,int peerport,const char*data,int len);
		//�������ݣ�ֻ��Ҫ�����Լ���sockfd���գ�����Ҫ�Է���ip�� ͬʱ���Ի�ȡ�Է��ĵ�ַ��Ϊ�������������
		//���չ㲥��Ϣ����������������Ϊֻ��Ҫ�Ӱ󶨵Ķ˿���ȡ��Ϣ������Ҫ����������
		int RecvMsg(OUT char*peerip,OUT int& peerport,OUT char*data,OUT int len);
		int RecvMsg(struct sockaddr *peeraddr,int* addrlen,char*data,int len);
private:
	void BroadCastInit()
	{
		const int opt=-1; //opt��Ϊ0��������㲥��
		SetSockOpt(m_sockfd,SOL_SOCKET,SO_BROADCAST,(char*)opt,sizeof(opt));
	}
	//���͹㲥��Ϣ��ָ�����ն˵�port.
	int BroadCastMsg(int bcastport,char*msg,int len);
private:
	int m_sockfd;
	int m_serverport;
	int m_part; // �ö�����ݵĽ�ɫ
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