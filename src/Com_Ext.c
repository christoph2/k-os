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

#include "Com_Ext.h"
#include "Os_PortCom.h"

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

#include "Com_If.h"

static const uint8 ComExt_NodeAddr = (uint8)0x55;

/*
**  Local function prototypes.
*/
#if KOS_MEMORY_MAPPING == STD_ON
STATIC  FUNC(void, OSEK_COM_CODE) ComExt_Schedule(void);
STATIC  FUNC(void, OSEK_COM_CODE) ComExt_N_Task(void);
STATIC  FUNC(void, OSEK_COM_CODE) ComExt_DL_Task(void);
STATIC  FUNC(void, OSEK_COM_CODE) ComExt_I_Task(void);


#else
static void ComExt_Schedule(void);
static void ComExt_N_Task(void);
static void ComExt_DL_Task(void);
static void ComExt_I_Task(void);


#endif /* KOS_MEMORY_MAPPING */

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
StatusType D_UUData_req(< D_Handle >, < D_TA >, < D_User_Data >)

StatusType D_UUData_con(< D_Handle >, D_TA, D_SA, < D_Result_UUDT >)

StatusType D_UUData_ind(< D_Handle >, < D_SA >, < D_User_Data >, < D_Result_UUDT >)

/*
**
**  Network-Layer.
**
*/

StatusType N_USData_req(< N_Handle >, < N_TA >, < N_User_Data >, < N_Length >)

StatusType N_USData_con(< N_Handle >, < N_TA >, < N_Result_USDT >)

StatusType N_USData_ind(< N_Handle >, < N_SA >, < N_User_Data >, < N_Length >, < N_Result_USDT >)

StatusType N_USData_FF_ind(< N_Handle >, < N_SA >, < N_Length >)

StatusType N_ChangeParameter_req(< N_Handle >, < N_BS_Value >, < N_ST_Value >)

StatusType N_ChangeParameter_con(< N_Handle >, < N_Result_ChangeParameter >)
#endif

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_COM_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_COM_CODE) ComExt_Init(void)
#else
void ComExt_Init(void)
#endif /* KOS_MEMORY_MAPPING */
{
/*    Can_Init(); */
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_COM_CODE) ComExt_Schedule(void)
#else
void ComExt_Schedule(void)
#endif /* KOS_MEMORY_MAPPING */
{
    FOREVER {
        ComExt_I_Task();
        ComExt_N_Task();
        ComExt_DL_Task();
    }
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_COM_CODE) ComExt_I_Task(void)
#else
void ComExt_I_Task(void)
#endif /* KOS_MEMORY_MAPPING */
{

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_COM_CODE) ComExt_N_Task(void)
#else
void ComExt_N_Task(void)
#endif /* KOS_MEMORY_MAPPING */
{

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_COM_CODE) ComExt_DL_Task(void)
#else
void ComExt_DL_Task(void)
#endif /* KOS_MEMORY_MAPPING */
{

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_COM_CODE) ComExt_RxHandler(void)
#else
void ComExt_RxHandler(void)
#endif /* KOS_MEMORY_MAPPING */
{

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_COM_CODE) ComExt_TxHandler(void)
#else
void ComExt_TxHandler(void)
#endif /* KOS_MEMORY_MAPPING */
{

}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(void, OSEK_COM_CODE) ComExt_TimeoutHandler(void)
#else
void ComExt_TimeoutHandler(void)
#endif /* KOS_MEMORY_MAPPING */
{

}


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_COM_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_COM_CODE) ComExt_SendMessage(MessageIdentifier Message, ApplicationDataRef DataRef)
#else
StatusType  ComExt_SendMessage(MessageIdentifier Message, ApplicationDataRef DataRef)
#endif /* KOS_MEMORY_MAPPING */
{
    Com_MessageObjectType * MessageObject;

    MessageObject = (Com_MessageObjectType *)&OSEK_COM_GET_MESSAGE_OBJECT(Message);
    Com_SetFlag(Message);
    PortCom_SendMessage(MessageObject->Address, DataRef);

    return E_OK;
}


#if KOS_MEMORY_MAPPING == STD_ON
FUNC(StatusType, OSEK_COM_CODE) ComExt_ReceiveMessage(MessageIdentifier Message, ApplicationDataRef DataRef)
#else
StatusType  ComExt_ReceiveMessage(MessageIdentifier Message, ApplicationDataRef DataRef)
#endif /* KOS_MEMORY_MAPPING */
{
    Com_MessageObjectType * MessageObject;

    MessageObject = (Com_MessageObjectType *)&OSEK_COM_GET_MESSAGE_OBJECT(Message);

    return E_OK;
}

void Com_TransmitMessages(void)
{
    uint8 idx;
    uint8 flag;
    Com_MessageObjectType * MessageObject;

#if 0
    for (idx = (uint8)0x00; idx < COM_NUMBER_OF_MESSAGES; ++idx) {
        flag = Com_GetFlag(idx);
        if (flag == Std_High) {
            MessageObject = (Com_MessageObjectType *)&OSEK_COM_GET_MESSAGE_OBJECT(idx);

            Com_ResetFlag(idx);
        }
    }
#endif
}

void Com_SetFlag(MessageIdentifier Message)
{
    // TODO: Implement!
}
