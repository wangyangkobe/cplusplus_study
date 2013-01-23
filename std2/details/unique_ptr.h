#ifndef STD_BY_EXAMPLES_DETAILS_UNIQUE_PTR_H
#define STD_BY_EXAMPLES_DETAILS_UNIQUE_PTR_H

#include "config.h"

namespace STD_NAMESPACE {

  template <class T, class D /* = default_delete<T>*/> class unique_ptr {
  public:
    typedef T* pointer;
    typedef T element_type;
    typedef D deleter_type;
  private:
    pointer px;

  public:
    // 20.7.1.2.1, constructors
    CONSTEXPR unique_ptr() NOEXCEPT : px(nullptr) {}
    explicit unique_ptr(pointer p) NOEXCEPT : px(p) {}
    /* TODO 20.7.1.2.1 (8)
    unique_ptr(pointer p, see below d1) NOEXCEPT;
    unique_ptr(pointer p, see below d2) NOEXCEPT;
    */
    unique_ptr(unique_ptr&& u) NOEXCEPT : px(nullptr) { u.swap(*this); }
    CONSTEXPR unique_ptr(nullptr_t) NOEXCEPT 
#ifdef CXX0X_DELEGATING_CTOR
      : unique_ptr() { }
#else
      : px(nullptr) {}
#endif // CXX0X_DELEGATING_CTOR

    template <class U, class E>
    unique_ptr(unique_ptr<U, E>&& u) NOEXCEPT;

    /* TODO auto_ptr support
    template <class U>
    unique_ptr(auto_ptr<U>&& u) NOEXCEPT;
    */
    // 20.7.1.2.2, destructor
    ~unique_ptr()
    {
      get_deleter()(px);
    }

    // 20.7.1.2.3, assignment
    unique_ptr& operator=(unique_ptr&& u) NOEXCEPT;
    template <class U, class E> unique_ptr& operator=(unique_ptr<U, E>&& u) NOEXCEPT;
    unique_ptr& operator=(nullptr_t) NOEXCEPT;
    
    // 20.7.1.2.4, observers
    typename add_lvalue_reference<T>::type operator*() const { return *px; }
    
    pointer operator->() const NOEXCEPT { return px; }
    pointer get() const NOEXCEPT        { return px; } 
    deleter_type& get_deleter() NOEXCEPT { static deleter_type d; return d; }
    const deleter_type& get_deleter() const NOEXCEPT { static deleter_type d; return d; }

#ifdef CXX0X_EXPLICIT_CONV
    explicit operator bool() const NOEXCEPT
    {
      return px != nullptr;
    }
#else
    typedef pointer (unique_ptr::*unspecified_bool_type)() const;

    operator unspecified_bool_type() const // NOEXCEPT
    {
      return px == 0 ? 0 : &unique_ptr::get;
    }
#endif // CXX0X_EXPLICIT_CONV
    
    // 20.7.1.2.5 modifiers
    pointer release() NOEXCEPT { pointer p(px); px = nullptr; return p; };
    void reset(pointer p = pointer()) NOEXCEPT { unique_ptr(p).swap(*this); }
    void swap(unique_ptr& u) NOEXCEPT
    {
      STD_NAMESPACE::swap(u.px, px);      
    }

    // disable copy from lvalue
#ifdef CXX0X_DELETE_MEM_FUNC
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
#else
  private:
    unique_ptr(const unique_ptr&);
    unique_ptr& operator=(const unique_ptr&);

#endif // CXX0X_DELETE_MEM_FUNC
  };

  //////////////////////////////////////////////////////////////////////////
  // unique_ptr for []

  template <class T> 
  struct _ArrayPtrBase
  {
    typedef T _pointer;
  };

  template <class T, class D> 
  class unique_ptr<T[], D> : private _ArrayPtrBase<T> {
  public:
    //typedef _ArrayPtrBase<T> _baseType;
    typedef typename _ArrayPtrBase::_pointer pointer;
    typedef T element_type;
    typedef D deleter_type;

  private:
    pointer px;

  public:
    //20.7.1.3.1, constructors
    CONSTEXPR unique_ptr() NOEXCEPT : px(nullptr) {}
    explicit unique_ptr(pointer p) NOEXCEPT : px(p) {}

    /* TODO
    unique_ptr(pointer p, see below d) NOEXCEPT;
    unique_ptr(pointer p, see below d) NOEXCEPT;
    */

    unique_ptr(unique_ptr&& u) NOEXCEPT;
    CONSTEXPR unique_ptr(nullptr_t) : unique_ptr() { }
    // destructor
    ~unique_ptr() { get_deleter()(px); }

    // assignment
    unique_ptr& operator=(unique_ptr&& u) NOEXCEPT { u.swap(*this); return (*this); }
    unique_ptr& operator=(nullptr_t) NOEXCEPT { reset(); return (*this); }
    //20.7.1.3.2, observers

    T& operator[](size_t i) const { return px[i]; }
    pointer get() const NOEXCEPT { return px; }
    deleter_type& get_deleter() NOEXCEPT  { static deleter_type d; return d; }
    const deleter_type& get_deleter() const NOEXCEPT  { static deleter_type d; return d; }
    
#ifdef CXX0X_EXPLICIT_CONV
    explicit operator bool() const NOEXCEPT
    {
      return px != nullptr;
    }
#else
    typedef pointer (unique_ptr::*unspecified_bool_type)() const;

    operator unspecified_bool_type() const // NOEXCEPT
    {
      return px == 0 ? 0 : &unique_ptr::get;
    }
#endif // CXX0X_EXPLICIT_CONV

    //20.7.1.3.3 modifiers
    pointer release() NOEXCEPT  { pointer p(px); px = nullptr; return p; };
    void reset(pointer p = pointer()) NOEXCEPT { unique_ptr(p).swap(*this); }
    void reset(nullptr_t) NOEXCEPT { reset(); }
    
    void swap(unique_ptr& u) NOEXCEPT
    {
      STD_NAMESPACE::swap(u.px, px);
    }

#ifdef CXX0X_DELETE_MEM_FUNC
    template <class U> void reset(U) = delete;

    // disable copy from lvalue
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
#else
  private:
    template <class U> void reset(U);

    // disable copy from lvalue
    unique_ptr(const unique_ptr&);
    unique_ptr& operator=(const unique_ptr&);
#endif
  };

}

#endif // #ifndef STD_BY_EXAMPLES_DETAILS_UNIQUE_PTR_H