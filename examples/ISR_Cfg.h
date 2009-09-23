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
 
#if !defined(__ISR_CFG_H)
#define __ISR_CFG_H

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

DECLARE_ISR2_VECTOR(OSSystemTimerVector);
DECLARE_ISR2_USER_HANDLER(OSSystemTimerVector);
#define MDCU_VECTOR OSSystemTimerVector

DECLARE_ISR1_VECTOR(SWI_Vector);
#define SWI_VECTOR SWI_Vector

DECLARE_ISR1_VECTOR(RTI_Vector);
#define RTI_VECTOR RTI_Vector

#if 0
DECLARE_ISR1_VECTOR(SCI0_Vector);
#define SCI0_VECTOR SCI0_Vector
#endif

DECLARE_ISR1_VECTOR(ATD0_Vector);
#define ATD0_VECTOR ATD0_Vector


#if 0
DECLARE_ISR(SPI0_Vector);
#define SPI0_VECTOR SPI0_Vector
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __ISR_CFG_H */
