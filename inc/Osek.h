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

#if !defined(__OSEK_H)
#define __OSEK_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "Std_Macros.h"
#include "Os_Cfg.h"
#include "Os_Error.h"
#include "Os_Vars.h"
#include "Os_Macros.h"
#include "Os_Api.h"
#include "Os_Alm.h"
#include "Os_Ctr.h"
#include "Os_Evt.h"
#include "Os_Exec.h"
#include "Os_Intr.h"
#include "Os_MLQ.h"
#include "Os_Res.h"
#include "Os_Task.h"
#include "Utl.h"
#include "Os_SchT.h"
#include "InstallIsr/ISR.h"
    
#if defined(__cplusplus)
}
#endif

#endif  /*  __OSEK_H  */
