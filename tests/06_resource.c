#include "romaos.h"
#include <stdio.h>
#include <stdlib.h>   // для exit

RESOURCE(res, 3);

int test_success = 0;

// Объявляем high_task до low_task, чтобы low_task могла её активировать
TASK(high_task, 5) {
    printf("High tries to get resource\n");
    GetResource(res);
    printf("High got resource, releasing\n");
    ReleaseResource(res);
    printf("High finished\n");
    if (test_success == 1) {
        ShutdownOS();          // всё прошло успешно
    } else {
        exit(1);               // что-то пошло не так
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

// Диспетчер, который запускает low_task и завершается
TASK(dispatcher, 0) {
    ActivateTask(low_task);
    TerminateTask();
}

int main(void) {
    StartOS(dispatcher);
    return 0;
}