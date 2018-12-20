//2018-12-20【Ex 系列hpp函数库】 by xjshi
/*
1)封装基于线程threadbase 和xtaskqueue的主动对象。
2）自带线程。（双线程）在队列里没有添加任务的时候，子线程会一直等待任务，如果主线程退出，则也退出。
3）pushmsg添加任务，定义类struct xMsgblock
4) 基于线程，继承该基类，启动就会产生一个新线程。
5) 目前队列只能存储1000个任务，这个后续需要改进。
*/
#include "xthreadbase.hpp"
#include "xTaskqueue.h"
typedef struct MsgBlock
{
	char* pdata;
	int len;
	void* pID;
}xMsgBlock;
namespace SAEBASE{
	template<typename Block>
	class ExActiveObj:public Threadbase
	{
	public:
		virtual void run()
		{
			do 
			{
				Block pblock;
				m_taskqueue.waitForTask(pblock);
				Onrequest(pblock);
			} while (1);
			
		}
		int startObj()
		{
			return start();
		}
		void stopIbj()
		{

		}
		int pushMsg(Block& block)
		{
			return m_taskqueue.pushTask(block);
		}
		virtual int Onrequest(Block& block)=0;
	private:
		xTaskqueue<Block> m_taskqueue;
	};

}