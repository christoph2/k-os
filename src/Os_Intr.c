/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

 * (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>

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
**  For now the *OS* and *All* Functions are synonyms.
**
*/

#include "Osek.h"


/*
**  Local function prototypes.
*/
static void SuspendCPUInterrrupts(void), ResumeCPUInterrrupts(void);



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
void OsIntr_InitInterrupts(void)
{
    InterruptDisableCounter = (uint8)0;
}


void ResumeAllInterrupts(void)
{
    ResumeCPUInterrrupts();
}


void SuspendAllInterrupts(void)
{
    SuspendCPUInterrrupts();
}


void ResumeOSInterrupts(void)
{
    ResumeCPUInterrrupts();
}


void SuspendOSInterrupts(void)
{
    SuspendCPUInterrrupts();
}


void SuspendCPUInterrrupts(void)
{
    uint8 istate = CPU_INTERRUPTS_DISABLED();

    CPU_DISABLE_ALL_INTERRUPTS();

    if ((InterruptDisableCounter++) == (uint8)0) {
        IStateSave = istate;
    }
}


void ResumeCPUInterrrupts(void) /* s. Autosar O92 !!!*/
{
    uint8 istate = CPU_INTERRUPTS_DISABLED();

    ASSERT((istate == TRUE) && (InterruptDisableCounter != (uint8)0));

    CPU_DISABLE_ALL_INTERRUPTS();

    if ((--InterruptDisableCounter == (uint8)0) && (IStateSave == FALSE)) {
        CPU_ENABLE_ALL_INTERRUPTS();
    }
}


#if defined(OS_FEATURE_ORTI_DEBUG)
ISRType GetISRID(void)
{
    return OsCurrentISRID;
}
#endif

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
