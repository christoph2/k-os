/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

 * (C) 2007-2010 by Christoph Schueler <github.com/Christoph2,
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
#include "Osek.h"

#if defined(OS_USE_INTERNAL_RESOURCES)
static uint16 BM_InternalResources;
#endif

void OsRes_InitResources(void)
{
    uint8 i;

#if defined(OS_USE_INTERNAL_RESOURCES)
    BM_InternalResources=(uint16)0;
#endif

#if defined(OS_USE_RESOURCES)
    for (i=(uint8)0;i<OS_NUMBER_OF_RESOURCES;++i) {
#if defined(OS_FEATURE_ORTI_DEBUG)
        Os_Resources[i].Locker=INVALID_TASK;
#endif
        Os_Resources[i].PriorPriorityOfTask=PRIO_NONE;
    }
#endif

}

StatusType GetResource(ResourceType ResID)
{
/*
**      Standard-Status:
**              – E_OK – no error.
**      Extended-Status:
**              – E_OS_ID – the resource identifier is invalid.
**              – E_OS_ACCESS – attempt to get resource which is already
**                occupied by any task or ISR, or the assigned in OIL
**                priority of the calling task or interrupt routine is higher
**                than the calculated ceiling priority.
*/
    SAVE_SERVICE_CONTEXT(OSServiceId_GetResource,ResID,NULL,NULL);

    ASSERT_VALID_RESOURCEID(ResID);
    ASSERT_VALID_GET_RESOURCE_ACCESS(ResID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK);    /* ISR-Level Resources not implemented yet.  */
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    DISABLE_ALL_OS_INTERRUPTS();
    if (ResID==RES_SCHEDULER) {
        OS_LOCK_SCHEDULER();
    } else {
#if defined(OS_USE_RESOURCES)
#if defined(OS_FEATURE_ORTI_DEBUG)
        Os_Resources[ResID].Locker=OsCurrentTID;      /* lock Resource. */
#endif
        /* save current priority. */
        Os_Resources[ResID].PriorPriorityOfTask=OsCurrentTCB->CurrentPriority;
        OsCurrentTCB->ResourceCount++;

        if (OS_ResourceConf[ResID].CeilingPriority<OsCurrentTCB->CurrentPriority) {
            /* Elevate Priority of running Task. */
           OsCurrentTCB->CurrentPriority=OS_ResourceConf[ResID].CeilingPriority;
           OsMLQ_ChangePrio(OsCurrentTID,OsCurrentTCB->CurrentPriority,OS_ResourceConf[ResID].CeilingPriority);

        }
#endif
    }
    ENABLE_ALL_OS_INTERRUPTS();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}

StatusType ReleaseResource(ResourceType ResID)
{
/*
**      Standard-Status:
**              – E_OK – no error.
**      Extended-Status:
**              – E_OS_ID – the resource identifier is invalid.
**              – E_OS_NOFUNC – attempt to release a resource which is
**                not occupied by any task or ISR, or another resource has
**                to be released before.
**              – E_OS_ACCESS – attempt to release a resource which has
**                a lower ceiling priority than the assigned in OIL priority
**                of the calling task or interrupt routine. This error code
**                returned only if E_OS_NOFUNC was not returned.
*/
    SAVE_SERVICE_CONTEXT(OSServiceId_ReleaseResource,ResID,NULL,NULL);

    ASSERT_VALID_RESOURCEID(ResID);
    ASSERT_RESOURCE_IS_OCCUPIED(ResID);
    ASSERT_VALID_RELEASE_RESOURCE_ACCESS(ResID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK);     /* ISR-Level Resources not implemented yet.  */
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    DISABLE_ALL_OS_INTERRUPTS();
    if (ResID==RES_SCHEDULER) {
        OS_UNLOCK_SCHEDULER();
    } else {
#if defined(OS_USE_RESOURCES)
#if defined(OS_FEATURE_ORTI_DEBUG)
        Os_Resources[ResID].Locker=INVALID_TASK;       /* unlock Resource. */
#endif
        OsCurrentTCB->ResourceCount--;

        if (OsCurrentTCB->CurrentPriority!=Os_Resources[ResID].PriorPriorityOfTask) {
            /* restore Priority. */
            OsCurrentTCB->CurrentPriority=Os_Resources[ResID].PriorPriorityOfTask;
            OsMLQ_ChangePrio(OsCurrentTID,OsCurrentTCB->CurrentPriority,Os_Resources[ResID].PriorPriorityOfTask);
        }
#endif
    }
    ENABLE_ALL_OS_INTERRUPTS();
    OS_COND_SCHEDULE_FROM_TASK_LEVEL();

    CLEAR_SERVICE_CONTEXT();
    return E_OK;
}

/*
**
**  Functions for Internal Resources.
**
*/

#if defined(OS_USE_INTERNAL_RESOURCES)

/* Hinweis: die 'Idle'-Task darf keine internen Resourcen haben!!! */
void OsRes_GetInternalResource(void)
{
    ResourceType InternalResource=OS_TaskConf[OsCurrentTID].InternalResource;

    if (InternalResource!=INTERNAL_RES_NONE && !Utl_BitGet(BM_InternalResources,InternalResource)) {
        OsCurrentTCB->CurrentPriority=OS_IntResourceConf[InternalResource].CeilingPriority;

        BM_InternalResources=Utl_BitSet(BM_InternalResources,InternalResource);
#if 0
        (void)OSSysPQChangePrio(OsCurrentTID,OsCurrentTCB->CurrentPriority);
#endif
        OsMLQ_ChangePrio(OsCurrentTID,OsCurrentTCB->CurrentPriority,OS_IntResourceConf[InternalResource].CeilingPriority);

    }
}

void OsRes_ReleaseInternalResource(void)
{
    if (OsCurrentTCB->CurrentPriority!=OS_TaskConf[OsCurrentTID].Priority) {
        OsCurrentTCB->CurrentPriority=OS_TaskConf[OsCurrentTID].Priority;
        BM_InternalResources=Utl_BitReset(BM_InternalResources,OS_TaskConf[OsCurrentTID].InternalResource);
#if 0
        (void)OSSysPQChangePrio(OsCurrentTID,OS_TaskConf[OsCurrentTID].Priority,OsCurrentTCB->CurrentPriority);
#endif
        OsMLQ_ChangePrio(OsCurrentTID,OsCurrentTCB->CurrentPriority,OS_TaskConf[OsCurrentTID].Priority);
    }
}
#endif /* OS_USE_INTERNAL_RESOURCES */
