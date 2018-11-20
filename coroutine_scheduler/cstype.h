//
//  cstype.h
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/18.
//  Copyright © 2018 王路远. All rights reserved.
//

#ifndef cstype_h
#define cstype_h

// Task context
// Stores registers.
typedef struct {
    long buffer[8];
} context_t;

// Task Handler
typedef void(*task_handler_t)(void);

// Task States
enum cs_task_state {
    cs_task_state_ready,
    cs_task_state_waiting,
    cs_task_state_sleeping,
};

// The Task Control Block
typedef struct cs_task {
    int tid;
    void *stack;
    context_t ctx;
    task_handler_t hander; // the task function
    enum cs_task_state state;
    struct cs_task *next_task;
    
    char desc[50]; // brief description
    
    long sleep_expired_time;
} task_t;


#endif /*cstype*/
