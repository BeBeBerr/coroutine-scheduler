//
//  tasks.c
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/19.
//  Copyright © 2018 王路远. All rights reserved.
//

#include "tasks.h"
#include "cscore.h"

int a = 0;

void task_A() {
    while (1) {
        printf("[%d] A: %d\n", get_current_tid(), ++a);
        cs_sleep(5);
        spin_once();
    }
}

void task_B() {
    while (1) {
        printf("[%d] B: %d\n",get_current_tid(), ++a);
        cs_sleep(5);
        spin_once();
    }
}
