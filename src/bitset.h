#ifndef __MINISTL_BITSET_H
#define __MINISTL_BITSET_H

#include <string>
#include <iostream>
using std::string;
using std::cout;
#include "alloc.h"

MINISTL_NAMESPACE_BEGIN

template<size_t N>
class bitset {
    public:
        bitset();        
        bitset(string& set);
        bitset(const bitset<N>& other);

        ~ bitset() ;

        // bit 访问
        size_t count() const ;
        bool any() const;
        bool none() const;
        bool all() const;
        bool test(size_t pos) const;

        bool operator [] (size_t pos) const;
        bool operator == (const bitset<N> bcs);
        bool operator != (const bitset<N> bcs);
        

        // bit 操作
        string to_string() const ;

        bitset<N>& operator&= (const bitset<N> bcs);
        bitset<N>& operator|= (const bitset<N> bcs);
        bitset<N>& operator^= (const bitset<N> bcs);
       
        bitset<N>& operator<<= (size_t pos);
        bitset<N>& operator>>= (size_t pos);
        bitset<N>  operator~ ();

        bitset<N>& set();
        bitset<N>& set(size_t pos);
        bitset<N>& reset();
        bitset<N>& reset(size_t pos);
        bitset<N>& flip();
        bitset<N>& flip(size_t pos);
        
        /*
        class reference {
            public:
                reference(size_t _pos):pos(_pos) { }
                ~reference() { }
                reference& operator =(bool val) { 
                    set(pos);
                    return *this;
                }
                reference& operator =(const reference& val) { 
                    if(val.start[val.pos])
                        reset(pos);
                    else
                        set(pos);
                    return *this;
                }
                
            private:
                size_t pos;
        };
        reference& operator [] (size_t pos)
            { return reference(pos); }
        */

    private:
        typedef unsigned char uchar;

        uchar* start;
        static const unsigned char bitsinbyte[256];
        
        typedef simple_alloc<uchar, default_alloc> data_allocator;           
};

template<size_t N>
const unsigned char bitset<N>::bitsinbyte[256] = {
    //0000 0000 - 0000 0001
    0,1,

    //0000 0010 - 0000 0011
    1,2,

    //0000 0100 - 0000 0111
    1,2,2,3,

    //0000 1000 - 0000 1111
    1,2,2,3,2,3,3,4,

    //0001 0000 - 0001 1111
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,

    //0010 0000 - 0011 1111
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,

    //0100 0000 - 0111 1111
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
    3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,

    //1000 0000 - 1111 1111
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
    3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
    3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
    3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
    4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
};

template<size_t N>
bitset<N>::bitset() {
    start = data_allocator::allocate(N / 8 + 1);
    memset(start, '\0', N / 8 + 1);
}       

template<size_t N>
bitset<N>::bitset(string& set):bitset() {
    for(int i = 0; i < set.size(); ++i)
        if(set[i] == '1') {
            int index = i / 8;
            int offset = i % 8;

            start[index] |= (1 << offset);
        }
}

template<size_t N>
bitset<N>::bitset(const bitset<N>& other) {
    start = data_allocator::allocate(N / 8 + 1);
    memcpy(start, other.start, N / 8 + 1);
}

template<size_t N>
bitset<N>::~bitset() {
    data_allocator::deallocate(start, N / 8 + 1);
}

template<size_t N>
size_t bitset<N>::count() const {
    size_t result = 0;

    for(int i = 0; i < N / 8; ++i)
        result += bitsinbyte[start[i]];

    for(int i = 0; i < N % 8; ++i)
        if(start[N / 8] & (1 << i))
            ++result; 

    return result;
}    

template<size_t N>
bool bitset<N>::any() const {
    for(uchar* cur = start; cur <= start + N / 8; ++cur)
        if(*cur != 0) 
            return true;
    
    return false;
}


template<size_t N>
bool bitset<N>::none() const {
    for(uchar* cur = start; cur <= start + N / 8; ++cur)
        if(*cur != 0) 
            return false;
    
    return true;
}

template<size_t N>
bool bitset<N>::all() const {
    for(int i = 0; i < N / 8; ++i)
        if(start[i] != 0XFF)
            return false;

    for(int i = 0; i < N % 8; ++i)
        if(!(start[N / 8] & (1 << i)))
            return false;
    
    return true; 
}

template<size_t N>
bool bitset<N>::test(size_t pos) const {
    if(pos > N)
        return false;

    return (*this).operator[](pos);
}


template<size_t N>
bool bitset<N>::operator[] (size_t pos) const {
    int index = (pos - 1) / 8;
    int offset = (pos - 1) % 8;

    return start[index] & (1 << offset);
}

