/*
 * k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2018 by Christoph Schueler <github.com/Christoph2,
 *                                     cpu12.gems@googlemail.com>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * s. FLOSS-EXCEPTION.txt
 */
#include "Osek.h"

TaskType Os_CurrentTID;
Os_TCBType * Os_CurrentTCB;
uint8 Os_Flags;    /* TODO: Isolate these flags!!! */

#if (OS_EXTENDED_STATUS == STD_ON) && (OS_FEATURE_CALLEVEL_CHECK == STD_ON)
Os_CallevelType Os_Callevel = OS_CL_INVALID;
Os_CallevelType Os_CallevelSaved = OS_CL_INVALID;
#endif

OS_DEFINE_GLOBAL_IF_DEBUGGING(OsCurrentISRID, uint8);
