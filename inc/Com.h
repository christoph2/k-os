/*
 * k_dk - Driver Kit for k_os (Konnex Operating-System based on the
 * OSEK/VDX-Standard).
 *
 * (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#if !defined(__COM_H)
#define __COM_H

#include "Os_Types.h"

#define DeclareComCallback(ComCb)   extern void ComCallback ## ComCb(void)
#define COMCallback(ComCb)          void ComCallback ## ComCb(void)
#define GetComCallbackName(ComCb)   ComCallback ## ComCb

#define DeclareComCallout(ComCo)    extern void ComCallout ## ComCo(void)
#define COMCallout(ComCo)           void ComCallout ## ComCo(void)
#define GetComCalloutName(ComCo)    ComCallout ## ComCo

/*
**      Datatypes.
*/
typedef enum tagCOMBool {COM_FALSE, COM_TRUE} COMBool;

typedef uint16                                                  MessageIdentifier;
typedef void *                                                  ApplicationDataRef;
typedef uint8                                                   COMLengthType;
typedef COMLengthType *                                         LengthRef;
typedef COMBool                                                 FlagValue;
typedef uint8                                                   COMApplicationModeType;
typedef enum tagCOMShutdownModeType {COM_SHUTDOWN_IMMEDIATE}    COMShutdownModeType;
typedef COMBool                                                 CalloutReturnType;
/*  typedef enum {COMServiceId_SendMessage,COMServiceId_xx} COMServiceIdType; */
typedef uint8 COMFlagType;

typedef void COMServiceIdType;

typedef void (*COMCallbackType)(void);
typedef void (*COMCalloutType)(void);

typedef enum tagCom_FilterAlgorithm {
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
} Com_FilterAlgorithm;

typedef struct tagCom_MessageSetEventType {
    TaskType        TaskID;
    EventMaskType   Mask;
} Com_MessageSetEventType;

typedef enum tagCom_MessageNotificationType {
    COM_NOTIFY_NONE,
    COM_ACTIVATETASK,
    COM_SETEVENT,
    COM_COMCALLBACK,
    COM_FLAG,
    COM_COMINMCALLBACK
} Com_MessageNotificationType;

typedef struct tagCom_ReceiverType {
    MessageIdentifier Message;
} Com_ReceiverType;

typedef enum tagCom_MessagePropertyType {
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
} Com_MessagePropertyType;

typedef union tagCom_MessageActionType {
    void *                      Dummy;
    Com_MessageSetEventType *   Event;
    TaskType                    TaskID;
    COMCallbackType             Callback;
    COMFlagType                 Flag;
} Com_MessageActionType;

typedef struct tagCom_MessageObjectType {
    Com_MessagePropertyType     Property;
    Com_MessageNotificationType Notification;
/*    void *Action; */
    Com_MessageActionType   Action;
    uint8                   Size;
#if 0
    uint8 Flag; /* State */
#endif
    const ApplicationDataRef *  Data;
    uint8                       NumReceivers;
    const Com_ReceiverType *    Receiver;
/*
    todo: 'TimeoutValue','UseTimeout' .
 */
} Com_MessageObjectType;

/*
**
**  AUTOSAR-Com.
**
*/
typedef enum tagCom_StatusType {
    COM_UNINIT,
    COM_INIT
}  Com_StatusType;

typedef uint8   Com_SignalIdType;
typedef uint8   Com_SignalGroupIdType;
typedef void *  Com_ApplicationDataRefType;
typedef uint8   Com_PduGroupIdType;

#if 0
typedef struct tagCom_ComfigType {
/* Implementation specific */
} Com_ComfigType;
#endif

/*
**      Function-Prototypes.
*/

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_COM_CODE)	StartCOM(COMApplicationModeType Mode);
FUNC(StatusType, OSEK_COM_CODE)	StopCOM(COMShutdownModeType Mode);
FUNC(COMApplicationModeType, OSEK_COM_CODE) GetCOMApplicationMode(void);
FUNC(StatusType, OSEK_COM_CODE)	InitMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
FUNC(StatusType, OSEK_COM_CODE)	StartPeriodic(void);
FUNC(StatusType, OSEK_COM_CODE)	StopPeriodic(void);
FUNC(StatusType, OSEK_COM_CODE)	SendMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
FUNC(StatusType, OSEK_COM_CODE)	SendDynamicMessage(MessageIdentifier Message, ApplicationDataRef DataRef, LengthRef LengthRef);
FUNC(StatusType, OSEK_COM_CODE)	SendZeroMessage(MessageIdentifier Message);
FUNC(StatusType, OSEK_COM_CODE)	GetMessageStatus(MessageIdentifier Message);
FUNC(StatusType, OSEK_COM_CODE)	ReceiveMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
FUNC(StatusType, OSEK_COM_CODE)	ReceiveDynamicMessage(MessageIdentifier Message, ApplicationDataRef DataRef, LengthRef LengthRef);
FUNC(COMServiceIdType, OSEK_COM_CODE) COMErrorGetServiceId(void);

/*  Routines provided by the application. */
StatusType StartCOMExtension(void);
#else
StatusType              StartCOM(COMApplicationModeType Mode);
StatusType              StopCOM(COMShutdownModeType Mode);
COMApplicationModeType  GetCOMApplicationMode(void);
StatusType              InitMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
StatusType              StartPeriodic(void);
StatusType              StopPeriodic(void);
StatusType              SendMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
StatusType              SendDynamicMessage(MessageIdentifier Message, ApplicationDataRef DataRef, LengthRef LengthRef);
StatusType              SendZeroMessage(MessageIdentifier Message);
StatusType              GetMessageStatus(MessageIdentifier Message);
StatusType              ReceiveMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
StatusType              ReceiveDynamicMessage(MessageIdentifier Message, ApplicationDataRef DataRef, LengthRef LengthRef);
COMServiceIdType        COMErrorGetServiceId(void);

/*  Routines provided by the application. */
StatusType              StartCOMExtension(void);
#endif /* KOS_MEMORY_MAPPING */

extern const Com_MessageObjectType Com_MessageObjects[];

#endif  /*  __COM_H */
