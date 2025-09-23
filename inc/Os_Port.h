/*
** k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
**
** (C) 2007-2025 by Christoph Schueler <github.com/Christoph2,
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
typedef int * OsPort_StackPointerType;

/*
**  Global Variables.
*/
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
// typedef uint8 InterruptStateType;
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
    P2VAR(Os_TaskFunctionType, AUTOMATIC, OSEK_OS_APPL_DATA) TaskFunc,
    P2VAR(uint8, AUTOMATIC, OSEK_OS_APPL_DATA) sp
    );
FUNC(void, OSEK_OS_CODE) OsPort_StartCurrentTask(void);
FUNC(uint32, OSEK_OS_CODE) OsPort_GetTimestamp(void);
FUNC(void, OSEK_OS_CODE) OsPort_KeyboardLoop(void);
FUNC(void, OSEK_OS_CODE) OsPort_SaveContext(void);
FUNC(void, OSEK_OS_CODE) OsPort_RestoreContext(void);
FUNC(void, OSEK_OS_CODE) OsPort_SwitchToISRContext(void);
FUNC(void, OSEK_OS_CODE) OsPort_CreateEvents(void);
FUNC(void, OSEK_OS_CODE) OsPort_SignalSchedulingEvent(void);
FUNC(void, OSEK_OS_CODE) OsPort_AdjustProcessPriorityClass(void);
#else
void    OsPort_Init(void);
void    OsPort_Shutdown(void);
uint8 * OsPort_TaskStackInit(TaskType TaskID, Os_TaskFunctionType const * TaskFunc, uint8 * sp);
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

#if 0
#define OsPort_DisableAllOsInterrupts() CPU_SAVE_AND_DISABLE_INTERRUPTS(OsPort_InterruptState)
#define OsPort_EnableAllOsInterrupts()  CPU_RESTORE_INTERRUPTS(OsPort_InterruptState)
#endif

extern const SizeType OS_TOS_ISR;

/*
**  Compiler and Platform Detection for Port Selection
*/

#if defined(__CSMC__)               /* Cosmic Compiler */
    #include "port/cpu12/cosmic/Os_Port_S12_Cosmic.h"

#elif defined(__GNUC__)             /* GNU GCC Compiler */

    /*
    **  Windows with MinGW/MSYS2
    */
    #if defined(__MINGW32__) || defined(__MINGW64__) || (defined(_WIN32) && defined(__GNUC__))
        #include "port/windows/Os_Port_Win32.h"

    /*
    **  Unix/Linux Platforms
    */
    #elif defined(__unix__) || defined(__unix) || defined(unix) || defined(__linux__)
        #include "port/pthreads/Os_Port_Unix.h"

    /*
    **  ARM Embedded Platforms
    */
    #elif defined(__arm__)
        #if defined(STM32F1xx) || defined(STM32F4xx) || defined(STM32F7xx) || \
            defined(STM32H7xx) || defined(STM32L4xx) || defined(STM32G4xx) || \
            defined(STM32F0xx) || defined(STM32F3xx) || defined(STM32L0xx) || \
            defined(STM32L1xx) || defined(STM32WBxx) || defined(STM32MPxx) || \
			defined(STM32L4R5xx)
            #include "port/stm32/Os_Port_STM32.h"
        #else
            #include "port/arm/gcc/Os_Port_arm_gcc.h"
        #endif

    /*
    **  AVR Microcontrollers
    */
    #elif defined(__AVR__)
        #include "port/ATMEGA644/gcc/Os_Port_avr_gcc.h"

    /*
    **  HC12/S12 Microcontrollers
    */
    #elif defined(MC6812) || defined(__HC12__)
        #include "port/cpu12/gcc/Os_Port_S12_gcc.h"

    /*
    **  MSP430 Microcontrollers
    */
    #elif defined(__MSP430__)
        #include "port/msp430/gcc/Os_Port_msp430_gcc.h"

    /*
    **  Coldfire Microcontrollers
    */
    #elif defined(__m68k__) && defined(__mcf5200__)
        #include "port/coldfire/gcc/Os_Port_coldfire_gcc.h"

    /*
    **  Cygwin Environment
    */
    #elif defined(__CYGWIN32__) || defined(__CYGWIN__)
        #include "port/i386/gcc/Os_Port_i386_gcc.h"

    /*
    **  RISC-V Architecture
    */
    #elif defined(__riscv) || defined(__riscv__)
        #include "port/riscv/gcc/Os_Port_riscv_gcc.h"

    /*
    **  Generic x86/x64 with GCC (fallback)
    */
    #elif defined(__i386__) || defined(__i386) || defined(i386) || \
          defined(__x86_64__) || defined(__x86_64) || defined(__amd64__) || defined(__amd64)
        #include "port/i386/gcc/Os_Port_i386_gcc.h"

    #else
        #error Unsupported Target for GCC. Please define your platform or add support.
    #endif

