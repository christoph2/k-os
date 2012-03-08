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
#if !defined(__OS_CTR_H)
#define __OS_CTR_H

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
**  Global Functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_OS_CODE) OsCtr_InitCounters(void);
#else
void OsCtr_InitCounters(void);
#endif /* KOS_MEMORY_MAPPING */



/*
**  Global Function-like Macros.
*/
#if 0
#define OS_INCREMENT_COUNTER_VALUE(CounterID)                                       \
    Os_CounterValues[(CounterID)] = (Os_CounterValues[(CounterID)] + (TickType)1) % \
                                    Os_CounterDefs[(CounterID)].CounterParams.maxallowedvalue
#endif

/* Yields to better code, at least on the CPU12. */
#define OS_INCREMENT_COUNTER_VALUE(CounterID)                                                         \
    _BEGIN_BLOCK                                                                                      \
    Os_CounterValues[(CounterID)]++;                                                                  \
    if (Os_CounterValues[(CounterID)] >= Os_CounterDefs[(CounterID)].CounterParams.maxallowedvalue) { \
        Os_CounterValues[(CounterID)] = (TickType)0;                                                  \
    }                                                                                                 \
    _END_BLOCK

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __OS_CTR_H */
