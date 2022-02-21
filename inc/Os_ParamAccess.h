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
/** @file Os_ParamAccess.h
 *  @brief Service identifier related stuff.
 *
 *
 *
 *  @author Christoph Schueler (cpu12.gems@googlemail.com)
 */
#if !defined(OS_PARAMACCESS)
#define OS_PARAMACCESS

#include "Os_Cfg.h"
#include "Os_Types.h"

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */


typedef enum tagOS_ServiceIdType {
    OSServiceId_NoService,
    /*  OSEK-OS 2.2.3. */
    OSServiceId_ActivateTask               = 2,
    OSServiceId_TerminateTask              = 4,
    OSServiceId_ChainTask                  = 6,
    OSServiceId_Schedule                   = 8,
    OSServiceId_GetTaskID                  = 10,
    OSServiceId_GetTaskState               = 12,
    OSServiceId_DisableAllInterrupts       = 14,
    OSServiceId_EnableAllInterrupts        = 16,
    OSServiceId_SuspendAllInterrupts       = 18,
    OSServiceId_ResumeAllInterrupts        = 20,
    OSServiceId_SuspendOSInterrupts        = 22,
    OSServiceId_ResumeOSInterrupts         = 24,
    OSServiceId_GetResource                = 26,
    OSServiceId_ReleaseResource            = 28,
    OSServiceId_SetEvent                   = 30,
    OSServiceId_ClearEvent                 = 32,
    OSServiceId_GetEvent                   = 34,
    OSServiceId_WaitEvent                  = 36,
    OSServiceId_GetAlarmBase               = 38,
    OSServiceId_GetAlarm                   = 40,
    OSServiceId_SetRelAlarm                = 42,
    OSServiceId_SetAbsAlarm                = 44,
    OSServiceId_CancelAlarm                = 46,
    OSServiceId_GetActiveApplicationMode   = 48,
    OSServiceId_StartOS                    = 50,
    OSServiceId_ShutdownOS                 = 52,
    /*  Autosar. */
    OSServiceId_GetApplicationID           = 54,
    OSServiceId_GetISRID                   = 56,
    OSServiceId_CallTrustedFunction        = 58,
    OSServiceId_CheckISRMemoryAccess       = 60,
    OSServiceId_CheckTaskMemoryAccess      = 62,
    OSServiceId_CheckObjectAccess          = 64,
    OSServiceId_CheckObjectOwnership       = 66,
    OSServiceId_StartScheduleTableRel      = 68,
    OSServiceId_StartScheduleTableAbs      = 70,
    OSServiceId_StopScheduleTable          = 72,
    OSServiceId_NextScheduleTable          = 74,
    OSServiceId_StartScheduleTableSynchron = 76,
    OSServiceId_SyncScheduleTable          = 78,
    OSServiceId_GetScheduleTableStatus     = 80,
    OSServiceId_SetScheduleTableAsync      = 82,
    OSServiceId_TerminateApplication       = 84,
    OSServiceId_DisableInterruptSource     = 86,
    OSServiceId_EnableInterruptSource      = 88,
    /* Counter-Interface. */
    OSServiceId_InitCounter            = 90,
    OSServiceId_IncrementCounter       = 92,
    OSServiceId_GetCounterInfo         = 94,
    OSServiceId_GetCounterValue        = 96,
    OSServiceId_GetElapsedCounterValue = 98,
    /* OSEK-COM 3.0.3 */
    COMServiceId_StartCOM              = 100,
    COMServiceId_StopCOM               = 102,
    COMServiceId_InitMessage           = 104,
    COMServiceId_StartPeriodic         = 106,
    COMServiceId_StopPeriodic          = 108,
    COMServiceId_SendMessage           = 110,
    COMServiceId_ReceiveMessage        = 112,
    COMServiceId_SendDynamicMessage    = 114,
    COMServiceId_ReceiveDynamicMessage = 116,
    COMServiceId_SendZeroMessage       = 118,
    COMServiceId_GetMessageStatus      = 120
} Os_ServiceIdType;

