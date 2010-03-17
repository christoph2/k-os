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
#include "Osek.h"

TaskType OsCurrentTID;
OsTCBType *OsCurrentTCB; 
uint8 OsFlags;

#if defined(OS_EXTENDED_STATUS) && defined(OS_USE_CALLEVEL_CHECK)
OsCallevelType OsCallevel=OS_CL_INVALID;
OsCallevelType OsCallevelSaved=OS_CL_INVALID;
#endif

#if defined(USE_ORTI)
uint8 OsCurrentISRID;

#endif
