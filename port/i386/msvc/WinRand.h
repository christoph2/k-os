/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

 * (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>

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
#if !defined(__WIN_RAND_H)
#define __WIN_RAND_H

#include <windows.h>

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */


BOOL WinRand_Init(void);
BOOL WinRand_RandomBytes(DWORD count, BYTE * randomBytes);
__int8 WinRand_I8(void);
__int16 WinRand_I16(void);
__int32 WinRand_I32(void);
__int64 WinRand_I64(void);


#if defined(__cplusplus)
}
#endif  /* __cplusplus */


#endif /* __WIN_RAND_H */
