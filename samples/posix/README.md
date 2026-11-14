# POSIX port status

The Linux pthread port provides a working OSEK runtime on POSIX systems.

## Implemented features

- **Task context switching**: Each OSEK task runs in its own pthread, blocked
  on a semaphore until the scheduler dispatches it. The scheduler loop in
  `OsPort_StartCurrentTask()` selects the highest-priority ready task and posts
  its semaphore. Non-preemptive tasks (SCHEDULE=NON) run to completion before
  the next task is dispatched; preemptive tasks (SCHEDULE=FULL) run concurrently.

- **Timer-driven counter updates**: A POSIX `timer_create`/`timer_settime` timer
  fires SIGALRM at 1 ms intervals. The signal handler is async-signal-safe: it
  sets a flag and posts a semaphore to a dedicated timer-dispatch thread, which
  then calls `IncrementCounter()` to advance counters, update alarms, and
  activate tasks.

- **Preemption**: The scheduler respects the SCHEDULE attribute of each task.
  Non-preemptive tasks block the scheduler until they terminate; preemptive
  tasks run concurrently in separate threads.

- **Operating-system shutdown**: When no tasks are ready and no alarms are
  active, the scheduler sets a shutdown flag, joins all task and timer threads,
  cleans up the POSIX timer, and returns from `StartOS()`.

## Architecture

- `OsPort_Init()` — creates per-task semaphores and the timer-dispatch semaphore.
- `OsPort_StartCurrentTask()` — creates pthreads for all tasks and the timer
  dispatch thread, then enters the main scheduling loop.
- `OsPort_SignalHandler()` — async-signal-safe; wakes the timer-dispatch thread.
- `OsPort_TimerDispatchThread()` — calls `IncrementCounter()` for each tick.
- `OsPort_TaskThreadEntry()` — blocks on a semaphore, runs the task function,
  then loops back to wait for the next dispatch.
- `OsPort_Shutdown()` — signals all threads to exit, joins them, and cleans up.

## Limitations

- The port uses `usleep()` in the idle loop instead of a proper wait-on-timer
  primitive, which adds small latency.
- Resource ceiling priority and internal resources are supported by the kernel
  but not enforced by the pthread scheduler.
- The port is not suitable for hard real-time use; it relies on the OS scheduler
  for thread preemption.
