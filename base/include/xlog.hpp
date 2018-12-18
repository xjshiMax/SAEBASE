//2018-12-13
/*
��װ��־��ӡ��
1��ʵ�ַݵȼ���ӡ error debug info 
2) ֧�ִ����ļ������������̨
3��--xxxx--���̰߳�ȫ
4����־�ļ��ֶι̶���ʱ�䣬�ļ������к�
5����־ÿСʱ����һ���ļ���ÿ��һ���ļ���
6����־ɾ���Ժ���������ɡ�
7����־������ ����������Ժ�����
8) ���ɵ�����ģʽ
*/
#include <stdio.h>
#include "xdatetime.hpp"
#ifdef WIN32
#include <io.h>
#define _FILENAME(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#else
#include <unistd.h>
#define _FILENAME(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#endif
namespace SAEBASE{
typedef enum level{
	XLOG_ERROR,
	XLOG_WORNING,
	XLOG_NOTICE,
	XLOG_INFO,
	XLOG_DEBUG,

}Level;
//���ģʽ

//��׼����̨�������������ļ�����������ͬʱ���
#define XLOG_STDOUT		0x01
#define XLOG_LOGFILE	0x02
#define XLOG_LOGALL		(XLOG_STDOUT|XLOG_LOGFILE)

class xlog
{
public:
	xlog():m_globalLvevel(XLOG_DEBUG),m_OutType(XLOG_STDOUT)
	{
#ifdef WIN32
		m_FilePath="E:/aaa-shi/";
#else
		m_FilePath="/home/":
#endif

	}
	xlog(int loglevel,int Outtype=XLOG_STDOUT)
	{
		m_globalLvevel=loglevel;
		m_OutType=Outtype;
#ifdef WIN32
		m_FilePath="E:/aaa-shi/";
#else
		m_FilePath="/home/":
#endif

	}
	void XlogError(const char*fmt,...)
	{
		va_list ap;
		va_start(ap,fmt);
		log_help(XLOG_ERROR,fmt,ap);
		va_end(ap);
	}
	void XlogDebug(const char*fmt,...)
	{
		va_list ap;
		va_start(ap,fmt);
		log_help(XLOG_DEBUG,fmt,ap);
		va_end(ap);
	}
	void XlogWorning(const char*fmt,...)
	{
		va_list ap;
		va_start(ap,fmt);
		log_help(XLOG_WORNING,fmt,ap);
		va_end(ap);
	}
	void XlogNotice(const char*fmt,...)
	{
		va_list ap;
		va_start(ap,fmt);
		log_help(XLOG_NOTICE,fmt,ap);
		va_end(ap);
	}
	void XlogInfo(const char*fmt,...)
	{
		va_list ap;
		va_start(ap,fmt);
			log_help(XLOG_INFO,fmt,ap);
		va_end(ap);
	}
	void log_inner(int loglevel,char* buf)
	{
		string timestamp=HelpTime::getCurrentTime();
		string logoutlstr="";
		if(loglevel>m_globalLvevel)  //��־�ȼ�����������־�ȼ�������ӡ
			return ;
		switch(loglevel)
		{
		case XLOG_ERROR:
			logoutlstr="ERROR";
			break;
		case XLOG_WORNING:
			logoutlstr="WORNING";
			break;
		case XLOG_NOTICE:
			logoutlstr="NOTICE";
			break;
		case XLOG_INFO:
			logoutlstr="INFO";
			break;
		case XLOG_DEBUG:
			logoutlstr="DEBUG";
			break;
		}
		string logfile=__FILE__;
		int logline = __LINE__;
		if(m_OutType & XLOG_STDOUT)
			fprintf(stdout,"%s[%s:%d][%s] %s\n",timestamp.c_str(),m_logfile.c_str(),m_logline,logoutlstr.c_str(),buf);
			//fprintf(stdout,"test");
		if(m_OutType &XLOG_LOGFILE)
		{
			char tempbuff[1024+64]={0};
			sprintf(tempbuff,"%s[%s:%d][%s] %s\n",timestamp.c_str(),m_logfile.c_str(),m_logline,logoutlstr.c_str(),buf);
			std::string filename=m_FilePath+HelpTime::getLogFileName()+".txt";
			FILE* fp=NULL;
			if(access(filename.c_str(),0)==-1)
			{
				fp=fopen(filename.c_str(),"wb+");
			}
			else
				fp=fopen(filename.c_str(),"ab+");
			string tempstr=string(tempbuff);
			fwrite(tempbuff,tempstr.size(),1,fp);
			fclose(fp);

		}
	}
public:
	void SetLogLevel(int loglevel){m_globalLvevel=loglevel;}
	void SetOutPutType(int type){m_OutType=type;}
	void SetFilePath(string filepath)
	{
		int l1=filepath.find_last_of("/");
		int l2=filepath.size();
		if (filepath.find_last_of("/")+1!=filepath.size())
			filepath+="/";
		m_FilePath=filepath;
		m_OutType = XLOG_LOGFILE | m_OutType;
	}
	void SetWritefileAndLine(string logfile,int logline)
	{
		m_logfile=logfile;
		m_logline=logline;
	}
private:
	int xvsnprintf(char*buf,size_t buflen,const char*format,va_list ap)
	{
		int r;
		if(!buflen)
			return 0;
#if defined WIN32
		r=_vsnprintf(buf,buflen,format,ap);
		if(r<0)
			_vscprintf(format,ap);
#else
		r=vsnprintf(buf,buflen,format,ap);
#endif
		buf[buflen-1]= '\0';
		return r;
	}
	//�����趨����־�Ϊ1024���ֽڡ�
	int log_help(int LOG_Level,const char *fmt ,va_list ap)
	{
		char buf[1024];
		if(fmt!=NULL)
			xvsnprintf(buf,sizeof(buf),fmt,ap);
		else
			buf[0] = '\0';
		log_inner(LOG_Level,buf);
		return 0;
	}

public:
	//ȫ�ִ�ӡ�ȼ�
	int m_globalLvevel;
	//��ӡ��ʽ
	int m_OutType;
	string m_FilePath;
	//�����ļ������к� ������ ����Ӷ�����ڶ����߳����⣬��ʱ�����
	string m_logfile;
	int m_logline;

};

class xlogger/*:public xsingleton<xlog>*/
{
public:
	static xlog* instance();

private:
	static xlog *g_log;
};
//xlog* xlogger::g_log=NULL;
xlog* xlogger::g_log=NULL;
xlog* xlogger::instance()
{
	if(g_log==NULL)
			g_log=new xlog();
	return g_log;
}


#define 	XLOG_ERROROUT(fmt,...)		do{xlogger::instance()->SetWritefileAndLine(_FILENAME(__FILE__),__LINE__);	xlogger::instance()->XlogError(fmt,##__VA_ARGS__);}while(0);
#define 	XLOG_WORNINGOUT(fmt,...)	do{xlogger::instance()->SetWritefileAndLine(_FILENAME(__FILE__),__LINE__);	xlogger::instance()->XlogWorning(fmt,##__VA_ARGS__);}while(0);
#define		XLOG_NOTICEOUT(fmt,...)		do{xlogger::instance()->SetWritefileAndLine(_FILENAME(__FILE__),__LINE__);	xlogger::instance()->XlogNotice((fmt,##__VA_ARGS__);}while(0);
#define		XLOG_INFOOUT(fmt,...)		do{xlogger::instance()->SetWritefileAndLine(_FILENAME(__FILE__),__LINE__);	xlogger::instance()->XlogInfo(fmt,##__VA_ARGS__);}while(0);
#define		XLOG_DEBUGOUT(fmt,...)		do{ \
	xlogger::instance()->SetWritefileAndLine(_FILENAME(__FILE__),__LINE__); \
	xlogger::instance()->XlogDebug(fmt,##__VA_ARGS__);	\
}while(0);

}