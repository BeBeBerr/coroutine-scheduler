//
//  main.c
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/17.
//  Copyright © 2018 王路远. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "cscore.h"
#include "cssysctrl.h"

extern cs_system_controller g_cs_system_controller;

int a = 0;

void *task_A() {
    while (1) {
        printf("A: %d\n", ++a);
        cs_sleep(3);
        spin_once();
    }
    return NULL;
}

void *task_B() {
    while (1) {
        //printf("B: %d\n", ++a);
        spin_once();
    }
    return NULL;
}


int main(int argc, const char * argv[]) {
    
    init_system_controller();
    
    create_task(task_A);
    
    create_task(task_B);

    
    start_scheduler();
    
    return 0;
}

