#ifndef __MINISTL_VECTOR_H
#define __MINISTL_VECTOR_H

#include "alloc.h"
#include "uninitialized.h"

// TO TEST
#include <iostream>
using std::cout;
using std::endl;

MINISTL_NAMESPACE_BEGIN

template <class T, class Alloc = default_alloc>
class vector {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef T*          iterator;
        typedef T&          reference;
        typedef ptrdiff_t   difference_type;

        // 构造函数
        vector():start(0), finish(0), end_of_storage(0) { }
        vector(size_t n, const T& value);
        vector(const vector<T>& other);
        template <class InputIterator>
        vector(InputIterator first, InputIterator last);
        vector(size_t n);

        // 析够函数
        ~vector();

        // 元素访问 
        iterator begin() const { return start; }
        iterator end() const { return finish; }
        size_t size() const { return size_t(finish - start); }
        size_t capacity() const { return size_t(end_of_storage - start); }
        bool empty() const { return start == finish; }
        reference operator[](size_t n) { return *(start + n); }
        reference back() { return *(finish - 1); }
        reference front() { return *start; }
        
        // 内容修改
        void push_back(const T& x) ;
        void pop_back() ;
        void swap(vector<T>& other);
        void clear();
        void resize(size_t n);

        // 增减
        void insert(iterator pos, const T& value) ;
        void insert(iterator pos, size_t n, const T& value) ;
        template <class InputIterator>
        void insert(iterator pos, InputIterator first, InputIterator last);
        
        void erase(iterator pos);
        void erase(iterator first, iterator last);

    private:
        template <class InputIterator>
        void __vector(InputIterator first, InputIterator last, __false_type);
        template <class Interger>
        void __vector(Interger n, const Interger& value, __true_type);

        template <class InputIterator>
        void __insert(iterator pos, InputIterator first, InputIterator last, __false_type);
         template <class Interger>
        void __insert(iterator pos, Interger n, const Interger& value, __true_type);

        typedef simple_alloc<T, default_alloc> data_allocator;
        iterator start;
        iterator finish;
        iterator end_of_storage;
};

template <class T, class Alloc>
vector<T, Alloc>::vector(const vector<T>& other) {
    size_t n = other.capacity();
    start = data_allocator::allocate(n);
    end_of_storage = start + n;
    finish = uninitialized_copy(other.start, other.finish, start);
}

template <class T, class Alloc>
vector<T, Alloc>::vector(size_t n, const T& value) {
    start = data_allocator::allocate(n);
    end_of_storage = start + n;
    finish = uninitialized_fill_n(start, n, value);
}

template <class T, class Alloc>
template <class InputIterator>
vector<T, Alloc>::vector(InputIterator first, InputIterator last){
    typedef typename __is_intger<InputIterator>::is_POD_type is_POD_type;
    __vector(first, last, is_POD_type());   
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::__vector(InputIterator first, InputIterator last, __false_type) {
    size_t n = size_t(last - first);
    start = data_allocator::allocate(n);
    end_of_storage = start + n;
    finish = uninitialized_copy(first, last, start);
}

template <class T, class Alloc>
template <class Interger>
void vector<T, Alloc>::__vector(Interger n, const Interger& value, __true_type) {
    start = data_allocator::allocate(n);
    end_of_storage = start + n;
    finish = uninitialized_fill_n(start, n, value);
}

template <class T, class Alloc>
vector<T, Alloc>::vector(size_t n) {
    start = data_allocator::allocate(n);
    end_of_storage = start + n;
    finish = start;
}

template <class T, class Alloc>
vector<T, Alloc>::~vector() {
    data_allocator::deallocate(start, capacity());
}

template <class T, class Alloc>
void vector<T, Alloc>::push_back(const T& x) {
    if(finish == end_of_storage)
        resize(2 * capacity());
    construct(finish, x);
    ++finish;
}

template <class T, class Alloc>
void vector<T, Alloc>::pop_back() {
    if(start != finish)
        destroy(--finish);
    
    if(finish - start <= capacity() / 4)
        resize(capacity() / 2);       
}

template <class T, class Alloc>
void vector<T, Alloc>::swap(vector<T>& other) {
    swap(start, other.start);
    swap(finish, other.finish);
    swap(end_of_storage, other.end_of_storage);
}

template <class T, class Alloc>
void vector<T, Alloc>::clear() {
    destroy(start, finish);
    finish = start;
}

template <class T, class Alloc>
void vector<T, Alloc>::resize(size_t n) {
    if(n > capacity()) {
        iterator new_start = data_allocator::allocate(n);
        iterator new_finish = uninitialized_copy(start, finish, new_start);
        
        data_allocator::deallocate(start, finish - start);
        start = new_start;
        finish = new_finish;
        end_of_storage = start + n;
    } else {
        iterator new_end_of_storage = start + n;

        if(new_end_of_storage < finish) {
            destroy(new_end_of_storage, finish);
            finish = new_end_of_storage;
        }

        data_allocator::deallocate(new_end_of_storage, end_of_storage - new_end_of_storage);
        end_of_storage = new_end_of_storage;
    }
}

template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator pos, const T& value) {
    insert(pos, 1, value);
}

