Features
========

Overview
--------
k_os implements a configuration-driven OSEK/VDX feature set focused on determinism and
predictable timing. Highlights include:

- Tasks: basic and extended tasks with priorities and activation semantics
- Events: SetEvent/WaitEvent/ClearEvent for extended task synchronization
- Resources: GetResource/ReleaseResource implementing Priority Ceiling Protocol (PCP)
- Alarms & Counters: relative and absolute alarms, counters, and scheduling tables
- Hooks: StartupHook, ShutdownHook, ErrorHook, PreTaskHook, PostTaskHook
- Optional OSEK-COM: message objects and dispatching (configurable)

API snippets
------------
Common services available in the OS API include:

- StartOS(AppMode)
- ShutdownOS(Status)
- ActivateTask(TaskID)
- TerminateTask()
- ChainTask(TaskID)
- GetTaskID(), GetTaskState()
- SetEvent(), ClearEvent(), WaitEvent()
- GetResource(), ReleaseResource()
- SetRelAlarm(), SetAbsAlarm(), CancelAlarm(), IncrementCounter()

Conformance classes
-------------------
k_os is designed to cover OSEK conformance classes (BCC1/BCC2/ECC1/ECC2). Which features
are active depends on the generated configuration and compile time feature switches
(see generated configuration headers such as ``Os_Cfg.h``).

Extended status and error handling
----------------------------------
If ``OS_EXTENDED_STATUS`` is enabled, additional parameter checks are performed and
ErrorHook calls may be triggered for API misuse.
