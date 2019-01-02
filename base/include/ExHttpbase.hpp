//2018-12-25��Ex ϵ��hpp�����⡿ by xjshi
/*
��װ����httpЭ�顣
1��������󴴽�http client
2) ���̣߳���ʹ�ø���
3������ֶκ��Զ��ֶ�
4����Ӧ����
Request Line<CRLF>
Header-Name: header-value<CRLF>
Header-Name: header-value<CRLF>
//һ������������<CRLF>��β
<CRLF>
body//��������
*/
#include <stdio.h>
#include <string>
#include <map>
#include "xHelpstring.hpp"
using namespace std;
namespace SAEBASE{

/*1.������ Request-Line
	Request-Line=Method SP Request-URI SP HTTP-Version CRLF
  2.Request-URI
	Request-URI="*" | ����uri|����·��|��֤
	"*": ֻ���ڷ���������  OPTIONS * HTTP/1.1
	����uri:
	����·����
	��֤�� authority ֻ������CONNECT
  3.��Ӧ�� Respose
	response=Status-Line *(һ����ͷ��) |response-header|entity-header CRLF [message-body]
  4.״̬�� Status-Line
	Status-Line=HTTP-Version SP Status-Code SP Reason-Phrase CRLF
*/
//http��Ϣ���� Method
	enum{
		EXHTTP_REQ_GET,
		EXHTTP_REQ_POST,
		EXHTTP_REQ_HEAD,
		EXHTTP_REQ_PUT,
		EXHTTP_REQ_DELETE,
		EXHTTP_REQ_OPTIONS,
		EXHTTP_REQ_TRACE,
		EXHTTP_REQ_CONNECT,
		EXHTTP_REQ_PATCH
	};
	//��������ͷ��
#define REQ_ACCEPT				"Accept"
#define REQ_ACCEPT_CHARSET		"Accept-Charset"
#define REQ_ACCEPR_ENCODING		"Accept-Encoding"
#define REQ_ACCEPT_LANGUAGE		"Accept-Language"
#define REQ_AUTHRIZATION		"Authorization"
#define REQ_EXPECT				"Expect"
#define REQ_FROM				"From"
#define REQ_HOST				"Host"
#define REQ_IF_MATCH			"If-Match"
#define REQ_MAX_FORWARDS		"Max-Forwards"
#define REQ_RANGE				"Range"
	//������Ӧͷ��
#define RESPONSE_ACCEPT_RANGES		"Accept-Ranges"
#define RESPONSE_AGE				"Age"
#define RESPONSE_ETAG				"Etag"
#define RESPONSE_LOCATION			"Location"
#define RESPONSE_PROXY_AUTENTICATE	"Proxy-Autenticate"
#define RESPONSE_RETRY_AFTER		"Retry-After"
#define RESPONSE_SERFER				"Server"
#define RESPONSE_VARY				"Vary"
#define RESPONSE_WWW_AUTHENTICATE	"WWW-Authenticate"
//�������� 1xx:����� ���󱻽��յ�����������
	static const char *informational_phrases[] = {
		/* 100 */ "Continue",
		/* 101 */ "Switching Protocols"
	};
//2xx �ɹ�
	static const char *success_phrases[] = {
		/* 200 */ "OK",
		/* 201 */ "Created",
		/* 202 */ "Accepted",
		/* 203 */ "Non-Authoritative Information",
		/* 204 */ "No Content",
		/* 205 */ "Reset Content",
		/* 206 */ "Partial Content"
	};
//3xx �ط�  - Ϊ�������������ȡ��һ���Ķ���
	static const char *redirection_phrases[] = {
		/* 300 */ "Multiple Choices",
		/* 301 */ "Moved Permanently",
		/* 302 */ "Found",
		/* 303 */ "See Other",
		/* 304 */ "Not Modified",
		/* 305 */ "Use Proxy",
		/* 307 */ "Temporary Redirect"
	};
//4xx �ͻ��˳���  - Ϊ�������������ȡ��һ���Ķ���
	static const char *client_error_phrases[] = {
		/* 400 */ "Bad Request",
		/* 401 */ "Unauthorized",
		/* 402 */ "Payment Required",
		/* 403 */ "Forbidden",
		/* 404 */ "Not Found",
		/* 405 */ "Method Not Allowed",
		/* 406 */ "Not Acceptable",
		/* 407 */ "Proxy Authentication Required",
		/* 408 */ "Request Time-out",
		/* 409 */ "Conflict",
		/* 410 */ "Gone",
		/* 411 */ "Length Required",
		/* 412 */ "Precondition Failed",
		/* 413 */ "Request Entity Too Large",
		/* 414 */ "Request-URI Too Large",
		/* 415 */ "Unsupported Media Type",
		/* 416 */ "Requested range not satisfiable",
		/* 417 */ "Expectation Failed"
	};
//5xx ����������  �������޷������Ȼ��Ч������
	static const char *server_error_phrases[] = {
		/* 500 */ "Internal Server Error",
		/* 501 */ "Not Implemented",
		/* 502 */ "Bad Gateway",
		/* 503 */ "Service Unavailable",
		/* 504 */ "Gateway Time-out",
		/* 505 */ "HTTP Version not supported"
	};
// ʵ��entity, �������Ӧ������Я���Ĳ��֣���Ϊʵ��ͷ�� ��ʵ������
//���õ�entity-head, ͨ���������Զ���ͷ��
//ʵ�����弴Ϊ���ǳ�˵��http����Ϣ�塣entity-body ��entity-header���ֶ��������������ͺͳ��ȣ�md5 ...
#define ALLOW		"Allow"
#define CONTENT_ENCODING	"Content-Encoding"
#define CONTENT_LANGUAGE	"Content-Language"
#define CONTENT_LENGTH		"Content-Length"
#define CONTENT_LOCATION	"Content-Location"
#define CONTENT_MD5			"Content-MD5"
#define CONTENT_TYPE		"Content-Type"
#define EXPIRES				"Expires"
#define LAST_MODIFIED		"Last-Modified"
#define EXTENSION_HEADER	"extension-header"
#define USER_AGENT			"User-Agent"

class HttpResponse
{
public:
	HttpResponse()
	{
		m_HttpVersion="HTTP/1.1";
		m_SP=" ";
		m_CRLF="\r\n";
	}
	HttpResponse(char*resp)
	{
		m_HttpVersion="HTTP/1.1";
		m_SP=" ";
		m_CRLF="\r\n";
		Parsestr(resp);
	}
	HttpResponse(HttpResponse& resp);
	HttpResponse& operator = (HttpResponse);
	void Parsestr(char*Msg)
	{
		try{
			string message=Msg;
			m_status_Line=message.find_first_of(m_CRLF);
			message=message.substr(message.find_first_of(m_CRLF)+2);
			string strhead=message.substr(0,message.find_first_of(m_CRLF+m_CRLF));
			while(strhead!="")
			{
				string temp=strhead.substr(0,strhead.find_first_of(m_CRLF));
				string strfield=temp.substr(0,temp.find_first_of(":"));
				string strvalue=temp.substr(temp.find_first_of(":")+1);
				strvalue=SAEBASE::term(strhead);
				m_Head.insert(pair<string,string>(strfield,strvalue));
				strhead=strhead.substr(strhead.find_first_of(m_CRLF)+2);
			}
			m_responsebody=message.substr(message.find_first_of(m_CRLF+m_CRLF)+4);
		}

	}

