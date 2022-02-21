/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

 * (C) 2007-2018 by Christoph Schueler <github.com/Christoph2,
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

#if (OS_FEATURE_GETSERVICEID == STD_ON) || (OS_FEATURE_PARAMETERACCESS == STD_ON) || (OS_FEATURE_ORTI_DEBUG == STD_ON)
OS_DEFINE_GLOBAL_IF_DEBUGGING(Os_ServiceContext, Os_ServiceContextType);

OS_DEFINE_GLOBAL_IF_DEBUGGING(OsLastError, StatusType);
#endif

#if (OS_FEATURE_ORTI_DEBUG == STD_ON)
#define OsError_SaveLastError(Error) (OsLastError = Error)
#else
#define OsError_SaveLastError(Error)
#endif

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Global functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsError_CallErrorHook(StatusType Error)
#else
void OsError_CallErrorHook(StatusType Error)
#endif /* KOS_MEMORY_MAPPING */
{
    if (((Os_Flags & OS_SYS_FLAG_IN_OS_ERROR_HOOK) != OS_SYS_FLAG_IN_OS_ERROR_HOOK)) {
        OsPort_DisableAllOsInterrupts();
        Os_Flags |= OS_SYS_FLAG_IN_OS_ERROR_HOOK;
        Os_SaveCallLevel(); /* s. 'os_alm' !!! */
        Os_SetCallLevel(OS_CL_ERROR_HOOK);
        OsError_SaveLastError(Error);
        ErrorHook(Error);
        Os_RestoreCallLevel();
        Os_Flags &= (uint8)~OS_SYS_FLAG_IN_OS_ERROR_HOOK;
        OsPort_EnableAllOsInterrupts();
    }
}


#if OS_FEATURE_COM == STD_ON
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) COM_ErrorCallErrorHook(StatusType Error)
#else
void COM_ErrorCallErrorHook(StatusType Error)
#endif /* KOS_MEMORY_MAPPING */
{
    if (((Os_Flags & OS_SYS_FLAG_IN_COM_ERROR_HOOK) != OS_SYS_FLAG_IN_COM_ERROR_HOOK)) {
        OsPort_DisableAllOsInterrupts();
        Os_Flags |= OS_SYS_FLAG_IN_COM_ERROR_HOOK;
        Os_SaveCallLevel();
        Os_SetCallLevel(OS_CL_ERROR_HOOK);
        OsError_SaveLastError(Error);
        COMErrorHook(Error);
        Os_RestoreCallLevel();
        Os_Flags &= (uint8)~OS_SYS_FLAG_IN_COM_ERROR_HOOK;
        OsPort_EnableAllOsInterrupts();
    }
}
#endif /* OS_FEATURE_COM */


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

