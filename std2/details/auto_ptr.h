#ifndef STD_BY_EXAMPLES_DETAILS_AUTO_PTR_H
#define STD_BY_EXAMPLES_DETAILS_AUTO_PTR_H

#include "config.h"

namespace STD_NAMESPACE {

  template <class Y> struct auto_ptr_ref // exposition only
  {
	  Y * ptr;
	  explicit auto_ptr_ref(Y *p): ptr(p){}
  };
  template <class X> class auto_ptr {
  public:
    typedef X element_type;
    //D.10.1.1construct/copy/destroy:
  private:
    element_type* px;
    bool owner;

    void swap(auto_ptr& u)
    {
      STD_NAMESPACE::swap(u.px, px);
      STD_NAMESPACE::swap(u.owner, owner);      
    }

  public:
    explicit auto_ptr(X* p =0) throw() : px(p), owner(p ? true : false) {}
    auto_ptr(auto_ptr& u) throw() : px(u.px), owner(false) { u.swap(*this); }        
    ~auto_ptr() throw() { if (owner) delete px; }


    /* TODO conversion
    template<class Y> auto_ptr(auto_ptr<Y>&) throw();
    template<class Y> auto_ptr& operator=(auto_ptr<Y>&) throw();
    */

    auto_ptr& operator=(auto_ptr& a) throw() { reset(a.release()); }

    /* TODO ref..
    auto_ptr& operator=(auto_ptr_ref<X> r) throw();
    */    

    //D.10.1.2members:
    X& operator*() const throw()  { return *px; }
    X* operator->() const throw() { return px; }
    X* get() const throw()        { return px; }

    X* release() throw() { X* p = px; px = 0; owner = false; return p; }
    void reset(X* p =0) throw() { auto_ptr(p).swap(*this); }

    //D.10.1.3conversions:
    /* TODO conversion
    auto_ptr(auto_ptr_ref<X>) throw();
    template<class Y> operator auto_ptr_ref<Y>() throw();
    template<class Y> operator auto_ptr<Y>() throw();
    */
	auto_ptr(auto_ptr_ref<X> ref) throw():auto_ptr(ref->ptr){}

	template<class Y> operator auto_ptr_ref<Y>() throw()
	{
		return auto_ptr_ref<Y>(this->release);
	}
	template<class Y> operator auto_ptr<Y>() throw()
	{
		return auto_ptr<Y>(this->release());
	}
  };
  template <> class auto_ptr<void>
  {
  public:
    typedef void element_type;
  };

}

#endif // #ifndef STD_BY_EXAMPLES_DETAILS_AUTO_PTR_H