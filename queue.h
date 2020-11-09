//
//  queue.h
//  cgs
//
//  Created by John Reeder on 11/8/20.
//  Copyright © 2020 John Reeder. All rights reserved.
//

#ifndef queue_h
#define queue_h

typedef void  (*queue_enqueue_call)(void* self, const void*);
typedef void  (*queue_dequeue_call)(void* self);
typedef int   (*queue_empty_call)(void* self);
typedef void* (*queue_front_call)(void* self);
typedef void  (*queue_free_call)(void* self);


typedef struct {
    queue_enqueue_call enqueue;
    queue_dequeue_call  dequeue;
    queue_empty_call empty;
    queue_front_call front;
    queue_free_call delete_storage;
}queue_function_calls;

typedef struct Queue {
    void* storage;
    
    void  (*delete_queue) (struct Queue*);
    void  (*enqueue) (struct Queue* self, const void* d);
    void  (*dequeue)  (struct Queue* self);
    int   (*empty)(struct Queue* self);
    void* (*front)  (struct Queue* self);
    
    queue_enqueue_call s_enqueue;
    queue_dequeue_call  s_dequeue;
    queue_empty_call s_empty;
    queue_front_call  s_front;
    queue_free_call s_free;

}Queue;

Queue* queue_create(void*, queue_function_calls*);


#endif /* queue_h */
