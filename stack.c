//
//  stack.c
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#include <stdlib.h>

#include "stack.h"

static void stack_push(cgsStack* self, const void* d) {
    self->s_push(self->storage, d);
}

static void stack_pop(cgsStack* self) {
    self->s_pop(self->storage);
}

static int stack_empty(cgsStack* self) {
    return self->s_empty(self->storage);
}

static void* stack_top(cgsStack* self) {
    return self->s_top(self->storage);
}

static void stack_delete(cgsStack* self) {
    self->s_free(self->storage);
    free(self);
}

cgsStack* stack_create(void* store, cgs_function_calls* f) {
    cgsStack* s = malloc(sizeof(cgsStack));
    s->storage = store;
    
    s->delete_stack = stack_delete;
    s->push = stack_push;
    s->pop  = stack_pop;
    s->empty = stack_empty;
    s->top  = stack_top;
    
    s->s_push = f->insert;
    s->s_pop = f->remove;
    s->s_empty = f->empty;
    s->s_top = f->front;
    s->s_free = f->delete_storage;
    
    return s;
}

