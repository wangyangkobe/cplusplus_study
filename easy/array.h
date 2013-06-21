#ifndef EASY_ARRAY_H
#define EASY_ARRAY_H

namespace easy
{
  template<class T, size_t SIZE>
  class array
  {
    T buf[SIZE];

  public:
    typedef T* iterator;  
    typedef const T* const_iterator;  

    typedef T* reverse_iterator;  
    typedef const T* const_reverse_iterator;  

    array() { static_assert(SIZE > 0, "expect at least one element!"); }

    // iterators
    iterator begin() { return &buf[0]; }
    const_iterator begin() const { return &buf[0]; };
    iterator end() { return &buf[SIZE]; }
    const_iterator end() const { return &buf[SIZE]; }

    // reverse iterator support
    reverse_iterator rbegin() { return &buf[SIZE-1]; }
    const_reverse_iterator rbegin() const { return &buf[SIZE-1]; }
    reverse_iterator rend() { return &buf[-1]; }
    const_reverse_iterator rend() const { return &buf[-1]; }

    // capacity
    size_t size() const { return SIZE; }
    bool empty() const {return SIZE == 0 };
    size_t max_size()  const { return SIZE; }

    // element access
    T& operator[](size_t index) { return buf[index]; }
    const T& operator[](size_t index) const { return buf[index]; }
    T& at(size_t index) { return buf[index]; } // missing exception!!!
    const T& at(size_t) const { return buf[index]; } // missing exception!!!
    T& front() { return buf[0]; }
    const T& front() const { return buf[0]; }
    T& back() { return buf[SIZE-1]; }
    const T& back() const { return buf[SIZE-1]; }
    
    const T* data() const { return buf; }
  };

}

#endif
