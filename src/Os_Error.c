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
#define OS_SAVE_LAST_ERROR(Error) (OsLastError = Error)
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
    if (((Os_Flags & OS_SYS_FLAG_IN_OS_ERROR_HOOK) != OS_SYS_FLAG_IN_OS_ERROR_HOOK)) {
        DISABLE_ALL_OS_INTERRUPTS();
        Os_Flags |= OS_SYS_FLAG_IN_OS_ERROR_HOOK;
        OS_SAVE_CALLEVEL(); /* s. 'os_alm' !!! */
        OS_SET_CALLEVEL(OS_CL_ERROR_HOOK);
        OS_SAVE_LAST_ERROR(Error);
        ErrorHook(Error);
        OS_RESTORE_CALLEVEL();
        Os_Flags &= (uint8)~OS_SYS_FLAG_IN_OS_ERROR_HOOK;
        ENABLE_ALL_OS_INTERRUPTS();
    }
}


#if OS_FEATURE_COM == STD_ON
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) COMErrorCallErrorHook(StatusType Error)
#else
void COMErrorCallErrorHook(StatusType Error)
#endif /* KOS_MEMORY_MAPPING */
{
    if (((Os_Flags & OS_SYS_FLAG_IN_COM_ERROR_HOOK) != OS_SYS_FLAG_IN_COM_ERROR_HOOK)) {
        DISABLE_ALL_OS_INTERRUPTS();
        Os_Flags |= OS_SYS_FLAG_IN_COM_ERROR_HOOK;
        OS_SAVE_CALLEVEL();
        OS_SET_CALLEVEL(OS_CL_ERROR_HOOK);
        OS_SAVE_LAST_ERROR(Error);
        COMErrorHook(Error);
        OS_RESTORE_CALLEVEL();
        Os_Flags &= (uint8)~OS_SYS_FLAG_IN_COM_ERROR_HOOK;
        ENABLE_ALL_OS_INTERRUPTS();
    }
}
#endif /* OS_FEATURE_COM */


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_OS_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

