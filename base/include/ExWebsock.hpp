//2019-01-02【Ex 系列hpp函数库】 by xjshi
/*
基于rfc6455 封装websocket 接口
1） 基本的报文构造和解析
2）错误码和状态码的设定
		|--Handshake Request--->|
		|						|
client	|<--	Accept	--------| server
		|						|
		|<---全双工通信---------->|
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
	string m_WebSocket_key;		//随机生成，提供基本的防护
	string m_WebSocket_Version;
};

