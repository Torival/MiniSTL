#ifndef __MINISTL_BIG_INTEGER_H
#define __MINISTL_BIG_INTEGER_H



MINISTL_NAMESPACE_BEGIN

#include <string>
#include <iostream>
using std::string;
using std::cout;
#include "alloc.h"

#define maxlen(st1, st2) (str1.length() > st2.length()? st1.length() : st2.length())

class big_integer {
    public:
        big_integer(const string _num): num(_num){}
        string to_string();
        
        friend inline big_integer operator+(const big_integer& ts, const big_integer& other);
        friend inline big_integer operator-(const big_integer& ts, const big_integer& other);
        friend inline big_integer operator*(const big_integer& ts, const big_integer& other);
        friend inline big_integer operator/(const big_integer& ts, const big_integer& other);

        friend inline bool operator<(const big_integer& ts, const big_integer& other);
        friend inline bool operator<=(const big_integer& ts, const big_integer& other);
        friend inline bool operator>(const big_integer& ts, const big_integer& other);
        friend inline bool operator>=(const big_integer& ts, const big_integer& other);
    private:
        string num;
}

inline big_integer operator+(const big_integer& ts, const big_integer& other){

    string result = "";
    int len1 = ts.length() - 1;
    int len2 = other.length() - 1;

    int flag = 0;

    for( ; len1 >= 0 && len2 >= 0; --len1, --len2){
        if(td[len1] + other[len2] + flag <= '9' + '0'){
            result += td[len1] + other[len2] + flag - '0';
            if(flag == 1) flag = 0;
        } else{
            result += td[len1] + other[len2] + flag - '9' - 1;
            flag = 1;
        }
    }
    reverse(result.begin(), result.end());
    return big_integer(num);
}

inline big_integer operator-(const big_integer& ts, const big_integer& other){

}

inline big_integer operator*(const big_integer& ts, const big_integer& other){

}

inline big_integer operator%(const big_integer& ts, const big_integer& other){

}

inline bool operator<(const big_integer&  ts, const big_integer&  other){
    if(ts.num.size() < other.num.size())
        return true;
    else if(ts.num.size() > other.num.size())
        return false;
    else{
        return ts.num > other.num;
    }
}

inline bool operator<=(const big_integer&  ts, const big_integer&  other){
    return !(other < ts);
}

inline bool operator>(const big_integer&  ts, const big_integer&  other){
    return other < ts;
}

inline bool operator>=(const big_integer&  ts, const big_integer&  other){
    return !(ts < other);
}
MINISTL_NAMESPACE_END
    
#endif
