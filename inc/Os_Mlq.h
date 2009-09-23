/*
 * k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2009 by Christoph Schueler <chris@konnex-tools.de>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#if !defined(__OS_MLQ_H)
#define __OS_MLQ_H

#include "Osek.h"

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

void OsMLQ_Init(void);
TaskType OsMLQ_GetHighestPrio(void);
boolean OsMLQ_TasksAreReady(void);
uint16 OsMLQ_GetBitmap(void);
void OsMLQ_AddTaskFirst(TaskType TaskID,PriorityType prio);
void OsMLQ_AddTaskLast(TaskType TaskID,PriorityType prio);
void OsMLQ_RemoveTask(TaskType TaskID);

void OsMLQ_ChangePrio(TaskType TaskID,PriorityType old_prio,PriorityType new_prio);

uint8 OsMLQ_GetLowestBitNumber(uint16 Bitmap);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /*  __OS_MLQ_H */
