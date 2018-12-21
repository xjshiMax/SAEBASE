//2018-12-20��Ex ϵ��hpp�����⡿ by xjshi
/*
1)��װ�����߳�threadbase ��xtaskqueue����������
2���Դ��̡߳���˫�̣߳��ڶ�����û����������ʱ�����̻߳�һֱ�ȴ�����������߳��˳�����Ҳ�˳���
3��pushmsg������񣬶�����struct xMsgblock
4) �����̣߳��̳иû��࣬�����ͻ����һ�����̡߳�
5) Ŀǰ����ֻ�ܴ洢1000���������������Ҫ�Ľ���
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