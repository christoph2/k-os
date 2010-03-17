/*
 *   2-CB (C-Control-II kompatible Virtuelle Maschine).
 *
 *  (C) 2007-2009 by Christoph Schueler <chris@konnex-tools.de,
 *                                       cpu12.gems@googlemail.com>
 *
 *   All Rights Reserved
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#if !defined(__ISR_CFG_H)
#define __ISR_CFG_H

DECLARE_ISR2_VECTOR(OSSystemTimerVector);
DECLARE_ISR2_USER_HANDLER(OSSystemTimerVector);
#define MDCU_VECTOR OSSystemTimerVector


DECLARE_ISR1_VECTOR(SWI_Vector);
#define SWI_VECTOR SWI_Vector

#if 0
/*
**  Declaration of CAN-ISRs.
*/
DECLARE_ISR1_VECTOR(CAN0TxVector);
DECLARE_ISR1_VECTOR(CAN0RxVector);
DECLARE_ISR1_VECTOR(CAN0ErrorVector);
DECLARE_ISR1_VECTOR(CAN0WakupVector);

DECLARE_ISR1_VECTOR(CAN1TxVector);
DECLARE_ISR1_VECTOR(CAN1RxVector);
DECLARE_ISR1_VECTOR(CAN1ErrorVector);
DECLARE_ISR1_VECTOR(CAN1WakupVector);

DECLARE_ISR1_VECTOR(CAN2TxVector);
DECLARE_ISR1_VECTOR(CAN2RxVector);
DECLARE_ISR1_VECTOR(CAN2ErrorVector);
DECLARE_ISR1_VECTOR(CAN2WakupVector);

DECLARE_ISR1_VECTOR(CAN3TxVector);
DECLARE_ISR1_VECTOR(CAN3RxVector);
DECLARE_ISR1_VECTOR(CAN3ErrorVector);
DECLARE_ISR1_VECTOR(CAN3WakupVector);

DECLARE_ISR1_VECTOR(CAN4TxVector);
DECLARE_ISR1_VECTOR(CAN4RxVector);
DECLARE_ISR1_VECTOR(CAN4ErrorVector);
DECLARE_ISR1_VECTOR(CAN4WakupVector);
/*
**  Installation of CAN-ISRs.
*/
#define CAN0T_VECTOR CAN0TxVector
#define CAN0R_VECTOR CAN0RxVector
#define CAN0E_VECTOR CAN0ErrorVector
#define CAN0W_VECTOR CAN0WakupVector

#define CAN1T_VECTOR CAN1TxVector
#define CAN1R_VECTOR CAN1RxVector
#define CAN1E_VECTOR CAN1ErrorVector
#define CAN1W_VECTOR CAN1WakupVector

#define CAN2T_VECTOR CAN2TxVector
#define CAN2R_VECTOR CAN2RxVector
#define CAN2E_VECTOR CAN2ErrorVector
#define CAN2W_VECTOR CAN2WakupVector

#define CAN3T_VECTOR CAN3TxVector
#define CAN3R_VECTOR CAN3RxVector
#define CAN3E_VECTOR CAN3ErrorVector
#define CAN3W_VECTOR CAN3WakupVector

#define CAN4T_VECTOR CAN4TxVector
#define CAN4R_VECTOR CAN4RxVector
#define CAN4E_VECTOR CAN4ErrorVector
#define CAN4W_VECTOR CAN4WakupVector

#endif

/*
**  CRG-Interrupts.
*/
#if 0
DECLARE_ISR1_VECTOR(CRG_LockInterrupt);
#define CRG_VECTOR CRG_LockInterrupt

DECLARE_ISR1_VECTOR(CRG_SelfClockModeInterrupt);
#define SCME_VECTOR CRG_SelfClockModeInterrupt
#endif

#endif /* __ISR_CFG_H */
