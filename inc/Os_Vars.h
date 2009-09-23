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

#if !defined(__OS_VARS_H)
#define __OS_VARS_H

extern uint8 OsFlags;
extern uint8 OsCCRSave;
extern TaskType OsCurrentTID;
extern OsTCBType *OsCurrentTCB;

#if defined(OS_EXTENDED_STATUS) && defined(OS_USE_CALLEVEL_CHECK)
extern OsCallevelType OsCallevel;
extern OsCallevelType OsCallevelSaved;
#endif

#if defined(USE_ORTI)
extern uint8 OsCurrentISRID;
#endif

#endif /* __OS_VARS_H */
