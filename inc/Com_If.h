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
#if !defined(__COM_IF_H)
#define __COM_IF_H

#include "Com.h"

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_COM_CODE) ComIf_UpdateAndNotifyReceivers(
    CONSTP2CONST(Com_MessageObjectType, AUTOMATIC, OSEK_COM_APPL_DATA) MessageSendObject,
    ApplicationDataRef DataRef
    );
#else
void ComIf_UpdateAndNotifyReceivers(Com_MessageObjectType const * const MessageSendObject, ApplicationDataRef DataRef);

void Com_SetFlag(MessageIdentifier Message);
void Com_ResetFlag(MessageIdentifier Message);
Std_LevelType Com_GetFlag(MessageIdentifier Message);

#endif  /* KOS_MEMORY_MAPPING */

#endif  /* __COM_IF_H */

