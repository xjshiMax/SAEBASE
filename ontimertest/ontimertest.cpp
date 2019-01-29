// ontimertest.cpp : 定义控制台应用程序的入口点。
// 2019/1/25 测试 OnTimerBase 类。定时执行
//linux 和windows均通过编译。

//#include "../base/include/xbaseclass.hpp"
//#include "../base/include/xtimeheap.hpp"
//#include "../base/include/xReactor.hpp"
//#include "../base/include/timebase.hpp"
#include "../base/include/ontimebase.hpp"
using namespace SAEBASE;
// class TimeEvent:public xEventHandler
// {
// 	virtual SAEBASE::handle_t GetHandler()const
// 	{
// 		return m_Eventfd;
// 	}
// 	virtual void HandleRead(int listentfd){}
// 	virtual void HandlerWrite(){}
// 	virtual void HandlerError(){}
// };
// class mytime :public xheaptimer
// {
// 
// };
// xheaptimer itimeon(5);
// void __cdecl funtimeon(void*arg)
// {
// 	printf("this is ontime\n");
// 
// 	//重新注册
// 	xReactor* reactor_=(xReactor*)arg;
// 	xheaptimer itimeon(5);
// 	itimeon.cb_func=funtimeon;
// 	itimeon.user_data=(void*)reactor_;
// 	reactor_->RegisterTimeTask(&itimeon);
// }

class mytimer:public OnTimerBase
{

public:
	mytimer(int timeout):OnTimerBase(timeout)
	 {}
	virtual void timeout()
	{
		printf("this is in timeout!\n");
	}
};
 void* func(void*arg)
 {
 	printf("基于对象，pthread\n");
 };
int main(int argc, char* argv[])
{
// 	TimeEvent ontime;
// 	xheaptimer itimeon(5);
// 	itimeon.cb_func=funtimeon;
// 	xReactor myReactor;
// 	itimeon.user_data=(void*)&myReactor;
// 	myReactor.RegisterTimeTask(&itimeon);
// 	myReactor.start();

	//mytimer time(5) ;
	//time.startTimer();
	//time.join();
	xThread mythread;
	mythread.start(func,NULL);
	mythread.join();
	return 0;
}

