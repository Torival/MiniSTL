#ifndef __MINISTL_ALGORITHM_H
#define __MINISTL_ALGORITHM_H

#include "iterator.h"

MINISTL_NAMESPACE_BEGIN


/* 
 * 数值算法，参见<stl_numeric.h>
 */

template <class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init) {
    for( ; first != last; ++first)
        init += *first;

    return init;
}

template <class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result){
    if(first == last) 
        return result;
    
    typedef typename iterator_traits<InputIterator>::value_type value_type;
    value_type temp, value = *first;
    
     for( ; first != last; ++first) {
         temp = *first;
         *++result = temp - value;
         value = temp;
     }

     return ++result;
}

template <class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
    for( ; first1 != last1; ++first1, ++first2)
        init += (*first1) * (*first2);
    
    return init;
}

template <class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
    if(first == last)
        return result;

    typedef typename iterator_traits<InputIterator>::value_type value_type;
    value_type value = *first;
    
    for( ; first != last; ++first) {
         *++result = value + *first;
          value = * first;
     }

     return ++result;
}

template <class ForwardIterator, class T>
void iota(ForwardIterator first, ForwardIterator last, T value) {
    for( ; first != last; ++first)
        *first = value++;
}


template <class T>
T max(T value1, T value2) {
    return value1 > value2 ? value1 : value2;
}

template <class T>
T min(T value1, T value2) {
    return value1 < value2 ? value1 : value2;
}

template <class InputIterator1, class InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
    for( ; first1 != last1; ++first1, ++first2)
        if(*first1 != *first2)
            return false;
    
    return true;
}
template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
    for( ; first != last; ++first)
        *first = value;
}


template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator result, Size n, const T& value) {
    for(; n > 0; --n, ++result)
        *result = value;
    
    return result;
}

template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
    for( ; first != last; ++first, ++result)
        *result = *first;
    
    return result;
}

template <class InputIterator, class OutputIterator>
OutputIterator copy_backward(InputIterator first, InputIterator last, OutputIterator result) {
    InputIterator cur = last - 1;
    for( ; cur >= first; --cur, ++result)
        *result = *cur;
    
    return result;
}













template <class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_val, const T& new_val) {
    for( ; first != last; ++first)
        if(*first == old_val)
            *first = new_val;
}

MINISTL_NAMESPACE_END
#endif
