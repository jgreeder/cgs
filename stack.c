//
//  stack.c
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#include <stdlib.h>

#include "stack.h"

static void stack_push(Stack* self, const void* d) {
    self->s_push(self->storage, d);
}

static void stack_pop(Stack* self) {
    self->s_pop(self->storage);
}

static int stack_empty(Stack* self) {
    return self->s_empty(self->storage);
}

static void* stack_top(Stack* self) {
    return self->s_top(self->storage);
}

static void stack_delete(Stack* self) {
    self->s_free(self->storage);
    free(self);
}

Stack* stack_create(void* store, stack_function_calls* f) {
    Stack* s = malloc(sizeof(Stack));
    s->storage = store;
    
    s->delete_stack = stack_delete;
    s->push = stack_push;
    s->pop  = stack_pop;
    s->empty = stack_empty;
    s->top  = stack_top;
    
    s->s_push = f->push;
    s->s_pop = f->pop;
    s->s_empty = f->empty;
    s->s_top = f->top;
    s->s_free = f->delete_storage;
    
    return s;
}

