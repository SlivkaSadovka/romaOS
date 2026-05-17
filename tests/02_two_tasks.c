#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "../src/task.h"
#include "../src/scheduler.h"

task_t taskA, taskB;

void yield(void) {}

void taskA_func(void) {
  for (int i = 0; i < 5; i++) {
    printf("Task A: %d\n", i);
    swapcontext(&taskA.ctx, &taskB.ctx);
  }
  printf("Task A finished.\n");
  exit(0);  
}

void taskB_func(void) {
  for (int i = 0; i < 5; i++) {
    printf("Task B: %d\n", i);
    swapcontext(&taskB.ctx, &taskA.ctx);
  }
  printf("Task A finished.\n");
  exit(0);  
}

int main(void) {
  taskA.prio = 0;
  taskA.entry = taskA_func;
  taskA.state = TASK_READY;

  taskB.prio = 0;
  taskB.entry = taskB_func;
  taskB.state = TASK_READY;

  getcontext(&taskA.ctx);
  taskA.ctx.uc_stack.ss_sp = taskA.stack;
  taskA.ctx.uc_stack.ss_size = STACK_SIZE;
  taskA.ctx.uc_link = NULL;
  makecontext(&taskA.ctx, taskA_func, 0);

  getcontext(&taskB.ctx);
  taskB.ctx.uc_stack.ss_sp = taskB.stack;
  taskB.ctx.uc_stack.ss_size = STACK_SIZE;
  taskB.ctx.uc_link = NULL;
  makecontext(&taskB.ctx, taskB_func, 0);

  printf("Starting tasks...\n");
  setcontext(&taskA.ctx);

  return 0;


}
