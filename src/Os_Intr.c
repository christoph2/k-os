/*
 * k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2013 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * s. FLOSS-EXCEPTION.txt
 */

/*
**
**  For now the *OS* and *All* Functions are synonyms.
**
*/

#include "Osek.h"
#include "Os_Intr.h"

/*
**  Local function prototypes.
*/
#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) SuspendCPUInterrrupts(void), ResumeCPUInterrrupts(void);


#else
static void SuspendCPUInterrrupts(void), ResumeCPUInterrrupts(void);


#endif /* KOS_MEMORY_MAPPING */

/*
**  Local variables.
*/
/*
**  !REQ!AS!OS299!
**  ... (it is assumed the static variables of this functions are initialized).
*/
static volatile uint8   InterruptDisableCounter = (uint8)0;
static boolean          IStateSave;

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Global functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsIntr_InitInterrupts(void)
#else
void OsIntr_InitInterrupts(void)
#endif /* KOS_MEMORY_MAPPING */
{
    InterruptDisableCounter = (uint8)0;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) ResumeAllInterrupts(void)
#else
void ResumeAllInterrupts(void)
#endif /* KOS_MEMORY_MAPPING */
{
    ResumeCPUInterrrupts();
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) SuspendAllInterrupts(void)
#else
void SuspendAllInterrupts(void)
#endif /* KOS_MEMORY_MAPPING */
{
    SuspendCPUInterrrupts();
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) ResumeOSInterrupts(void)
#else
void ResumeOSInterrupts(void)
#endif /* KOS_MEMORY_MAPPING */
{
    ResumeCPUInterrrupts();
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) SuspendOSInterrupts(void)
#else
void SuspendOSInterrupts(void)
#endif /* KOS_MEMORY_MAPPING */
{
    SuspendCPUInterrrupts();
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) SuspendCPUInterrrupts(void)
#else
void SuspendCPUInterrrupts(void)
#endif /* KOS_MEMORY_MAPPING */
{
    const uint8 istate = CPU_INTERRUPTS_DISABLED();

    CPU_DISABLE_ALL_INTERRUPTS();

    if ((InterruptDisableCounter++) == (uint8)0) {
        IStateSave = istate;
    }
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) ResumeCPUInterrrupts(void)
#else
void ResumeCPUInterrrupts(void) /* s. Autosar O92 !!!*/
#endif /* KOS_MEMORY_MAPPING */
{
    const uint8 istate = CPU_INTERRUPTS_DISABLED();

    ASSERT((istate == TRUE) && (InterruptDisableCounter != (uint8)0));

    CPU_DISABLE_ALL_INTERRUPTS();

    if ((--InterruptDisableCounter == (uint8)0) && (IStateSave == FALSE)) {
        CPU_ENABLE_ALL_INTERRUPTS();
    }
}


#if (OS_FEATURE_ORTI_DEBUG == STD_ON)
#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(ISRType, OSEK_OS_CODE) GetISRID(void)
#else
ISRType GetISRID(void)
#endif /* KOS_MEMORY_MAPPING */
{
    return OsCurrentISRID;
}


#endif

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
