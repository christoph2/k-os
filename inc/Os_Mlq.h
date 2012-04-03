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
#if !defined(__OS_MLQ_H)
#define __OS_MLQ_H

#include "Osek.h"

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsMLQ_Init(void);
FUNC(TaskType, OSEK_OS_CODE) OsMLQ_GetHighestPrio(void);
FUNC(boolean, OSEK_OS_CODE) OsMLQ_TasksAreReady(void);
FUNC(uint16, OSEK_OS_CODE) OsMLQ_GetBitmap(void);
FUNC(void, OSEK_OS_CODE) OsMLQ_AddTaskFirst(TaskType TaskID, PriorityType prio);
FUNC(void, OSEK_OS_CODE) OsMLQ_AddTaskLast(TaskType TaskID, PriorityType prio);
FUNC(void, OSEK_OS_CODE) OsMLQ_RemoveTask(TaskType TaskID);
FUNC(void, OSEK_OS_CODE) OsMLQ_ChangePrio(TaskType TaskID, PriorityType old_prio, PriorityType new_prio);
FUNC(uint8, OSEK_OS_CODE) OsMLQ_GetLowestBitNumber(uint16 Bitmap);
#else
void        OsMLQ_Init(void);
TaskType    OsMLQ_GetHighestPrio(void);
boolean     OsMLQ_TasksAreReady(void);
uint16      OsMLQ_GetBitmap(void);
void        OsMLQ_AddTaskFirst(TaskType TaskID, PriorityType prio);
void        OsMLQ_AddTaskLast(TaskType TaskID, PriorityType prio);
void        OsMLQ_RemoveTask(TaskType TaskID);
void        OsMLQ_ChangePrio(TaskType TaskID, PriorityType old_prio, PriorityType new_prio);
uint8       OsMLQ_GetLowestBitNumber(uint16 Bitmap);


#endif /* KOS_MEMORY_MAPPING */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /*  __OS_MLQ_H */
