/*
** Minimal RP2040 sample harness for k_os.
*/

#include "pico/stdlib.h"

#include "Os.h"
#include "Os_Cfg.h"
#include "Com.h"

DeclareTask(TaskA);
DeclareTask(TaskB);
DeclareTask(Loop);
DeclareTask(InitTask);
DeclareTask(Cyclic);
ISR2(TC2Timer);

TASK(TaskA)
{
    (void)TerminateTask();
}

TASK(TaskB)
{
    (void)TerminateTask();
}

TASK(Loop)
{
    (void)ChainTask(TaskB);
}

TASK(InitTask)
{
    (void)ActivateTask(Cyclic);
    (void)TerminateTask();
}

TASK(Cyclic)
{
    (void)TerminateTask();
}

ISR2(TC2Timer)
{
    (void)ActivateTask(Cyclic);
}

void StartupHook(void)
{
}

void ShutdownHook(StatusType error)
{
    (void)error;
}

void ErrorHook(StatusType error)
{
    (void)error;
}

int main(void)
{
    stdio_init_all();
    StartOS(DefaultAppMode);
    for (;;) {
        __asm volatile("wfi");
    }
}
