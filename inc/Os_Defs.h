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
#if !defined(__OSEKDEFS_H)
#define __OSEKDEFS_H

/**********************************************************************************************************
*
*                                              DESCRIPTION
*
* Filename      : osdefs.h
* Version       : V1.00
* Programmer(s) : CS
*********************************************************************************************************
* Note(s)       :
*
*********************************************************************************************************
* Notice(s)     :
*
**********************************************************************************************************/

/**********************************************************************************************************
*                                                 MODULE
**********************************************************************************************************/

/**********************************************************************************************************
*                                            REVISION-HISTORY
**********************************************************************************************************/

/**********************************************************************************************************
*                                             INCLUDE-FILES
**********************************************************************************************************/
#include "Os_Types.h"

/**********************************************************************************************************
*                                                DEFINES
**********************************************************************************************************/
#define PRIO_NONE                       ((PriorityType)0x00)
#define PRIO_SCHEDULER                  ((PriorityType)0xff)

#define RES_SCHEDULER                   ((ResourceType) - 1)

#define INTERNAL_RES_NONE               ((ResourceType)0xff)
#define INTERNAL_RES_SCHEDULER          ((ResourceType)0)

#define INVALID_TASK                    ((TaskType)0)

#define OSDEFAULTAPPMODE                ((AppModeType)0)

/*  System-Flags. */
#define OS_SYS_FLAG_SCHED_OCCUPIED      ((uint8)0x01)
#define OS_SYS_FLAG_ISR_LEVEL           ((uint8)0x02)
#define OS_SYS_FLAG_IN_OS_ERROR_HOOK    ((uint8)0x04)
#define OS_SYS_FLAG_IN_COM_ERROR_HOOK   ((uint8)0x08)
/* Task-Attributes  */
#define OS_TASK_ATTR_FULLPREEMPT        ((uint8)0x01)
#define OS_TASK_ATTR_EXTENDED           ((uint8)0x02)
#define OS_TASK_ATTR_INT_RESOURCE       ((uint8)0x04)
#define OS_TASK_ATTR_SHARED_STACK       ((uint8)0x08)

#define INVALID_OSAPPLICATION           ((ApplicationType) - 1)
#define OSDEFAULTAPPLICATION            ((ApplicationType)0)

#define INVALID_ISR                     ((ISRType) - 1)

/**********************************************************************************************************
*                                                 MACROS
**********************************************************************************************************/
#define GetTaskName(Task)               Func ## Task
#define DeclareTask(Task)               void Func ## Task(void)
#define DeclareAlarmCallback(Alarmcb)   void AlarmCallback ## Alarmcb(void)

#define TASK(TaskName)                  void Func ## TaskName(void)

/*
**
**
**
*/

#define ALARMCALLBACK(Alarmcb)          void AlarmCallback ## Alarmcb(void)
#define GetAlarmCallbackName(Alarmcb)   AlarmCallback ## Alarmcb

#define DeclareResource(Resource)
#define DeclareEvent(Event)
#define DeclareAlarm(Alarm)

/*
**      AUTOSAR-OS.
*/

/*  MACROS. */
#define OSMEMORY_IS_READABLE()
#define OSMEMORY_IS_WRITEABLE()
#define OSMEMORY_IS_EXECUTABLE()
#define OSMEMORY_IS_STACKSPACE()

/**********************************************************************************************************
*                                               ERROR-CODES
**********************************************************************************************************/

/**********************************************************************************************************
*                                               DATA-TYPES
**********************************************************************************************************/

/**********************************************************************************************************
*                                            GLOBAL VARIABLES
**********************************************************************************************************/

/**********************************************************************************************************
*                                                EXTERNS
**********************************************************************************************************/

/**********************************************************************************************************
*                                          FUNCTION PROTOTYPES
**********************************************************************************************************/

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) IdleTimeHook(void);
#else
void IdleTimeHook(void);
#endif /* KOS_MEMORY_MAPPING */
DeclareTask(OsExec_IdleTask);

/**********************************************************************************************************
*                                          CONFIGURATION ERRORS
**********************************************************************************************************/

/**********************************************************************************************************
*                                               MODULE END
**********************************************************************************************************/

#endif  /*  __OSEKDEFS_H  */

