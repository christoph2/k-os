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
#if !defined(__OS_ALM_H)
#define __OS_ALM_H

#include "Os_Types.h"


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsAlm_InitAlarms(void);
FUNC(void, OSEK_OS_CODE) OsAlm_NotifyAlarm(AlarmType AlarmID);
FUNC(void, OSEK_OS_CODE) OsAlm_StartAlarm(uint8 num);
FUNC(void, OSEK_OS_CODE) OsAlm_StopAlarm(uint8 num);
FUNC(boolean, OSEK_OS_CODE) OsAlm_IsRunning(uint8 num);
#else
void    OsAlm_InitAlarms(void);
void    OsAlm_NotifyAlarm(AlarmType AlarmID);
void    OsAlm_StartAlarm(uint8 num);
void    OsAlm_StopAlarm(uint8 num);
boolean OsAlm_IsRunning(uint8 num);
#endif /* KOS_MEMORY_MAPPING */

/*Os_AlarmStateType*/ uint16 OsAlm_GetActiveAlarms(void);


#endif /* __OS_ALM_H */

