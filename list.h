//
//  list.h
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#ifndef list_h
#define list_h

typedef void  (*list_free_item)(void*);
typedef void  (*list_print_item)(void*);
typedef void* (*list_allocate_item)(const void*);
typedef int   (*list_compare_item)(const void*, const void*);


typedef struct {
    list_allocate_item allocater;
    list_free_item free_item;
    list_print_item print_item;
    list_compare_item compare;
} list_item_functions;

typedef struct list_item {
    struct list_item* next;
    struct list_item* prev;
    void *data;
}list_item;

typedef struct List {
    int size;
    list_item* head;
    list_item* tail;
    void  (*delete_list)(struct List* self);

    void (*insert_front)(struct List* self, const void* n);
    void (*insert_at)(struct List* self, const void* n, const int i);
    void (*insert_priority)(struct List* self, const void* n);
    void (*insert_back)(struct List* self, const void* n);

    void (*remove_front)(struct List* self);
    void (*remove_back)(struct List* self);
    void (*print)(struct List* self);

    int   (*empty)(struct List* self);
    void* (*front)(struct List* self);
    void* (*back)(struct List* self);

    list_allocate_item allocate;
    list_free_item     free_item;
    list_print_item    print_item;
    list_compare_item  compare;
}List;

list_item_functions LST_STORE;
List* list_create(list_item_functions*);

#endif /* list_h */
