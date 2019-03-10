//2018-12-5
//创建基于对象的线程池
//      threadpool --------taskbase.run()
//2019-3-3 修改线程池 by xjshi
/* 
xthreadPool 作为一个管理类，不需要wait操作。统一的任务池
threadobj在类外边定义带线程的对象，但是没有自己的任务池。
*/
#include <stdio.h>
#include <string>
#include <list>
#include <exception>
#include "xAutoLock.hpp"
#include "xbaseclass.hpp"
#include "xthreadbase.hpp"
#include "xTaskqueue.h"
using namespace std;
namespace SAEBASE{
//任务对象的即基类。每个job都是xtaskbase派生类的对象
//对于继承xtaskbase的类，最好显示写构造和析构函数。
class xtaskbase
{
public:
	xtaskbase(void *arg=NULL,const std::string taskName = "")
		:arg_(arg),taskName_(taskName)
	{

	}
	virtual ~xtaskbase(){}
	void setArg(void *arg){arg_=arg;};
	virtual int run() {return 0;} ; //任务类重写run，在里面实现业务逻辑。
protected:
	void* arg_;
	std::string taskName_;

};

class xsimpleThreadPool
{
public:
	xsimpleThreadPool(){}
	xsimpleThreadPool(const char*poolname):m_PoolName(poolname){}
	void initPool(size_t LowThreadNumber=4,size_t HightThreadNumber=16)
	{
		m_threadNum=LowThreadNumber;
	}
	void startPool(bool defaultpools=false )
	{
		for(int i=0;i<m_threadNum;i++)
		{
			xThread workThread;
			workThread.start(threadproxy,this);
			m_threadList.push_back(&workThread);
			//workThread.start(threadproxy,this);
		}
	}
	void stopPool(bool defaultpools=true)
	{
		if(shutdown)
		{
			return  ;
		}
		shutdown = true;
		m_threadCond.broadCast();
		for( vector<xThread*>::iterator ite=m_threadList.begin();ite!=m_threadList.end();ite++)
		{
			(*ite)->join();
			(*ite)->destory();
		}
	}	
    void pushObj(xtaskbase*task)
	{
		xAutoLock L(m_threadLock);
		m_taskList.push_back(task);
		m_threadCond.signal();
	}
	void waitforAllTaskDone(void)
	{
		for( vector<xThread*>::iterator ite=m_threadList.begin();ite!=m_threadList.end();ite++)
		{
			(*ite)->join();
		}
	}
#ifdef WIN32
	 static unsigned int __stdcall threadproxy(void* arg);
#else
	 static void*  __stdcall threadproxy(void* arg);
#endif

private:
	static std::deque<xtaskbase*> m_taskList;
	std::vector<xThread*> m_threadList;
	int m_threadNum;
	static xMutex m_threadLock;
	static xCondition m_threadCond;
	static bool shutdown;
	string m_PoolName;
};
bool xsimpleThreadPool::shutdown=false;
xMutex xsimpleThreadPool::m_threadLock;
xCondition xsimpleThreadPool::m_threadCond;
std::deque<xtaskbase*> xsimpleThreadPool::m_taskList;
#ifdef WIN32
	 unsigned int __stdcall xsimpleThreadPool::threadproxy(void* arg)
#else
	 void*  __stdcall xsimpleThreadPool::threadproxy(void* arg)
#endif
{
	pthread_t tid = pthread_self();
	xThread* pthread=(xThread*)arg;
	while(1)
	{
		xAutoLock L(m_threadLock);
		while(m_taskList.size()==0&&!shutdown)
		{
			m_threadCond.wait(m_threadLock);
		}
		if(shutdown)
		{
			pthread->destory();
		}
		xtaskbase* task=m_taskList.front();
		m_taskList.pop_front();
		task->run();
	}
	return 0;
}
//需要设置成不可复制的类
class xthreadPool/*:protected Noncopyable*/
{
public:
	//typedef typename xTaskqueue<xtaskbase> xTaskqueue_type; 
	xthreadPool(const char* poolname):m_strPoolName(poolname)
	{
		//m_strPoolName=string(poolname);
	}
	virtual ~xthreadPool(){stopPool();};
	//初始化线程池
	//	LowThreadNumber 最小贤臣数量
	//	HightThreadNumber 最大线程数量
	//	IdleTimeout		线程超时等待时间
	//	Tasklistsize	任务队列默认大小
	//	线程堆栈大小  
	void initPool(size_t LowThreadNumber=4,size_t HightThreadNumber=16,
		size_t IdleTimeout=30,size_t Tasklistsize=3000,size_t threadStackSize=1024*1024 )
		{
			stopPool();
			m_nFreeThreadCount=0;
			m_nIdleTimeout=IdleTimeout;
			m_threadNum = LowThreadNumber;
			m_nThreadStackSize = threadStackSize;
			m_tasklist.resizeQueue(Tasklistsize);
		}
	//简版
	void initsimplePool(size_t ThreadNumber=4,size_t IdleTimeout=30,size_t Tasklistsize=3000,size_t threadStackSize=1024*1024);
	void startPool(bool defaultpools=false );
	void stopPool(bool defaultpools=true)  //默认defaultpools为true，清除掉所有线程
	{
		xAutoLock lock(m_lockForThread);
		m_tasklist.setDeadstatus();
		std::list<threadobj*>::iterator ite = m_ThreadList.begin();
		while(ite!=m_ThreadList.end())
		{
			(*ite)->destory();
			ite++;
		}
		m_tasklist.clearAllTask();
	}
	bool pushObj();
	//push的时候注意，尽量不要使用局部变量，否则在还没有调用任务的run之前，该对象就已经析构，会造成奇怪的问题。
	bool trypushObj(xtaskbase*node,const struct timespec & Timeout=maxTimeWait)
	{
		//if(m_boStartPool && isBusy())/
		//这里有个讲究，不直接插入xtaskbase的子类对象，因为如果存对象，在插入的时候，会被强制转换成基类，丧失子类的部分
		//如果存子类指针，在waitForTask 接口无法将指针作为传出参数，这样取不到该指针的地址。
		//用pair<xtaskbase*，bool> 作为值类型，存入，这样取出的时候，可以将它的引用取出，这样，封装了多态问题，
		//同时bool型变量也可用来以后拓展。
		m_tasklist.pushTask(pair<xtaskbase*, bool>(node,false));
		return true;

		
	}
	void waitforAllTaskDone(void) // 等待所有任务结束
	{
		//xAutoLock L(m_lockForFinishJob);
		//while(m_nJobNotFinish)
			//m_CondForFinishJob.wait(m_lockForFinishJob);
	}
class threadobj:public Threadbase
{
public:
 	void beginthreadobj(xthreadPool* threadpool)
 	{
 		m_parentPool=threadpool;
 		start();
 	}
 	virtual void run()
 	{	
  		pair<xtaskbase*,bool> temptask;
  		while(1)
  		{
   			if(waitforjob(m_parentPool->m_tasklist,maxTimeWait,temptask))
   			{
				xtaskbase*ptask=temptask.first;
				if(ptask)
				{
   					(ptask)->run();
				}
  				//m_parentPool->onFinishTask();
   			}
  		}
 	}
  	bool waitforjob(xTaskqueue<pair<xtaskbase*,bool>>&tasklist,const struct timespec &timeout,OUT pair<xtaskbase*,bool> &tasknode)
  	{
		//pair<xtaskbase*,bool> temptask;
		if(!tasklist.waitForTask(tasknode,timeout))
			return false;
  		return true;
  	}
  	xtaskbase* m_task;
  	xMutex m_mutex;
  	xthreadPool* m_parentPool;
  	friend class xthreadPool;
	//pair<xtaskbase*,bool> m_task; //
};

