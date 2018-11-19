//
//  runloop.c
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/19.
//  Copyright © 2018 王路远. All rights reserved.
//

#include <sys/time.h>

#include "runloop.h"

extern cs_system_controller g_cs_system_controller;

void check_and_set_sleeping_task(task_t *task) {
    if (task->state != cs_task_state_sleeping) {
        print_error("%s", "The checking task is not sleeping.");
        return;
    }
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long current_time = tv.tv_sec; // get current time stamp
    
    if (current_time - task->sleep_expired_time >= 0) {
        // time expired, change the state back to ready
        // so it can run next period
        task->state = cs_task_state_ready;
    }
}

// The scheduler
void *start_run_loop() {
    
    g_cs_system_controller.running_task = g_cs_system_controller.runloop;
    
    while (1) {
        //printf("running run loop\n");
        
        g_cs_system_controller.running_task = g_cs_system_controller.running_task->next_task;
        
        if (g_cs_system_controller.running_task == NULL) {
            // if moved to the last task, then start at the first one
            g_cs_system_controller.running_task = g_cs_system_controller.first_task;
        }
        
        if (g_cs_system_controller.running_task->state == cs_task_state_ready) {
            // if the task state is ready, then switch to it and run
            switch_task(g_cs_system_controller.runloop, g_cs_system_controller.running_task);
        } else {
            // else, try to run next task
            switch (g_cs_system_controller.running_task->state) {
                case cs_task_state_sleeping:
                    // the task is sleeping, check if it can run
                    check_and_set_sleeping_task(g_cs_system_controller.running_task);
                    break;
                default:
                    break;
            }
            continue; // continue the run loop (try next task)
        }
        
        
        //printf("back to run loop\n");
    }
    return NULL;
}
