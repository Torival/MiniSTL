#ifndef __MINISTL_LIST_H
#define __MINISTL_LIST_H

#include "config.h"

MINISTL_NAMESPACE_BEGIN

template <class T>
struct list_node {
    T data;
    list_node* next;
    list_node* prev;
};


template <class T, class >
struct list_iterator {

};

MINISTL_NAMESPACE_END

    
#endif
