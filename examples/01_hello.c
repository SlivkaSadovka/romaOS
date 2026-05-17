#include "../src/task.h"
#include "../src/kernel.c"

void hello_task(void) {
  printf("Hello, RomaOS!\n");
}

int main(void) {
  task_t my_task;
  my_task.prio = 0;
  my_task.entry = hello_task;
  my_task.state = TASK_READY;

  os_start(&my_task);

  return 0;
}
