#include <iostream>
#include <vector>
using namespace std;

class BaseItem{
public:
  virtual BaseItem* clone()
  {
    return new BaseItem(*this);
  }
  virtual void sayHello()
  {
    cout<<"Hello, I am class BaseItem!"<<endl;
  }
  virtual ~BaseItem(){}
};

class ChildItem:public BaseItem{
public:
  ChildItem* clone()
  {
    return new ChildItem(*this);
  }
  void sayHello(){
    cout<<"Hello, I am class ChildItem!"<<endl;
  }
};


template <typename T>
class Handle
{
public:
  Handle():baseItem(NULL), refCount(new size_t(0)) {}
  Handle(T& object):baseItem(object.clone()), refCount(new size_t(1)) {}

  Handle(const Handle<T>& other):baseItem(other.baseItem), refCount(other.refCount)
  {
    ++*other.refCount;
  }

  Handle& operator= (const Handle<T>& other)
  {
    ++*other.refCount;
    dec_count();
    baseItem = other.baseItem;
    refCount = other.refCount;
    return *this;
  }

  const T* operator->() const {return baseItem;};
  const T& operator*() const {return *baseItem;};

  T* operator->() {return baseItem;};
  T& operator*() {return *baseItem;};

  virtual ~Handle(void)
  {
    dec_count();
  }
private:
  T *baseItem;
  std::size_t* refCount;
  void dec_count()
  {
    if (-- *refCount == 0 && baseItem != NULL)
      {
	delete baseItem;
	delete refCount;
      }
  }
};

int main()
{
  BaseItem item1;
  ChildItem item2;

  vector<Handle<BaseItem> > vec;
  vec.push_back(Handle<BaseItem>(item1));
  vec.push_back(Handle<BaseItem>(item2));

  for (vector<Handle<BaseItem> >::iterator iter = vec.begin();
       iter != vec.end(); iter++)
    {
      (*iter)->sayHello();
    }
  return 0;
}
