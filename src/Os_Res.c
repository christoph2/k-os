/*
 * k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2018 by Christoph Schueler <github.com/Christoph2,
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
#include "Osek.h"
#include "Os_Res.h"
#include "Os_Mlq.h"
#include "Utl.h"

/*
**  Local variables.
*/
#if (OS_FEATURE_INTERNAL_RESOURCES == STD_ON)
static uint16 BM_InternalResources;
#endif

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Global functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsRes_InitResources(void)
#else
void OsRes_InitResources(void)
#endif /* KOS_MEMORY_MAPPING */
{
#if (OS_FEATURE_RESOURCES == STD_ON)
    uint8 i;

#if (OS_FEATURE_INTERNAL_RESOURCES == STD_ON)
    BM_InternalResources = (uint16)0;
#endif

#if (OS_FEATURE_RESOURCES == STD_ON)

    for (i = (uint8)0; i < OS_NUMBER_OF_RESOURCES; ++i) {
#if (OS_FEATURE_ORTI_DEBUG == STD_ON)
        Os_Resources[i].Locker = INVALID_TASK;
#endif
        Os_Resources[i].PriorPriorityOfTask = PRIO_NONE;
    }

#endif
#endif /* OS_FEATURE_RESOURCES */
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) GetResource(ResourceType ResID)
#else
StatusType GetResource(ResourceType ResID)
#endif /* KOS_MEMORY_MAPPING */
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
    Os_SaveServiceContext(OSServiceId_GetResource, ResID, NULL, NULL);
#if (OS_FEATURE_RESOURCES == STD_ON)
    ASSERT_VALID_RESOURCEID(ResID);
    ASSERT_VALID_GET_RESOURCE_ACCESS(ResID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK);    /* ISR-Level Resources not implemented yet.  */
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    OsPort_DisableAllOsInterrupts();

    if (ResID == RES_SCHEDULER) {
        OsExec_LockScheduler();
    } else {
#if (OS_FEATURE_RESOURCES == STD_ON)
#if (OS_FEATURE_ORTI_DEBUG == STD_ON)
        Os_Resources[ResID].Locker = Os_CurrentTID;      /* lock Resource. */
#endif
        /* save current priority. */
        Os_Resources[ResID].PriorPriorityOfTask = Os_CurrentTCB->CurrentPriority;
        Os_CurrentTCB->ResourceCount++;

        if (OS_ResourceConf[ResID].CeilingPriority < Os_CurrentTCB->CurrentPriority) {
            /* Elevate Priority of running Task. */
            Os_CurrentTCB->CurrentPriority = OS_ResourceConf[ResID].CeilingPriority;
            OsMLQ_ChangePrio(Os_CurrentTID, Os_CurrentTCB->CurrentPriority, OS_ResourceConf[ResID].CeilingPriority);
        }

#endif
    }

    OsPort_EnableAllOsInterrupts();
#else
    UNREFERENCED_PARAMETER(ResID);
#endif /* OS_FEATURE_RESOURCES */
    Os_ClearServiceContext();
    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_OS_CODE) ReleaseResource(ResourceType ResID)
#else
StatusType ReleaseResource(ResourceType ResID)
#endif /* KOS_MEMORY_MAPPING */
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
    Os_SaveServiceContext(OSServiceId_ReleaseResource, ResID, NULL, NULL);
#if (OS_FEATURE_RESOURCES == STD_ON)
    ASSERT_VALID_RESOURCEID(ResID);
    ASSERT_RESOURCE_IS_OCCUPIED(ResID);
    ASSERT_VALID_RELEASE_RESOURCE_ACCESS(ResID);
    ASSERT_VALID_CALLEVEL(OS_CL_TASK);     /* ISR-Level Resources not implemented yet.  */
    ASSERT_INTERRUPTS_ENABLED_AT_TASK_LEVEL();

    OsPort_DisableAllOsInterrupts();

    if (ResID == RES_SCHEDULER) {
        OsExec_UnlockScheduler();
    } else {
#if (OS_FEATURE_RESOURCES == STD_ON)
#if (OS_FEATURE_ORTI_DEBUG == STD_ON)
        Os_Resources[ResID].Locker = INVALID_TASK;       /* unlock Resource. */
#endif
        Os_CurrentTCB->ResourceCount--;

        if (Os_CurrentTCB->CurrentPriority != Os_Resources[ResID].PriorPriorityOfTask) {
            /* restore Priority. */
            Os_CurrentTCB->CurrentPriority = Os_Resources[ResID].PriorPriorityOfTask;
            OsMLQ_ChangePrio(Os_CurrentTID, Os_CurrentTCB->CurrentPriority, Os_Resources[ResID].PriorPriorityOfTask);
        }

#endif
    }

    OsPort_EnableAllOsInterrupts();
    OsExec_CondScheduleFromTaskLevel();
#else
    UNREFERENCED_PARAMETER(ResID);
#endif /* OS_FEATURE_RESOURCES */
    Os_ClearServiceContext();
    return E_OK;
}


/*
**
**  Functions for Internal Resources.
**
*/
#if (OS_FEATURE_INTERNAL_RESOURCES == STD_ON)

/* Hinweis: die 'Idle'-Task darf keine internen Resourcen haben!!! */
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsRes_GetInternalResource(void)
#else
void OsRes_GetInternalResource(void)
#endif /* KOS_MEMORY_MAPPING */
{
    ResourceType InternalResource = OS_TaskConf[Os_CurrentTID].InternalResource;

    if (InternalResource != INTERNAL_RES_NONE && !UTL_BIT_GET16(BM_InternalResources, InternalResource)) {
        Os_CurrentTCB->CurrentPriority = OS_IntResourceConf[InternalResource].CeilingPriority;

        BM_InternalResources = UTL_BIT_SET16(BM_InternalResources, InternalResource);
#if 0
        (void)OSSysPQChangePrio(Os_CurrentTID, Os_CurrentTCB->CurrentPriority);
#endif
        OsMLQ_ChangePrio(Os_CurrentTID, Os_CurrentTCB->CurrentPriority, OS_IntResourceConf[InternalResource].CeilingPriority);

    }
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsRes_ReleaseInternalResource(void)
#else
void OsRes_ReleaseInternalResource(void)
#endif /* KOS_MEMORY_MAPPING */
{
    if (Os_CurrentTCB->CurrentPriority != OS_TaskConf[Os_CurrentTID].Priority) {
        Os_CurrentTCB->CurrentPriority  = OS_TaskConf[Os_CurrentTID].Priority;
        BM_InternalResources           = UTL_BIT_RESET16(BM_InternalResources, OS_TaskConf[Os_CurrentTID].InternalResource);
#if 0
        (void)OSSysPQChangePrio(Os_CurrentTID, OS_TaskConf[Os_CurrentTID].Priority, Os_CurrentTCB->CurrentPriority);
#endif
        OsMLQ_ChangePrio(Os_CurrentTID, Os_CurrentTCB->CurrentPriority, OS_TaskConf[Os_CurrentTID].Priority);
    }
}


#endif /* OS_FEATURE_INTERNAL_RESOURCES */

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
