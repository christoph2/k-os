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

#include "Com_Ext.h"

/*
• InitCOM and CloseCOM:
    These service are used to initialise and release the platform specific
    communication resources.
• StartCOM:
    This service initialises internal COM data areas, calls message initialisation
    routines and starts the OSEK communication module.
• StopCOM:
    This is used to terminate a session of OSEK COM, release resources where
    applicable.
• StopPeriodical and StartPeriodical :
    These services start or stop the periodical transmission of all messages using
    the periodical or the mixed transmission mode. It is sometimes useful to
    suspend periodical activity without necessarily closing down the whole of
    COM.
*/

/*
**
**    2.2.9.1 Definitions (Communication Deadline Monitoring).
**        - Start Timer: start of measurement of elapsed time.
**        - Timer Running: continuous measurement of elapsed time.
**        - Cancel Timer: stop measurement of elapsed time
**        - Time-out Interval : maximum time allowed for a particular monitored process
**        - Time-out: elapsed time has exceeded the time-out interval .
**
*/

/*
**
**  Naming for Layers: Interaction Layer (INL), Network Layer (NWL), Data Link Layer (DLL).
**
*/

/*
    Interaction layer / Network layer interface
    ===========================================
    Interaction layer parameters    Network layer service parameters
    ----------------------------------------------------------------
    SymbolicName                    N_Handle
    Recipients                      N_TA
    Sender                          N_SA
    BS_Value                        N_BS_Value  // Nur relevant f. 'ChangeProtocolParameters' - OSEK-COM 2.2
    ST_Value                        N_ST_Value  // "                                                       "
    Data                            N_User_Data
    Length                          N_Length    // dynamic Messages.
*/

/*
**    A message shall be assigned to a unique set of sub-network frame addressing attributes (e.g.
**    ISO 15765-2, F_NORMAL addressing format : CAN identifier).
*/

static const uint8 ComExt_NodeAddr=(uint8)0x55;

static void ComExt_Schedule(void);
static void ComExt_N_Task(void);
static void ComExt_DL_Task(void);
static void ComExt_I_Task(void);

/*
**
**  Each  external Messagecan contain  a 'TestMessageValue()'-Function-Pointer,
**  pointing to a Fliter-Function.
**
*/

/* 
**
** Data-Link-Layer .
**
*/


#if 0
StatusType D_UUData_req(<D_Handle>,<D_TA>,<D_User_Data>)

StatusType D_UUData_con(<D_Handle>,D_TA,D_SA,<D_Result_UUDT>)

StatusType D_UUData_ind(<D_Handle>,<D_SA>,<D_User_Data>,<D_Result_UUDT>)


/* 
**
**  Network-Layer.
**
*/

StatusType N_USData_req(<N_Handle>,<N_TA>,<N_User_Data>,<N_Length>)

StatusType N_USData_con(<N_Handle>,<N_TA>,<N_Result_USDT>)

StatusType N_USData_ind(<N_Handle>,<N_SA>,<N_User_Data>,<N_Length>,<N_Result_USDT>)

StatusType N_USData_FF_ind(<N_Handle>,<N_SA>,<N_Length>)

StatusType N_ChangeParameter_req(<N_Handle>,<N_BS_Value>,<N_ST_Value>)

StatusType N_ChangeParameter_con(<N_Handle>,<N_Result_ChangeParameter>)
#endif

void ComExt_Init(void)
{
/*    Can_Init(); */
}


void ComExt_Schedule(void)
{
    FOREVER {
        ComExt_I_Task();
        ComExt_N_Task();
        ComExt_DL_Task();
    }
}

void ComExt_I_Task(void)
{
    
}


void ComExt_N_Task(void)
{
    
}

void ComExt_DL_Task(void)
{
    
}



void ComExt_RxHandler(void)
{
    
}


void ComExt_TxHandler(void)
{
    
}

void ComExt_TimeoutHandler(void)
{
    
}
