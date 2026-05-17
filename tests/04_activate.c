#include "romaos.h"
#include <stdio.h>
#include <stdlib.h>   // для exit

int a_ok = 0, b_ok = 0;

// ---------- рабочие задачи ----------
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

// ---------- диспетчер ----------
TASK(dispatcher, 0) {
    printf("Dispatcher: activating taskA and taskB.\n");
    ActivateTask(taskA);
    ActivateTask(taskB);
    printf("Dispatcher done.\n");
    TerminateTask();
}

// ---------- проверка ----------
int main(void) {
    StartOS(dispatcher);
    // если всё прошло правильно, обе задачи выполнились
    if (!a_ok || !b_ok) exit(1);
    return 0;
}