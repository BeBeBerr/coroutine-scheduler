//
//  runloop.c
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/19.
//  Copyright © 2018 王路远. All rights reserved.
//

#include "runloop.h"

extern cs_system_controller g_cs_system_controller;

// The scheduler
void *start_run_loop() {
    
    g_cs_system_controller.running_task = g_cs_system_controller.runloop;
    
    while (1) {
        printf("running run loop\n");
        
        g_cs_system_controller.running_task = g_cs_system_controller.running_task->next_task;
        
        if (g_cs_system_controller.running_task == NULL) {
            g_cs_system_controller.running_task = g_cs_system_controller.first_task;
        }
        
        switch_task(g_cs_system_controller.runloop, g_cs_system_controller.running_task);
        
        printf("back to run loop\n");
    }
    return NULL;
}
