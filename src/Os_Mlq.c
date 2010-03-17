/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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
#include "Os_Mlq.h"

typedef struct tagQueueConfigurationType {
    uint8 size;
    TaskType * const data;
} QueueConfigurationType;


typedef struct tagQueueType {
    uint8 head,tail;
    uint8 entries;
} QueueType;

void OsMLQ_Init(void);

TaskType OsMLQ_GetHighestPrio(void);
void OsMLQ_AddTaskFirst(TaskType TaskID,PriorityType prio);
void OsMLQ_AddTaskLast(TaskType TaskID,PriorityType prio);
void OsMLQ_RemoveTask(PriorityType prio);

static void OsMLQ_InitQueue(uint8 num);
static void OsMLQ_PushFront(uint8 num,TaskType TaskID);
static void OsMLQ_PushBack(uint8 num,TaskType TaskID);
static void OsMLQ_PopFront(uint8 num);
/*  static void OsMLQ_PopBack(uint8 num);  */
static boolean OsMLQ_IsEmpty(uint8 num);
/* static boolean OsMLQ_IsFull(uint8 num); */
static TaskType OsMLQ_Front(uint8 num);
/* static TaskType OsMLQ_Back(uint8 num); */


#if defined(OS_USE_RESOURCES)
#define PRIORITY_FOR_TASK(TaskID)   OS_TCB[(TaskID)].CurrentPriority
#else
#define PRIORITY_FOR_TASK(TaskID)   OS_TaskConf[(TaskID)].Priority
#endif

/*
**  CONFIG.
*/
#define OS_MLQ_NUMBER_OF_PRIORITIES   ((uint8)3)

static TaskType MLQ_QueueData[10];  /*  OS_NUMBER_OF_TASKS */

const QueueConfigurationType MLQ_QueueDef[OS_MLQ_NUMBER_OF_PRIORITIES]={ /* MLQ_ReadyQueueDefs */
    {(uint8)3,&MLQ_QueueData[0]},
    {(uint8)3,&MLQ_QueueData[3]},
    {(uint8)4,&MLQ_QueueData[6]}
};


static QueueType MLQ_ReadyQueue[OS_MLQ_NUMBER_OF_PRIORITIES];
static uint16 BM_NotEmpty;  /* Bitmap for non-empty queues.  */


/*
**
**  Global Functions.
**
*/


void OsMLQ_Init(void)
{
    uint8 idx;

    BM_NotEmpty=(uint16)0x0000;

    for (idx=(uint8)0;idx<OS_MLQ_NUMBER_OF_PRIORITIES;++idx) {
        OsMLQ_InitQueue(idx);    /*  todo: Konfig.-Parameter definieren!!!   */
    }

}


TaskType OsMLQ_GetHighestPrio(void)
{
    uint8 queue_num;
    TaskType TaskID=INVALID_TASK;

    if (BM_NotEmpty!=(uint16)0x0000) {
        queue_num=OsMLQ_GetLowestBitNumber((~BM_NotEmpty+(uint16)1) & BM_NotEmpty)-((uint8)1);
        TaskID=OsMLQ_Front(queue_num);
    }

    return TaskID;
}


boolean OsMLQ_TasksAreReady(void)
{
    return (BM_NotEmpty!=(uint16)0x0000);
}


uint16 OsMLQ_GetBitmap(void)
{
    return BM_NotEmpty;
}


void OsMLQ_AddTaskFirst(TaskType TaskID,PriorityType prio)    /* stack. */
{
    ASSERT((prio!=PRIO_NONE) && (INVERT_NIBBLE(prio)<=OS_MLQ_NUMBER_OF_PRIORITIES));

    BM_NotEmpty=Utl_BitSet(BM_NotEmpty,(uint8)prio);
    OsMLQ_PushFront(INVERT_NIBBLE(prio),TaskID);
}


void OsMLQ_AddTaskLast(TaskType TaskID,PriorityType prio)     /* queue. */
{
    ASSERT((prio!=PRIO_NONE) && (INVERT_NIBBLE(prio)<=OS_MLQ_NUMBER_OF_PRIORITIES));

    BM_NotEmpty=Utl_BitSet(BM_NotEmpty,(uint8)prio);
    OsMLQ_PushBack(INVERT_NIBBLE(prio),TaskID);
}


void OsMLQ_RemoveTask(TaskType TaskID)
{
    uint8 prio;

    ASSERT((TaskID!=INVALID_TASK) && (TaskID<=OS_NUMBER_OF_TASKS));

    prio=PRIORITY_FOR_TASK(TaskID);
    ASSERT((prio!=PRIO_NONE) && (INVERT_NIBBLE(prio)<=OS_MLQ_NUMBER_OF_PRIORITIES));

    OsMLQ_PopFront(INVERT_NIBBLE(prio));
    if (OsMLQ_IsEmpty(INVERT_NIBBLE(prio))) {
        BM_NotEmpty=Utl_BitReset(BM_NotEmpty,prio);
    }
}


void OsMLQ_ChangePrio(TaskType TaskID,PriorityType old_prio,PriorityType new_prio)
{
    ASSERT((new_prio!=PRIO_NONE) && (INVERT_NIBBLE(new_prio)<=OS_MLQ_NUMBER_OF_PRIORITIES));

    OS_TCB[TaskID].CurrentPriority=new_prio;
}

/*
**
**  Local Functions.
**
*/
static void OsMLQ_InitQueue(uint8 num)
{
    Utl_MemSet((void*)&MLQ_ReadyQueue[num],(uint8)'\0',(uint16)sizeof(QueueType));
    Utl_MemSet((void*)MLQ_QueueDef[num].data,(uint8)'\0',(uint16)MLQ_QueueDef[num].size);
}


static void OsMLQ_PushFront(uint8 num,uint8 TaskID)
{
    QueueType * const rq=&MLQ_ReadyQueue[num];
    QueueConfigurationType const * const qc=&MLQ_QueueDef[num];

    (rq->head==(uint8)0x00) ? rq->head=qc->size-(uint8)1 : --rq->head;

    qc->data[rq->head]=TaskID;
    rq->entries++;
}


static void OsMLQ_PushBack(uint8 num,TaskType TaskID)
{
    QueueType * const rq=&MLQ_ReadyQueue[num];
    QueueConfigurationType const * const qc=&MLQ_QueueDef[num];

    qc->data[rq->tail]=TaskID;

    rq->tail=(rq->tail+(uint8)1) % qc->size;
    rq->entries++;
}


static void OsMLQ_PopFront(uint8 num)
{
#if 0
    MLQ_ReadyQueue[num].head=(MLQ_ReadyQueue[num].head+(uint8)1) % MLQ_QueueDef[num].size;
#endif

    MLQ_ReadyQueue[num].entries--;
    MLQ_ReadyQueue[num].head++;
    if (MLQ_ReadyQueue[num].head==MLQ_QueueDef[num].size) {
        MLQ_ReadyQueue[num].head=(uint8)0x00;
    }
}


static boolean OsMLQ_IsEmpty(uint8 num)
{
    return MLQ_ReadyQueue[num].entries==(uint8)0x00;
}


#if 0
static boolean OsMLQ_IsFull(uint8 num)
{
    return MLQ_ReadyQueue[num].entries==MLQ_QueueDef[num].size;
}
#endif


static TaskType OsMLQ_Front(uint8 num)
{
    return MLQ_QueueDef[num].data[MLQ_ReadyQueue[num].head];
}


#if 0
static TaskType OsMLQ_Back(uint8 num)
{
    return MLQ_QueueDef[num].data[MLQ_ReadyQueue[num].tail];
}
#endif
