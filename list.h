//
//  list.h
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#ifndef list_h
#define list_h

#include "util.h"

typedef struct cgs_list_item {
    struct cgs_list_item* next;
    struct cgs_list_item* prev;
    void *data;
}cgs_list_item;

typedef struct cgsList {
    int size;
    cgs_list_item* head;
    cgs_list_item* tail;
    void  (*delete_list)(struct cgsList* self);

    void (*insert_front)(struct cgsList* self, const void* n);
    void (*insert_at)(struct cgsList* self, const void* n, const int i);
    void (*insert_priority)(struct cgsList* self, const void* n);
    void (*insert_back)(struct cgsList* self, const void* n);

    void (*remove_front)(struct cgsList* self);
    void (*remove_back)(struct cgsList* self);

    int   (*empty)(struct cgsList* self);
    void* (*front)(struct cgsList* self);
    void* (*back)(struct cgsList* self);
    void* (*at)(struct cgsList* self, const int i);


    cgs_allocate_item allocate;
    cgs_free_item     free_item;
    cgs_print_item    print_item;
    cgs_compare_key  compare;
}cgsList;

cgs_item_functions CGS_LST_STORE;
cgsList* list_create(cgs_item_functions*);

#endif /* list_h */
