#include "romaos.h"
#include <stdio.h>
#include <signal.h>

TASK(high_prio_task, 0) {
    printf("High priority task running!\n");
    TerminateTask();
}

TASK(low_prio_task, 10) {
    printf("Low priority task started.\n");
    raise(SIGRTMIN+0);
    printf("Low priority task resumed after ISR.\n");
    TerminateTask();
}

ISR(irq0, 0) {
    ENTER_ISR();
    printf("IRQ 0: activating high priority task.\n");
    ISRActivateTask(high_prio_task);
    LEAVE_ISR();
}

int main(void) {
    StartOS(low_prio_task);
    return 0;
}