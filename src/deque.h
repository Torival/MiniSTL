#ifndef __MINISTL_VECTOR_H
#define __MINISTL_VECTOR_H

#include "alloc.h"
#include "uninitialized.h"

// TO TEST
#include <iostream>
using std::cout;
using std::endl;

MINISTL_NAMESPACE_BEGIN

inline size_t deque_table_size(size){
    return size < 512 ? size_t(512 / size) : size_t(1);
}
template <class T>
struct deque_iterator{
    typedef deque_iterator<T>   iterator;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;
    typedef T                   value_type;
    typedef T*                  table_pointer;
    typedef T**                 map_pointer;

    table_pointer first;
    table_pointer last;
    table_pointer cur;
    map_pointer   node;

    deque_iterator():first(0), last(0), cur(0), node(0){ }

    deque_iterator(T* _cur, T** _node):cur(_cur), first(_cur),
                    last(deque_table_size(sizeof(T))), node(_node) { }
    deque_iterator(const deque_iterator& it):first(it.first), last(it.last),
                    cur(it.cur), node(it.node) { }

    void set_node(map_pointer new_node);

    T  operator*() const 
    {   return *cur;            }
    T* operator->() const
    {   return cur;             }
    T  operator[](difference_type n) const
    {   return *(*this + n);    } 

    // ++i; -> 调用 i.operator++();
    // i++; -> 调用 i.operator++(0);
    // --i; -> 调用 i.operator--();
    // i--; -> 调用 i.operator--(0);
    deque_iterator& operator++();       // ++ 前缀
    deque_iterator& operator--();
    deque_iterator  operator++(int);    //  ++ 后缀
    deque_iterator  operator--(int);

    deque_iterator& operator+=(difference_type n);
    deque_iterator& operator-=(difference_type n);

    deque_iterator  operator+(difference_type n);
    deque_iterator  operator-(difference_type n);

    difference_type operator-(const deque_iterator it);

    bool operator==(const deque_iterator& it)
    {   return cur == it.cur;   }
    bool operator!=(const deque_iterator& it)
    {   return !(*cur == it);   }
    bool operator<(const deque_iterator& it)
    {   return node == it.node ? cur < it.cur : node < it.node; }
    bool operator>(const deque_iterator& it)
    {   return it < *this;      }
    bool operator<=(const deque_iterator& it)
    {   return !(it < *this);   }
    bool operator>=(const deque_iterator& it)
    {   return !(*this < it);   }
};

template <class T>
void deque_iterator<T>::set_node(map_pointer new_node){
    node = new_node;
    first = *new_node;
    last = first + difference_type(deque_table_size(sizeof(T)));
}

template <class T>
deque_iterator& deque_iterator<T>::operator++(){
    ++cur;
    if(cur == last){
        set_node(node + 1);
        cur = first;
    }
    return *this;
}

template <class T>
deque_iterator& deque_iterator<T>::operator--(){
    if(cur == first){
        set_node(node + 1);
        cur = last;
    }
    --cur;
    return *this;
}

template <class T>
deque_iterator  deque_iterator<T>::operator++(int){
    deque_iterator<T> it = *this;
    ++*this;
    return it;
}

template <class T>
deque_iterator  deque_iterator<T>::operator--(int){
    deque_iterator<T> it = *this;
    --*this;
    return it;
}

template <class T>
deque_iterator& deque_iterator<T>::operator+=(difference_type n){
    if(last - cur - 1 > n) {
        cur += n;
    } else {
        n -= last - cur - 1;
        set_node(node + 1 + n / deque_table_size(sizeof(T)));
        cur = first + n % deque_table_size(sizeof(T));
    }
    return *this;
}

template <class T>
deque_iterator& deque_iterator<T>::operator-=(difference_type n){
    if(cur - last >= n) {
        cur -= n;
    } else {
        n -= cur - last;
        set_node(node - 1 - n / deque_table_size(sizeof(T)));
        cur = last - 1 - n % deque_table_size(sizeof(T));
    }
    return *this;
}

template <class T>
deque_iterator  deque_iterator<T>::operator-(difference_type n){
    deque_iterator<T> it(*this);
    if(it.cur - it.last >= n) {
        it.cur -= n;
    } else {
        n -= it.cur - it.last;
        set_node(it.node - 1 - n / deque_table_size(sizeof(T)));
        it.cur = it.last - 1 - n % deque_table_size(sizeof(T));
    }
    return it;
}

template <class T>
difference_type deque_iterator<T>::operator-(const deque_iterator& it){
    return (node - it.node)*deque_table_size(sizeof(T)) + (cur - it.cur);
}

template <class T>
deque_iterator  deque_iterator<T>::operator+(difference_type n){
    deque_iterator<T> it(*this);
    if(it.last - it.cur - 1 >= n) {
        it.cur += n;
    } else {
        n -= it.last - it.cur - 1;
        set_node(it.node + 1 + n / deque_table_size(sizeof(T)));
        it.cur = it.first + n % deque_table_size(sizeof(T));
    }
    return it;
}

template <class T>
inline deque_iterator<T> operator+(ptrdiff_t n, const deque_iterator& it) {
    return it + n;
}

template <class T, class Alloc = default_alloc>
class deque_base{
public:
    deque_base():map(0), map_size(0), start(), finish() {}
    deque_base(size_t num):map(0), map_size(0), start(), finish() 
    {   initialize_map(num);  }
    ~deque_base();

    // 构造，销毁table
    T*   allocate_node()
    {   return node_allocator::allocate(deque_table_size(sizeof(T)));   }
    void deallocate_node(T* ptr)
    {   node_allocator::deallocate(ptr, deque_table_size(sizeof(T)));   }
    
    // 构造，销毁map
    T**  allocate_map(size_t n)
    {   return map_allocator::allocate(n);  }
    void deallocate_map(T** ptr, size_t n)
    {   map_allocator::deallocate(ptr, n);  }

    void create_nodes(T** start, T** finish);
    void destroy_nodes(T** start, Tp** finish);

    enum {  initial_map_size = 8    };
    void initialize_map(size_t num);

private:

    typedef simple_alloc<T, default_alloc>  node_allocator;
    typedef simple_alloc<T, default_alloc>  map_allocator;

private:
    T**      map;
    size_t   map_size; 
    iterator start;
    iterator finish;
};

template <class T, class Alloc>
void deque_base<T, Alloc>::create_nodes(T** start, T** finish){
    for( ; start != finish; ++start)
        *start = allocate_node();
}

template <class T, class Alloc>
void deque_base<T, Alloc>::destroy_nodes(T** start, Tp** finish){
    for( ; start ! finish; ++start)
        deallocate_node(*start);
}

template <class T, class Alloc>
void deque_base<T, Alloc>::initialize_map(size_t num){
    size_t node_count = num / deque_table_size(sizeof(T)) + 1;
    map_size = max(initial_map_size, node_count + 2);
    map = map_allocator::allocate(map_size);

    start = map + (map - node_count) / 2;
    finish = start + node_count;

    create_nodes(start, finish);
    start.set_node(start);
    finish.set_node(finish - 1);
    start.cur = start.first;
    finish.cur = finish.first + num % deque_table_size(sizeof(T));

}

template <class T, class Alloc>
deque_base<T, Alloc>::~deque_base() {
    if(map){
        destroy_nodes(start.node, finish.node + 1);
        deallocate_map(map, map_size);
    }
}
MINISTL_NAMESPACE_END

#endif