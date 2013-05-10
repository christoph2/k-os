/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2013 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__OS_VARS_H)
#define __OS_VARS_H

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

#include "Os_Macros.h"

extern uint8        Os_Flags;
extern uint8        OsCCRSave;
extern TaskType     Os_CurrentTID;
extern Os_TCBType *  Os_CurrentTCB;

#if defined(OS_EXTENDED_STATUS) && defined(OS_FEATURE_CALLEVEL_CHECK)
extern Os_CallevelType   Os_Callevel;
extern Os_CallevelType   Os_CallevelSaved;
#endif

OS_DECLARE_GLOBAL_IF_DEBUGGING(OsCurrentISRID, uint8);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __OS_VARS_H */
