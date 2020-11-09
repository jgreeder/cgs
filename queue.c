//
//  queue.c
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#include <stdlib.h>

#include "queue.h"

static void delete_queue(Queue* self) {
    self->s_free(self->storage);
    free(self);
}

static void queue_enqueue(Queue* self, const void* d) {
    self->s_enqueue(self->storage, d);
}

static void queue_dequeue(Queue* self) {
    self->s_dequeue(self->storage);
}

static void* queue_front(Queue* self) {
    return self->s_front(self->storage);
}

static int queue_empty(Queue* self) {
    return self->s_empty(self->storage);
}


Queue* queue_create(void* store, queue_function_calls* f) {
    Queue* q = malloc(sizeof(Queue));
    q->storage = store;
    
    q->delete_queue = delete_queue;
    q->enqueue = queue_enqueue;
    q->dequeue  = queue_dequeue;
    q->empty = queue_empty;
    q->front  = queue_front;
    
    q->s_enqueue = f->enqueue;
    q->s_dequeue = f->dequeue;
    q->s_empty = f->empty;
    q->s_front = f->front;
    q->s_free = f->delete_storage;
    
    return q;
}
