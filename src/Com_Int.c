/*
 * k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2009 by Christoph Schueler <chris@konnex-tools.de>
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

/*
**
**  Implements OSEK-COM internal Communications.
**
*/

#include "Com_Int.h"

extern const ComMessageObjectType Com_MessageObjects[];

StatusType ComIntSendMessage(MessageIdentifier Message,ApplicationDataRef DataRef)
{
    ComMessageObjectType *MessageSendObject;
        
    MessageSendObject=(ComMessageObjectType *)&GET_MESSAGE_OBJECT(Message);
    
    if (MessageSendObject->NumReceivers!=(uint8)0) {
        ComIfUpdateAndNotifyReceivers((ComMessageObjectType *)MessageSendObject,DataRef);
        OS_COND_SCHEDULE_FROM_TASK_LEVEL();
    }
    
    return E_OK;    
}


StatusType ComIntReceiveMessage(MessageIdentifier Message,ApplicationDataRef DataRef)
{
    ComMessageObjectType *MessageObject;
        
    MessageObject=(ComMessageObjectType *)&GET_MESSAGE_OBJECT(Message);    

    DISABLE_ALL_OS_INTERRUPTS();
    Utl_MemCopy((void*)DataRef,(void*)MessageObject->Data,(uint16)MessageObject->Size);
    ENABLE_ALL_OS_INTERRUPTS();
    
    return E_OK;
}
