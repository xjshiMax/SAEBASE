//2018-12-27【Ex 系列hpp函数库】 by xjshi
/*
1) 单线程调用，request之后阻塞同步收到response.
2) 默认短链接，发送之后，等过超时时间收不到恢复，则断开连接，然后返回
3) 面向对象，随时定义，随时调用
*/

#include "ExHttpbase.hpp"
#include "ExTcpClient.hpp"
#include <string>
#include <regex>
#define Maxresponse 1024*1024
using namespace std;
namespace SAEBASE{
	class ExHttpClient
	{
	public:
		//http请求
		//timeout :5s
		//url： http://192.168.1.108:8080/xxx 此格式
		HttpResponse Request(string  method,string url,string Msg,int timeout=5)
		{
			HttpRequest request_;
			request_.SetMethod("POST");
			request_.SetURL(url);
			request_.SetHead(REQ_ACCEPT,"*/*");
			request_.SetHead(REQ_HOST,"ExHttpclient");
			request_.SetUserdata(Msg);
			request_.SetHead(CONTENT_ENCODING,"utf-8");
			request_.SetHead(CONTENT_LENGTH,Msg.length());
			string strtemp=url.substr( url.find_first_of("/")+1);
			string strip=strtemp.substr(0,strtemp.find_first_of(":"));
			strtemp=strtemp.substr(strtemp.find_first_of(":")+1);
			string port=strtemp.substr(strtemp.find_first_of("/"));
			SetSockOpt(m_transport_sock.getSockfd(),SOL_SOCKET,SO_RCVTIMEO,(char*)timeout,sizeof(int));
			m_transport_sock.connectTCP((char*)strip.c_str(),atoi(port.c_str()));
			string httpcontent=request_.PostReq2str();
			m_transport_sock.sendMsg((char*)httpcontent.c_str(),httpcontent.length());
			char strresp[Maxresponse];
			m_transport_sock.recieve(strresp,Maxresponse);
			HttpResponse HttpResp(strresp);
			return HttpResp;
		}
		HttpResponse Request(string );

	private:
		string M_RequesrLine;
		string m_Method;
		string m_Url;
		string m_HttpVersion;
		//常用默认头域

		vector<string> m_HeadSet;
		ExTcpClient m_transport_sock;

	};
};