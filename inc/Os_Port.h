/*
** k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
**
** (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
**                                      cpu12.gems@googlemail.com>
**
** All Rights Reserved
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; version 2 of the License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
** s. FLOSS-EXCEPTION.txt
**/
/** @file Os_Port.h
 *  @brief Interface function to the actual microcontroller port.
 *
 *
 *
 *  @author Christoph Schueler (cpu12.gems@googlemail.com)
 */
#if !defined(__OS_PORT_H)
#define __OS_PORT_H

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

//#include "kdk/installisr/ISR.h"
#include "Os_Cfg.h"
#include "Os_Macros.h"
#include "CPU_Primitives.h"

/*
**  Global Types.
*/
typedef uint16 * OsPort_StackPointerType;

/*
**  Global Variables.
*/
#if defined(_MSC_VER)
typedef uint8 InterruptStateType;
#endif

extern InterruptStateType OsPort_InterruptState;

/*
**  Global Functions.
*/
void OsPort_SuspendHook(uint16 taskID);

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsPort_Init(void);
FUNC(void, OSEK_OS_CODE) OsPort_Shutdown(void);
FUNC(uint8 *, OSEK_OS_CODE) OsPort_TaskStackInit(
    P2VAR(TaskFunctionType, AUTOMATIC, OSEK_OS_APPL_DATA) TaskFunc,
    P2VAR(uint8, AUTOMATIC, OSEK_OS_APPL_DATA) sp
    );
FUNC(void, OSEK_OS_CODE) OsPort_StartCurrentTask(void);
FUNC(uint32, OSEK_OS_CODE) OsPort_GetTimestamp(void);
FUNC(void, OSEK_OS_CODE) OsPort_KeyboardLoop(void);
FUNC(void, OSEK_OS_CODE) OsPort_SaveContext(void);
FUNC(void, OSEK_OS_CODE) OsPort_RestoreContext(void);
FUNC(void, OSEK_OS_CODE) OsPort_SwitchToISRContext(void)
FUNC(void, OSEK_OS_CODE) OsPort_CreateEvents(void);
FUNC(void, OSEK_OS_CODE) OsPort_SignalSchedulingEvent(void);
FUNC(void, OSEK_OS_CODE) OsPort_AdjustProcessPriorityClass(void);
#else
void    OsPort_Init(void);
void    OsPort_Shutdown(void);
uint8 * OsPort_TaskStackInit(TaskType TaskID, TaskFunctionType const * TaskFunc, uint8 * sp);
void OsPort_StartCurrentTask(void);
uint32  OsPort_GetTimestamp(void);
void OsPort_KeyboardLoop(void);
void OsPort_SaveContext(void);
void OsPort_RestoreContext(void);
void OsPort_SwitchToISRContext(void);
void OsPort_CreateEvents(void);
void OsPort_SignalSchedulingEvent(void);
void OsPort_AdjustProcessPriorityClass(void);
#endif /* KOS_MEMORY_MAPPING */

#define OsPort_DisableAllOsInterrupts() CPU_SAVE_AND_DISABLE_INTERRUPTS(OsPort_InterruptState)
#define OsPort_EnableAllOsInterrupts()  CPU_RESTORE_INTERRUPTS(OsPort_InterruptState)

extern const SizeType OS_TOS_ISR;

#if defined(__CSMC__)               /* Cosmic               */
    #include "port/cpu12/cosmic/Os_Port_S12_Cosmic.h"
#elif defined(__GNUC__)             /* GNU GCC              */

    #if defined(__arm__)
        #include "port/arm/gcc/Os_Port_arm_gcc.h"
    #elif defined(__AVR__)
        #include "port/ATMEGA644/gcc/Os_Port_avr_gcc.h"
    #elif defined(MC6812)
        #include "port/cpu12/gcc/Os_Port_S12_gcc.h"
    #elif defined(__MSP430__)
        #include "port/msp430/gcc/Os_Port_msp430_gcc.h"
    #elif defined( __CYGWIN32__) || defined(__CYGWIN__) /* && defined(__I386__) */
        #include "port/i386/gcc/Os_Port_i386_gcc.h"
    #else
        #error Unsupported Target for GCC.
    #endif

#elif defined(__HIWARE__)           /* Metrowerks/Freescale */

    #if defined(__HC12__)
        #include "port/cpu12/codewarrior/Os_Port_S12_hiware.h"
    #else
        #error Unsupported Target for Metrowerks.
    #endif

#elif defined(__IAR_SYSTEMS_ICC__)  /* IAR Systems          */

    #if defined(__ICCARM__)
        #include "port/arm/iar/Os_Port_arm_IAR.h"
    #elif defined(__ICCHCS12__)
        #include "port/cpu12/iar/Os_Port_S12_IAR.h"
    #elif defined(__ICC430__)
        #include "port/msp430/iar/Os_Port_MSP430_IAR.h"
    #else
        #error Unsupported Target for IAR-ICC.
    #endif

#elif defined(__IMAGECRAFT__)       /* Imagecraft           */

    #include "port/cpu12/imagecraft/Os_Port_S12_icc.h"

#elif defined(__18CXX )

    #include "port/pic/mpl_c18/Os_Port_pic_mplc18.h"

#elif defined(__PCH__) || defined(__PCB__) || defined(__PCM__)

    #include "port/pic/ccsc/Os_Port_pic_pch.h"
#elif defined(_MSC_VER)

/* Microsoft Visual C. */

    #include "port/i386/msvc/Os_Port_i386_mscv.h"

#else                               /* todo: Add Support    */
    #error Unsupported Compiler.
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __OS_PORT_H  */

