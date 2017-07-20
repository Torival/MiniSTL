#ifndef __MINISTL_ITERATOR_H
#define __MINISTL_ITERATOR_H

#include <cstddef>
#include "config.h"

MINISTL_NAMESPACE_BEGIN

struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category    iterator_category;
    typedef T           value_type;
    typedef Distance    difference_type;
    typedef Pointer     pointer; 
    typedef Reference   reference;
};

template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::difference_type      difference_type;
    typedef typename Iterator::pointer              pointer;
    typedef typename Iterator::reference            reference;
};

template <class T>
struct iterator_traits<T*>{
    typedef         random_access_iterator_tag     iterator_category;
    typedef         T                              value_type;
    typedef         ptrdiff_t                      difference_type;
    typedef         T*                             pointer;
    typedef         T&                             reference;
};

template <class T>
struct iterator_traits<const T*> {
    typedef         random_access_iterator_tag     iterator_category;
    typedef         T                              value_type;
    typedef         ptrdiff_t                      difference_type;
    typedef         T*                             pointer;
    typedef         T&                             reference;
};

template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&) {
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

/*
 * 返回指针的原因有很多，比如 value_type 禁止拷贝、或者不能默认构造、又或者
 * 构造一个需要大量开销，返回指针只需要解引用即可得到原类型。
 */
 
 
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// 同上
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 迭代器之间的距离
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type 
distance(InputIterator start, InputIterator end) {
    return __distance(start, end, iterator_category(start));
}

template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type 
__distance(RandomAccessIterator start, RandomAccessIterator end, random_access_iterator_tag) {
    return end - start;
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type 
__distance(InputIterator start, InputIterator end, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    
    for( ; start != end; ++start)
        ++n;

    return n;
}

// 将迭代器移动 n 步
template <class InputIterator, class Distance>
inline void advance(InputIterator start, Distance n) {
    __advance(start, n, iterator_category(start));
}

template <class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator start, Distance n, random_access_iterator_tag) {
    start += n;
}

template <class BidirectionalIterator, class Distance>
inline void __advance(BidirectionalIterator start, Distance n, bidirectional_iterator_tag) {
    if(n >= 0)
        while(n--)  ++start;
    else
        while(n++)  --start;
}

template <class InputIterator, class Distance>
inline void __advance(InputIterator start, Distance n, input_iterator_tag) {
    while(n--)  ++start;
}
MINISTL_NAMESPACE_END

#endif
