/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
                                        cpu12.gems@googlemail.com>

   All Rights Reserved

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

   s. FLOSS-EXCEPTION.txt
 */
/*
**
**  Hardware-Dependencies  (Context-Switching, Interrupt-Handling etc.).
**
*/

#if !defined(__OS_PORT_H)
#define __OS_PORT_H

#include "Osek.h"
#include "InstallISR/ISR.h"
#include "Hw_Cfg.h"
#include "Os_Cfg.h"

#if defined(__IAR_SYSTEMS_ICC__)
    #include <intrinsics.h>
#endif  /* __IAR_SYSTEMS_ICC__ */

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

/*
**  Global Types.
*/
typedef uint16 * OsPort_StackPointerType;

/*
**  Global Variables.
*/
extern InterruptStateType OsPort_InterruptState;

/*
**  Global Functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Init(void);
FUNC(uint8 *, OSEK_OS_CODE) OsPort_TaskStackInit(
    P2VAR(TaskFunctionType, AUTOMATIC, OSEK_OS_APPL_DATA) TaskFunc, 
    P2VAR(uint8, AUTOMATIC, OSEK_OS_APPL_DATA) sp
);
FUNC(uint32, OSEK_OS_CODE) OsPort_GetTimestamp(void);
#else
void    OsPort_Init(void);
uint8 * OsPort_TaskStackInit(TaskFunctionType * TaskFunc, uint8 * sp);
uint32  OsPort_GetTimestamp(void);
#endif /* KOS_MEMORY_MAPPING */


#define DISABLE_ALL_OS_INTERRUPTS() CPU_SAVE_AND_DISABLE_INTERRUPTS(OsPort_InterruptState)
#define ENABLE_ALL_OS_INTERRUPTS()  CPU_RESTORE_INTERRUPTS(OsPort_InterruptState)

#if defined(__IAR_SYSTEMS_ICC__)
void    OS_START_CURRENT_TASK(void);
void    OS_SAVE_CONTEXT(void);
void    OS_RESTORE_CONTEXT(void);
void    OS_ISR_CONTEXT(void);


#endif /* __IAR_SYSTEMS_ICC__ */

extern const SizeType OS_TOS_ISR;

/*
**  Port-Macros.
*/

#if defined(__IMAGECRAFT__)
#define OS_SAVE_CONTEXT()             \
    _BEGIN_BLOCK                      \
    asm ("ldy        _OsCurrentTCB"); \
    asm ("sts        0,y");           \
    _END_BLOCK
#elif defined(__HIWARE__)
#define OS_SAVE_CONTEXT()   \
    _BEGIN_BLOCK            \
    __asm ldy OsCurrentTCB; \
    __asm sts       0, y;   \
    _END_BLOCK
#elif defined(__CSMC__)
#define OS_SAVE_CONTEXT()            \
    _BEGIN_BLOCK                     \
    _asm("ldy       _OsCurrentTCB"); \
    _asm("sts       0,y");           \
    _END_BLOCK
#elif defined(__GNUC__)
/* todo: Testen!!! */
#define OS_SAVE_CONTEXT()           \
    _BEGIN_BLOCK                    \
    __asm__("movw   _.tmp,2,-sp");  \
    __asm__("movw   _.z,2,-sp");    \
    __asm__("movw   _.xy,2,-sp");   \
    __asm__("ldy    OsCurrentTCB"); \
    __asm__("sts    0,y");          \
    _END_BLOCK

#endif

#if defined(__IMAGECRAFT__)
#define OS_RESTORE_CONTEXT()          \
    _BEGIN_BLOCK                      \
    asm ("ldy        _OsCurrentTCB"); \
    asm ("lds        0,y");           \
    _END_BLOCK
#elif defined(__HIWARE__)
#define OS_RESTORE_CONTEXT() \
    _BEGIN_BLOCK             \
    __asm ldy OsCurrentTCB;  \
    __asm lds       0, y;    \
    _END_BLOCK
#elif defined(__CSMC__)
#define OS_RESTORE_CONTEXT()         \
    _BEGIN_BLOCK                     \
    _asm("xref _OsCurrentTCB");      \
    _asm("ldy       _OsCurrentTCB"); \
    _asm("lds       0,y");           \
    _END_BLOCK
#elif defined(__GNUC__)
/* todo: Testen!!! */
#define OS_RESTORE_CONTEXT()        \
    _BEGIN_BLOCK                    \
    __asm__("ldy    OsCurrentTCB"); \
    __asm__("movw   2,sp+,_.xy");   \
    __asm__("movw   2,sp+,_.z");    \
    __asm__("movw   2,sp+,_.tmp");  \
    __asm__("lds    0,y");          \
    _END_BLOCK

#endif

#if defined(__IMAGECRAFT__)
#define OS_START_CURRENT_TASK()       \
    _BEGIN_BLOCK                      \
    asm ("ldy        _OsCurrentTCB"); \
    asm ("lds        0,y");           \
    CPU_RETURN_FROM_INTERRUPT();      \
    _END_BLOCK
#elif defined(__HIWARE__)
#define OS_START_CURRENT_TASK()  \
    _BEGIN_BLOCK                 \
    __asm ldy OsCurrentTCB;      \
    __asm lds       0, y;        \
    CPU_RETURN_FROM_INTERRUPT(); \
    _END_BLOCK
#elif defined(__CSMC__)
#define OS_START_CURRENT_TASK()      \
    _BEGIN_BLOCK                     \
    _asm("xref _OsCurrentTCB");      \
    _asm("ldy       _OsCurrentTCB"); \
    _asm("lds       0,y");           \
    CPU_RETURN_FROM_INTERRUPT();     \
    _END_BLOCK

#elif defined(__GNUC__)
#define OS_START_CURRENT_TASK()     \
    _BEGIN_BLOCK                    \
    __asm__("ldy    OsCurrentTCB"); \
    __asm__("lds    0,y");          \
    CPU_RETURN_FROM_INTERRUPT();    \
    _END_BLOCK

#endif

#if defined(__IMAGECRAFT__)
#define OS_ISR_CONTEXT()    asm ("lds    OS_TOS_ISR")
#elif defined(__HIWARE__)
#define OS_ISR_CONTEXT()    __asm lds OS_TOS_ISR
#elif defined(__CSMC__)
#define OS_ISR_CONTEXT()    _asm("xref _OS_TOS_ISR\nlds   _OS_TOS_ISR")
#elif defined(__GNUC__)
#define OS_ISR_CONTEXT()    __asm__("lds    OS_TOS_ISR")
#endif

void OsPortInit(void);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __OS_PORT_H  */

