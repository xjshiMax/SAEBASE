//2019-01-02��Ex ϵ��hpp�����⡿ by xjshi
/*
����rfc6455 ��װwebsocket �ӿ�
1�� �����ı��Ĺ���ͽ���
2���������״̬����趨
		|--Handshake Request--->|
		|						|
client	|<--	Accept	--------| server
		|						|
		|<---ȫ˫��ͨ��---------->|
		|						|
		|------close----------->|

*/
#include <string>
using namespace std;
class webSockReq
{
	webSockReq();
	int Encode2str();
	int Parse2Reg(char*strreq);
private:
	string m_Upgrade;
	string m_Commection;
	string m_Host;
	string m_Origin;
	string m_WebSocket_key;		//������ɣ��ṩ�����ķ���
	string m_WebSocket_Version;
};

