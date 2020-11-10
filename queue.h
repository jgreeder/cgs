//
//  queue.h
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#ifndef queue_h
#define queue_h

#include "util.h"

typedef struct cgsQueue {
    void* storage;
    
    void  (*delete_queue) (struct cgsQueue* self);
    void  (*enqueue) (struct cgsQueue* self, const void* d);
    void  (*dequeue)  (struct cgsQueue* self);
    int   (*empty)(struct cgsQueue* self);
    void* (*front)  (struct cgsQueue* self);
    
    cgs_insert_call s_enqueue;
    cgs_remove_call  s_dequeue;
    cgs_empty_call s_empty;
    cgs_front_call  s_front;
    cgs_free_call s_free;

}cgsQueue;

cgsQueue* queue_create(void*, cgs_function_calls*);


#endif /* queue_h */
