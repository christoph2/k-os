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

#include "Com_CanTp.h"
//#include "mcu/s12/inc/S12_Can.h"
#include "Utl.h"

/*
**
**  REFERENCES:
**  -----------
**  ISO 15765-2-2004 [1]
**
*/

static void Com_CanTp_MainFunctionTx(void);
void        Com_CanTp_Test(void);


#if 0
CanNM in Erlang:
================

bus_sleep_mode(_) ->
    [ {power_off,{call,?MODULE,powerOff,[]}},
      {bus_sleep_mode,{call,?MODULE,main,[]}},
      {bus_sleep_mode,{call,?MODULE,'CanNm_RxIndication',[id(),u8()]}},
      {repeat_message_state,{call,?MODULE,'Nm_PassiveStartUp',[]}},
      {repeat_message_state,{call,?MODULE,'CanNm_NetworkRequest',[]}}].
repeat_message_state(_) ->
    [ {normal_operation_state,{call,?MODULE,main,[]}},
      {ready_sleep_state,{call,?MODULE,main,[]}},
      {repeat_message_state,{call,?MODULE,main,[]}},
      {repeat_message_state,{call,?MODULE,'CanNm_RxIndication',[id(),u8()]}},
      {repeat_message_state,{call,?MODULE,'CanNm_TxConfirmation',[id()]}}].

next_state_data(repeat_message_state,repeat_message_state,S,_V,{_,_,main,_}) ->
    S#can_nm{repeatMessageTimer = S#can_nm.repeatMessageTimer-1,
           nmTimeoutTimer =
             case S#can_nm.nmTimeoutTimer of
              0 -> ?NMTIMEOUT;
              N -> N-1
             end};
next_state_data(repeat_message_state,repeat_message_state,S,_V,{_,_,_,_}) ->
 S#can_nm{repeatMessageTimer = S#can_nm.repeatMessageTimer-1,
          nmTimeoutTimer = ?NMTIMEOUT};
#endif

#if 0
4.2
== =
    N_USData.request(Mtype, N_SA, N_TA, N_TAtype, N_AE, < MessageData >, < Length >)
    N_USData.confirmation(Mtype, N_SA, N_TA, N_TAtype, N_AE, < N_Result >)
    N_USData_FF.indication(Mtype, N_SA, N_TA, N_TAtype, N_AE, < Length >)
    N_USData.indication(Mtype, N_SA, N_TA, N_TAtype, N_AE, < MessageData >, < Length >, < N_Result >)

    N_ChangeParameter.request(Mtype, N_SA, N_TA, N_TAtype, N_AE, < Parameter >, < Parameter_Value >)
    N_ChangeParameter.confirm(Mtype, N_SA, N_TA, N_TAtype, N_AE, < Parameter >, < Result_ChangeParameter >)

/* Glossar */
    == == == == ==
    Mtype       - Message Type
    N_AE        - network address extension
    N_SA        - network source address
    N_TA        - network target address
    N_TAtype    - network target address type
    -- -

    (N_SA, N_TA, N_TAtype) == > Address - Information.
#endif

#if 0
#define NHASH 29989
#define MULT 31

unsigned int hash(char *p)
{
    unsigned int h = 0
    for ( ; *p; p++)
        h = MULT * h + *p
    return h % NHASH
}
#endif

typedef enum tagStateType {
    IDLE,
    SINGLE_FRAME,
    FIRST_FRAME,
    CF_FRAME,
    FC_FRAME,
    FC_FRAME_REQ,
    WAIT_FOR_L_DATA_IND,    /* [CF | FC] */
    WAIT_FOR_L_DATA_CON,    /* [SF | FC | FF] */
    LAST_FRAME
} StateType;

typedef enum tagCom_CanTpEventType {
/*    IDLE, */
/*    SINGLE_FRAME, */
/*    FIRST_FRAME, */
/*    CF_FRAME, */
/*    FC_FRAME, */
    TIMEOUT,
    L_DATA_IND,     /* [CF | FC] */
    L_DATA_CON,     /* [SF | FC | FF] */
    N_DATA_REQ
} Com_CanTpEventType;

#if 0
Main - States
---------- -
IDLE
RX_UUDT
RX_USDT
TX_UUDT
TX_USDT
#endif

