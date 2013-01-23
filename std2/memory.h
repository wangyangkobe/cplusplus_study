#ifndef STD_BY_EXAMPLES_MEMORY
#define STD_BY_EXAMPLES_MEMORY

#include "details/config.h"

namespace STD_NAMESPACE {

  // header <exception>

  // TODO fill with life...
  class exception
  {   
  public:
    exception();
    exception(const char* const &);

    virtual ~exception() NOEXCEPT {}
    virtual const char* what() const { return "TODO"; } 
  };

  //
  template<class T> void swap(T& a, T& b) NOEXCEPT
  {
    // TODO C++98 Code
    T tmp = a;
    a = b;
    b = tmp;
  }

  // header <type_traits>
  template <class T>
  struct add_lvalue_reference
  {
    typedef T& type; 
  };

  template <class T>
  struct add_lvalue_reference<T&&>
  {
    typedef T type; 
  };

  // TODO create a standard conform allocator 
  struct default_alloc {};

#ifndef CXX0X_NULLPTR_T
  typedef decltype(nullptr) nullptr_t;
#endif // CXX0X_NULLPTR_T



  template<class T> struct default_delete;
  template<class T> struct default_delete<T[]>;
  template<class T, class D = default_delete<T>> class unique_ptr;
  template<class T, class D> class unique_ptr<T[], D>;
  template<class T, class D> void swap(unique_ptr<T, D>& x, unique_ptr<T, D>& y) NOEXCEPT;
  template<class T1, class D1, class T2, class D2>
  bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
    template<class T1, class D1, class T2, class D2>
  bool operator!=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
  template<class T1, class D1, class T2, class D2>
  bool operator<(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
  template<class T1, class D1, class T2, class D2>
  bool operator<=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
  template<class T1, class D1, class T2, class D2>
  bool operator>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
  template<class T1, class D1, class T2, class D2>
  bool operator>=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
  template <class T, class D>
  bool operator==(const unique_ptr<T, D>& x, nullptr_t) NOEXCEPT;
  template <class T, class D>
  bool operator==(nullptr_t, const unique_ptr<T, D>& y) NOEXCEPT;
  template <class T, class D>
  bool operator!=(const unique_ptr<T, D>& x, nullptr_t) NOEXCEPT;
  template <class T, class D>
  bool operator!=(nullptr_t, const unique_ptr<T, D>& y) NOEXCEPT;
  template <class T, class D>
  bool operator<(const unique_ptr<T, D>& x, nullptr_t);
  template <class T, class D>
  bool operator<(nullptr_t, const unique_ptr<T, D>& y);
  template <class T, class D>
  bool operator<=(const unique_ptr<T, D>& x, nullptr_t);
  template <class T, class D>
  bool operator<=(nullptr_t, const unique_ptr<T, D>& y);
  template <class T, class D>
  bool operator>(const unique_ptr<T, D>& x, nullptr_t);
  template <class T, class D>
  bool operator>(nullptr_t, const unique_ptr<T, D>& y);
  template <class T, class D>
  bool operator>=(const unique_ptr<T, D>& x, nullptr_t);
  template <class T, class D>
  bool operator>=(nullptr_t, const unique_ptr<T, D>& y);

  
  //////////////////////////////////////////////////////////////////////////
  // deleter
    
  template <class T> struct default_delete {
#ifdef CXX0X_DEFAULT_CTOR
    CONSTEXPR default_delete() NOEXCEPT = default;
#else
    CONSTEXPR default_delete() NOEXCEPT {}
#endif // CXX0X_DEFAULT_CTOR

    template <class U> default_delete(const default_delete<U>&) NOEXCEPT {}
    void operator()(T*) const;
  };
    
  template <class T> struct default_delete<T[]> {
#ifdef CXX0X_DEFAULT_CTOR
    CONSTEXPR default_delete() NOEXCEPT = default;
#else
    CONSTEXPR default_delete() NOEXCEPT {}
#endif
    void operator()(T*) const;

#ifdef CXX0X_DELETE_MEM_FUNC
    template <class U> void operator()(U*) const = delete;
#else
  private:
    template <class U> void operator()(U*);
  public:

#endif // CXX0X_DELETE_MEM_FUNC    
    };

  template <class T> 
  inline void default_delete<T>::operator()(T* p) const
  {
    delete p;
  }  

  template <class T> 
  inline void default_delete<T[]>::operator()(T* p) const
  {
    delete[] p;
  }  

  // 20.7.2.1 
  class bad_weak_ptr: public STD_NAMESPACE::exception {
  public:
    bad_weak_ptr() NOEXCEPT;
    virtual const char* what() const NOEXCEPT { return "bad_weak_ptr"; } 
  };
 
}

#include "details/unique_ptr.h"
#include "details/auto_ptr.h"
#include "details/shared_ptr.h"


#endif // #define STD_BY_EXAMPLES_MEMORY