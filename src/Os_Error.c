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
#include "Os_Error.h"

#if defined(OS_USE_GETSERVICEID) || defined(OS_USE_PARAMETERACCESS) || defined(OS_FEATURE_ORTI_DEBUG)
OS_DEFINE_GLOBAL_IF_DEBUGGING(Os_ServiceContext,Os_ServiceContextType);

OS_DEFINE_GLOBAL_IF_DEBUGGING(OsLastError,StatusType);
#endif

#if defined(OS_FEATURE_ORTI_DEBUG)
#define OS_SAVE_LAST_ERROR(Error)   OsLastError=Error
#else
#define OS_SAVE_LAST_ERROR(Error)
#endif


void OsErrorCallErrorHook(StatusType Error)
{
    if (((OsFlags & OS_SYS_FLAG_IN_OS_ERROR_HOOK)!=OS_SYS_FLAG_IN_OS_ERROR_HOOK)) {
        DISABLE_ALL_OS_INTERRUPTS();
        OsFlags|=OS_SYS_FLAG_IN_OS_ERROR_HOOK;
        OS_SAVE_CALLEVEL(); /* s. 'os_alm' !!! */
        OS_SET_CALLEVEL(OS_CL_ERROR_HOOK);
        OS_SAVE_LAST_ERROR(Error);
        ErrorHook(Error);
        OS_RESTORE_CALLEVEL();
        OsFlags&=~OS_SYS_FLAG_IN_OS_ERROR_HOOK;
        ENABLE_ALL_OS_INTERRUPTS();
    }
}


void COMErrorCallErrorHook(StatusType Error)
{
    if (((OsFlags & OS_SYS_FLAG_IN_COM_ERROR_HOOK)!=OS_SYS_FLAG_IN_COM_ERROR_HOOK)) {
        DISABLE_ALL_OS_INTERRUPTS();
        OsFlags|=OS_SYS_FLAG_IN_COM_ERROR_HOOK;
        OS_SAVE_CALLEVEL();
        OS_SET_CALLEVEL(OS_CL_ERROR_HOOK);
        OS_SAVE_LAST_ERROR(Error);
        COMErrorHook(Error);
        OS_RESTORE_CALLEVEL();
        OsFlags&=~OS_SYS_FLAG_IN_COM_ERROR_HOOK;
        ENABLE_ALL_OS_INTERRUPTS();
    }
}


#if defined(OS_USE_GETSERVICEID) && defined(OS_USE_PARAMETERACCESS)
void OSSaveServiceContext(Os_ServiceIdType id,void *param1,void *param2,void *param3)
{
        Os_ServiceContext.id=id;
        Os_ServiceContext.param1=param1;
        Os_ServiceContext.param2=param2;
        Os_ServiceContext.param3=param3;
}
#endif


#if defined(OS_USE_GETSERVICEID) && !defined(OS_USE_PARAMETERACCESS)
void OSSaveServiceContext(Os_ServiceIdType id)
{
        Os_ServiceContext.id=id;
}
#endif


#if !defined(OS_USE_GETSERVICEID) && defined(OS_USE_PARAMETERACCESS)
void OSSaveServiceContext(void *param1,void *param2,void *param3)
{
        Os_ServiceContext.param1=param1;
        Os_ServiceContext.param2=param2;
        Os_ServiceContext.param3=param3;
}
#endif
