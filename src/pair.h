#ifndef __MINISTL_PAIR_H
#define __MINISTL_PAIR_H

#include "config.h"

MINISTL_NAMESPACE_BEGIN

template <class T1, class T2>
struct pair {
    typedef T1 Key;
    typedef T2 Value;

    Key key;
    Value val;

    pair() : key(Key()), val(Value()) { }
    pair(const Key _key, const Value _val) : key(_key), val(_val) { }
    
    template <class U1, class U2>
    pair(const pair<U1, U2>& p) : key(p.key), val(p.val) { } 
};

template <class T1, class T2>
inline bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y){
    return (x.key == y.key) && (x.val == y.val);
}

template <class T1, class T2>
inline bool operator!=(const pair<T1, T2>& x, const pair<T1, T2>& y){
    return !(x == y);
}

template <class T1, class T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y){
    return (x.key < y.key) || (!(y.key < x.key) && x.val < y.val);
}

template <class T1, class T2>
inline bool operator>(const pair<T1, T2>& x, const pair<T1, T2>& y){
    return y < x;
}

template <class T1, class T2>
inline bool operator<=(const pair<T1, T2>& x, const pair<T1, T2>& y){
    return !(y < x);
}

template <class T1, class T2>
inline bool operator>=(const pair<T1, T2>& x, const pair<T1, T2>& y){
    return !(x < y);
}

template <class T1, class T2>
inline pair<T1, T2> make_pair(const T1& key, const T2& val) {
    return pair<T1, T2>(key, val);
}

MINISTL_NAMESPACE_END
#endif
