#ifndef __MINISTL_FUNCTION_H
#define __MINISTL_FUNCTION_H

#include "config.h"

MINISTL_NAMESPACE_BEGIN

// 二元运算 
template <class T>
struct plus {
    T operator()(const T& x, const T& y){   return x + y; }
}

template <class T>
struct minus {
    T operator()(const T& x, const T& y){   return x - y; }
}

template <class T>
struct multiplies {
    T operator()(const T& x, const T& y){   return x + y; }
}

template <class T>
struct divides {
    T operator()(const T& x, const T& y){   return x / y; }
}

template <class T>
struct modulus {
    T operator()(const T& x, const T& y){   return x % y; }
}

template <class T>
struct equal_to {
    bool operator()(const T& x, const T& y){   return x == y; }
}

template <class T>
struct not_equal_to {
    bool operator()(const T& x, const T& y){   return x != y; }
}

template <class T>
struct greater {
    bool operator()(const T& x, const T& y){   return x > y; }
}

template <class T>
struct greater_equal {
    bool operator()(const T& x, const T& y){   return x >= y; }
}

template <class T>
struct less {
    bool operator()(const T& x, const T& y){   return x < y; }
}

template <class T>
struct less_equal {
    bool operator()(const T& x, const T& y){   return x <= y; }
}

template <class T>
struct logical_and {
    bool operator()(const T& x, const T& y){   return x && y; }
}

template <class T>
struct logical_or {
    bool operator()(const T& x, const T& y){   return x || y; }
}

// 一元运算
template <class T>
struct logical_not {
    bool operator()(const T& x){   return !x; }
}

template <class T>
struct negate {
    T operator()(const T& x){   return -x; }
}

// 单位元
template <class T>
inline _Tp identity_element(plus<T>) {
    return T(0);
}

template <class T>
inline _Tp identity_element(multiplies<T>) {
    return T(1);
}

MINISTL_NAMESPACE_END

#endif