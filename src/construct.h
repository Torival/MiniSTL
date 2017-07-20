#ifndef __MINISTL_CONSTRUCT_H
#define __MINISTL_CONSTRUCT_H
/*
 * 全局构造函数：construct
 * 全局析够函数：destroy
 */

#include <new>
#include "type_traits.h"
#include "config.h"
#include "iterator.h"

MINISTL_NAMESPACE_BEGIN
template <class T1, class T2>
inline void construct(T1* ptr, const T2& value) {
    new (ptr) T1(value);    // placement new
}

template <class T>
inline void destroy(T* ptr) {
    ptr->~T();
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator start, ForwardIterator end, __false_type) {
    for( ; start != end; ++start)
        destroy(&*start);
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator start, ForwardIterator end, __true_type) { }

template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator start, ForwardIterator end, T*) {
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(start, end, trivial_destructor());
}

template <class ForwardIterator> 
inline void destroy(ForwardIterator start, ForwardIterator end) {
    __destroy(start, end, value_type(start));
}

MINISTL_NAMESPACE_END
#endif
