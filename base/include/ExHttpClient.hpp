//2018-12-27��Ex ϵ��hpp�����⡿ by xjshi
/*
1) ���̵߳��ã�request֮������ͬ���յ�response.
2) Ĭ�϶����ӣ�����֮�󣬵ȹ���ʱʱ���ղ����ָ�����Ͽ����ӣ�Ȼ�󷵻�
3) ���������ʱ���壬��ʱ����
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
		//http����
		//timeout :5s
		//url�� http://192.168.1.108:8080/xxx �˸�ʽ
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
		//����Ĭ��ͷ��

		vector<string> m_HeadSet;
		ExTcpClient m_transport_sock;

	};
};