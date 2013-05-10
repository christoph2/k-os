/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

 * (C) 2007-2013 by Christoph Schueler <github.com/Christoph2,
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
#include "Os_Mlq.h"
#include "Utl.h"

#if !defined(OS_SCHED_BM_ONLY)
#if KOS_MEMORY_MAPPING == STD_ON
STATIC  FUNC(void, OSEK_OS_CODE)     OsMLQ_InitQueue(uint8 num);
STATIC  FUNC(void, OSEK_OS_CODE)     OsMLQ_PushFront(uint8 num, TaskType TaskID);
STATIC  FUNC(void, OSEK_OS_CODE)     OsMLQ_PushBack(uint8 num, TaskType TaskID);
STATIC  FUNC(void, OSEK_OS_CODE)     OsMLQ_PopFront(uint8 num);


/*  STATIC FUNC(void, OSEK_OS_CODE) OsMLQ_PopBack(uint8 num);  */
static FUNC(boolean, OSEK_OS_CODE)  OsMLQ_IsEmpty(uint8 num);
/* STATIC  FUNC(boolean, OSEK_OS_CODE) OsMLQ_IsFull(uint8 num); */
STATIC FUNC(TaskType, OSEK_OS_CODE) OsMLQ_Front(uint8 num);


#else
static void OsMLQ_InitQueue(uint8 num);
static void OsMLQ_PushFront(uint8 num, TaskType TaskID);
static void OsMLQ_PushBack(uint8 num, TaskType TaskID);
static void OsMLQ_PopFront(uint8 num);


/*  static void OsMLQ_PopBack(uint8 num);  */
static boolean OsMLQ_IsEmpty(uint8 num);


/* static boolean OsMLQ_IsFull(uint8 num); */
static TaskType OsMLQ_Front(uint8 num);


#endif /* KOS_MEMORY_MAPPING */

/* static TaskType OsMLQ_Back(uint8 num); */
#endif /* !OS_SCHED_BM_ONLY*/

#if defined(OS_USE_RESOURCES)
#define PRIORITY_FOR_TASK(TaskID)   OS_TCB[(TaskID)].CurrentPriority
#else
#define PRIORITY_FOR_TASK(TaskID)   OS_TaskConf[(TaskID)].Priority
#endif

#if defined(OS_SCHED_BM_ONLY)

#endif

