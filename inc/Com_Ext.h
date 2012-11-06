/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__COM_EXT_H)
#define __COM_EXT_H

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

#include "Osek.h"
#include "Com.h"
#include "Com_If.h"
/* #include "mcu/s12/inc/S12_Can.h" / * todo: 'CanIf.h'!!! * / */

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_COM_CODE) ComExt_Init(void);
FUNC(void, OSEK_COM_CODE) ComExt_RxHandler(void);
FUNC(void, OSEK_COM_CODE) ComExt_TxHandler(void);
FUNC(void, OSEK_COM_CODE) ComExt_TimeoutHandler(void);
FUNC(StatusType, OSEK_COM_CODE) ComExt_SendMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
FUNC(StatusType, OSEK_COM_CODE) ComExt_ReceiveMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
#else
void        ComExt_Init(void);
void        ComExt_RxHandler(void);
void        ComExt_TxHandler(void);
void        ComExt_TimeoutHandler(void);
StatusType  ComExt_SendMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
StatusType  ComExt_ReceiveMessage(MessageIdentifier Message, ApplicationDataRef DataRef);
#endif /* KOS_MEMORY_MAPPING */


void ComPort_SendMessage(Com_MessageObjectType const * const MessageObject, ApplicationDataRef DataRef);  // TODO: Memory-Mapping!!!

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __COM_EXT_H */

