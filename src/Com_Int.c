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

/*
**
**  Implements OSEK-COM internal Communications.
**
*/

#include "Com_Int.h"
#include "Utl.h"

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_COM_CODE) ComInt_SendMessage(MessageIdentifier Message, ApplicationDataRef DataRef)
#else
StatusType ComInt_SendMessage(MessageIdentifier Message, ApplicationDataRef DataRef)
#endif /* KOS_MEMORY_MAPPING */
{
    Com_MessageObjectType const * const MessageObject = (Com_MessageObjectType *) &OSEK_COM_GET_MESSAGE_OBJECT(Message);

    if (MessageObject->NumReceivers != (uint8)0) {
        ComIf_UpdateAndNotifyReceivers(MessageObject, DataRef);
        OsExec_CondScheduleFromTaskLevel();
    }

    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_COM_CODE) ComInt_ReceiveMessage(MessageIdentifier Message, ApplicationDataRef DataRef)
#else
StatusType ComInt_ReceiveMessage(MessageIdentifier Message, ApplicationDataRef DataRef)
#endif /* KOS_MEMORY_MAPPING */
{
    Com_MessageObjectType const * const MessageObject = (Com_MessageObjectType *)&OSEK_COM_GET_MESSAGE_OBJECT(Message);

    OsPort_DisableAllOsInterrupts();
    Utl_MemCopy((void *)DataRef, (void *)MessageObject->Data, (SizeType)MessageObject->Size);
    OsPort_EnableAllOsInterrupts();

    return E_OK;
}
