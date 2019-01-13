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
#include<stdint.h>
#include<stdlib.h>
#include <time.h>
#include<BaseTsd.h>
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
#define Keystring "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
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
				//strvalue=strterm(strhead);
				m_Head.insert(pair<string,string>(strfield,strvalue));
				strhead=strhead.substr(strhead.find_first_of(m_CRLF)+2);
			}
			m_requsetbody=message.substr(message.find_first_of(m_CRLF+m_CRLF)+4);
		}
		catch(std::exception & ee)
		{

		}
	}
	static string EasyHandshake(char* wsurl)
	{
		string req="GET ";
		req+=wsurl;
		req+=" HTTP/1.1\r\n";
		req+="Host: ExWebsockClient\r\n";
		req+="Upgrade: websocket\r\n";
		req+="Connection: Upgrade\r\n";
		req+="Sec-WebSocket-Key: cXdlcmdiZnJ0Z2hraXVn\r\n";
		req+="Origin: http://tool.hibbba.com\r\n";
		req+="Sec-WebSocket-Version:13\r\n";
		req+="\r\n";
		return req;
	}
public:
	string m_CRLF;
	string m_wburl;
	string m_Upgrade;
	string m_Commection;
	string m_Host;
	string m_Origin;
	string m_WebSocket_key;		//������ɣ��ṩ�����ķ����������16λ�ַ�����base64���������
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

0��FIN bit �� opecode(4bit) ������ʾ֡�Ƿ��Ƭ
	i:��ʼ֡��1����--��Ϣ��Ƭ��ʼ֡�Ĺ����ǣ�FIN=0��opcode���㣩������FIN=0,opcode>0;
	ii:����֡��0��������---�������ɸ���0��������֡��ɣ���FIN=0,opcode=0;
	iii:��ֹ֡--FIN=1,opcode=0;--->����֡Ҳ�����������
1)���ڸ�����֡�Ľ������������������
��һ���ֽڣ��������������ֽ����������ֽ����ʾ��
	i:���λ������Ϣ�Ƿ���������Ϊ1�������ϢΪ��Ϣβ�������Ϊ0��������������ݰ�������3λ������չ���壬
	���û����չԼ������������λ=Ϊ0��FIN=data[0]&0x80
	ii:����λbit����������Ϣ�����ͣ���Ϣ�ݶ���15�֣����л��м�����Ԥ���á�
	opcode=data[0]&0x0F.
	0x0:����һ������֡
	0x1:����һ���ı�֡
	0x2:����һ��������֡
	0x3-7: ����
	0x8: �������ӹر�
	0x9:����ping
	0xA:����pong
	0xB-F ����
�ڶ����ֽ�
	��Ϣ�ĵڶ����ֽ���Ҫ���������������Ϣ���ȣ����λ��0��1�������Ƿ������봦��
	MASK=data[1]&0x80   ���Ϊ1 ���������Masking-key �ⲿ�֣����Ϊ0����û�С�
	ʣ��7bit����������Ϣ���ȣ�����7bit������ܵ�127.�������ֵ����3�������
	i:��Ϣ��������126�洢���ȣ�����Ϣ���ȼ�ΪPayload len,Extended payload lengthΪ0
	ii:��Ϣ��������UNIT16���������ֵΪ126.�������ֽ�Ϊ��Ϣ���ȣ�Extended payload lengthΪUINT16
	iii:��Ϣ���ȴ���UNIT16�������payload len=127,�������8�ֽڵ���Ϣ���ȣ�UINT64��Extended payload lengthΪUINT64
	����������ʱ����Ϣ���Ⱥ�������4�ֽڵ�Masking-key��
�������֣�
	����
