//
//  list.c
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>

#include "list.h"

static void* list_allocater(const void* lst);
static void list_free(void * lst);
static void list_print_open(void * lst);
static int list_priority(const void* l, const void* r);

list_item_functions LST_STORE = {list_allocater, list_free, list_print_open, list_priority};

static list_item* new_item(const void* i, list_allocate_item allocate_item) {
    list_item* n = malloc(sizeof(list_item));
    n->next = NULL;
    n->data = allocate_item(i);

    return n;
}

static void del_item(list_item* i, list_free_item f) {
    f(i->data);
    free(i);
}

static void list_print(List* self) {
    list_item* it = self->head;
    for (int i = 0; i < self->size; i++) {
        self->print_item(it->data);
        it = it->next;
    }
    printf("Size: %d\n", self->size);
}

static void list_delete_list(List* self) {
    if (self->empty(self)) return;
    list_item* it;
    for(int i=0; i < self->size; i++) {
        it = self->head;
        self->head = self->head->next;
        del_item(it, self->free_item);
    }
    free(self);
}

static void list_insert_priority(List* self, const void* n) {
    if (self->compare == NULL) return;
    
    list_item* nn = new_item(n, self->allocate);

    if (self->empty(self)){
        self->head = self->tail = nn;
    } else if (self->compare(self->tail->data, nn->data) <= 0) {
        self->tail->next = nn;
        self->tail = nn;
    } else if (self->compare(self->head->data, nn->data) == 1) {
        nn->next = self->head;
        self->head = nn;
    }
    else {
        list_item* it = self->head;

        while(it != self->tail &&
              it->next &&
              self->compare(it->next->data, nn->data) <= 0)
            it = it->next;

        nn->next = it->next;
        it->next = nn;
    }
    self->size++;
}

static void list_insert_front(List* self, const void* n) {
    list_item* nn = new_item(n, self->allocate);

    if (self->empty(self)){
        self->head = self->tail = nn;
    } else {
        nn->next = self->head;
        self->head->prev = nn;
        self->head = nn;
    }

    self->size++;
}

static void list_insert_at(List* self, const void* n, const int i) {
    if (i < 0) return;
    else if (i == 0 || self->empty(self))
        self->insert_front(self, n);
    else if (i >= self->size-1)
        self->insert_back(self, n);
    else {
        list_item* nn = new_item(n, self->allocate);

        list_item* it = self->head;
        for (int j = 1; j < i; j++) {
            it = it->next;
        }
        nn->next = it->next;
        it->next->prev = nn;
        it->next = nn;
        nn->prev = it;

        self->size++;
    }
}

static void list_insert_end(List* self, const void* n) {
    list_item* nn = new_item(n, self->allocate);

    if (self->empty(self)){
        self->head = self->tail = nn;
    } else {
        nn->prev = self->tail;
        self->tail->next = nn;
        self->tail = nn;
    }
    self->size++;
}

static int list_empty(List* self) {
    return (self->size == 0);
}

static void* list_front(List* self) {
    if(self->head)
        return self->head->data;
    else
        return NULL;
}

static void* list_back(List* self) {
    if (self->tail)
        return self->tail->data;
    else
        return NULL;
}

static void  list_remove_front(List* self){
    if (self->empty(self)) return;

    list_item* temp = self->head;
    self->head = self->head->next;
    self->size -= 1;

    del_item(temp, self->free_item);
}

static void  list_remove_back(List* self){
    if (self->empty(self)) return;

    list_item* temp = self->tail;
    self->tail = self->tail->prev;
    self->size -= 1;

    del_item(temp, self->free_item);
}

List* list_create(list_item_functions* f)
{
    List* list = (List*)malloc(sizeof(List));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    list->delete_list = list_delete_list;

    list->insert_front = list_insert_front;
    list->insert_at    = list_insert_at;
    list->insert_priority = list_insert_priority;
    list->insert_back   = list_insert_end;

    list->remove_front = list_remove_front;
    list->remove_back  = list_remove_back;

    list->empty = list_empty;
    list->front = list_front;
    list->back  = list_back;

    list->print = list_print;

    list->allocate = f->allocater;
    list->free_item = f->free_item;
    list->print_item = f->print_item;
    list->compare = f->compare;

    return list;
}


static void* list_allocater(const void* lst){
    const list_item_functions* f = lst;
    return list_create(&(list_item_functions){f->allocater, f->free_item, f->print_item, f->compare});
}

static void list_free(void * lst) {
    list_delete_list(lst);
}

static void list_print_open(void * lst) {
    
}

static int list_priority(const void* l, const void* r) {
    return ((List*)l)->size < ((List*)r)->size;
}