	void SetHead(string field,string strvalue);
	int GetHead(string field,OUT string &strvalue);
	const string Encode2str(HttpResponse&resp)
	{
		
	}
	const string ReqTstr()
	{
// 		string AllMsg;
// 		m_s
	 const string s;
	 return s;

	}
	string m_status_Line;
	string m_Method;
	string m_requesturl;
	string m_HttpVersion;
	string m_CRLF;
	string m_SP;
	string m_responsebody;
	map<string,string> m_Head;
	string m_OrigMsg;
};
class HttpRequest
{
public:
	HttpRequest()
	{
		m_HttpVersion="HTTP/1.1";
		m_SP=" ";
		m_CRLF="\r\n";
		m_userData="";
	}
	HttpRequest(char*data);
	HttpRequest(HttpRequest&req);
	void Parsestr(char*Msg);
	void SetHead(string field,string strvalue);
	void SetMethod(string method)
	{
		m_Method=method;
	}
	void SetURL(string url)
	{
		m_requesturl=url;
	}
	void SetUserdata(string userdata)
	{
		m_userData=userdata;
	}
	int GetHead(string field,OUT string &strvalue);
	const string Encode2str(HttpResponse&);
	const string PostReq2str()
	{
		m_start_Line=m_Method+m_SP+m_requesturl+m_SP+m_HttpVersion+m_CRLF;
		m_AllMsg+=m_start_Line;
		map<string,string>::iterator headit=m_HeadSet.begin();
		string strHead="";
		while(headit!=m_HeadSet.end())
		{
			strHead+=headit->first+"="+headit->second+m_CRLF;
			headit++;
		}
		m_AllMsg+=strHead;
		m_AllMsg+=m_CRLF;
		m_AllMsg+=m_userData;
		return m_AllMsg;
	}
	HttpRequest*Parsestr(char*Msg)
	{
		try{
			string message=Msg;
			m_start_Line=message.find_first_of(m_CRLF);
			message=message.substr(message.find_first_of(m_CRLF)+2);
			string strhead=message.substr(0,message.find_first_of(m_CRLF+m_CRLF));
			while(strhead!="")
			{
				string temp=strhead.substr(0,strhead.find_first_of(m_CRLF));
				string strfield=temp.substr(0,temp.find_first_of(":"));
				string strvalue=temp.substr(temp.find_first_of(":")+1);
				strvalue=SAEBASE::term(strhead);
				m_HeadSet.insert(pair<string,string>(strfield,strvalue));
				strhead=strhead.substr(strhead.find_first_of(m_CRLF)+2);
			}
			m_responsebody=message.substr(message.find_first_of(m_CRLF+m_CRLF)+4);
			//m_status_Line=message.find_first_of(m_CRLF);
			//message=message.substr(message.find_first_of(m_CRLF)+2);
			//string strhead=message.substr(0,message.find_first_of(m_CRLF+m_CRLF));
			//m_responsebody=message.substr(message.find_first_of(m_CRLF+m_CRLF)+4);

			m_AllMsg=Msg;
			m_userData=m_responsebody;
			m_Method=m_start_Line.find_first_of(" ");
			string temp=m_start_Line.substr(m_start_Line.find_first_of(" ")+1);
			m_requesturl=temp.find_first_of(" ");
			m_HttpVersion=temp.substr(temp.find_first_of(" ")+1);
			m_HttpVersion=term(m_HttpVersion);

		}
		return this;
	}
	string m_start_Line;
	string m_Method;
	string m_requesturl;
	string m_HttpVersion;
	string m_CRLF;
	string m_SP;
	map<string,string> m_HeadSet;
	string m_userData;
	string m_AllMsg;
};
};