#if (OS_FEATURE_PARAMETERACCESS == STD_ON)
    #define OSError_ActivateTask_TaskID()                           ((TaskType)Os_ServiceContext.param1)
    #define OSError_ChainTask_TaskID()                              ((TaskType)Os_ServiceContext.param1)
    #define OSError_GetTaskID_TaskID()                              ((TaskRefType)Os_ServiceContext.param1)
    #define OSError_GetTaskState_TaskID()                           ((TaskType)Os_ServiceContext.param1)
    #define OSError_GetTaskState_State()                            ((TaskStateRefType)Os_ServiceContext.param2)
    #define OSError_GetResource_ResID()                             ((ResourceType)Os_ServiceContext.param1)
    #define OSError_ReleaseResource_ResID()                         ((ResourceType)Os_ServiceContext.param1)
    #define OSError_SetEvent_TaskID()                               ((TaskType)Os_ServiceContext.param1)
    #define OSError_SetEvent_Mask()                                 ((EventMaskType)Os_ServiceContext.param2)
    #define OSError_ClearEvent_Mask()                               ((EventMaskType)Os_ServiceContext.param1)
    #define OSError_GetEvent_TaskID()                               ((TaskType)Os_ServiceContext.param1)
    #define OSError_GetEvent_Event()                                ((EventMaskRefType)Os_ServiceContext.param2)
    #define OSError_WaitEvent_Mask()                                ((EventMaskType)Os_ServiceContext.param1)
    #define OSError_GetAlarmBase_AlarmID()                          ((AlarmType)Os_ServiceContext.param1)
    #define OSError_GetAlarmBase_Info()                             ((AlarmBaseRefType)Os_ServiceContext.param2)
    #define OSError_GetAlarm_AlarmID()                              ((AlarmType)Os_ServiceContext.param1)
    #define OSError_GetAlarm_Tick()                                 ((TickRefType)Os_ServiceContext.param2)
    #define OSError_SetRelAlarm_AlarmID()                           ((AlarmType)Os_ServiceContext.param1)
    #define OSError_SetRelAlarm_increment()                         ((TickType)Os_ServiceContext.param2)
    #define OSError_SetRelAlarm_cycle()                             ((TickType)Os_ServiceContext.param3)
    #define OSError_SetAbsAlarm_AlarmID()                           ((AlarmType)Os_ServiceContext.param1)
    #define OSError_SetAbsAlarm_start()                             ((TickType)Os_ServiceContext.param2)
    #define OSError_SetAbsAlarm_cycle()                             ((TickType)Os_ServiceContext.param3)
    #define OSError_CancelAlarm_AlarmID()                           ((AlarmType)Os_ServiceContext.param1)
    #define OSError_CallTrustedFunction_FunctionIndex()             ((TrustedFunctionIndexType)Os_ServiceContext.param1)
    #define OSError_CallTrustedFunction_FunctionParams()            ((TrustedFunctionParameterRefType)Os_ServiceContext.param2)
    #define OSError_CheckISRMemoryAccess_ISRID()                    ((ISRType)Os_ServiceContext.param1)
    #define OSError_CheckISRMemoryAccess_Address()                  ((MemoryStartAddressType)Os_ServiceContext.param2)
    #define OSError_CheckISRMemoryAccess_Size()                     ((MemorySizeType)Os_ServiceContext.param3)
    #define OSError_CheckTaskMemoryAccess_TaskID()                  ((TaskType)Os_ServiceContext.param1)
    #define OSError_CheckTaskMemoryAccess_Address()                 ((MemoryStartAddressType)Os_ServiceContext.param2)
    #define OSError_CheckTaskMemoryAccess_Size()                    ((MemorySizeType)Os_ServiceContext.param3)
    #define OSError_CheckObjectAccess_ApplID()                      ((ApplicationType)Os_ServiceContext.param1)
    #define OSError_CheckObjectAccess_ObjectType()                  ((ObjectTypeType)Os_ServiceContext.param2)
    #define OSError_CheckObjectOwnership_ObjectType()               ((ObjectTypeType)Os_ServiceContext.param1)
    #define OSError_StartScheduleTableRel_ScheduleTableID()         ((ScheduleTableType)Os_ServiceContext.param1)
    #define OSError_StartScheduleTableRel_Offset()                  ((TickType)Os_ServiceContext.param2)
    #define OSError_StartScheduleTableAbs_ScheduleTableID()         ((ScheduleTableType)Os_ServiceContext.param1)
    #define OSError_StartScheduleTableAbs_Tickvalue()               ((TickType)Os_ServiceContext.param2)
    #define OSError_StopScheduleTable_ScheduleTableID()             ((ScheduleTableType)Os_ServiceContext.param1)
    #define OSError_NextScheduleTable_ScheduleTableID_From()        ((ScheduleTableType)Os_ServiceContext.param1)
    #define OSError_NextScheduleTable_ScheduleTableID_To()          ((ScheduleTableType)Os_ServiceContext.param2)
    #define OSError_StartScheduleTableSynchron_ScheduleTableID()    ((ScheduleTableType)Os_ServiceContext.param1)
    #define OSError_SyncScheduleTable_SchedTableID()                ((ScheduleTableType)Os_ServiceContext.param1)
    #define OSError_SyncScheduleTable_Value()                       ((TickType)Os_ServiceContext.param2)
    #define OSError_GetScheduleTableStatus_ScheduleID()             ((ScheduleTableType)Os_ServiceContext.param1)
    #define OSError_GetScheduleTableStatus_ScheduleStatus()         ((ScheduleTableStatusRefType)Os_ServiceContext.param2)
    #define OSError_SetScheduleTableAsync_ScheduleID()              ((ScheduleTableType)Os_ServiceContext.param1)
    #define OSError_IncrementCounter_CounterID()                    ((CounterType)Os_ServiceContext.param1)
    #define OSError_TerminateApplication_RestartOption()            ((RestartType)Os_ServiceContext.param1)
    #define OSError_DisableInterruptSource_DisableISR()             ((ISRType)Os_ServiceContext.param1)
    #define OSError_EnableInterruptSource_EnableISR()               ((ISRType)Os_ServiceContext.param1)

    #define OSError_InitCounter_CounterID()                         ((CounterType)Os_ServiceContext.param1)
    #define OSError_InitCounter_InitialValue()                      ((TickType)Os_ServiceContext.param2)
    #define OSError_CounterTrigger_CounterID()                      ((CounterType)Os_ServiceContext.param1)
    #define OSError_GetCounterInfo_CounterID()                      ((CounterType)Os_ServiceContext.param1)
    #define OSError_GetCounterInfo_Info()                           ((CtrInfoRefType)Os_ServiceContext.param2)
    #define OSError_GetCounterValue_CounterID()                     ((CounterType)Os_ServiceContext.param1)
    #define OSError_GetCounterValue_Value()                         ((TickRefType)Os_ServiceContext.param2)
    #define OSError_GetElapsedCounterValue_CounterID()              ((CounterType)Os_ServiceContext.param1)
    #define OSError_GetElapsedCounterValue_Value()                  ((TickRefType)Os_ServiceContext.param2)
    #define OSError_GetElapsedCounterValue_ElapsedValue()           ((TickRefType)Os_ServiceContext.param2)

    #define COM_Error_StartCOM_Mode()                               ((COMApplicationModeType)Os_ServiceContext.param1)
    #define COM_Error_StopCOM_Mode()                                ((COMShutdownModeType)Os_ServiceContext.param1)
    #define COM_Error_InitMessage_Message()                         ((MessageIdentifier)Os_ServiceContext.param1)
    #define COM_Error_InitMessage_DataRef()                         ((ApplicationDataRef)Os_ServiceContext.param2)
    #define COM_Error_SendMessage_Message()                         ((MessageIdentifier)Os_ServiceContext.param1)
    #define COM_Error_SendMessage_DataRef()                         ((ApplicationDataRef)Os_ServiceContext.param2)
    #define COM_Error_ReceiveMessage_Message()                      ((MessageIdentifier)Os_ServiceContext.param1)
    #define COM_Error_ReceiveMessage_DataRef()                      ((ApplicationDataRef)Os_ServiceContext.param2)
    #define COM_Error_SendDynamicMessage_Message()                  ((MessageIdentifier)Os_ServiceContext.param1)
    #define COM_Error_SendDynamicMessage_DataRef()                  ((ApplicationDataRef)Os_ServiceContext.param2)
    #define COM_Error_SendDynamicMessage_LengthRef()                ((LengthRef)Os_ServiceContext.param3)
    #define COM_Error_ReceiveDynamicMessage_Message()               ((MessageIdentifier)Os_ServiceContext.param1)
    #define COM_Error_ReceiveDynamicMessage_DataRef()               ((ApplicationDataRef)Os_ServiceContext.param2)
    #define COM_Error_ReceiveDynamicMessage_LengthRef()             ((LengthRef)Os_ServiceContext.param3)
    #define COM_Error_SendZeroMessage_Message()                     ((MessageIdentifier)Os_ServiceContext.param1)
    #define COM_Error_GetMessageStatus_Message()                    ((MessageIdentifier)Os_ServiceContext.param1)
