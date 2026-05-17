#include "../include/romaos.h"
#include <stdio.h>

TASK(hello, 0) {
    printf("Hello, RomaOS!\n");
    TerminateTask();
}

int main(void) {
    StartOS(hello);
    return 0;
}