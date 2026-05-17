#include "romaos.h"
#include <stdio.h>

TASK(taskB, 1) {
    for (int i = 0; i < 5; i++) {
        printf("Task B: %d\n", i);
        yield();
    }
    printf("Task B finished.\n");
    TerminateTask();
}

TASK(taskA, 0) {
    ActivateTask(taskB);

    for (int i = 0; i < 5; i++) {
        printf("Task A: %d\n", i);
        yield();
    }
    printf("Task A finished.\n");
    TerminateTask();
}

int main(void) {
    StartOS(taskA);
    return 0;
}