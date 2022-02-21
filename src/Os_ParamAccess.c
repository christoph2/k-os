/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2018 by Christoph Schueler <github.com/Christoph2,
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
#include "Os_ParamAccess.h"


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
#if (OS_FEATURE_GETSERVICEID == STD_ON) && (OS_FEATURE_PARAMETERACCESS == STD_ON)
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OS_SaveServiceContext(Os_ServiceIdType id,
                                              P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param1,
                                              P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param2,
                                              P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA) param3
                                              )
#else
void OS_SaveServiceContext(Os_ServiceIdType id, void * param1, void * param2, void * param3)
#endif /* KOS_MEMORY_MAPPING */
{
    Os_ServiceContext.id       = id;
    Os_ServiceContext.param1   = param1;
    Os_ServiceContext.param2   = param2;
    Os_ServiceContext.param3   = param3;
}
#endif


#if (OS_FEATURE_GETSERVICEID == STD_ON) && !(OS_FEATURE_PARAMETERACCESS == STD_ON)
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) void OS_SaveServiceContext(Os_ServiceIdType id)
#else
void OS_SaveServiceContext(Os_ServiceIdType id)
#endif /* KOS_MEMORY_MAPPING */
{
    Os_ServiceContext.id = id;
}
#endif


#if !(OS_FEATURE_GETSERVICEID == STD_ON) && (OS_FEATURE_PARAMETERACCESS == STD_ON)
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) void OS_SaveServiceContext(P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA)param1,
                                                   P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA)param2,
                                                   P2VAR(void, AUTOMATIC, OSEK_OS_APPL_DATA)param3
                                                   )
#else
void OS_SaveServiceContext(void * param1, void * param2, void * param3)
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

