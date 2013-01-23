#ifndef STD_BY_EXAMPLES_DETAILS_SHARED_PTR_H
#define STD_BY_EXAMPLES_DETAILS_SHARED_PTR_H

#include "config.h"

#include <cassert>

namespace STD_NAMESPACE {

  template<class T> class weak_ptr;

  namespace detail
  {  

    struct shared_ptr_counter_base
    {    
      size_t sp_;
      size_t wp_;

      shared_ptr_counter_base() : sp_(1), wp_(1) {}
      virtual ~shared_ptr_counter_base() {}

      virtual void destroy_px() = 0;
      
      void add_ref_sp() { ++sp_; add_ref_wp(); }      
      void add_ref_wp() { ++wp_; }

      void release_ref_sp() 
      { 
        --sp_; 
        if(!sp_) destroy_px();
        release_ref_wp(); 
      }
      void release_ref_wp() 
      { 
        --wp_; 
        if (!wp_)
        {
          delete this;
        }
      }

      size_t counter_sp() const { return sp_; }
      size_t counter_wp() const { return wp_; }
    };
  
  template<class T, class D = default_delete<T>> 
  struct shared_ptr_counter : public shared_ptr_counter_base
  {
    // TODO missing support for deleter!!!!
          
    T* px;
  
    shared_ptr_counter(T* p) : shared_ptr_counter_base(), px(p) {}

    virtual ~shared_ptr_counter()
    {
      if(px) delete px;
    }

    virtual void destroy_px()
    {
      delete px; px = nullptr;
    }

    void swap(shared_ptr_counter& u)
    {
      STD_NAMESPACE::swap(u.px, px);
      STD_NAMESPACE::swap(u.sp_, sp_);
      STD_NAMESPACE::swap(u.wp_, wp_);
    }

  };

  }

  template<class T> class shared_ptr {
  public:
    typedef T element_type;
    typedef T* pointer;
  private:
    pointer px;
    detail::shared_ptr_counter_base* counter;

    template<class T, class D, class A>
    void _alloc(detail::shared_ptr_counter<T, D> tmp, A a) 
    {
      detail::shared_ptr_counter<T, D>* tmpP = new detail::shared_ptr_counter<T, D>(nullptr); 
      px = tmp.px;      
      tmp.swap(*tmpP);      
      counter = tmpP;
    }

  public:
    //20.7.2.2.1, constructors:
    CONSTEXPR shared_ptr() NOEXCEPT : px(nullptr), counter(nullptr) {}
    template<class Y> explicit shared_ptr(Y* p) : px(p), counter(nullptr) { if (p) { _alloc(detail::shared_ptr_counter<T, default_delete<T>>(p), default_alloc() ); }}
    template<class Y, class D> shared_ptr(Y* p, D d) : px(nullptr), counter(nullptr) { if (p) { _alloc(detail::shared_ptr_counter<T, D>(p), default_alloc() ); }}
    template<class Y, class D, class A> shared_ptr(Y* p, D d, A a) : px(nullptr), counter(nullptr) { if (p) { _alloc(detail::shared_ptr_counter<T, D>(p), a); }}
    
    template <class D> shared_ptr(nullptr_t p, D d) : px(nullptr), counter(nullptr) {}
    template <class D, class A> shared_ptr(nullptr_t p, D d, A a) : px(nullptr), counter(nullptr) {}
    /* TODO ???
    template<class Y> shared_ptr(const shared_ptr<Y>& r, T *p) NOEXCEPT;
    */
    shared_ptr(const shared_ptr& r) NOEXCEPT : px(r.px), counter(r.counter) { counter->add_ref_sp(); }
    template<class Y> shared_ptr(const shared_ptr<Y>& r) NOEXCEPT : px(r.px), counter(r.counter) { counter->add_ref_sp(); }
    shared_ptr(shared_ptr&& r) NOEXCEPT : px(r.px), counter(r.counter) { counter->add_ref_sp(); }
    template<class Y> shared_ptr(shared_ptr<Y>&& r) NOEXCEPT : px(r.px), counter(r.counter) { counter->add_ref_sp(); }
    /* TODO weak_ptr, auto_ptr and unique_ptr support 
    template<class Y> explicit shared_ptr(const weak_ptr<Y>& r);
    template<class Y> shared_ptr(auto_ptr<Y>&& r);
    template <class Y, class D> shared_ptr(unique_ptr<Y, D>&& r);
    */

#ifdef CXX0X_DELEGATING_CTOR
    CONSTEXPR shared_ptr(nullptr_t) : shared_ptr() { }
#else
    CONSTEXPR shared_ptr(nullptr_t) : px(nullptr), counter(nullptr) {}
#endif // CXX0X_DELEGATING_CTOR
        
    //20.7.2.2.2, destructor:
    ~shared_ptr() { if(counter) counter->release_ref_sp(); }
    //20.7.2.2.3, assignment:
    shared_ptr& operator=(const shared_ptr& r) NOEXCEPT;
    template<class Y> shared_ptr& operator=(const shared_ptr<Y>& r) NOEXCEPT;
    shared_ptr& operator=(shared_ptr&& r) NOEXCEPT;
    template<class Y> shared_ptr& operator=(shared_ptr<Y>&& r) NOEXCEPT;
    template<class Y> shared_ptr& operator=(auto_ptr<Y>&& r);
    template <class Y, class D> shared_ptr& operator=(unique_ptr<Y, D>&& r);

    //20.7.2.2.4, modifiers:
    void swap(shared_ptr& r) NOEXCEPT { STD_NAMESPACE::swap(r.px, px); STD_NAMESPACE::swap(r.counter, counter); }
    void reset() NOEXCEPT { if (counter) counter->release_ref_sp(); px = nullptr; counter = nullptr; }
    template<class Y> void reset(Y* p);
    template<class Y, class D> void reset(Y* p, D d);
    template<class Y, class D, class A> void reset(Y* p, D d, A a);
    
    //20.7.2.2.5, observers:
    T& operator*() const NOEXCEPT   { return *px; }
    T* get() const NOEXCEPT         { return px; }    
    T* operator->() const NOEXCEPT  { return px; }    

    long use_count() const NOEXCEPT { counter ? counter->counter_sp() : 0; }
    bool unique() const NOEXCEPT    { use_count() == 1; }


#ifdef CXX0X_EXPLICIT_CONV
    explicit operator bool() const NOEXCEPT
    {
      return px != nullptr;
    }
#else
    typedef pointer (shared_ptr::*unspecified_bool_type)() const;

    operator unspecified_bool_type() const NOEXCEPT
    {
      return px == 0 ? 0 : &shared_ptr::get;
    }
#endif // CXX0X_EXPLICIT_CONV


    /* TODO extern functions
    template<class U> bool owner_before(shared_ptr<U> const& b) const;
    template<class U> bool owner_before(weak_ptr<U> const& b) const;
    */
  };

