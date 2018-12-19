//2018-12-18���Ӷ�ʱ������
/*
1����ʱ��������һ���̣߳�
2���̳ж�ʱ�����࣬Ȼ�����һֱִ�С�
3) ȷ�������̻߳�ȴ������������˳�����Ȼ��ʱ���߳�Ҳû���㹻��ʱ��ִ�С�
*/
#include "xAutoLock.hpp"
#include "xthreadbase.hpp"
namespace SAEBASE{
//��ʱ�����뼶��
class OnTimerBase:public Threadbase
{
public:	
	OnTimerBase(int timeout):m_TimeOut(timeout){}
	int startTimer()
	{
		return start();
	}
	void stopTimer()
	{
		//join();
	}
	virtual void timeout()=0;
private:
	virtual void run()
	{
		while(1){
		xAutoLock L(m_mutex);
		struct timespec tc;
		tc.tv_sec=m_TimeOut;
		tc.tv_nsec=0;
		//m_SleepCondition.timewait(m_mutex,tc);
		m_SleepCondition.timewait(m_mutex,tc);
		this->timeout();
		}

	}
private:
	int m_TimeOut;
	xCondition m_SleepCondition;
	xMutex m_mutex;
};
}