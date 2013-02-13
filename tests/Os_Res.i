
%module Os_Res

%include Types.i

%{
#include "Osek.h"
%}

%inline {
StatusType  GetResource(ResourceType ResID);
StatusType  ReleaseResource(ResourceType ResID);
void    OsRes_InitResources(void);
//void    OsRes_GetInternalResource(void);
//void    OsRes_ReleaseInternalResource(void);
}



