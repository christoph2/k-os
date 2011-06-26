/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

 * (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
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

void OsMLQ_Init(void);

TaskType    OsMLQ_GetHighestPrio(void);
void        OsMLQ_AddTaskFirst(TaskType TaskID, PriorityType prio);
void        OsMLQ_AddTaskLast(TaskType TaskID, PriorityType prio);
void        OsMLQ_RemoveTask(PriorityType prio);

#if !defined(OS_SCHED_BM_ONLY)
static void OsMLQ_InitQueue(uint8 num);
static void OsMLQ_PushFront(uint8 num, TaskType TaskID);
static void OsMLQ_PushBack(uint8 num, TaskType TaskID);
static void OsMLQ_PopFront(uint8 num);

/*  static void OsMLQ_PopBack(uint8 num);  */
static boolean OsMLQ_IsEmpty(uint8 num);

/* static boolean OsMLQ_IsFull(uint8 num); */
static TaskType OsMLQ_Front(uint8 num);

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

/*
**
**  Global Functions.
**
*/

void OsMLQ_Init(void)
{
    BM_NotEmpty = (uint16)0x0000U;
#if !defined(OS_SCHED_BM_ONLY)
    uint8 idx;

    for (idx = (uint8)0; idx < OS_MLQ_NUMBER_OF_PRIORITIES; ++idx) {
        OsMLQ_InitQueue(idx);
    }

#endif
}

TaskType OsMLQ_GetHighestPrio(void)
{
#if !defined(OS_SCHED_BM_ONLY)
    uint8 queue_num;
#endif
    TaskType TaskID = INVALID_TASK;

    if (BM_NotEmpty != (uint16)0x0000U) {
#if defined(OS_SCHED_BM_ONLY)
        TaskID = OsMLQ_GetLowestBitNumber((~BM_NotEmpty + (uint16)1) & BM_NotEmpty);
#else
        queue_num  = OsMLQ_GetLowestBitNumber((~BM_NotEmpty + (uint16)1) & BM_NotEmpty) - ((uint8)1);
        TaskID     = OsMLQ_Front(queue_num);
#endif
    }

    return TaskID;
}

boolean OsMLQ_TasksAreReady(void)
{
    return BM_NotEmpty != (uint16)0x0000U;
}

uint16 OsMLQ_GetBitmap(void)
{
    return BM_NotEmpty;
}

void OsMLQ_AddTaskFirst(TaskType TaskID, PriorityType prio)    /* stack. */
{
    ASSERT((prio != PRIO_NONE) && (INVERT_NIBBLE(prio) <= OS_MLQ_NUMBER_OF_PRIORITIES));

    /* BM_NotEmpty=Utl_BitSet(BM_NotEmpty,(uint8)prio); */
    UTL_BIT_SET16(BM_NotEmpty, (uint8)prio);

#if !defined(OS_SCHED_BM_ONLY)
    OsMLQ_PushFront(INVERT_NIBBLE(prio), TaskID);
#endif
}

void OsMLQ_AddTaskLast(TaskType TaskID, PriorityType prio)     /* queue. */
{
    ASSERT((prio != PRIO_NONE) && (INVERT_NIBBLE(prio) <= OS_MLQ_NUMBER_OF_PRIORITIES));

    /* BM_NotEmpty=Utl_BitSet(BM_NotEmpty,(uint8)prio); */
    UTL_BIT_SET16(BM_NotEmpty, (uint8)prio);
#if !defined(OS_SCHED_BM_ONLY)
    OsMLQ_PushBack(INVERT_NIBBLE(prio), TaskID);
#endif
}

void OsMLQ_RemoveTask(TaskType TaskID)
{
    uint8 prio;

    ASSERT((TaskID != INVALID_TASK) && (TaskID <= OS_NUMBER_OF_TASKS));

    prio = PRIORITY_FOR_TASK(TaskID);
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
void OsMLQ_ChangePrio(TaskType TaskID, PriorityType old_prio, PriorityType new_prio)
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
static void OsMLQ_InitQueue(uint8 num)
{
    Utl_MemSet((void *)&MLQ_ReadyQueue[num], (uint8)'\0', (uint16)sizeof(OsMLQ_QueueType));
    Utl_MemSet((void *)MLQ_QueueDef[num].data, (uint8)'\0', (uint16)MLQ_QueueDef[num].size);
}

static void OsMLQ_PushFront(uint8 num, uint8 TaskID)
{
    OsMLQ_QueueType * const                     rq = &MLQ_ReadyQueue[num];
    OsMLQ_QueueConfigurationType const * const  qc = &MLQ_QueueDef[num];

    (rq->head == (uint8)0x00) ? rq->head = qc->size - (uint8)1 : --rq->head;

    qc->data[rq->head] = TaskID;
    rq->entries++;
}

static void OsMLQ_PushBack(uint8 num, TaskType TaskID)
{
    OsMLQ_QueueType * const                     rq = &MLQ_ReadyQueue[num];
    OsMLQ_QueueConfigurationType const * const  qc = &MLQ_QueueDef[num];

    qc->data[rq->tail] = TaskID;

    rq->tail = (rq->tail + (uint8)1) % qc->size;
    rq->entries++;
}

static void OsMLQ_PopFront(uint8 num)
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

static boolean OsMLQ_IsEmpty(uint8 num)
{
    return MLQ_ReadyQueue[num].entries == (uint8)0x00;
}

static TaskType OsMLQ_Front(uint8 num)
{
    return MLQ_QueueDef[num].data[MLQ_ReadyQueue[num].head];
}

#if 0
static boolean OsMLQ_IsFull(uint8 num)
{
    return MLQ_ReadyQueue[num].entries == MLQ_QueueDef[num].size;
}

static TaskType OsMLQ_Back(uint8 num)
{
    return MLQ_QueueDef[num].data[MLQ_ReadyQueue[num].tail];
}

#endif
#endif /* !OS_SCHED_BM_ONLY*/
