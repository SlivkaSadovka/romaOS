#ifndef TASK_H
#define TASK_H

#include <ucontext.h>

#define STACK_SIZE 4096

typedef enum {
  TASK_SUSPENDED = 0,
  TASK_READY,
  TASK_RUNNING,
  TASK_WAITING
} task_state_t;

typedef struct {
  int prio;
  void (*entry)(void);
  ucontext_t ctx;
  unsigned char stack[STACK_SIZE];
  task_state_t state;
} task_t;

#endif
