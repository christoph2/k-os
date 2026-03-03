/*
** k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
**
** (C) 2007-2026 by Christoph Schueler <github.com/Christoph2,
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

#ifndef OS_PORT_RP2040_H
#define OS_PORT_RP2040_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* RP2040 specific includes (usually from pico-sdk) */
/* We assume the environment provides the necessary headers if used with pico-sdk */
/* For standalone, we might need to define some registers */

#ifndef OSPORT_SYSTICK_FREQUENCY_HZ
#define OSPORT_SYSTICK_FREQUENCY_HZ 1000U
#endif
#ifndef OSPORT_SYSCLK_HZ
#define OSPORT_SYSCLK_HZ 125000000UL
#endif

#define OSPORT_PENDSV_PRIORITY 0x3 /* RP2040 has 2 bits for priority: 0, 1, 2, 3 */
#define OSPORT_SYSTICK_PRIORITY 0x0
#if defined(__ARM_ARCH_8M_MAIN__) || defined(PICO_RP2350)
#define OSPORT_HAS_BASEPRI 1
#else
#define OSPORT_HAS_BASEPRI 0
#endif

#define OSPORT_INITIAL_XPSR 0x01000000U

extern volatile uint32_t OsPort_SystemTickCounter;

void OsPort_InitSysTick(void);
void OsPort_InitPendSV(void);
void OsPort_TriggerContextSwitch(void);
uint32_t OsPort_GetSystemTick(void);
void OsPort_DelayMs(uint32_t delay);

void SysTick_Handler(void);
void PendSV_Handler(void);
void HardFault_Handler(void);

#define OSPORT_STACK_FILL_PATTERN 0xDEADBEEF
#define OSPORT_STACK_ALIGNMENT 8U

#ifdef __cplusplus
}
#endif

#endif /* OS_PORT_RP2040_H */
