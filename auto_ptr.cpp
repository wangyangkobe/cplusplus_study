#include <iostream>
#include <cassert>
using namespace std;

//http://www.josuttis.com/libbook/util/autoptr.hpp.html
//1. 智能指针不能共享指向对象的所有权
//2. 智能指针不能指向数组。因为其实现中调用的是delete而非delete[]
//3. 智能指针不能作为容器类的元素。

template <typename T>
class auto_ptr
{
public:
	explicit auto_ptr(T* p = NULL):m_p(p){}
	auto_ptr(auto_ptr& other);
 	auto_ptr& operator=(auto_ptr& other);
	T* get() const { return m_p; }
	void reset(T* p = NULL);
	T& operator * (){ return *m_p; }
	T* operator -> (){ return m_p; }
	~auto_ptr()
	{
		if(m_p)
			delete m_p;
		m_p = NULL;
	}	
private:
	T* m_p;
	T* release();
};


int main()
{
	auto_ptr<int> p(new int(3));
	cout<<*p<<endl;
	
	int *b = new int(6);
	p.reset(b);
	cout<<*p.get()<<endl;

	auto_ptr<int> p1(new int(10));
	p = p1;
	cout<<*p<<endl;
	assert(NULL == p1.get());

	auto_ptr<int> p2(new int(100));
	auto_ptr<int>p3(p2);
	cout<<*p3<<endl;
	assert(NULL == p2.get());
}



template <typename T>
T* auto_ptr<T>::release()
{
	T* temp = this->m_p;
	this->m_p = NULL;
	return temp;
}

template <typename T>
auto_ptr<T>::auto_ptr(auto_ptr& other)
{
	m_p = other.release();
}

template <typename T>
auto_ptr<T>& auto_ptr<T>::operator=(auto_ptr& other)
{	
	if(this == &other)
		return *this;
	delete m_p;
	m_p = other.release();
	return *this;
}

template <typename T>
void auto_ptr<T>::reset(T* p)
{
	if(m_p != p)
	{
		delete m_p;
		m_p = p;	
	}
}	
