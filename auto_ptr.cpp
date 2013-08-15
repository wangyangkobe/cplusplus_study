#include <iostream>
#include <cassert>
using std::cout;
using std::endl;
//http://www.josuttis.com/libbook/util/autoptr.hpp.html
//1. 智能指针不能共享指向对象的所有权
//2. 智能指针不能指向数组。因为其实现中调用的是delete而非delete[]
//3. 智能指针不能作为容器类的元素。

template <typename T>
class auto_ptr_ref
{
public:
	auto_ptr_ref(T* p):m_p(p){}
	T* m_p;
};

template <typename T>
class auto_ptr
{
public:
	explicit auto_ptr(T* p = NULL):m_p(p){}
	auto_ptr(auto_ptr& other);
 	auto_ptr& operator=(auto_ptr& other);
	T* get() const { return m_p; }
	void reset(T* p = NULL);
	T& operator * () const { return *m_p; }
	T* operator -> () const { return m_p; }

	template <typename Y>
	operator auto_ptr_ref<Y> ()
	{
		return auto_ptr_ref<Y>(this->release());		
	}

	auto_ptr(auto_ptr_ref<T> other):m_p(other.m_p){}       

	~auto_ptr()
	{
		delete m_p;
		m_p = NULL;
	}	
private:
	T* m_p;
	T* release();
};

void test(auto_ptr<int> p)
{
	if(p.get())
		cout<<*p<<endl;
}
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

	test(auto_ptr<int>()); //test的参数为临时变量，当掉用test的时候会调用
	//auto_ptr的拷贝构造函数，但是拷贝构造函数是auto_ptr的引用是左值，而test
	//的参数为const（也就算右值）,so通过auto_ptr_ref将右值转换为左值。
}



template <typename T>
T* auto_ptr<T>::release()
{
	T* temp = m_p;
	m_p = NULL;
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
