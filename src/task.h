#ifndef TASK_H
#define TASK_H

#include <ucontext.h>
#include "list.h"

#define STACK_SIZE 4096

typedef enum {
  TASK_SUSPENDED = 0,
  TASK_READY,
  TASK_RUNNING,
  TASK_WAITING
} task_state_t;

typedef struct {
  int prio;
  int base_prio;
  void (*entry)(void);
  ucontext_t ctx;
  unsigned char stack[STACK_SIZE];
  task_state_t state;
  list_node_t sched_node;
  list_t resources; 
} task_t;

static inline void task_init(task_t *task, int prio, void (*entry)(void)) {
    task->prio = prio;
    task->base_prio = prio;
    task->entry = entry;
    task->state = TASK_SUSPENDED;
    task->sched_node.next = NULL;
    task->sched_node.prev = NULL;
    list_init(&task->resources);
}

#endif
