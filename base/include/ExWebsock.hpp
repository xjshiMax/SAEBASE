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
3） 握手阶段的客户端请求，大致遵循http协议。
i:握手请求必须是GET方法
ii：HTTP版本必须是1.1及以上。
GET /chat HTTP/1.1
Host: Server.example.com
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhxxxvgdfsfsfsfs
Origin: http://example.com
Sec-WebSocket-Protocol: chat syperchat
Sec-WebSocket-Version:13

Request Line<CRLF>
Header-Name: header-value<CRLF>
Header-Name: header-value<CRLF>
//一个或多个，均以<CRLF>结尾
<CRLF>
由于是get方法，没有body.

4) 服务端。 如果响应时101，则表示服务端接收，握手成功，其他均不成功。
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: s3pPLMBITxaQ9YGzzhZRbk+xOo=


*/
#include <string>
#include <map>
#include "xHelpstring.hpp"
using namespace std;
namespace SAEBASE{
//请求常用头部,必须带有的字段：request-uri,Host,Upgrade,Connection,Sec-WebSocket-Key

//用于保护防止 浏览器中未授权的脚本使用Websocket API  连接使用websocket 服务。如果服务端拒绝
//客户端的连接，可以随便发一个http错误。
#define WEBREQ_ORIGIN			"Origin"
#define WEBREQ_SECWEBSOCKETKEY	"Sec-WebSocket-Key"
#define WEBREQ_HOST				"Host"
//值必须是 websocket
#define WEBREQ_UPGRADE			"Upgrade"
#define WEBREQ_CONNECTION		"Connection"
//值必须是 13
#define WEBREQ_SECWEBSOCKETVERSION "Sec-WebSocket-Version"

//常用响应头部 ,必须包含的头有：status-line,Upgrade,Connection,Sec-WebSocket-Accept
#define WEBRESP_SWCWEBSOCKETACCEPT	"Sec-WebSocket-Accept"

//都可用的可选字段
//子协议选择项。服务器在返回时根据请求的子协议选择一个可以接收的，并在放回报文中标明。
#define Sec_WebSocket_Protocol "Sec-WebSocket-Protocol"
#define WEBRESP_SECWEBSOCKETEXTENSIONSL	"Sec-WebSocket-Extensions"
class webSockReq
{
public:
	webSockReq()
	{
		m_CRLF="\r\n";
	}
	string& Encode2str()
	{
		string Message="";
		string request_line="GET "+m_wburl+"HTTP/1.1"+m_CRLF;
		Message+=WEBREQ_HOST+string(":");
		Message+=m_Host+m_CRLF;
		Message+=WEBREQ_UPGRADE+string(":")+m_Host+m_CRLF;
		Message+=WEBREQ_CONNECTION+string(":");
		Message+=m_Commection+m_CRLF;
		Message+=WEBREQ_SECWEBSOCKETKEY+string(":");
		Message+=m_WebSocket_key+m_CRLF;
		Message+=WEBREQ_SECWEBSOCKETVERSION+string(":");
		Message+=m_WebSocket_Version+m_CRLF;
		Message+=WEBREQ_ORIGIN+string(":");
		Message+=m_Origin+m_CRLF;
		Message+=Sec_WebSocket_Protocol+string(":");
		Message+=m_webSocket_Protocol+m_CRLF;
		return Message;
	}
	int Parse2Reg(char*strreq)
	{
		try{
			string message=strreq;
			m_request_line=message.find_first_of(m_CRLF);
			message=message.substr(message.find_first_of(m_CRLF)+2);
			string strhead=message.substr(0,message.find_first_of(m_CRLF+m_CRLF));
			while(strhead!="")
			{
				string temp=strhead.substr(0,strhead.find_first_of(m_CRLF));
				string strfield=temp.substr(0,temp.find_first_of(":"));
				string strvalue=temp.substr(temp.find_first_of(":")+1);
				strvalue=strterm(strhead);
				m_Head.insert(pair<string,string>(strfield,strvalue));
				strhead=strhead.substr(strhead.find_first_of(m_CRLF)+2);
			}
			m_requsetbody=message.substr(message.find_first_of(m_CRLF+m_CRLF)+4);
		}
		catch(std::exception & ee)
		{

		}
	}
public:
	string m_CRLF;
	string m_wburl;
	string m_Upgrade;
	string m_Commection;
	string m_Host;
	string m_Origin;
	string m_WebSocket_key;		//随机生成，提供基本的防护
	string m_WebSocket_Version;
	string m_webSocket_Protocol;
	string m_request_line;
	string m_requsetbody;
	map<string,string> m_Head;
};
class webSockResp
{
public:
	webSockResp()
	{
		m_CRLF="\r\n";
		m_SP=" ";
	}
	string& Encode2str()
	{
		string Message="";
		string request_line="HTTP/1.1"+m_SP+m_respCode+m_SP+"Switching Protocols"+m_CRLF;

		Message+=WEBREQ_UPGRADE+string(":")+m_Upgrade+m_CRLF;
		Message+=WEBREQ_CONNECTION+string(":");
		Message+=m_Commection+m_CRLF;
		Message+=WEBRESP_SWCWEBSOCKETACCEPT+string(":");
		Message+=m_WebSocket_Accept+m_CRLF;
		Message+=WEBREQ_SECWEBSOCKETVERSION+string(":");
		Message+=m_WebSocket_Version+m_CRLF;
		Message+=Sec_WebSocket_Protocol+string(":");
		Message+=m_WebSocket_Protocol+m_CRLF;
		return Message;
	}
	int Parse2Resp(char*)
	{

	}
public:
	string m_status_line;
	string m_respCode;
	string m_CRLF;
	string m_SP;
	string m_Upgrade;
	string m_Commection;
	string m_WebSocket_Accept;

	string m_WebSocket_Protocol;
	string m_WebSocket_Version;
};

//握手以后的数据帧
/*
1                   2                   3
1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
1）最多14字节的头部+数据
2）FIN bit 和 opecode(4bit) 用来表示帧是否分片
	i:开始帧（1个）--消息分片起始帧的构成是（FIN=0，opcode非零），即：FIN=0,opcode>0;
	ii:传输帧（0个或多个）---是由若干个（0个或多个）帧组成；即FIN=0,opcode=0;
	iii:终止帧--FIN=1,opcode=0;--->不分帧也是这种情况。
*/
class Dataframe
{
public:
	Dataframe()
	{
		m_FIN=0;
		m_code=0;
		m_ExtendPadlen=0;
		m_MASK=0;
		m_Payloadlen=0;
		m_MsgBody=NULL;
	}
	bool FrameParse(const char* Msg)
	{
		if(Msg=NULL)
			return false;
		string strmsg=Msg;
		if(strmsg.length()<14)
			return false;
		m_FIN=*Msg&0x01;
		m_code=*Msg&0xF0;
		m_MASK=*(Msg+1)&0x01;
		const char*temp=Msg;
		temp=&(*temp>>17);
		m_ExtendPadlen=*temp;
		temp=&(*temp>>64);
		m_MaskKey=*temp;
		m_MsgBody=&(*temp>>32);
		return 0;
	}
	Dataframe&  Encode2Frame();
public:
	int	 m_FIN;			//取前1bit.
	char m_code;		//取前4bit.
	char m_Payloadlen;	//取8bit.
	__int64 m_ExtendPadlen;//取64bit.
	int m_MASK;			//取32bit
	char* m_MsgBody;
	int m_MaskKey;

};
}

