#ifndef EASY_SHARED_PTR_H
#define EASY_SHARED_PTR_H

namespace easy{

  namespace detail
  {
    template<class T>
    struct counter
    {
      size_t sp_;
      size_t wp_;
      
      T * px;  // private data

      counter(T* p) : sp_(1), wp_(1), px(p) {}

      void addwp() { ++wp_; }
      void addsp() { ++sp_; addwp(); }      

      void releasewp() 
      { 
        --wp_; 
        if (0 == wp_)
          delete this;
      }

      void releasesp() 
      { 
        --sp_;
        if(0 == sp_) delete px;
        releasewp();
      }
    };
  }

  template<class T>
  class shared_ptr 
  {
    detail::counter<T> * cnt; // private data
    T * px;                   // private data

    void make_counter(T * p)
    {
      cnt = new detail::counter<T>(p);
    }
    void addref_counter() { if (cnt) cnt->addsp(); }
    void release_counter() { if (cnt) cnt->releasesp(); }

  public:
    explicit shared_ptr(T* p=0):cnt(0), px(p){ make_counter(p); }
    ~shared_ptr() {  release_counter(); } // Destructor

    shared_ptr(const shared_ptr& other) : cnt(other.cnt), px(other.px) { addref_counter(); }
    shared_ptr& operator=(const shared_ptr& other)
    {
      if (other.cnt != cnt)
      {
        release_counter();
        cnt = other.cnt;
        addref_counter();
      }
      return *this;
    }

    T & operator*() const  { return *px; }
    T * operator->() const { return px;  }
    T * get() const        { return px;  } 

#ifdef CXX0X_EXPLICIT_CONV
    explicit operator bool() const  { return px != 0; }
#else
    typedef T * (shared_ptr::*unspecified_bool_type)() const;

    operator unspecified_bool_type() const // never throws
    {
      return px == 0? 0: &shared_ptr::get;
    }
#endif

    void reset(T * p = 0)              // Reset the pointer to other pointer or NULL (Free the memory)
    {
      shared_ptr(p).swap(*this);
    }

    void swap(shared_ptr& b) // never throws
    {
      T * tmp = b.px; detail::counter<T> * tmp2 = b.cnt;      
      b.px = px; b.cnt = cnt;
      px = tmp; cnt = tmp2;
    }
  };

}

#endif

