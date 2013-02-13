
%module Os_Evt

%include Types.i

%{
#include "Osek.h"
%}

%inline{
StatusType  SetEvent(TaskType TaskID, EventMaskType Mask);
StatusType  ClearEvent(EventMaskType Mask);
StatusType  GetEvent(TaskType TaskID, EventMaskRefType Event);
StatusType  WaitEvent(EventMaskType Mask);
}

