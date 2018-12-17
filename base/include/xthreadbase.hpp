#pragma once
#ifdef WIN32
#include <process.h>
#include <Windows.h>
#endif
#include "stdio.h"
#include "timebase.hpp"
#include "xAutoLock.hpp"
//pthread_mutex_t zx;
namespace SAEBASE{
xMutex zx;
#pragma once
typedef unsigned int (__stdcall*pfunc)(void*);
//���ڶ����ģʽ
class Threadbase
{
public:
	Threadbase(bool bDetach=true);
	virtual ~Threadbase(){};
	virtual void run()=0;		//ҵ��ӿ�
	int start();			//�����߳�
	int join();				//�ȴ��߳̽���
	void destory();			//�����߳����������Դ

	int get_thread_id(){return thr_id;}
	int set_thread_id(unsigned long thrID){thr_id=thrID;}

protected:
	static unsigned int __stdcall thread_proxy(void* arg);

private:
	size_t thr_id;			//�߳�id
	bool bExit_;			//�߳��Ƿ�Ҫ�˳���־

};
//��������ģʽ
class xThread
{
public:
	xThread(bool bDetach=true):thr_id(0)
	{

	}
	virtual ~xThread(){};
	int start(pfunc func,void *arg);			//�����߳�
	int join();				//�ȴ��߳̽���
	void destory();			//�����߳����������Դ

	int get_thread_id(){return thr_id;}
	int set_thread_id(unsigned long thrID){thr_id=thrID;}
public:
	size_t thr_id;
	bool bExit_;
};


unsigned int __stdcall Threadbase::thread_proxy(void* arg)
{

	xAutoLock antolock(zx);
	timeobj proxylife;
	Threadbase* pbase=reinterpret_cast<Threadbase*> (arg);
	pbase->run();
	//Sleep(3000);
	return 0;
}

Threadbase::Threadbase(bool bDetach)
{
	thr_id=0;
}
int Threadbase::start()
{

#ifdef WIN32
	unsigned int nval=_beginthreadex(0,0,thread_proxy,this,0,&thr_id);
	thr_id=nval;
#endif
	return 0;
}
int Threadbase::join()
{
#ifdef WIN32
	//WAIT_OBJECT ��ʾִ�н���
	if(WaitForSingleObject(reinterpret_cast<HANDLE>(thr_id),INFINITE)==WAIT_OBJECT_0)
	{
		printf("\n join thread %d finish\n",thr_id);
	}
#endif
	return 0;
}
void Threadbase::destory()
{
	CloseHandle(reinterpret_cast<HANDLE>(thr_id));
}
//////////////////////////////////////////////////////////////////////////
//xThread ʵ��


int xThread::start(pfunc func,void *arg)
{
#ifdef WIN32
	unsigned int nval=_beginthreadex(0,0,func,arg,0,&thr_id);
	thr_id=nval;
#endif
	return 0;
}
int xThread::join()
{
#ifdef WIN32
	//WAIT_OBJECT ��ʾִ�н���
	if(WaitForSingleObject(reinterpret_cast<HANDLE>(thr_id),INFINITE)==WAIT_OBJECT_0)
	{
		printf("\n join thread %d finish\n",thr_id);
	}
#endif
	return 0;
}
void xThread::destory()
{
	CloseHandle(reinterpret_cast<HANDLE>(thr_id));
}
}