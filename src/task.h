#ifndef TASK_H
#define TASK_H

typedef enum {
  TASK_SUSPENDED = 0,
  TASK_READY,
  TASK_RUNNING,
  TASK_WAITING
} task_state_t;

typedef struct {
  int prio; // приоритет
  task_state_t state;
  void (*entry)(void); // указатель на функцию
} task_t;

#endif
