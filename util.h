//
//  util.h
//  cgs
//
//  Created by John Reeder on 11/9/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#ifndef util_h
#define util_h

typedef void  (*cgs_free_item)(void*);
typedef void  (*cgs_print_item)(void*);
typedef void* (*cgs_allocate_item)(const void*);
typedef int   (*cgs_compare_key)(const void*, const void*);

typedef struct cgs_item_functions {
    cgs_allocate_item item_allocater;
    cgs_free_item free_item;
    cgs_compare_key compare;
    cgs_allocate_item key_allocater;
    cgs_free_item free_key;
} cgs_item_functions;


typedef void  (*cgs_insert_call)(void* self, const void*);
typedef void  (*cgs_remove_call)(void* self);
typedef int   (*cgs_empty_call)(void* self);
typedef void* (*cgs_front_call)(void* self);
typedef void  (*cgs_free_call)(void* self);

typedef struct {
    cgs_insert_call  insert;
    cgs_remove_call  remove;
    cgs_empty_call   empty;
    cgs_front_call   front;
    cgs_free_call    delete_storage;
}cgs_function_calls;

#endif /* util_h */
