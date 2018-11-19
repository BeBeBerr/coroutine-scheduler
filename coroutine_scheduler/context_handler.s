//
//  context_handler.s
//  coroutine_scheduler
//
//  Created by 王路远 on 2018/11/17.
//  Copyright © 2018 王路远. All rights reserved.
//

# Save and restore task context.

// Save the switching from task context.
.text
.align 4
.globl _save_context // on macOS, function name begin with _

_save_context:
    // the last element of the stack is the returning address.
    // use rsi register to save the returning address (last PC).
    // Note: we changed the value of stack pointer rsp.
    // now, rsp is pointing to the position just before this function was called.
    pop %rsi
    // set return value to zero
    // to indicate this is the switching from task
    // but not the switching to task.
    movl $0, %eax

    // saving the calle saved registers...
    // rdi register is the 1st parameter.
    movq %rbx, (%rdi)
    // save rsp, (pointing to the position before this function was called).
    movq %rsp, 8(%rdi)

    push %rsi // change rsp back, so the function can return as usual.

    movq %rbp, 16(%rdi)
    movq %r12, 24(%rdi)
    movq %r13, 32(%rdi)
    movq %r14, 40(%rdi)
    movq %r15, 48(%rdi)
    movq %rsi, 56(%rdi) // save PC (where the switching from task will continue)

    // return with 0
    ret


// Restore the switching to task context
.text
.align 4
.globl _restore_context

_restore_context:
    // set the return value to 1 (different with 0)
    // Note: when restoring the task, it will continue at the 'return' position
    // so, set eax register will change the return value.
    // this indicate that the task is being restored
    // but not tryting to switch to another one.
    movl $1, %eax

    // restore all the registers
    // the sequence should be exactly same with the saving context function.
    movq (%rdi), %rbx
    movq 8(%rdi), %rsp
    movq 16(%rdi), %rbp
    movq 24(%rdi), %r12
    movq 32(%rdi), %r13
    movq 40(%rdi), %r14
    movq 48(%rdi), %r15

    // this function won't 'return'
    // eax register is changing the return value of the saving function.
    jmp *56(%rdi)