#else
    #define OSError_ActivateTask_TaskID()                           ((TaskType)0)
    #define OSError_ChainTask_TaskID()                              ((TaskType)0)
    #define OSError_GetTaskID_TaskID()                              ((TaskRefType)0)
    #define OSError_GetTaskState_TaskID()                           ((TaskType)_0)
    #define OSError_GetTaskState_State()                            ((TaskStateRefType)0)
    #define OSError_GetResource_ResID()                             ((ResourceType)0)
    #define OSError_ReleaseResource_ResID()                         ((ResourceType)0)
    #define OSError_SetEvent_TaskID()                               ((TaskType)0)
    #define OSError_SetEvent_Mask()                                 ((EventMaskType)0)
    #define OSError_ClearEvent_Mask()                               ((EventMaskType)0)
    #define OSError_GetEvent_TaskID()                               ((TaskType)0)
    #define OSError_GetEvent_Event()                                ((EventMaskRefType)0)
    #define OSError_WaitEvent_Mask()                                ((EventMaskType)0)
    #define OSError_GetAlarmBase_AlarmID()                          ((AlarmType)0)
    #define OSError_GetAlarmBase_Info()                             ((AlarmBaseRefType)0)
    #define OSError_GetAlarm_AlarmID()                              ((AlarmType)0)
    #define OSError_GetAlarm_Tick()                                 ((TickRefType)0)
    #define OSError_SetRelAlarm_AlarmID()                           ((AlarmType)0)
    #define OSError_SetRelAlarm_increment()                         ((TickType)0)
    #define OSError_SetRelAlarm_cycle()                             ((TickType)0)
    #define OSError_SetAbsAlarm_AlarmID()                           ((AlarmType)0)
    #define OSError_SetAbsAlarm_start()                             ((TickType)0)
    #define OSError_SetAbsAlarm_cycle()                             ((TickType)0)
    #define OSError_CancelAlarm_AlarmID()                           ((AlarmType)0)
    #define OSError_CallTrustedFunction_FunctionIndex()             ((TrustedFunctionIndexType)0)
    #define OSError_CallTrustedFunction_FunctionParams()            ((TrustedFunctionParameterRefType)0)
    #define OSError_CheckISRMemoryAccess_ISRID()                    ((ISRType)0)
    #define OSError_CheckISRMemoryAccess_Address()                  ((MemoryStartAddressType)0)
    #define OSError_CheckISRMemoryAccess_Size()                     ((MemorySizeType)0)
    #define OSError_CheckTaskMemoryAccess_TaskID()                  ((TaskType)0)
    #define OSError_CheckTaskMemoryAccess_Address()                 ((MemoryStartAddressType)0)
    #define OSError_CheckTaskMemoryAccess_Size()                    ((MemorySizeType)0)
    #define OSError_CheckObjectAccess_ApplID()                      ((ApplicationType)0)
    #define OSError_CheckObjectAccess_ObjectType()                  ((ObjectTypeType)0)
    #define OSError_CheckObjectOwnership_ObjectType()               ((ObjectTypeType)0)
    #define OSError_StartScheduleTableRel_ScheduleTableID()         ((ScheduleTableType)0)
    #define OSError_StartScheduleTableRel_Offset()                  ((TickType)0)
    #define OSError_StartScheduleTableAbs_ScheduleTableID()         ((ScheduleTableType)0)
    #define OSError_StartScheduleTableAbs_Tickvalue()               ((TickType)0)
    #define OSError_StopScheduleTable_ScheduleTableID()             ((ScheduleTableType)0)
    #define OSError_NextScheduleTable_ScheduleTableID_From()        ((ScheduleTableType)0)
    #define OSError_NextScheduleTable_ScheduleTableID_To()          ((ScheduleTableType)0)
    #define OSError_StartScheduleTableSynchron_ScheduleTableID()    ((ScheduleTableType)0)
    #define OSError_SyncScheduleTable_SchedTableID()                ((ScheduleTableType)0)
    #define OSError_SyncScheduleTable_Value()                       ((TickType)0)
    #define OSError_GetScheduleTableStatus_ScheduleID()             ((ScheduleTableType)0)
    #define OSError_GetScheduleTableStatus_ScheduleStatus()         ((ScheduleTableStatusRefType)0)
    #define OSError_SetScheduleTableAsync_ScheduleID()              ((ScheduleTableType)0)
    #define OSError_IncrementCounter_CounterID()                    ((CounterType)0)
    #define OSError_TerminateApplication_RestartOption()            ((RestartType)0)
    #define OSError_DisableInterruptSource_DisableISR()             ((ISRType)0)
    #define OSError_EnableInterruptSource_EnableISR()               ((ISRType)0)

    #define OSError_InitCounter_CounterID()                         ((CounterType)0)
    #define OSError_InitCounter_InitialValue()                      ((TickType)0)
    #define OSError_CounterTrigger_CounterID()                      ((CounterType)0)
    #define OSError_GetCounterInfo_CounterID()                      ((CounterType)0)
    #define OSError_GetCounterInfo_Info()                           ((CtrInfoRefType)0)
    #define OSError_GetCounterValue_CounterID()                     ((CounterType)0)
    #define OSError_GetCounterValue_Value()                         ((TickRefType)0)
    #define OSError_GetElapsedCounterValue_CounterID()              ((CounterType)0)
    #define OSError_GetElapsedCounterValue_Value()                  ((TickRefType)0)
    #define OSError_GetElapsedCounterValue_ElapsedValue()           ((TickRefType)0)

    #define COM_Error_StartCOM_Mode()                               ((COMApplicationModeType)0)
    #define COM_Error_StopCOM_Mode()                                ((COMShutdownModeType)0)
    #define COM_Error_InitMessage_Message()                         ((MessageIdentifier)0)
    #define COM_Error_InitMessage_DataRef()                         ((ApplicationDataRef)0)
    #define COM_Error_SendMessage_Message()                         ((MessageIdentifier)0)
    #define COM_Error_SendMessage_DataRef()                         ((ApplicationDataRef)0)
    #define COM_Error_ReceiveMessage_Message()                      ((MessageIdentifier)0)
    #define COM_Error_ReceiveMessage_DataRef()                      ((ApplicationDataRef)0)
    #define COM_Error_SendDynamicMessage_Message()                  ((MessageIdentifier)0)
    #define COM_Error_SendDynamicMessage_DataRef()                  ((ApplicationDataRef)0)
    #define COM_Error_SendDynamicMessage_LengthRef()                ((LengthRef)0)
    #define COM_Error_ReceiveDynamicMessage_Message()               ((MessageIdentifier)0)
    #define COM_Error_ReceiveDynamicMessage_DataRef()               ((ApplicationDataRef)0)
    #define COM_Error_ReceiveDynamicMessage_LengthRef()             ((LengthRef)0)
    #define COM_Error_SendZeroMessage_Message()                     ((MessageIdentifier)0)
    #define COM_Error_GetMessageStatus_Message()                    ((MessageIdentifier)0)
