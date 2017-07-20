#ifndef __MINISTL_UNINITIALIZED_H
#define __MINISTL_UNINITIALIZED_H

/*
 * 全局拷贝函数：uninitialized_copy
 * 全局填充函数：uninitialized_fill, uninitialized_fill_n
 */

#include "construct.h"
#include "algorithm.h"
#include "type_traits.h"

MINISTL_NAMESPACE_BEGIN

template <class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator start, InputIterator end,
                                                ForwardIterator result, __true_type) {
    return mini_STL::copy(start, end, result);
}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator start, InputIterator end,
                                                ForwardIterator result, __false_type) {
    for( ; start != end; ++start, ++result)
        construct(&*result, *start);
    
    return result;
}

template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator __uninitialized_copy(InputIterator start, InputIterator end,
                                            ForwardIterator result, T*) {
    typedef typename __type_traits<T>::is_POD_type is_POD;

    return __uninitialized_copy_aux(start, end, result, is_POD());
} 

template <class InputIterator, class ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator start, InputIterator end, ForwardIterator result) {
    return __uninitialized_copy(start, end, result, value_type(start));
}

template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator start, ForwardIterator end, const T& val, __true_type) {
    mini_STL::fill(start, end, val);
}

template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator start, ForwardIterator end, const T& val, __false_type) {
    for(; start != end; ++start)
        construct(&*start, val);
}

template <class ForwardIterator, class T, class T1>
inline void __uninitialized_fill(ForwardIterator start, ForwardIterator end, const T& val, T1*) {
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    __uninitialized_fill_aux(start, end, val, is_POD());
}

template <class ForwardIterator, class T>
void uninitialized_fill(ForwardIterator start, ForwardIterator end, const T& val) {
    __uninitialized_fill(start, end, val, value_type(start));
}

template <class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator start, Size n, const T& val, __true_type) {
    return fill_n(start, n, val);
}

template <class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator start, Size n, const T& val, __false_type) {
    for( ; n > 0; --n, ++start)
        construct(&*start, val);
    
    return start;
}

template <class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator start, Size n, const T& val, T1*) {
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_n_aux(start, n, val, is_POD());
}

template <class ForwardIterator, class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator start, Size n, const T& val) {
    return __uninitialized_fill_n(start, n, val, value_type(start));
}

MINISTL_NAMESPACE_END
#endif
