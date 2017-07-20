#ifndef __MINISTL_ALLOC_H
#define __MINISTL_ALLOC_H

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "config.h"

MINISTL_NAMESPACE_BEGIN

class malloc_alloc;
class default_alloc;

template <class T, class Alloc = default_alloc>
class simple_alloc {
    public:
        static T* allocate(size_t n)
            { return 0 == n ? 0 : (T*)Alloc::allocate(n*sizeof(T)); }
        static T* allocate( void )
            { return (T*)Alloc::allocate(sizeof(T)); }
        static void deallocate(T* ptr, size_t n)
            { if(n != 0) Alloc::deallocate(ptr, n*sizeof(T)); }
        static void deallocate(T* ptr)
            { Alloc::deallocate(ptr, sizeof(T)); }
};

// 第一空间配置器
class malloc_alloc{
    public:
        // TODO 内存不足
        static void* allocate(size_t n)
            { return malloc(n); }
        static void deallocate(void* ptr, size_t )
            { free(ptr); }
        static void* reallocate(void* ptr, size_t old_sz, size_t new_sz)
            { return realloc(ptr, new_sz); }
};

// 第二空间配置器
class default_alloc{
    public:
        static void* allocate(size_t n) ;
        static void  deallocate(void* ptr, size_t n) ;
        static void* reallocate(void* ptr, size_t old_sz, size_t new_sz) ;

        // TO test 
        static size_t left() {return end_free - start_free;}
        static void test_free_list() {
            obj* my_free_list;
            for(int i = 0; i < FREELIST_COUNT; i++) {
                int count = 0;
                my_free_list = free_list[i];
                while(my_free_list != 0){
                    ++count;
                    my_free_list = my_free_list->next;
                }
                std::cout << '|' << count << '\t';
            }
            std::cout <<'|'<< std::endl;
        }
        
    private:
        static const int ALGIN = 8;         // free_list 上调边界
        static const int MAX_BYTES = 128;   // free_list 上限
        static const int FREELIST_COUNT = MAX_BYTES / ALGIN; // free_list 个数

        union obj{
            union obj* next;    // 作为指针
            char data[1];       // 作为数据
        };

        static obj* volatile free_list[FREELIST_COUNT];
            ;

        // 内存池
        static char* start_free;
        static char* end_free;
        static size_t heap_size;

    private:
        //  (x + 111B) & 000B  上调到8的倍数
        static size_t round_up(size_t bytes) 
            { return (bytes + ALGIN - 1) & ~(ALGIN - 1); }
            
        //  获取在 free_list 中的下标
        static size_t index(size_t bytes)
            { return (bytes + ALGIN - 1) / ALGIN - 1; }
        
        static void* refill(size_t n) ;
        //  配置count个大小为size的块
        static char* chunk_alloc(size_t size, int& count) ;

              
};

default_alloc::obj* volatile default_alloc::free_list[FREELIST_COUNT] = 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
char*  default_alloc::start_free = 0;
char*  default_alloc::end_free   = 0;
size_t default_alloc::heap_size  = 0;

void* default_alloc::allocate(size_t n) {
    if(n > (size_t)MAX_BYTES)
        return malloc_alloc::allocate(n);
    
    obj* volatile * my_free_list = free_list + index(n);
    obj* result = *my_free_list;
    
    if(result == 0)
        return refill(round_up(n));
    
    *my_free_list = result->next;
    return result;
}

void default_alloc::deallocate(void* ptr, size_t n) {
    if(n > (size_t)MAX_BYTES)
        return malloc_alloc::deallocate(ptr, n);

    obj* volatile * my_free_list = free_list + index(n);
    obj* temp = (obj*)ptr;

    ((obj*)ptr)->next = *my_free_list;
    *my_free_list = temp;
}

void* default_alloc::reallocate(void* ptr, size_t old_sz, size_t new_sz) {
    if(old_sz > (size_t)MAX_BYTES && new_sz > (size_t)MAX_BYTES)
        return malloc_alloc::reallocate(ptr, old_sz, new_sz);

    if(round_up(old_sz) == round_up(new_sz))
        return ptr;
    
    void*  result  = allocate(new_sz);
    size_t copy_sz = old_sz > new_sz ? new_sz : old_sz;
    
    memcpy(result, ptr, copy_sz);
    deallocate(ptr, old_sz);
    
    return result;
}

void* default_alloc::refill(size_t n) {
    int count = 20;
    char* chunk = chunk_alloc(n, count);

    // 如果刚好一块，直接返回
    if(count == 1)
        return  (void*)chunk;
    
    obj* volatile * my_free_list = free_list + index(n);
    obj* next_obj = 0;
    obj* cur_obj = 0;

    // 多余的块以链表组织起来
    *my_free_list = next_obj = (obj*)(chunk + n);
    for(int i = 1; i < count; ++i) {
        cur_obj = next_obj;
        next_obj = (obj*)((char*)next_obj + n);
        cur_obj->next = next_obj;
    }
    cur_obj->next = 0;

    // 返回第一块
    return chunk;
}

char* default_alloc::chunk_alloc(size_t size, int& count) {
    size_t total_bytes = size * count;
    size_t left_bytes  = end_free - start_free;
    char* result;

    // 剩下的空间足够分配count个块
    if(left_bytes >= total_bytes) {
        result = start_free;
        start_free += total_bytes;
        
        return result;
    }

    // 还够一个以上
    if(left_bytes >= size) {
        count = left_bytes / size;
        result = start_free;
        start_free += count * size;

        return result;
    }

    // 把剩下不够分配的空间合并到对于块上
    if(left_bytes > 0) {
        obj* volatile * my_free_list = free_list + index(left_bytes);
        ((obj*)start_free)->next = *my_free_list;
        *my_free_list = (obj*)start_free;
    }

    size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);
    start_free = (char*)malloc(bytes_to_get);
    // 如果分配空间失败，从比size大的块中找一个
    if(start_free == 0) {
        obj* volatile * my_free_list;
        for(int i = index(size) + 1; i < FREELIST_COUNT; ++i) {
            my_free_list = free_list + i;
            if(*my_free_list != 0) {
                start_free = result = (char*)*my_free_list;
                *my_free_list = (*my_free_list)->next;
                // 把这个大块减去 size 大小，然后放到内存池
                end_free = start_free + (i+1) * ALGIN;
                start_free += size;

                return result;
            }
        }
    }

    heap_size += bytes_to_get;
    result = start_free;
    
    end_free = start_free + bytes_to_get;
    start_free += total_bytes;
    
    return result;
}

MINISTL_NAMESPACE_END
#endif 
