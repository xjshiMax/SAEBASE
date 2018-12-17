//2018-12-4
//封装智能指针
//1）
//2）
//3）
#include "../base/include/xAtomicInt32.hpp"
#define AtomicCountType unsigned int 
template<class Type>
class xsmartPtr
{
public:
	xsmartPtr(Type* pObj):m_Count(1),m_Ptr(pObj){}
	~xsmartPtr(){
		delete m_Ptr;
		m_Ptr=NULL;
	}
	template<typename pType>
	static inline void decRef(pType*ptr)
	{
		if(ptr!=NULL&&ptr->m_Count.dec()==0)
			delete ptr;
	}
	template<typename pType>
	static inline pType* addRef(pType *ptr)
	{
		if(ptr!=NULL&&ptr->m_Count.inc())
			return ptr;
	}
	//获取原始指针
	inline Type *getImplPtr(void){return m_Ptr;}
	//获取原始引用
	inline Type &getInplRef(void) {return *m_Ptr;}
	inline const Type * getConstImplPtr(void) const {return m_Ptr;}
	inline const Type * getConstImplRef(void) const {return *m_Ptr;}
	AtomicCountType getCount() {return m_Count.get();}
private:
	xAtomicInt32 m_Count;	//引用计数
	Type       m_Ptr;		//真实指针，当引用技术不为0时指向分配的地址，否则被释放，指向NULL.
};

//对智能指针再次进行封装，并重载操作符和运算符
template<class ptrType>
class xsmartRef
{
public:
	xsmartRef(void):m_pImp(NULL){}
	xsmartRef(const xsmartRef &ref):m_pImp(NULL) {reset(ref);}
	//explicit 修饰的函数，实参转换为形参不能隐式转换。一般修饰单个参数的构造。多个构造自动去掉了隐式转换
	explicit xsmartRef(Type* ptr):m_pImp(ptr == NULL? NULL:new xsmartPtr(ptr)){}
	template <class Type2>
	explicit xsmartRef(Type* ptr):m_pImp(ptr==NULL ? NULL:new xsmartPtr(ptr)){}
	~xsmartRef(){release();}

	//重置指针内容
	//减少当前智能指针的引用技术，如果引用计数为0，则删除掉。然后将右边的
	//对象赋给他，引用技术加一。
	inline const xsmartRef& reset(const xsmartRef&rh){
		if(this!=&rh)
		{
			xsmartPtr::decRef(m_pImp);
			m_pImp = xsmartPtr::addRef(rh.m_pImp);
		}
		return *this;
	}

	//重置当前的对象指针
	inline const xsmartRef &reset(ptrType * ptr)
	{
		release();
		if(ptr!=NULL)	m_pImp = new xsmartPtr(ptr);
		return *this;
	}

	//这里可以使用子类对基类的智能指针进行重置
	inline const xsmartRef & reset(Type2 *ptr)
	{
		release();
		if(ptr!=NULL)	m_pImp = new xsmartPtr(ptr);
		return *this;
	}

	inline const xsmartRef& operator = (Type*ptr) {return reset(ptr);}
	inline const xsmartRef &operator = (xsmartRef& rh) {return reset(rh);}
	template<typename Type2>

	// if(Inst)...  判断对象是否为空。 如果真正的指针为空，则返回空，否则返回this.
	inline const xsmartRef& operator = (Type2 * ptr) {return reset(ptr);}
	//if(!Inst)...  判断是否存在，或则是否为空
	bool operator !() const
	{
		return (m_pImp == NULL|| m_pImp->getConstImplPtr()==NULL)
	}
	operator void *() const
	{
		if(m_pImp!=NULL&&m_pImp->getConstImplPtr()!=NULL)
			return (void*)this;
		return NULL;
	}
	inline const ptrType* operator ->() const {return getConstPtr();}
	inline const ptrType & operator *() const throw(std::rntime_error) {return getConstRef();}
	inline ptrType * operator ->() {return getPtr();}
	inline ptrType * operator ->() {return getRef();}

	inline void release()
	{
		xsmartPtr::decRef(m_pImp);
		m_pImp=NULL;
	}
	inline ptrType * getPtr(void){return (m_pImp==NULL)?NULL:m_pImp->getImplPtr();}
	inline const ptrType*  getConstPtr(void) const {return (m_pImp==NULL)?NULL:m_pImp->getConstImplPtr();}
	inline ptrType & getRef(void) 
	{
		if(m_pImp==NULL)
			throw std::runtime_error("Not Init Counting Pointer");
		return (m_pImp == NULL)?NULL:m_pImp->getImplRef();
	}
	inline const Type& getConstRef(void) const throw(std::runtime_error)
	{
		if(m_pImp==NULL)
			throw std::runtime_error("Not Init Counting Pointer");
		return m_pImp->getConstImplRef();
	}
	inline void swap(xsmartRef& rhs)
	{
		std::swap(m_pImp,rhs->m_pImp);
	}
	inline AtomicCountType getRefCount const {return m_pImp==NULL?0:m_pImp->getCount();}
	inline void incRef(void) {xsmartPtr::addRef(m_pImp);}
	inline void decRef(void) {xsmartPtr::decRef(m_pImp);}
protected:
	xsmartPtr * m_pImp;
};