static uint8 data0[7] = {0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
static uint8    data1[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
static uint8    data2[8 * 16] = {
    "#0_3456789ABCDEF"
    "#1_3456789ABCDEF"
    "#2_3456789ABCDEF"
    "#3_3456789ABCDEF"
    "#4_3456789ABCDEF"
    "#5_3456789ABCDEF"
    "#6_3456789ABCDEF"
    "#7_3456789ABCDEF"
};

const IsoTp_PduType tpdu[] = {
    { CANTP_AI(CANTP_NORMAL_FIXED_FUNCTIONAL,                                 0x20, 0x10),      7, (uint8 *)data0        },
    { CANTP_AI(CANTP_MIXED29_FUNCTIONAL,                                      0x20, 0x10),      6, (uint8 *)data1        },
/*    { CANTP_AI(CANTP_NORMAL_FIXED_FUNCTIONAL,   0x20, 0x10), 6,         (uint8*)data1}, */
    { CANTP_AI(CANTP_NORMAL_FIXED_FUNCTIONAL,                                 0x20, 0x10), 8 * 16, (uint8 *)&data2       },
};

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_COM_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

static void Com_CanTp_MainFunctionTx(void)
{

}


#if 0
/* STATES: WAIT_FOR_???_IND, WAIT_FOR_???_CON */

WAIT_FOR_L_DATA_IND [CF | FC]
WAIT_FOR_L_DATA_CON [SF | FC | FF]

WAIT_FOR_ ? ? ? _IND
WAIT_FOR_ ? ? ? _CON

WAIT_FOR_ ? ? ? _IND
WAIT_FOR_ ? ? ? _CON

#endif

typedef struct tagStateVarType {
    IsoTp_PduType *     N_PDU;
    MSCan_MessageType * L_PDU;
    uint8               RemoteAddress;
    uint16              NPDULength;
    uint8               FlowStatus;
    uint8               BlockSize;
    uint8               STmin;
    StateType           State; /* todo: allgemein gültiger State!!! */
    uint32              ID;
    uint16              Offset;
    uint16              DataLength;
    uint8               SN;
    uint8               CurrentBlock;
    uint8               PreviousSN;
    uint8               AE;
} StateVarType;

/* //////////////////////////////////// */
/* //////////////////////////////////// */
/* //////////////////////////////////// */

#define NUM_SMS 2

typedef enum tagIsoTp_TimerStateType {
    ISOTP_STOPPED,
    ISOTP_STARTED
/* uin16 Timeout, State   */
} IsoTp_TimerStateType;

typedef struct tagIsoTp_TimerType {
    IsoTp_TimerStateType    State;
    uint16                  Ticks;
} IsoTp_TimerType;

static IsoTp_TimerType IsoTp_Timers[NUM_SMS];

static uint8 IsoTp_ActiveTimers;  /* uint8, uint16, uint32 depending of the NoOfConnections supported. */

static void IsoTp_ActivateTimer(uint8 TimerNumber, /*TickType*/ uint16 TimeoutValue);
static void IsoTp_CancelTimer(uint8 TimerNumber);
static void IsoTo_Cbk_Timer(void); /* todo: MainFunction!!! */


static void IsoTp_ActivateTimer(uint8 TimerNumber, /*TickType*/ uint16 TimeoutValue)
{
    IsoTp_ActiveTimers                 = UTL_BIT_SET16(IsoTp_ActiveTimers, TimerNumber);
    IsoTp_Timers[TimerNumber].Ticks    = TimeoutValue;
}


static void IsoTp_CancelTimer(uint8 TimerNumber)
{
    IsoTp_ActiveTimers = Utl_BitReset(IsoTp_ActiveTimers, TimerNumber);
}


static void IsoTo_Cbk_Timer(void)
{
    uint8   idx = (uint8)0x00;
    uint16  Timers;

    Timers = IsoTp_ActiveTimers;

    while (Timers != (uint8)0x00) {
        if ((Timers & BIT0) == BIT0) {
            if ((IsoTp_Timers[idx].Ticks--) == (uint16)0x0000u) {
                /* OK, fire Timeout.    */
                IsoTp_CancelTimer(idx);
/*                printf("Timer #%02x expired.\n",idx); */
                /* check: den konkreten Timeout-Typen aus dem State ermitteln ??? */
            }
        }

        Timers >>= (uint8)1;
        idx++;
    }
}


/* //////////////////////////////////// */
/* //////////////////////////////////// */
/* //////////////////////////////////// */

#if 0
perfect - your - sentences s .30
Donald Trump 101                            s .150
English Grammar

for the utterly confused s .53

Riemannsche Zeta - Funktion.
#endif

static void Com_CanTp_TransmitStateMachine(StateVarType * State, Com_CanTpEventType Event)    /* EVENT */
{
    State->AE = CANTP_IS_NORMAL_ADDRESSING(State->N_PDU->ID);

/*    Utl_MemSet(State->L_PDU->SDU, 0, 8); */

    if (State->State == CF_FRAME) {
        CANTP_SET_CF_PCI(State->L_PDU, State->SN, State->AE);

        State->L_PDU->DLR  = MSCAN_MAX_LSDU_LEN;
        State->L_PDU->ID   = State->N_PDU->ID;
        Utl_MemCopy((void *)&State->L_PDU->SDU[(uint8)1 + State->AE],
                    (void *)&State->N_PDU->SDU[State->Offset], MSCAN_MAX_LSDU_LEN - (uint8)0x01 - State->AE
                    );
        State->Offset += MSCAN_MAX_LSDU_LEN - (uint8)1 - State->AE;
        State->SN      = (State->SN + (uint8)0x01) & 0x0f;
/*            State->State = CF_FRAME; */

        S12Can_Transmit(0, State->L_PDU, 0, NULL);

        if (State->Offset >= State->N_PDU->Len) {
            State->State = IDLE;
        }
    } else if (State->State == IDLE) {
        if (State->N_PDU->Len >= (CANTP_MIN_PDU_LEN_USDT - State->AE)) {
            /* Multiple frames. */

            /* this is STATE First-Frame!!! */
            CANTP_SET_FF_PCI(State->L_PDU, State->N_PDU->Len, State->AE);

            State->L_PDU->DLR  = MSCAN_MAX_LSDU_LEN;
            State->L_PDU->ID   = State->N_PDU->ID;
            State->Offset      = MSCAN_MAX_LSDU_LEN - (uint8)2 - State->AE;
            Utl_MemCopy((void *)&State->L_PDU->SDU[(uint8)2 + State->AE], (void *)State->N_PDU->SDU, State->Offset);
            State->SN      = (uint8)0x01;
            State->State   = CF_FRAME;

            S12Can_Transmit(0, State->L_PDU, 0, NULL);
        } else {
            /* Single frame PDU. */
            State->L_PDU->ID   = State->N_PDU->ID;
            State->ID          = State->N_PDU->ID;
            State->L_PDU->DLR  = State->N_PDU->Len;
            CANTP_SET_SF_PCI(State->L_PDU, State->N_PDU->Len, State->AE);

            if (State->AE == (uint8)0x01) {
                State->L_PDU->SDU[0] = State->RemoteAddress;
            }

            Utl_MemCopy((void *)&State->L_PDU->SDU[(uint8)1 + State->AE], (void *)State->N_PDU->SDU, State->N_PDU->Len);
            State->State = WAIT_FOR_L_DATA_CON;

            S12Can_Transmit(0, State->L_PDU, 0, NULL);
        }
    }
}


MSCan_MessageType rpdus[] = {
    { 0x18DB2010, 0x07, {0x07, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff} },
    { 0x18CD2010, 0x06, {0x55, 0x06, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66} }
};

MSCan_MessageType segmented[] = {
    {0x18DB2010, 0x08, {0x10, 0x80, 0x23, 0x30, 0x5f, 0x33 ,  0x34, 0x35} },
    {0x18DB2010, 0x08, {0x21, 0x36, 0x37, 0x38, 0x39,  0x41, 0x42 , 0x43} },
    {0x18DB2010, 0x08, {0x22, 0x44, 0x45, 0x46, 0x23,  0x31,  0x5f, 0x33} },
    {0x18DB2010, 0x08, {0x23, 0x34, 0x35, 0x36, 0x37,  0x38,  0x39, 0x41} },
};

#define N_PDU_LEN_MAX ((uint16)0x07)    /* NICHT GUT!!! */

/* /// */
/* ///   Com_CanTp_PSM(StateVarType * State, Com_CanTp_EventType Event, ErrorType * Error); // protocol state machine. */
/* ///   RETURNS <OK | NOT_OK>. */
/* /// */

static void Com_CanTp_ReceiveStateMachine(StateVarType * State, Com_CanTpEventType Event)
{
    State->AE = CANTP_IS_NORMAL_ADDRESSING(State->L_PDU->ID);
    uint8 npci;

    if (State->AE == (uint8)0x01) {
        State->RemoteAddress = State->L_PDU->SDU[0];
    }

    npci = State->L_PDU->SDU[0 + State->AE] & (uint8)0xF0;

    switch (npci) {
        case CANTP_NPCI_TYPE_SF :  /* Single Frame.        */
            State->DataLength = State->L_PDU->SDU[0 + State->AE] & ((uint8)0x0f);

            if ( (State->DataLength == (uint8)0x00) || (State->DataLength > (MSCAN_MAX_LSDU_LEN - (uint8)0x01 - State->AE) )) {
                /* [1] 6.5.2.2 */
                /* todo: set State!!! */
                break;
            }

            Utl_MemCopy((void *)State->N_PDU->SDU, (void *)&State->L_PDU->SDU[(uint8)1 + State->AE], State->DataLength);
            break;
        case CANTP_NPCI_TYPE_FF :  /* First Frame.         */
            /* state=RX_FIRST_FRAME;    // todo: check TAtype!!! */
            State->DataLength = ((State->L_PDU->SDU[0 + State->AE] & ((uint8)0x0f)) << 8) | State->L_PDU->SDU[1];

            if (State->DataLength < (uint16)(MSCAN_MAX_LSDU_LEN - State->AE)) {
                /* [1] 6.5.3.3 */
                /* todo: set State!!! */
                break;
            }

            /* todo: check Overflow!!! */
            /************************************************************/
            /* Hinweis: Für Sender und Empfänger scheinbar gleich...      */
            State->Offset = N_PDU_LEN_MAX - (uint8)1 - State->AE;
            /* BytesRemaining = DataLength - Offset; */
/*            SegmentsToRcv = BytesRemaining / N_PDU_LEN_MAX; */
/*            BytesInLastFrame = (uint8)(BytesRemaining % N_PDU_LEN_MAX); */
            /************************************************************/

            /* todo: Einheitlicher Punkt, an dem kopiert wird!!! */
            /* Utl_MemCopy((void*)RcvPDU,(void*)&pdu->SDU[2],N_PDU_LEN_MAX-(uint8)1); */

            Utl_MemCopy((void *)State->N_PDU->SDU, (void *)&State->L_PDU->SDU[(uint8)2 + State->AE],
                        MSCAN_MAX_LSDU_LEN - (uint8)2 - State->AE
                        );

            State->SN          = (uint8)0x00;
            State->PreviousSN  = State->SN;

            /* todo: Flowcontrol Frame with BS, STmin !!! */

            State->State = CF_FRAME;
            break;
        case CANTP_NPCI_TYPE_CF :  /* Consecutive Frame.   */
            /* State->L_PDU->DLR = MIN(State->L_PDU->DLR, MSCAN_MAX_LSDU_LEN); */

            State->SN = State->L_PDU->SDU[0 + State->AE] & ((uint8)0x0f);

            if (State->SN != (((State->PreviousSN + (uint8)0x01)) & (uint8)0x0f)) {
                /* todo: SN error handling ([1] 6.5.4.3) <N_Result> = N_WRONG_SN, IDLE-State (i.e. abort receiption). */
                break;
            }

            Utl_MemCopy((void *)&State->N_PDU->SDU[State->Offset],
                        (void *)&State->L_PDU->SDU[(uint8)1 + State->AE], MSCAN_MAX_LSDU_LEN - (uint8)1 - State->AE
                        );

            State->Offset += N_PDU_LEN_MAX - State->AE;
            /* Wenn Padding verwendet wird, muss die Anzahl der Frames gezählt werden!!! */
            /* Wirklich??? */

            if (State->Offset >= State->DataLength) {
                /* Finished!!! */
            }

            /* Utl_MemCopy((void*)&RcvPDU[data_offset],(void*)&pdu->SDU[1],7); */

            State->PreviousSN = State->SN;
            break;
        case CANTP_NPCI_TYPE_FC :  /* Flow Control.        */
            State->FlowStatus  = State->L_PDU->SDU[0 + State->AE] & ((uint8)0x0f);
            State->BlockSize   = State->L_PDU->SDU[1 + State->AE];
            State->STmin       = State->L_PDU->SDU[2 + State->AE];

            if (State->FlowStatus >= CANTP_FS_RES_LOWER) {
                /* todo: FS error-handling ([1] 6.5.5.3). <N_Result> = N_INVALID_FS */
                break;  /* check: in diesem Fall besser 'return' !? */
            }

            break;
        default :
            break;
    }
}


static uint8 RcvSDU[0x80];

void Com_CanTp_Test(void)
{
    StateVarType        State0, State1;
    MSCan_MessageType   lpdu0, lpdu1;
    IsoTp_PduType       RcvPDU;

/* K-Boo-K  - K-Boot-Kit. */

    State0.L_PDU           = &lpdu0;
    State0.N_PDU           = (IsoTp_PduType * )&tpdu[2];
    State0.RemoteAddress   = 0xaa;
    State0.State           = IDLE;
    State1.N_PDU           = &RcvPDU;
    State1.N_PDU->SDU      = (uint8 *)RcvSDU;

    FOREVER {
        Com_CanTp_TransmitStateMachine(&State0, N_DATA_REQ);

        Utl_MemCopy((void *)State1.L_PDU, (void *)State0.L_PDU, sizeof(MSCan_MessageType));
        Com_CanTp_ReceiveStateMachine(&State1, L_DATA_IND);
    }

#if 0

    State0.L_PDU = (MSCan_MessageType * )&segmented[0];
    Com_CanTp_ReceiveStateMachine(&State0, L_DATA_IND);
    State0.L_PDU = (MSCan_MessageType * )&segmented[1];
    Com_CanTp_ReceiveStateMachine(&State0, L_DATA_IND);
    State0.L_PDU = (MSCan_MessageType * )&segmented[2];

    Com_CanTp_ReceiveStateMachine(&State0, L_DATA_IND);
    Com_CanTp_TransmitStateMachine(&State, N_DATA_REQ);
    State.L_PDU->ID = tpdu[0].ID;
    Com_CanTp_TransmitStateMachine(&State, L_DATA_CON);

    State.N_PDU = (IsoTp_PduType * )&tpdu[1];
    Com_CanTp_TransmitStateMachine(&State, N_DATA_REQ);
#endif
    State0.State   = IDLE;
    State0.N_PDU   = (IsoTp_PduType * )&tpdu[2];
    Com_CanTp_TransmitStateMachine(&State0, N_DATA_REQ);
    Com_CanTp_TransmitStateMachine(&State0, N_DATA_REQ);
    Com_CanTp_TransmitStateMachine(&State0, N_DATA_REQ);
    Com_CanTp_TransmitStateMachine(&State0, N_DATA_REQ);

#if 0
    State.L_PDU = (MSCan_MessageType * )&rpdus[0];
    Com_CanTp_ReceiveStateMachine(&State, L_DATA_IND);

    State.L_PDU = (MSCan_MessageType * )&rpdus[1];
    Com_CanTp_ReceiveStateMachine(&State, L_DATA_IND);
#endif
}


#if 0
Hi Christoph,

I am a researcher at the K.U.Leuven, Belgium.I have found your open source implementation of the EIB / KNX stack on github.

I am interested in building my own development board and run a KNX stack on it.Once this has been completed,
I will try to incorporate additional functionality in the stack(outside the KNX standard).

I was wondering

if you have more documentation or can offer some help

for me to perform the following tasks :
    -On what platform can you run the code ? microcontrollers ? Do you use a specific development board

    for this ?
    -What kind

    if development tools

    do
        you use ?
        -Any other information ...

        Kind regards,
        Stefaan Seys
        --
        Dr.Ir.Stefaan Seys | Postdoctoral Researcher | Katholieke Universiteit Leuven
   Dept.Electrical Engineering(ESAT) / COSIC | Kasteelpark Arenberg 10, B - 3001 Leuven, Belgium
        Tel + 32 (0) 16 32 1134 | Fax + 32 (0) 16 32 19 69 | http : /* homes.esat.kuleuven.be/~sseys/ */

#endif

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_COM_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