#endif

#if (OS_FEATURE_GETSERVICEID == STD_ON) || (OS_FEATURE_PARAMETERACCESS == STD_ON) || (OS_FEATURE_ORTI_DEBUG == STD_ON)
typedef struct tagOs_ServiceContextType {
    #if (OS_FEATURE_GETSERVICEID == STD_ON) || (OS_FEATURE_ORTI_DEBUG == STD_ON)
    Os_ServiceIdType id;
    #endif
    #if (OS_FEATURE_PARAMETERACCESS == STD_ON)
    /*@null@*/ void *   param1;
    /*@null@*/ void *   param2;
    /*@null@*/ void *   param3;
    #endif
} Os_ServiceContextType;
#endif

OS_DECLARE_GLOBAL_IF_DEBUGGING(Os_ServiceContext, Os_ServiceContextType);
OS_DECLARE_GLOBAL_IF_DEBUGGING(OsLastError, StatusType);


/*********************************************************************************
    Service-Context-Functions.
 **********************************************************************************/

#if (OS_FEATURE_GETSERVICEID == STD_ON)
    #define OSErrorGetServiceId()   Os_ServiceContext.id
    #define Os_ClearServiceContext() Os_ServiceContext.id = OSServiceId_NoService
#else
    #define OSErrorGetServiceId()   ((OS_ServiceIdType)0)
    #define Os_ClearServiceContext()
