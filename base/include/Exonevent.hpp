//继承xEventHandler，便于接收数据
#include "xbaseclass.hpp"
namespace SAEBASE{
#define MAXREADSIZE		1024*8
class Onevent:public xEventHandler
{
public:
	virtual int Ondata(int socketfd,char*data,int len)=0;
	virtual int Onclose(int socketfd){return 0;}

	virtual handle_t GetHandler()const =0;
private:
	virtual void HandleRead(int listentfd){
		char buf[MAXREADSIZE]={0};
		int len=MAXREADSIZE;
		int iret = ReadSocket(listentfd,buf,len);
		len=sizeof(buf);
		if(iret==0)
		{
			CloseSocket(listentfd);
			this->Onclose(listentfd);
		}
		else if(iret>=0)
			if(strlen(buf))
				this->Ondata(listentfd,buf,iret);
		else
			return ;
	}


};

}