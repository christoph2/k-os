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
#include "Os_Error.h"
#include "Os_Vars.h"

#if defined(OS_USE_GETSERVICEID) || defined(OS_USE_PARAMETERACCESS) || defined(OS_FEATURE_ORTI_DEBUG)
OS_DEFINE_GLOBAL_IF_DEBUGGING(Os_ServiceContext, Os_ServiceContextType);

OS_DEFINE_GLOBAL_IF_DEBUGGING(OsLastError, StatusType);
#endif

#if defined(OS_FEATURE_ORTI_DEBUG)
#define OS_SAVE_LAST_ERROR(Error) OsLastError = Error
#else
#define OS_SAVE_LAST_ERROR(Error)
#endif


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Global functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsErrorCallErrorHook(StatusType Error)
#else
void OsErrorCallErrorHook(StatusType Error)
#endif /* KOS_MEMORY_MAPPING */
{
    if (((OsFlags & OS_SYS_FLAG_IN_OS_ERROR_HOOK) != OS_SYS_FLAG_IN_OS_ERROR_HOOK)) {
        DISABLE_ALL_OS_INTERRUPTS();
        OsFlags |= OS_SYS_FLAG_IN_OS_ERROR_HOOK;
        OS_SAVE_CALLEVEL(); /* s. 'os_alm' !!! */
        OS_SET_CALLEVEL(OS_CL_ERROR_HOOK);
        OS_SAVE_LAST_ERROR(Error);
        ErrorHook(Error);
        OS_RESTORE_CALLEVEL();
        OsFlags &= ~OS_SYS_FLAG_IN_OS_ERROR_HOOK;
        ENABLE_ALL_OS_INTERRUPTS();
    }
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) COMErrorCallErrorHook(StatusType Error)
#else
void COMErrorCallErrorHook(StatusType Error)
#endif /* KOS_MEMORY_MAPPING */
{
    if (((OsFlags & OS_SYS_FLAG_IN_COM_ERROR_HOOK) != OS_SYS_FLAG_IN_COM_ERROR_HOOK)) {
        DISABLE_ALL_OS_INTERRUPTS();
        OsFlags |= OS_SYS_FLAG_IN_COM_ERROR_HOOK;
        OS_SAVE_CALLEVEL();
        OS_SET_CALLEVEL(OS_CL_ERROR_HOOK);
        OS_SAVE_LAST_ERROR(Error);
        COMErrorHook(Error);
        OS_RESTORE_CALLEVEL();
        OsFlags &= ~OS_SYS_FLAG_IN_COM_ERROR_HOOK;
        ENABLE_ALL_OS_INTERRUPTS();
    }
}


#if defined(OS_USE_GETSERVICEID) && defined(OS_USE_PARAMETERACCESS)
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OSSaveServiceContext(Os_ServiceIdType id,
    P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param1,
    P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param2,
    P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param3
)
#else
void OSSaveServiceContext(Os_ServiceIdType id, void * param1, void * param2, void * param3)
#endif /* KOS_MEMORY_MAPPING */
{
    Os_ServiceContext.id       = id;
    Os_ServiceContext.param1   = param1;
    Os_ServiceContext.param2   = param2;
    Os_ServiceContext.param3   = param3;
}
#endif


#if defined(OS_USE_GETSERVICEID) && !defined(OS_USE_PARAMETERACCESS)
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) void OSSaveServiceContext(Os_ServiceIdType id)
#else
void OSSaveServiceContext(Os_ServiceIdType id)
#endif /* KOS_MEMORY_MAPPING */
{
    Os_ServiceContext.id = id;
}
#endif

#if !defined(OS_USE_GETSERVICEID) && defined(OS_USE_PARAMETERACCESS)
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) void OSSaveServiceContext(
    P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param1,
    P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param2,
    P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA)param3
)
#else
void OSSaveServiceContext(void * param1, void * param2, void * param3)
#endif /* KOS_MEMORY_MAPPING */
{
    Os_ServiceContext.param1   = param1;
    Os_ServiceContext.param2   = param2;
    Os_ServiceContext.param3   = param3;
}
#endif

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