/*
**  LLVM Clang Compiler
*/
#elif defined(__clang__)

    /*
    **  Windows with Clang
    */
    #if defined(_WIN32) || defined(_WIN64)
        #include "port/windows/clang/Os_Port_Win32_clang.h"

    /*
    **  Unix/Linux with Clang
    */
    #elif defined(__unix__) || defined(__linux__)
        #include "port/unix/clang/Os_Port_unix_clang.h"

    /*
    **  ARM with Clang
    */
    #elif defined(__arm__)
        #include "port/arm/clang/Os_Port_arm_clang.h"

    #else
        #error Unsupported Target for Clang Compiler.
    #endif

/*
**  Microsoft Visual C++
*/
#elif defined(_MSC_VER)
    #include "port/windows/msvc/Os_Port_Win32_msvc.h"

/*
**  Intel C++ Compiler
*/
#elif defined(__INTEL_COMPILER) || defined(__ICC)
    #if defined(_WIN32) || defined(_WIN64)
        #include "port/windows/icc/Os_Port_Win32_icc.h"
    #elif defined(__linux__)
        #include "port/unix/icc/Os_Port_unix_icc.h"
    #else
        #error Unsupported Platform for Intel C++ Compiler.
    #endif

/*
**  Metrowerks/Freescale CodeWarrior
*/
#elif defined(__HIWARE__)
    #if defined(__HC12__)
        #include "port/cpu12/codewarrior/Os_Port_S12_hiware.h"
    #else
        #error Unsupported Target for Metrowerks Compiler.
    #endif

/*
**  IAR Systems Compiler
*/
#elif defined(__IAR_SYSTEMS_ICC__)
    #if defined(__ICCARM__)
        #include "port/arm/iar/Os_Port_arm_IAR.h"
    #elif defined(__ICCHCS12__)
        #include "port/cpu12/iar/Os_Port_S12_IAR.h"
    #elif defined(__ICC430__)
        #include "port/msp430/iar/Os_Port_MSP430_IAR.h"
    #else
        #error Unsupported Target for IAR-ICC.
    #endif

/*
**  Imagecraft Compiler
*/
#elif defined(__IMAGECRAFT__)
    #include "port/cpu12/imagecraft/Os_Port_S12_icc.h"

/*
**  MPLAB C18 Compiler (PIC18)
*/
#elif defined(__18CXX)
    #include "port/pic/mpl_c18/Os_Port_pic_mplc18.h"

/*
**  CCS C Compiler (PIC)
*/
#elif defined(__PCH__) || defined(__PCB__) || defined(__PCM__)
    #include "port/pic/ccsc/Os_Port_pic_pch.h"

/*
**  Keil C Compiler
*/
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
    #include "port/arm/keil/Os_Port_arm_keil.h"

/*
**  Texas Instruments Compiler
*/
#elif defined(__TI_COMPILER_VERSION__)
    #if defined(__MSP430__)
        #include "port/msp430/ti/Os_Port_MSP430_ti.h"
    #elif defined(__ARM_ARCH)
        #include "port/arm/ti/Os_Port_arm_ti.h"
    #else
        #error Unsupported Target for TI Compiler.
    #endif

/*
**  Green Hills Compiler
*/
#elif defined(__ghs__)
    #include "port/ghs/Os_Port_ghs.h"

/*
**  Unknown Compiler
*/
#else
    #error Unsupported Compiler. Please add support for your compiler in Os_Port.h.
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __OS_PORT_H  */