template<size_t N>
bool bitset<N>::operator== (const bitset<N> bcs) {
    uchar* cur1 = start;
    uchar* cur2 = bcs.start;
    
    for(; cur1 <= start + N/8; ++cur1, ++cur2)
        if(*cur1 != *cur2)
            return false;

    return true; 
}

template<size_t N>
bool bitset<N>::operator!= (const bitset<N> bcs) {
    return !((*this).operator==(bcs));
}


template<size_t N>
string bitset<N>::to_string() const {
    string out = "";
    uchar* cur = start;
    
    for(; cur < start + N / 8; ++cur)
        for(int i = 0; i < 8; ++i)
            if(*cur & (1 << i))
                out += '1';
            else
                out += '0';

    for(int i = 0; i < N % 8; ++i)
        if(*cur & (1 << i))
                out += '1';
            else
                out += '0';

    return out;    
}

template<size_t N>
bitset<N>& bitset<N>::operator&= (const bitset<N> bcs) {
    uchar* cur1 = start;
    uchar* cur2 = bcs.start;
    
    for( ; cur1 <= start + N / 8; ++cur1, ++cur2)
        *cur1 &= *cur2;
    
    return *this;
}

template<size_t N>
bitset<N>& bitset<N>::operator|= (const bitset<N> bcs) {
    uchar* cur1 = start;
    uchar* cur2 = bcs.start;
    
    for( ; cur1 <= start + N / 8; ++cur1, ++cur2)
        *cur1 |= *cur2;

    return *this;
}

template<size_t N>
bitset<N>& bitset<N>::operator^= (const bitset<N> bcs) {
    uchar* cur1 = start;
    uchar* cur2 = bcs.start;
    
    for( ; cur1 <= start + N / 8; ++cur1, ++cur2)
        *cur1 ^= *cur2;
    
    return *this;
}

template<size_t N>
bitset<N> bitset<N>::operator~ () {
    bitset<N> val(*this);
    uchar* cur = val.start;

    for( ; cur <= start + N / 8; ++cur)
        *cur = ~*cur;

    return val;
}

template<size_t N>
bitset<N>& bitset<N>::operator<<= (size_t pos) {
    int index = 1;
    
    for(size_t i = pos; i < N; ++i) {
        if((*this)[i])
            set(index);
        else
            reset(index);   
        ++index;
    }
    
    while(index <= N)
        reset(index++);
        

    return *this;
}

template<size_t N>
bitset<N>& bitset<N>::operator>>= (size_t pos) {
    int index = N;
    
    for(size_t i = N - pos; i > 0; --i) {
        if((*this)[i])
            set(index);
        else
            reset(index);
            
        --index;
    }
    while(index > 0)
        reset(index--);

    return *this;
}


template<size_t N>
bitset<N>& bitset<N>::set() {
    uchar* cur = start;
    
    for( ; cur <= start + N / 8; ++cur)
        *cur &= 0xFF;
    
    return *this;
}

template<size_t N>
bitset<N>& bitset<N>::set(size_t pos) {
    if(pos > N)
        return *this;

    uchar value = 1 << ((pos - 1) % 8);
    start[(pos - 1) / 8] |= value;

    return *this;
}

template<size_t N>
bitset<N>& bitset<N>::reset() {
    uchar* cur = start;
    
    for( ; cur <= start + N / 8; ++cur)
        *cur &= 0x00;
    
    return *this;
}

template<size_t N>
bitset<N>& bitset<N>::reset(size_t pos) {
    if(pos > N)
        return *this;
    
    uchar value = 1 << ((pos - 1) % 8);
    start[(pos - 1) / 8] &= ~value;

    return *this;
}

template<size_t N>
bitset<N>& bitset<N>::flip() {
    uchar* cur = start;

    for( ; cur <= start + N / 8; ++cur)
        *cur = ~*cur;
    
    return *this;
}

template<size_t N>
bitset<N>& bitset<N>::flip(size_t pos) {
    if(pos > N)
        return *this;
    
    if((*this).operator[]((pos - 1)))
        reset(pos);
    else
        set(pos);
    
    return *this;
}

template<size_t N>
inline bitset<N> operator&  (const bitset<N>& lbs, const bitset<N>& rbs) {
    bitset<N> val(lbs);
    val &= rbs;
    
    return val;
}

template<size_t N>
inline bitset<N> operator|  (const bitset<N>& lbs, const bitset<N>& rbs) {
    bitset<N> val(lbs);
    val |= rbs;

    return val;
}

template<size_t N>
inline bitset<N> operator^  (const bitset<N>& lbs, const bitset<N>& rbs) {
    bitset<N> val(lbs);
    val ^= rbs;
    
    return val;
}

MINISTL_NAMESPACE_END

#endif

