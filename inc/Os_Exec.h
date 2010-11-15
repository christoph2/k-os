/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

 * (C) 2007-2010 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__OS_EXEC_H)
#define __OS_EXEC_H

OS_DECLARE_GLOBAL_IF_DEBUGGING(Os_AppMode,AppModeType);

void OsExec_StartHighestReadyTask(void);
boolean OsExec_HigherPriorityThenCurrentReady(void);

void OsExec_TaskReturnGuard(void);
void OsExec_ScheduleFromISR(void);

#endif /* __OS_EXEC_H */