	void onFinishTask()
	{
		xAutoLock L(m_lockForFinishJob);
		--m_nJobNotFinish;
		m_CondForFinishJob.signal();
	}
	bool addFreeThread()
	{
		xAutoLock L(m_lockForThread);
		++m_nFreeThreadCount;
		return true;
	}
	void destoryThread(){}
	void joinAllThread()
	{
		//xAutoLock L(m_lockForThread);
		std::list<threadobj*>::iterator ite = m_ThreadList.begin();
		while(ite!=m_ThreadList.end())
		{
			(*ite)->join();
			ite++;
		}
		
	}
protected:
	const string	m_strPoolName;
	volatile bool	m_boStartPool;
	size_t			m_threadNum;
	volatile size_t	m_nFreeThreadCount;
	size_t			m_nIdleTimeout;
	size_t			m_nThreadStackSize;
	size_t			m_nJobNotFinish;
	xMutex			m_lockForThread,m_lockForFinishJob;
	xCondition		m_CondForThreadStop,m_CondForBuildThread,m_CondForFinishJob;
	xTaskqueue<pair<xtaskbase*, bool>>		m_tasklist;  //这里放入容器，必须得是指针才会有多态
	std::list<threadobj*> m_ThreadList;		//这里在startPool的时候不能定义局部变量，否则线程还没执行完，对象被释放了会导致一些奇怪的错误。
	xMutex			m_waitforjob;
	static xMutex   m_poollock;
};
xMutex  xthreadPool::m_poollock;
void xthreadPool::initsimplePool(size_t ThreadNumber,size_t IdleTimeout,size_t Tasklistsize,size_t threadStackSize)
{
	stopPool();
	m_nFreeThreadCount=0;
	m_nIdleTimeout=IdleTimeout;
	m_threadNum = ThreadNumber;
	m_nThreadStackSize = threadStackSize;
	m_tasklist.resizeQueue(Tasklistsize);

}
void xthreadPool::startPool(bool defaultpools )
{
	m_tasklist.setActive();// 设置任务队列为开启状态，否则添加和读取都会失败
	//size_t threadnum = m_threadNum?m_threadNum:4
	for(size_t i=0;i<m_threadNum;i++)
	{
		//xAutoLock L(m_lockForThread);
		threadobj *thread=new threadobj;
		//m_ThreadList.push_back(&thread);
		thread->beginthreadobj(this);
		m_ThreadList.push_back(thread);
	}

}
};