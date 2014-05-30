/*
 * k_dk - Driver Kit for k_os (Konnex Operating-System based on the
 * OSEK/VDX-Standard).
 *
 * (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
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

/** @file Com.h
 *  @brief Function, types, and definitions related to OSEK COM v3.0.3
 *
 *  
 *
 *  @author Christoph Schueler (cpu12.gems@googlemail.com)
 */

/** @page callbacks Callbacks
 *
 *  A callback is one of the notification mechanisms that can be invoked in response to an event in the IL. 
 *  No parameters are passed to a callback routine and they do not have a return value.
 *  A callback routine runs either on interrupt level or on task level. Thus, the OS restrictions of
 *  usage of system functions for interrupt service routines as well as for tasks apply.
 */


#if 0
COMCallback(CallbackRoutineName)
{
}
#endif

#if !defined(__COM_H)
#define __COM_H

#include "Os_Types.h"

/**
 *  @def DeclareComCallback(ComCb)
 *  Declares a @ref callbacks callback function.
*/
#define DeclareComCallback(ComCb)   extern void ComCallback ## ComCb(void)

/**
 *  @def COMCallback(ComCb)
 *  Use COMCallback to implement a @ref callbacks callback function.
 *
 *  e.g. 
 *  COMCallback(CallbackRoutineName)
 *  {
 *  }
*/
#define COMCallback(ComCb)          void ComCallback ## ComCb(void)
#define GetComCallbackName(ComCb)   ComCallback ## ComCb

#define DeclareComCallout(ComCo)    extern void ComCallout ## ComCo(void)
#define COMCallout(ComCo)           void ComCallout ## ComCo(void)
#define GetComCalloutName(ComCo)    ComCallout ## ComCo

/*
**      Datatypes.
*/

/** @enum The boolean datatype used by OSEK COM.
 *  
 */
typedef enum tagCOMBool {COM_FALSE, COM_TRUE} COMBool;


/** @typedef
 *  
 */

/** @typedef MessageIdentifier
 *  OSEK COM message object identifier.
 */
typedef uint16                                                  MessageIdentifier;

/** @typedef ApplicationDataRef
 *  Reference to a data field in the application.
 *
 *  Pointer to the data field of an application message.
 */
typedef void *                                                  ApplicationDataRef;

/** @typedef COMLengthType
 *  Data length.
 */
typedef uint8                                                   COMLengthType;

/** @typedef LengthRef
 *  Pointer to a data field containing length information.
 */
typedef COMLengthType *                                         LengthRef;

/** @typedef FlagValue
 *  Current state of a message flag.
 */
typedef COMBool                                                 FlagValue;

/** @typedef COMApplicationModeType
 *  Identifier for selected COM application mode.
 */
typedef uint8                                                   COMApplicationModeType;

/** @enum COMShutdownModeType
 *  Identifier for selected COM shutdown mode.
 */
typedef enum tagCOMShutdownModeType {COM_SHUTDOWN_IMMEDIATE}    COMShutdownModeType;

/** @typedef CalloutReturnType
 *  Indicates at the exit of a callout whether the IL shall continue or abandon further
 *  processing of the current message or I-PDU.
 */
typedef COMBool                                                 CalloutReturnType;
/*  typedef enum {COMServiceId_SendMessage,COMServiceId_xx} COMServiceIdType; */

/** @typedef COMFlagType
 *  
 */
typedef uint8 COMFlagType;

/** @typedef COMServiceIdType
 *  Unique identifier of an OSEK COM service. 
 *  Example: COMServiceId_SendMessage.
 */
typedef void COMServiceIdType;

/** @typedef COMCallbackType
 *  Function pointer to @ref callbacks callback.
 */
typedef void (*COMCallbackType)(void);

/** @typedef COMCalloutType
 *  Function pointer to callout.
 */
typedef void (*COMCalloutType)(void);

/** @enum Com_FilterAlgorithm
 *  Filters define conditions wheter or not to discard messages.
 */