static OsMLQ_QueueType  MLQ_ReadyQueue[OS_MLQ_NUMBER_OF_PRIORITIES];
static uint16           BM_NotEmpty; /* Bitmap for non-empty queues.  */

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**
**  Global Functions.
**
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsMLQ_Init(void)
#else
void OsMLQ_Init(void)
#endif /* KOS_MEMORY_MAPPING */
{
#if !defined(OS_SCHED_BM_ONLY)
    uint8 idx;

    BM_NotEmpty = (uint16)0x0000U;

    for (idx = (uint8)0; idx < OS_MLQ_NUMBER_OF_PRIORITIES; ++idx) {
        OsMLQ_InitQueue(idx);
    }

#else
    BM_NotEmpty = (uint16)0x0000U;
#endif
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(TaskType, OSEK_OS_CODE) OsMLQ_GetHighestPrio(void)
#else
TaskType OsMLQ_GetHighestPrio(void)
#endif /* KOS_MEMORY_MAPPING */
{
#if !defined(OS_SCHED_BM_ONLY)
    uint8 queue_num;
#endif
    TaskType TaskID = INVALID_TASK;

    if (BM_NotEmpty != (uint16)0x0000U) {
#if defined(OS_SCHED_BM_ONLY)
        TaskID = OsMLQ_GetLowestBitNumber(((uint16) ~BM_NotEmpty + (uint16)1) & BM_NotEmpty);
#else
        queue_num  = OsMLQ_GetLowestBitNumber(((uint16) ~BM_NotEmpty + (uint16)1) & BM_NotEmpty) - ((uint8)1);
        TaskID     = OsMLQ_Front(queue_num);
#endif
    }

    return TaskID;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(boolean, OSEK_OS_CODE) OsMLQ_TasksAreReady(void)
#else
boolean OsMLQ_TasksAreReady(void)
#endif /* KOS_MEMORY_MAPPING */
{
    return (boolean)(BM_NotEmpty != (uint16)0x0000U);
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(uint16, OSEK_OS_CODE) OsMLQ_GetBitmap(void)
#else
uint16 OsMLQ_GetBitmap(void)
#endif /* KOS_MEMORY_MAPPING */
{
    return BM_NotEmpty;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsMLQ_AddTaskFirst(TaskType TaskID, PriorityType prio)
#else
void OsMLQ_AddTaskFirst(TaskType TaskID, PriorityType prio)    /* stack. */
#endif /* KOS_MEMORY_MAPPING */
{
    ASSERT(((prio != PRIO_NONE) && (INVERT_NIBBLE(prio)) <= OS_MLQ_NUMBER_OF_PRIORITIES));

    /* BM_NotEmpty=Utl_BitSet(BM_NotEmpty,(uint8)prio); */
    UTL_BIT_SET16(BM_NotEmpty, (uint8)prio);

#if !defined(OS_SCHED_BM_ONLY)
    OsMLQ_PushFront(INVERT_NIBBLE(prio), TaskID);
#endif
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsMLQ_AddTaskLast(TaskType TaskID, PriorityType prio)
#else
void OsMLQ_AddTaskLast(TaskType TaskID, PriorityType prio)     /* queue. */
#endif /* KOS_MEMORY_MAPPING */
{
    ASSERT((prio != PRIO_NONE) && (INVERT_NIBBLE(prio) <= OS_MLQ_NUMBER_OF_PRIORITIES));

    /* BM_NotEmpty=Utl_BitSet(BM_NotEmpty,(uint8)prio); */
    UTL_BIT_SET16(BM_NotEmpty, (uint8)prio);
#if !defined(OS_SCHED_BM_ONLY)
    OsMLQ_PushBack(INVERT_NIBBLE(prio), TaskID);
#endif
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsMLQ_RemoveTask(TaskType TaskID)
#else
void OsMLQ_RemoveTask(TaskType TaskID)
#endif /* KOS_MEMORY_MAPPING */
{
    uint8 prio = PRIORITY_FOR_TASK(TaskID);

    ASSERT((TaskID != INVALID_TASK) && (TaskID <= OS_NUMBER_OF_TASKS));

    ASSERT((prio != PRIO_NONE) && (INVERT_NIBBLE(prio) <= OS_MLQ_NUMBER_OF_PRIORITIES));
#if !defined(OS_SCHED_BM_ONLY)
    OsMLQ_PopFront(INVERT_NIBBLE(prio));

    if (OsMLQ_IsEmpty(INVERT_NIBBLE(prio))) {
        /* BM_NotEmpty=Utl_BitReset(BM_NotEmpty,prio); */
        UTL_BIT_RESET16(BM_NotEmpty, (uint8)prio);
    }

#else
    /* BM_NotEmpty=Utl_BitReset(BM_NotEmpty,prio); */
    UTL_BIT_RESET16(BM_NotEmpty, (uint8)prio);
#endif
}


#if defined(OS_USE_RESOURCES) || defined(OS_USE_INTERNAL_RESOURCES)
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsMLQ_ChangePrio(TaskType TaskID, PriorityType old_prio, PriorityType new_prio)
#else
void OsMLQ_ChangePrio(TaskType TaskID, PriorityType old_prio, PriorityType new_prio)
#endif /* KOS_MEMORY_MAPPING */
{
    ASSERT((new_prio != PRIO_NONE) && (INVERT_NIBBLE(new_prio) <= OS_MLQ_NUMBER_OF_PRIORITIES));

    OS_TCB[TaskID].CurrentPriority = new_prio;
}


#endif

/*
**
**  Local Functions.
**
*/
#if !defined(OS_SCHED_BM_ONLY)
#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsMLQ_InitQueue(uint8 num)
#else
static void OsMLQ_InitQueue(uint8 num)
#endif /* KOS_MEMORY_MAPPING */
{
    Utl_MemSet((void *)&MLQ_ReadyQueue[num], (uint8)'\0', (uint16)sizeof(OsMLQ_QueueType));
    Utl_MemSet((void *)MLQ_QueueDef[num].data, (uint8)'\0', (uint16)MLQ_QueueDef[num].size);
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsMLQ_PushFront(uint8 num, uint8 TaskID)
#else
static void OsMLQ_PushFront(uint8 num, uint8 TaskID)
#endif /* KOS_MEMORY_MAPPING */
{
    OsMLQ_QueueType * const                     rq = &MLQ_ReadyQueue[num];
    OsMLQ_QueueConfigurationType const * const  qc = &MLQ_QueueDef[num];

    (rq->head == (uint8)0x00) ? rq->head = qc->size - (uint8)1 : --rq->head;

    qc->data[rq->head] = TaskID;
    rq->entries++;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsMLQ_PushBack(uint8 num, TaskType TaskID)
#else
static void OsMLQ_PushBack(uint8 num, TaskType TaskID)
#endif /* KOS_MEMORY_MAPPING */
{
    OsMLQ_QueueType * const                     rq = &MLQ_ReadyQueue[num];
    OsMLQ_QueueConfigurationType const * const  qc = &MLQ_QueueDef[num];

    qc->data[rq->tail] = TaskID;

    rq->tail = (rq->tail + (uint8)1) % qc->size;
    rq->entries++;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, OSEK_OS_CODE) OsMLQ_PopFront(uint8 num)
#else
static void OsMLQ_PopFront(uint8 num)
#endif /* KOS_MEMORY_MAPPING */
{
#if 0
    MLQ_ReadyQueue[num].head = (MLQ_ReadyQueue[num].head + (uint8)1) % MLQ_QueueDef[num].size;
#endif

    MLQ_ReadyQueue[num].entries--;
    MLQ_ReadyQueue[num].head++;

    if (MLQ_ReadyQueue[num].head == MLQ_QueueDef[num].size) {
        MLQ_ReadyQueue[num].head = (uint8)0x00;
    }
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_OS_CODE) OsMLQ_IsEmpty(uint8 num)
#else
static boolean OsMLQ_IsEmpty(uint8 num)
#endif /* KOS_MEMORY_MAPPING */
{
    return MLQ_ReadyQueue[num].entries == (uint8)0x00;
}


#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(TaskType, OSEK_OS_CODE) OsMLQ_Front(uint8 num)
#else
static TaskType OsMLQ_Front(uint8 num)
#endif /* KOS_MEMORY_MAPPING */
{
    return MLQ_QueueDef[num].data[MLQ_ReadyQueue[num].head];
}


#if 0
#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_OS_CODE) OsMLQ_IsFull(uint8 num)
#else
static boolean OsMLQ_IsFull(uint8 num)
#endif /* KOS_MEMORY_MAPPING */
{
    return MLQ_ReadyQueue[num].entries == MLQ_QueueDef[num].size;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(TaskType, OSEK_OS_CODE) OsMLQ_Back(uint8 num)
#else
static TaskType OsMLQ_Back(uint8 num)
#endif /* KOS_MEMORY_MAPPING */
{
    return MLQ_QueueDef[num].data[MLQ_ReadyQueue[num].tail];
}


#endif
#endif /* !OS_SCHED_BM_ONLY*/

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