#endif

#if KOS_MEMORY_MAPPING == STD_ON
#if (OS_FEATURE_GETSERVICEID == STD_ON) && (OS_FEATURE_PARAMETERACCESS == STD_ON)
FUNC(void, OSEK_OS_CODE) OS_SaveServiceContext(Os_ServiceIdType id,
                                              /*@null@*//*@in@*/ P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param1,
                                              /*@null@*//*@in@*/ P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param2,
                                              /*@null@*//*@in@*/ P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param3
                                              );

#endif

#if (OS_FEATURE_GETSERVICEID == STD_ON) && !(OS_FEATURE_PARAMETERACCESS == STD_ON)
FUNC(void, OSEK_OS_CODE) OS_SaveServiceContext(Os_ServiceIdType id);

#endif

#if !(OS_FEATURE_GETSERVICEID == STD_ON) && (OS_FEATURE_PARAMETERACCESS == STD_ON)
FUNC(void, OSEK_OS_CODE) OS_SaveServiceContext(
    /*@null@*//*@in@*/ P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param1,
    /*@null@*//*@in@*/ P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param2,
    /*@null@*//*@in@*/ P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param3
    );

#endif
#else
#if (OS_FEATURE_GETSERVICEID == STD_ON) && (OS_FEATURE_PARAMETERACCESS == STD_ON)
void OS_SaveServiceContext(Os_ServiceIdType          id,
                          /*@null@*//*@in@*/ void * param1,
                          /*@null@*//*@in@*/ void * param2,
                          /*@null@*//*@in@*/ void * param3
                          );


