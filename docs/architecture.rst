Architecture
============

Project layout
--------------
The repository is organized into several top-level directories. Key areas include:

- ``inc/`` — public OS API headers (OSEK services, types, error codes)
- ``src/`` — OS implementation (scheduler, hooks, error handling, OSEK-COM)
- ``port/`` — hardware and platform-specific ports (cpu12, msp430, stm32, windows, pthreads, rp2040)
- ``kosek/`` — OIL tooling and code generators (Python)
- ``samples/`` — example applications and sample configurations

Important modules and files
---------------------------
- ``inc/Os_Types.h`` — central OSEK/OS types, including ``StatusType`` and error codes
- ``src/Os_Error.c`` — ErrorHook invocation and error handling
- ``src/Com_If.c`` — OSEK-COM interface and dispatcher (if COM is enabled)
- ``port/<arch>/*`` — port-specific interrupt primitives and context-switching glue

Memory mapping
--------------
Optional memory mapping is available via ``MemMap.h`` and controlled by ``KOS_MEMORY_MAPPING``.
Sections may include ``OSEK_OS_CODE``, ``OSEK_COM_CODE`` and ``OSEK_OS_VAR_*`` to place code/data
in target-specific memory regions.

Porting notes
-------------
Each port must provide:
- interrupt enable/disable primitives
- context switching primitives
- timer/counter abstraction
- stack initialization and management

See the ``port/`` directory for existing port implementations:
- Windows: ``port/windows/`` (Win32 + pthreads simulation)
- POSIX: ``port/pthreads/``
- ARM Cortex-M: ``port/stm32/``
- RP2040: ``port/rp2040/``

Compatibility and namespaces
----------------------------
k_os uses AUTOSAR-like base types (``Std_Types.h``) but keeps OS-specific types in ``Os_Types.h``.
Avoid symbol collisions between generic definitions by including the correct headers in each module.
