#ifndef EASY_SCOPED_PTR_H
#define EASY_SCOPED_PTR_H

#include "../std2/details/config.h"

namespace easy
{
  template<class T>
  class scoped_ptr // noncopyable
  {
    T * px;  // private data

  public:
    explicit scoped_ptr(T* p=0):px(p){} // Constructor
    ~scoped_ptr() { delete px; } 	      // Destructor     - free the memory

#ifdef CXX0X_DELETE_MEM_FUNC
    scoped_ptr(scoped_ptr const &) = delete;             // no copies
    scoped_ptr & operator=(scoped_ptr const &) = delete; // no copies
#else
  private:
    scoped_ptr(scoped_ptr const &);             // no copies
    scoped_ptr & operator=(scoped_ptr const &); // no copies
  public:

#endif

    T & operator*() const  { return *px; }
    T * operator->() const { return px;  }
    T * get() const        { return px;  } // Zugriff auf rohen (raw) Speicher

#ifdef CXX0X_EXPLICIT_CONV
    explicit operator bool() const  { return px != 0; }
#else
    typedef T * (scoped_ptr::*unspecified_bool_type)() const;

    operator unspecified_bool_type() const // never throws
    {
      return px == 0? 0: &scoped_ptr::get;
    }
#endif

    void reset(T * p = 0)              // Reset the pointer to other pointer or NULL (Free the memory)
    {
      scoped_ptr(p).swap(*this);
    }

    void swap(scoped_ptr & b) // never throws
    {
      T * tmp = b.px;
      b.px = px;
      px = tmp;
    }
  };

}

#endif