#endif

#if (OS_FEATURE_GETSERVICEID == STD_ON) && !(OS_FEATURE_PARAMETERACCESS == STD_ON)
void OS_SaveServiceContext(Os_ServiceIdType id);


#endif

#if !(OS_FEATURE_GETSERVICEID == STD_ON) && (OS_FEATURE_PARAMETERACCESS == STD_ON)
void OS_SaveServiceContext(
    /*@null@*//*@in@*/ void *   param1,
    /*@null@*//*@in@*/ void *   param2,
    /*@null@*//*@in@*/ void *   param3
    );


#endif
#endif /* KOS_MEMORY_MAPPING */

#if !(OS_FEATURE_GETSERVICEID == STD_ON) && !(OS_FEATURE_PARAMETERACCESS == STD_ON)
#define Os_SaveServiceContext(id, param1, param2, param3)
#else
    #if (OS_FEATURE_GETSERVICEID == STD_ON) && (OS_FEATURE_PARAMETERACCESS == STD_ON)
        #define Os_SaveServiceContext(id, param1, param2, param3) \
    OS_SaveServiceContext(id, (void *)param1, (void *)param2, (void *)param3)
    #endif
    #if (OS_FEATURE_GETSERVICEID == STD_ON) && !(OS_FEATURE_PARAMETERACCESS == STD_ON)
        #define Os_SaveServiceContext(id, param1, param2, param3) \
    OS_SaveServiceContext(id)
    #endif
    #if !(OS_FEATURE_GETSERVICEID == STD_ON) && (OS_FEATURE_PARAMETERACCESS == STD_ON)
        #define Os_SaveServiceContext(id, param1, param2, param3) \
    OS_SaveServiceContext((void *)param1, (void *)param2, (void *)param3)
    #endif
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /*  OS_PARAMACCESS */