template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator pos, size_t n, const T& value) {
    if(pos < start || pos >= finish)
        return ;

    if(n > end_of_storage - finish) {
        iterator new_start = data_allocator::allocate(capacity() + 2 * n);
        iterator new_finish = new_start;
        iterator new_end_of_storage = new_start + capacity() + 2 * n;
        
       
        new_finish = uninitialized_copy(start, pos, new_start);
        new_finish = uninitialized_fill_n(new_finish, n, value);
        new_finish = uninitialized_copy(pos, finish, new_finish);

        data_allocator::deallocate(start, capacity());
        start = new_start;
        finish = new_finish;
        end_of_storage = new_end_of_storage;
    } else {
        iterator new_finish = finish + n;

        for(iterator it = finish - 1; it >= pos; --it)
            construct(it + n, *it);

        uninitialized_fill_n(pos, n, value);
        finish = new_finish;
    }
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last) {
    typedef typename __is_intger<InputIterator>::is_POD_type is_POD_type;
    __insert(pos, first, last, is_POD_type());
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::__insert(iterator pos, InputIterator first, InputIterator last, __false_type) {
    if(pos < start || pos >= finish)
        return ;

    size_t n = 0;

    if(last > last); 
        n = size_t(last - first);
    
    if(n > end_of_storage - finish) {
        iterator new_start = data_allocator::allocate(capacity() + 2 * n);
        iterator new_finish = new_start;
        iterator new_end_of_storage = new_start + capacity() + 2 * n;
        
        new_finish = uninitialized_copy(start, pos, new_start);
        new_finish = uninitialized_copy(first, last, new_finish);
        new_finish = uninitialized_copy(pos, finish, new_finish);

        data_allocator::deallocate(start, capacity());
        start = new_start;
        finish = new_finish;
        end_of_storage = new_end_of_storage;
    } else {
        iterator new_finish = finish + n;
        for(iterator it = finish - 1; it >= pos; --it)
            construct(it + n, *it);

        uninitialized_copy(first, last, pos);
        finish = new_finish;
    }
}

template <class T, class Alloc>
template <class Interger>
void vector<T, Alloc>::__insert(iterator pos, Interger n, const Interger& value, __true_type) {
    insert(pos, (size_t)n, (const T&) value);
}

template <class T, class Alloc>
void vector<T, Alloc>::erase(iterator pos) {
    if(pos < start || pos >= finish)
        return ;

    destroy(pos);
    uninitialized_copy(pos + 1, finish, pos);
    --finish;
}

template <class T, class Alloc>
void vector<T, Alloc>::erase(iterator first, iterator last) {
    if(last > first && first >= start && last <= finish) {
        destroy(first, last);

        if(last < finish)
        uninitialized_copy(last, finish, first);
        finish -= last - first;
    }
}

template <class T>
bool operator == (const vector<T>& ts, const vector<T>& other) {
    return (ts.size() == other.size()) && equal(ts.begin(), ts.end(), other.begin());
}

template <class T>
bool operator != (const vector<T>& ts, const vector<T>& other) {
    return !(ts == other);
}

MINISTL_NAMESPACE_END

#endif
