#ifndef PORT_H
#define PORT_H

#include <signal.h>
#include <ucontext.h>

typedef void (*isr_t)(int sig, siginfo_t *info, void *ucontext);

extern int current_irq;
extern ucontext_t *current_ucontext;

int port_irq_register(int irq, isr_t handler);
void port_enter_isr(int irq, ucontext_t *ctx);
void port_leave_isr(void);
void port_init(void);

#endif