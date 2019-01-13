//2019/1/6 带线程的reactor 
/*
1） 在些实际应用的时候发现，如果reactor不带线程，那么每次在外部调用start都会阻塞。
2） 给复用加一个自己的工作线程，start（）以后，主线程仍然能正常执行，
*/
#include "xthreadbase.hpp"
#include "xReactor.hpp"
namespace SAEBASE{
class xReactorwithThread:public xReactor,public Threadbase
{
public:
	virtual void run()
	{
		xReactor::start();
	}
	void startReactorWithThread()
	{
		Threadbase::start();
	}
};
}