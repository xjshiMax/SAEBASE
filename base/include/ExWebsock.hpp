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
3�� ���ֽ׶εĿͻ������󣬴�����ѭhttpЭ�顣
i:�������������GET����
ii��HTTP�汾������1.1�����ϡ�
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
//һ������������<CRLF>��β
<CRLF>
������get������û��body.

4) ����ˡ� �����Ӧʱ101�����ʾ����˽��գ����ֳɹ������������ɹ���
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
//������ͷ��,������е��ֶΣ�request-uri,Host,Upgrade,Connection,Sec-WebSocket-Key

//���ڱ�����ֹ �������δ��Ȩ�Ľű�ʹ��Websocket API  ����ʹ��websocket �����������˾ܾ�
//�ͻ��˵����ӣ�������㷢һ��http����
#define WEBREQ_ORIGIN			"Origin"
#define WEBREQ_SECWEBSOCKETKEY	"Sec-WebSocket-Key"
#define WEBREQ_HOST				"Host"
//ֵ������ websocket
#define WEBREQ_UPGRADE			"Upgrade"
#define WEBREQ_CONNECTION		"Connection"
//ֵ������ 13
#define WEBREQ_SECWEBSOCKETVERSION "Sec-WebSocket-Version"

//������Ӧͷ�� ,���������ͷ�У�status-line,Upgrade,Connection,Sec-WebSocket-Accept
#define WEBRESP_SWCWEBSOCKETACCEPT	"Sec-WebSocket-Accept"

//�����õĿ�ѡ�ֶ�
//��Э��ѡ����������ڷ���ʱ�����������Э��ѡ��һ�����Խ��յģ����ڷŻر����б�����
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
	string m_WebSocket_key;		//������ɣ��ṩ�����ķ���
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

//�����Ժ������֡
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
1�����14�ֽڵ�ͷ��+����
2��FIN bit �� opecode(4bit) ������ʾ֡�Ƿ��Ƭ
	i:��ʼ֡��1����--��Ϣ��Ƭ��ʼ֡�Ĺ����ǣ�FIN=0��opcode���㣩������FIN=0,opcode>0;
	ii:����֡��0��������---�������ɸ���0��������֡��ɣ���FIN=0,opcode=0;
	iii:��ֹ֡--FIN=1,opcode=0;--->����֡Ҳ�����������
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
	int	 m_FIN;			//ȡǰ1bit.
	char m_code;		//ȡǰ4bit.
	char m_Payloadlen;	//ȡ8bit.
	__int64 m_ExtendPadlen;//ȡ64bit.
	int m_MASK;			//ȡ32bit
	char* m_MsgBody;
	int m_MaskKey;

};
}

