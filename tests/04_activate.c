#include "romaos.h"
#include <stdio.h>
#include <stdlib.h>

int a_ok = 0, b_ok = 0;

TASK(taskB, 2) {
    printf("Task B started.\n");
    for (int i = 0; i < 3; i++) {
        printf("Task B: %d\n", i);
        yield();
    }
    b_ok = 1;
    printf("Task B finished.\n");
    TerminateTask();
}

TASK(taskA, 1) {
    printf("Task A started.\n");
    for (int i = 0; i < 3; i++) {
        printf("Task A: %d\n", i);
        yield();
    }
    a_ok = 1;
    printf("Task A finished.\n");
    TerminateTask();
}

TASK(dispatcher, 0) {
    printf("Dispatcher: activating taskA and taskB.\n");
    ActivateTask(taskA);
    ActivateTask(taskB);
    printf("Dispatcher done.\n");
    TerminateTask();
}

int main(void) {
    StartOS(dispatcher);
    if (!a_ok || !b_ok) exit(1);
    return 0;
}