*/
class Dataframe
{
public:
	Dataframe()
	{
		m_FIN=0;
		m_code=0;
		m_ExtendPaylen=0;
		m_MASK=0;
		m_Payloadlen=0;
		m_MsgBody=NULL;
		m_IsWholeMsg=false;
	}
	bool FrameParse(const char* Msg)
	{
		const char* Data=Msg;
		int Startbyte=0;
		if(Msg==NULL)
			return false;
		if(strlen(Msg)<4)
			return false;

		m_FIN=(Data[Startbyte]&0x80)>>7;
		if(m_FIN==1)
			m_IsWholeMsg=true;
		m_code=(Data[Startbyte]&0x0F);
		Startbyte+=1;//��һ���ֽڽ�����ϣ���ʼ�����ڶ����ֽ�
		m_MASK=(Data[Startbyte]&0x80)>>7;
		m_Payloadlen=Data[Startbyte]&0x7F;
		Startbyte+=1; //�ڶ����ֽڽ������
		if(m_Payloadlen<126)
		{
			m_RealDatalen=m_Payloadlen;
			m_ExtendPaylen=0;
			if(m_MASK==1) //��������
			{
				m_MaskKey=UINT32((uint32_t *)&Data[Startbyte]);
				Startbyte+=4;
			}
		}
		else if(m_Payloadlen==126)
		{
			UINT16 ExtendPayloadLen = UINT16((uint32_t *)&Data[Startbyte]);
			m_ExtendPaylen= ExtendPayloadLen;
			m_RealDatalen=m_ExtendPaylen;
			Startbyte+=2;
			if(m_MASK==1) //��������
			{
				m_MaskKey=UINT64((uint32_t *)&Data[Startbyte]);
				Startbyte+=4;
			}
		}
		else if(m_Payloadlen==127)
		{
			UINT64 ExtendPayloadLen = UINT64((uint32_t *)&Data[Startbyte]);
			m_ExtendPaylen= ExtendPayloadLen;
			m_RealDatalen=m_ExtendPaylen;
			Startbyte+=8;
			if(m_MASK==1) //��������
			{
				m_MaskKey=UINT64((uint32_t *)&Data[Startbyte]);
				Startbyte+=4;
			}
		}
		else
		{
			return false;
		}
		m_MsgBody=&Data[Startbyte];
		return true;
	}
	string&  Encode2str()
	{
		if(m_RealDatalen>65535)
		{
			return string("fail") ;//��������벻����2���ֽ�
		}
		UINT8 headlen=2;
		headlen+=(m_RealDatalen<126)?0:2;
		headlen+=(m_MASK==1)?4:0;

		UINT8* HeadM=new UINT8[headlen];
		memset(HeadM,0,headlen);
		HeadM[0]= static_cast<uint8_t>(0x80 | m_FIN);
		HeadM[0]=HeadM[0]|(0x0F&m_code);
		HeadM[1]=static_cast<uint8_t>(0x80 | m_MASK);
		if(m_RealDatalen<126)
			HeadM[1]=HeadM[1]|(0x7E&m_Payloadlen);
		else if(m_RealDatalen>126 &&m_RealDatalen<65535)
		{
			HeadM[1]=HeadM[1]|0x7E;
			(UINT16)*((UINT16*)&HeadM[2])=m_RealDatalen;
		}
		else
		{
			return string("fail"); //���ȹ���
		}
		if(m_MASK==1)
		{
			//��������maskkey������Ҫ�ⲿ���룬�����16λ�ַ�����base64���������
			(UINT32)*((UINT32*)&HeadM[4])=m_MaskKey;
		}

		return (char*)HeadM+string(m_MsgBody);
		
	}
	char* Encode2s(int fin,char code,int mask,unsigned int datalen,char*databody,int &msglen)
	{
		if(datalen>65535)
		{
			return "fail" ;//��������벻����2���ֽ�
		}
		UINT8 headlen=2;
		headlen+=(datalen<126)?0:2;
		headlen+=(mask==1)?4:0;

		UINT8* HeadM=new UINT8[headlen];
		memset(HeadM,0,headlen);
		HeadM[0]= (static_cast<uint8_t>(0x01 & fin))<<7;
		HeadM[0]=HeadM[0]|(0x0F&code);
		HeadM[1]=(static_cast<uint8_t>(0x01 & mask))<<7;
		if(datalen<126)
			HeadM[1]=HeadM[1]|(0x7F&datalen);
		else if(datalen>126 &&datalen<65535)
		{
			HeadM[1]=HeadM[1]|0x7E;
			(UINT16)*((UINT16*)&HeadM[2])=datalen;
		}
		else
		{
			return "fail"; //���ȹ���
		}
		if(mask)
		{
			(UINT32)*((UINT32*)&HeadM[headlen-4])=createMaskkey();
		
		}
		char *msg=new char[headlen+datalen+1];
		memset(msg,0,headlen+datalen+1);
		memcpy(msg,HeadM,headlen);
		memcpy(msg+headlen,databody,datalen);
		if(mask==1)
		{
			//�����������Ϣ���ݼ��ܡ�����Ϊ4�ֽڣ���Ϣ���ݵ��ֽڰ�λ��i ȡ i mod 4 λ�õ�һ�������ֽ� �������ֱ��ȫ����Ϣ������ɡ�
			//������ͬ���Ĳ������ɡ�
			for (int i = 0; i < datalen; i++) {
				msg[i + headlen] ^= (msg + headlen - 4)[i % 4];
				//cout<<"after:"<<msg[i + headlen]<<endl;
			}
			//cout<<std::endl;
		}
		
		msg[headlen+datalen]='\0';
		string resultmsg=msg;
		//delete []msg;
		delete []HeadM;
		msglen=headlen+datalen;
		return msg;
	}
	static unsigned int createMaskkey()
	{
		UINT8 key[4]={0};
		srand(time(0));
		for(int i=0;i<4;i++)
		{
			UINT8 temp=rand()%(0xF-0x1)+0x1;
			key[i]=(rand()%(0xF-0x1)+0x1)|(temp<<4);
		}
		return *((UINT32*)key);
	}
public:
	int	 m_FIN;			//ȡǰ1bit.
	char m_code;		//ȡǰ4bit.
	char m_Payloadlen;	//ȡ8bit.
	uint64_t m_ExtendPaylen;//ȡ64bit.
	int m_MASK;			//ȡ32bit
	const char* m_MsgBody;
	int m_MaskKey;
//�����ֶ�
	bool m_IsWholeMsg;	//��������֡�Ƿ�����
	int m_RealDatalen;

};
}

