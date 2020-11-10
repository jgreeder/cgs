//
//  queue.c
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#include <stdlib.h>
#include <assert.h>

#include "queue.h"

static void delete_queue(cgsQueue* self) {
    self->s_free(self->storage);
    free(self);
}

static void queue_enqueue(cgsQueue* self, const void* d) {
    self->s_enqueue(self->storage, d);
}

static void queue_dequeue(cgsQueue* self) {
    assert(!self->s_empty(self->storage));
    self->s_dequeue(self->storage);
}

static void* queue_front(cgsQueue* self) {
    assert(!self->s_empty(self->storage));
    return self->s_front(self->storage);
}

static int queue_empty(cgsQueue* self) {
    return self->s_empty(self->storage);
}


cgsQueue* queue_create(void* store, cgs_function_calls* f) {
    assert(store && f);
    
    cgsQueue* q = malloc(sizeof(cgsQueue));
    
    q->storage = store;
    
    q->delete_queue = delete_queue;
    q->enqueue = queue_enqueue;
    q->dequeue  = queue_dequeue;
    q->empty = queue_empty;
    q->front  = queue_front;
    
    q->s_enqueue = f->insert;
    q->s_dequeue = f->remove;
    q->s_empty = f->empty;
    q->s_front = f->front;
    q->s_free = f->delete_storage;
    
    return q;
}