  /* TODO extern functions
  //20.7.2.2.6, shared_ptr creation
  template<class T, class... Args> shared_ptr<T> make_shared(Args&&... args);
  template<class T, class A, class... Args>
  shared_ptr<T> allocate_shared(const A& a, Args&&... args);
  //20.7.2.2.7, shared_ptr comparisons:
  template<class T, class U>
  bool operator==(const shared_ptr<T>& a, const shared_ptr<U>& b) NOEXCEPT;
  template<class T, class U>
  bool operator!=(const shared_ptr<T>& a, const shared_ptr<U>& b) NOEXCEPT;
  template<class T, class U>
  bool operator<(const shared_ptr<T>& a, const shared_ptr<U>& b) NOEXCEPT;
  template<class T, class U>
  bool operator>(const shared_ptr<T>& a, const shared_ptr<U>& b) NOEXCEPT;
  template<class T, class U>
  bool operator<=(const shared_ptr<T>& a, const shared_ptr<U>& b) NOEXCEPT;
  template<class T, class U>
  bool operator>=(const shared_ptr<T>& a, const shared_ptr<U>& b) NOEXCEPT;
  template <class T>
  bool operator==(const shared_ptr<T>& a, nullptr_t) NOEXCEPT;
  template <class T>
  bool operator==(nullptr_t, const shared_ptr<T>& b) NOEXCEPT;
  template <class T>
  bool operator!=(const shared_ptr<T>& a, nullptr_t) NOEXCEPT;
  template <class T>
  bool operator!=(nullptr_t, const shared_ptr<T>& b) NOEXCEPT;
  template <class T>
  bool operator<(const shared_ptr<T>& a, nullptr_t) NOEXCEPT;
  template <class T>
  bool operator<(nullptr_t, const shared_ptr<T>& b) NOEXCEPT;
  template <class T>
  bool operator<=(const shared_ptr<T>& a, nullptr_t) NOEXCEPT;
  template <class T>
  bool operator<=(nullptr_t, const shared_ptr<T>& b) NOEXCEPT;
  template <class T>
  bool operator>(const shared_ptr<T>& a, nullptr_t) NOEXCEPT;
  template <class T>
  bool operator>(nullptr_t, const shared_ptr<T>& b) NOEXCEPT;
  template <class T>
  bool operator>=(const shared_ptr<T>& a, nullptr_t) NOEXCEPT;
  template <class T>
  bool operator>=(nullptr_t, const shared_ptr<T>& b) NOEXCEPT;
  */

  //20.7.2.2.8, shared_ptr specialized algorithms:
  template<class T> void swap(shared_ptr<T>& a, shared_ptr<T>& b) NOEXCEPT;

  /* TODO casts
  //20.7.2.2.9, shared_ptr casts:
  template<class T, class U>
  shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r) NOEXCEPT;
  template<class T, class U>
  shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r) NOEXCEPT;
  template<class T, class U>
  shared_ptr<T> const_pointer_cast(const shared_ptr<U>& r) NOEXCEPT;
  //20.7.2.2.10, shared_ptr get_deleter:
  template<class D, class T> D* get_deleter(const shared_ptr<T>& p) NOEXCEPT;
  */

  /* TODO ostream support
  //20.7.2.2.11, shared_ptr I/O:
  template<class E, class T, class Y>
  basic_ostream<E, T>& operator<< (basic_ostream<E, T>& os, const shared_ptr<Y>& p);
  */

}

#endif // #ifndef STD_BY_EXAMPLES_DETAILS_SHARED_PTR_H