typedef enum tagCom_FilterAlgorithm {
    COM_FILTER_ALWAYS,                      /**< No filtering is performed so that the message always passes */
    COM_FILTER_NEVER,                       /**< The filter removes all messages */
    COM_FILTER_MASKEDNEWEQUALSX,            /**< Pass messages whose masked value is equal to a specific value */
    COM_FILTER_MASKEDNEWDIFFERSX,           /**< Pass messages whose masked value is not equal to a specific value */
    COM_FILTER_NEWISEQUAL,                  /**< Pass messages which have not changed */
    COM_FILTER_NEWISDIFFERENT,              /**< Pass messages which have changed */
    COM_FILTER_MASKEDNEWEQUALSMASKEDOLD,    /**< Pass messages where the masked value has not changed */
    COM_FILTER_MASKEDNEWDIFFERSMASKEDOLD,   /**< Pass messages where the masked value has changed */
    COM_FILTER_NEWISWITHIN,                 /**< Pass a message if its value is within a predefined boundary */
    COM_FILTER_NEWISOUTSIDE,                /**< Pass a message if its value is outside a predefined boundary */
    COM_FILTER_NEWISGREATER,                /**< Pass a message if its value has increased */
    COM_FILTER_NEWISLESSOREQUAL,            /**< Pass a message if its value has not increased */
    COM_FILTER_NEWISLESS,                   /**< Pass a message if its value has decreased */
    COM_FILTER_NEWISGREATEROREQUAL,         /**< Pass a message if its value has not decreased */
    COM_FILTER_ONEEVERYN                    /**< Start: occurrence = 0.
                                                Each time the message is received or
                                                transmitted, occurrence is incremented by 1
                                                after filtering.
                                                Length of occurrence is 8 bit (minimum). */
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

typedef enum tagCom_TransmissionMode {
    TRANSMISSION_MODE_DIRECT,
    TRANSMISSION_MODE_PERIODIC,
    TRANSMISSION_MODE_MIXED
} Com_TransmissionMode;

typedef enum tagCom_TransferProperty {
    TRANSFER_PROPERTY_TRIGGERED,
    TRANSFER_PROPERTY_PENDING
} Com_TransferProperty;

typedef union tagCom_MessageActionType {
    void *                      Dummy;
    Com_MessageSetEventType *   Event;
    TaskType                    TaskID;
    COMCallbackType             Callback;
    COMFlagType                 Flag;
} Com_MessageActionType;


typedef struct tagCom_AddressInformation {
    uint8   Bus;
    uint32  Address;
} Com_AddressInformation;


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

    Com_AddressInformation  const * const   Address;    // TODO: Nur CCC[0|1] !!!
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
FUNC(StatusType, OSEK_COM_CODE) StartCOM(COMApplicationModeType Mode);
FUNC(StatusType, OSEK_COM_CODE) StopCOM(COMShutdownModeType Mode);
FUNC(COMApplicationModeType, OSEK_COM_CODE) GetCOMApplicationMode(void);
FUNC(StatusType, OSEK_COM_CODE) InitMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
FUNC(StatusType, OSEK_COM_CODE) StartPeriodic(void);
FUNC(StatusType, OSEK_COM_CODE) StopPeriodic(void);
FUNC(StatusType, OSEK_COM_CODE) SendMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
FUNC(StatusType, OSEK_COM_CODE) SendDynamicMessage(MessageIdentifier Message, ApplicationDataRef DataRef, LengthRef LengthRef);
FUNC(StatusType, OSEK_COM_CODE) SendZeroMessage(MessageIdentifier Message);
FUNC(StatusType, OSEK_COM_CODE) GetMessageStatus(MessageIdentifier Message);
FUNC(StatusType, OSEK_COM_CODE) ReceiveMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
FUNC(StatusType, OSEK_COM_CODE) ReceiveDynamicMessage(MessageIdentifier Message, ApplicationDataRef DataRef, LengthRef LengthRef);
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
StatusType StartCOMExtension(void);


#endif /* KOS_MEMORY_MAPPING */

extern const Com_MessageObjectType Com_MessageObjects[];

#endif  /*  __COM_H */
