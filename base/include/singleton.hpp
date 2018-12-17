#include "xReactor.hpp"
template <class T>
class xsingleton
{
public:
	static inline T* instance();
	void release();
protected:
	xsingleton(void){}
	~xsingleton(void){}
	static T* _instance;
};

template <class T>
inline T* xsingleton<T>::instance()
{
	if(!_instance)
		_instance = new T;
	return _instance;
}

template <class T>
void xsingleton<T>::release()
{
	if (!_instance)
		return;
	delete _instance;
	_instance = 0;
}

#define DECLARE_SINGLETON_MEMBER(_Ty)   \
	template <> _Ty* xsingleton<_Ty>::_instance = NULL;

class Global : public xsingleton<Global>
{
public:
	Global(void);
	~Global(void);

	xReactor* g_reactor_ptr;
};

#define sGlobal Global::instance()

DECLARE_SINGLETON_MEMBER(Global);

Global::Global(void)
{
	g_reactor_ptr = new xReactor();
}

Global::~Global(void)
{
	delete g_reactor_ptr;
	g_reactor_ptr = NULL;
}
