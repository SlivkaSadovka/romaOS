#include "romaos.h"
#include <stdlib.h>
#include <stdio.h>

int w1_done = 0, w2_done = 0;

TASK(worker1, 1) {
    w1_done = 1;
    printf("Task 1 working...\n");
    yield();
    if (!w2_done) exit(1);
    printf("Task 1 done.\n");
    TerminateTask();
}

TASK(worker2, 2) {
    w2_done = 1;
    printf("Task 2 working...\n");
    yield();
    printf("Task 2 done.\n");
    TerminateTask();
}

TASK(task_dispatcher, 0) {
    ActivateTask(worker1);
    ActivateTask(worker2);
    TerminateTask();
}

int main(void) {
    StartOS(task_dispatcher);
    return 0;
}