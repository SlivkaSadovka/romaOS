#include "romaos.h"
#include <stdio.h>
#include <stdlib.h>

RESOURCE(res, 3);

int test_success = 0;

TASK(high_task, 5) {
    printf("High tries to get resource\n");
    GetResource(res);
    printf("High got resource, releasing\n");
    ReleaseResource(res);
    printf("High finished\n");
    if (test_success == 1) {
        ShutdownOS();
    } else {
        exit(1);
    }
}

TASK(low_task, 10) {
    printf("Low getting resource\n");
    GetResource(res);
    printf("Low got resource. Activating High via ISR.\n");
    raise(SIGRTMIN+0);
    printf("Low releasing resource\n");
    ReleaseResource(res);
    test_success = 1;
    printf("Low finished\n");
    TerminateTask();
}

ISR(irq0, 0) {
    ENTER_ISR();
    printf("ISR: activating High\n");
    ISRActivateTask(high_task);
    LEAVE_ISR();
}

TASK(dispatcher, 0) {
    ActivateTask(low_task);
    TerminateTask();
}

int main(void) {
    StartOS(dispatcher);
    return 0;
}