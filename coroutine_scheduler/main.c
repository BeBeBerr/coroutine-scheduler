//
//  main.c
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/17.
//  Copyright © 2018 王路远. All rights reserved.
//

#include <stdio.h>

#include "cscore.h"
#include "cssysctrl.h"


int main(int argc, const char * argv[]) {
    
    init_system_controller();

    start_scheduler();
    
    return 0;
}

