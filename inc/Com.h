/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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
#if !defined(__COM_H)
#define __COM_H

#include "Os_Types.h"

#define DeclareComCallback(ComCb) extern void ComCallback ## ComCb(void)
#define COMCallback(ComCb) void ComCallback ## ComCb(void)
#define GetComCallbackName(ComCb)   ComCallback ## ComCb

#define DeclareComCallout(ComCo) extern void ComCallout ## ComCo(void)
#define COMCallout(ComCo) void ComCallout ## ComCo(void)
#define GetComCalloutName(ComCo)   ComCallout ## ComCo

/*
**      Datatypes.
*/
typedef enum tagCOMBool {COM_FALSE, COM_TRUE} COMBool;

typedef uint16 MessageIdentifier;
typedef void* ApplicationDataRef;
typedef uint8 COMLengthType;
typedef COMLengthType *LengthRef;
typedef COMBool FlagValue;
typedef uint8 COMApplicationModeType;
typedef enum tagCOMShutdownModeType {COM_SHUTDOWN_IMMEDIATE} COMShutdownModeType;
typedef COMBool CalloutReturnType;
/*  typedef enum {COMServiceId_SendMessage,COMServiceId_xx} COMServiceIdType; */

typedef uint8 COMFlagType;

typedef void COMServiceIdType;  /*  !!! NUR Vorrübergehend !!!  */



typedef void (*COMCallbackType)(void);
typedef void (*COMCalloutType)(void);

/*
**      Function-Prototypes.
*/
StatusType StartCOM(COMApplicationModeType Mode);
StatusType StopCOM(COMShutdownModeType Mode);
COMApplicationModeType GetCOMApplicationMode(void);
StatusType InitMessage(MessageIdentifier Message,ApplicationDataRef DataRef);
StatusType StartPeriodic(void); 
StatusType StopPeriodic(void);

/*  FlagValue ReadFlag_<Flag>();    */
/*  void ResetFlag_<Flag>();  */

StatusType SendMessage(MessageIdentifier Message,ApplicationDataRef DataRef);
StatusType SendDynamicMessage(MessageIdentifier Message,ApplicationDataRef DataRef,LengthRef LengthRef);
StatusType SendZeroMessage(MessageIdentifier Message);
StatusType GetMessageStatus(MessageIdentifier Message);        
StatusType ReceiveMessage(MessageIdentifier Message,ApplicationDataRef DataRef);
StatusType ReceiveDynamicMessage(MessageIdentifier Message,ApplicationDataRef DataRef,LengthRef LengthRef);
COMServiceIdType COMErrorGetServiceId(void);

/*  Routines provided by the application. */
StatusType StartCOMExtension(void);

/* todo: Filter-Algorithmen in einem Funktions-Pointer-Array ablegen!!! */
/*
F_Always True No filtering is performed so that the message always passes
F_Never False The filter removes all messages
F_MaskedNewEqualsX (new_value&mask) == x Pass messages whose masked value is equal to a specific value
F_MaskedNewDiffersX (new_value&mask) != x Pass messages whose masked value is not equal to a specific value
F_NewIsEqual new_value == old_value Pass messages which have not changed
F_NewIsDifferent new_value != old_value Pass messages which have changed
F_MaskedNewEqualsMaskedOld (new_value&mask) ==(old_value&mask) Pass messages where the masked value has not changed
F_MaskedNewDiffersMaskedOld (new_value&mask) != (old_value&mask) Pass messages where the masked value has changed
F_NewIsWithin min <= new_value <=max Pass a message if its value is within a predefined boundary
F_NewIsOutside (min > new_value) OR (new_value > max) Pass a message if its value is outside a predefined boundary
F_NewIsGreater new_value > old_value Pass a message if its value has increased
F_NewIsLessOrEqual new_value <= old_value Pass a message if its value has not increased
F_NewIsLess new_value < old_value Pass a message if its value has decreased
F_NewIsGreaterOrEqual new_value >= old_value Pass a message if its value has not decreased
F_OneEveryN occurrence % period == offset Pass a message once every N message occurrences. Start: occurrence = 0. Each time the message is received or transmitted, occurrence is incremented by 1 after filtering. Length of occurrence is 8 bit (minimum).
*/

typedef enum tagComFilterAlgorithm {
    COM_FILTER_ALWAYS,
    COM_FILTER_NEVER,
    COM_FILTER_MASKEDNEWEQUALSX,
    COM_FILTER_MASKEDNEWDIFFERSX,
    COM_FILTER_NEWISEQUAL,
    COM_FILTER_NEWISDIFFERENT,
    COM_FILTER_MASKEDNEWEQUALSMASKEDOLD,
    COM_FILTER_MASKEDNEWDIFFERSMASKEDOLD,
    COM_FILTER_NEWISWITHIN,
    COM_FILTER_NEWISOUTSIDE,
    COM_FILTER_NEWISGREATER,
    COM_FILTER_NEWISLESSOREQUAL,
    COM_FILTER_NEWISLESS,
    COM_FILTER_NEWISGREATEROREQUAL,
    COM_FILTER_ONEEVERYN
} ComFilterAlgorithm;

typedef struct tagMessageSetEventType {
    TaskType TaskID;
    EventMaskType Mask;
} MessageSetEventType;


typedef enum tagComMessageNotificationType {
    COM_NOTIFY_NONE,
    COM_ACTIVATETASK,
    COM_SETEVENT,
    COM_COMCALLBACK,
    COM_FLAG
} ComMessageNotificationType;

typedef struct tagComReceiverType {
    MessageIdentifier Message;
} ComReceiverType;

typedef enum tagComMessagePropertyType {
    /* internal Messages */
    SEND_STATIC_INTERNAL,
    RECEIVE_UNQUEUED_INTERNAL,
    RECEIVE_QUEUED_INTERNAL,
    SEND_ZERO_INTERNAL,
    RECEIVE_ZERO_INTERNAL,
    /* external Messages */
    SEND_STATIC_EXTERNAL,
    SEND_DYNAMIC_EXTERNAL,
    SEND_ZERO_EXTERNAL,
    RECEIVE_ZERO_EXTERNAL,
    RECEIVE_UNQUEUED_EXTERNAL,
    RECEIVE_QUEUED_EXTERNAL,
    RECEIVE_DYNAMIC_EXTERNAL
} ComMessagePropertyType;

typedef union tagMessageActionType {
    void *Dummy;
    MessageSetEventType *Event;
    TaskType TaskID;
    COMCallbackType Callback;
    COMFlagType Flag;
} MessageActionType;

typedef struct tagComMessageObjectType {
    ComMessagePropertyType Property;
    ComMessageNotificationType Notification;
/*    void *Action; */
    MessageActionType Action;
    uint8 Size;
#if 0    
    uint8 Flag; /* State */
#endif    
    const ApplicationDataRef *Data;
    uint8 NumReceivers;
    const ComReceiverType *Receiver;
/*
    todo: 'TimeoutValue','UseTimeout' .
*/    
} ComMessageObjectType;

/*
**
**  AUTOSAR-Com.
**
*/
typedef enum tagCom_StatusType {
    COM_UNINIT,
    COM_INIT
}  Com_StatusType;

typedef uint8 Com_SignalIdType;
typedef uint8 Com_SignalGroupIdType;
typedef void* Com_ApplicationDataRefType;
typedef uint8 Com_PduGroupIdType;

#if 0
typedef struct tagCom_ComfigType {
/* Implementation specific */    
} Com_ComfigType;
#endif

#endif  /*  __COM_H */

