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
#include<stdint.h>
#include<stdlib.h>
#include <time.h>
#include<BaseTsd.h>
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
	string m_WebSocket_key;		//随机生成，提供基本的防护，随机的16位字符经过base64编码而来。
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

0）FIN bit 和 opecode(4bit) 用来表示帧是否分片
	i:开始帧（1个）--消息分片起始帧的构成是（FIN=0，opcode非零），即：FIN=0,opcode>0;
	ii:传输帧（0个或多个）---是由若干个（0个或多个）帧组成；即FIN=0,opcode=0;
	iii:终止帧--FIN=1,opcode=0;--->不分帧也是这种情况。
1)关于该数据帧的解析，分三部分情况。
第一个字节：（报文是网络字节序，用网络字节序表示）
	i:最高位描述消息是否结束，如果为1，则该消息为消息尾部，如果为0，则后续还有数据包；后面3位用于扩展定义，
	如果没有扩展约定的情况则必须位=为0。FIN=data[0]&0x80
	ii:第四位bit用于描述消息的类型，消息暂定有15种，其中还有几种是预设置。
	opcode=data[0]&0x0F.
	0x0:代表一个继续帧
	0x1:代表一个文本帧
	0x2:代表一个二进制帧
	0x3-7: 保留
	0x8: 代表连接关闭
	0x9:代表ping
	0xA:代表pong
	0xB-F 保留
第二个字节
	消息的第二个字节主要用于描述掩码和消息长度，最高位用0或1来描述是否有掩码处理。
	MASK=data[1]&0x80   如果为1 ，则后面有Masking-key 这部分，如果为0，则没有。
	剩下7bit用来描述消息长度，由于7bit最多自能到127.所以这个值代表3种情况：
	i:消息内容少于126存储长度，则消息长度即为Payload len,Extended payload length为0
	ii:消息超度少于UNIT16的情况，此值为126.后面两字节为消息长度，Extended payload length为UINT16
	iii:消息长度大于UNIT16的情况，payload len=127,后面紧跟8字节的消息长度（UINT64）Extended payload length为UINT64
	当存在掩码时，消息长度后面会跟随4字节的Masking-key。
第三部分：
	数据
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
		Startbyte+=1;//第一个字节解析完毕，开始解析第二个字节
		m_MASK=(Data[Startbyte]&0x80)>>7;
		m_Payloadlen=Data[Startbyte]&0x7F;
		Startbyte+=1; //第二个字节解析完毕
		if(m_Payloadlen<126)
		{
			m_RealDatalen=m_Payloadlen;
			m_ExtendPaylen=0;
			if(m_MASK==1) //存在掩码
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
			if(m_MASK==1) //存在掩码
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
			if(m_MASK==1) //存在掩码
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
			return string("fail") ;//长度最长必须不超过2个字节
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
			return string("fail"); //长度过长
		}
		if(m_MASK==1)
		{
			//这里生成maskkey，不需要外部传入，随机的16位字符经过base64编码而来。
			(UINT32)*((UINT32*)&HeadM[4])=m_MaskKey;
		}

		return (char*)HeadM+string(m_MsgBody);
		
	}
	char* Encode2s(int fin,char code,int mask,unsigned int datalen,char*databody,int &msglen)
	{
		if(datalen>65535)
		{
			return "fail" ;//长度最长必须不超过2个字节
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
			return "fail"; //长度过长
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
			//利用掩码给信息内容加密。掩码为4字节，消息内容的字节按位置i 取 i mod 4 位置的一个掩码字节 与其异或，直到全部消息处理完成。
			//解码做同样的操作即可。
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
	int	 m_FIN;			//取前1bit.
	char m_code;		//取前4bit.
	char m_Payloadlen;	//取8bit.
	uint64_t m_ExtendPaylen;//取64bit.
	int m_MASK;			//取32bit
	const char* m_MsgBody;
	int m_MaskKey;
//辅助字段
	bool m_IsWholeMsg;	//数据最终帧是否到来。
	int m_RealDatalen;

};
}

