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
#if !defined(__COM_CAN_TP_H)
#define __COM_CAN_TP_H

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

#include "Com.h"

//#include "mcu/inc/mscan.h"
/* #include "Utl.h" */

/*
**  Global Defines.
*/
#define CANTP_MAX_PDU_LEN       ((uint16)0x0fffU)
#define CANTP_MIN_PDU_LEN_USDT  ((uint16)0x0008U)

#define CANTP_PDU_LEN_FF        ((uint8)0x06)
#define CANTP_PDU_LEN_CF        ((uint8)0x07)

/* NCPI-Types.  */
#define CANTP_NPCI_TYPE_SF      ((uint8)0x00)   /* Single Frame.        */
#define CANTP_NPCI_TYPE_FF      ((uint8)0x10)   /* First Frame.         */
#define CANTP_NPCI_TYPE_CF      ((uint8)0x20)   /* Consecutive Frame.   */
#define CANTP_NPCI_TYPE_FC      ((uint8)0x30)   /* Flow Control.        */

/* Flow-Status-Parameter.  */
#define CANTP_FS_CTS            ((uint8)0x00)
#define CANTP_FS_WT             ((uint8)0x01)
#define CANTP_FS_OVFLW          ((uint8)0x02)
#define CANTP_FS_RES_LOWER      ((uint8)0x03)

/*
**  Global Function-like Macros.
*/

#define J1939_21_AI(pr, dp, pf, ps, sa)     \
    ((((uint32)(pr) & (uint8)0x07) << 26) | \
     (((uint32)(dp) & (uint8)0x01) << 24) | \
     (((uint32)(pf) & (uint8)0xff) << 16) | \
     (((uint32)(ps) & (uint8)0xff) << 8)  | \
     ((uint32)(sa)  & (uint8)0xff))

#define J1939_21_DEFAULT_PRIORITY       ((uint8)0x06)

/** PDU Formats **/

/* Diagnostics */
#define CANTP_NORMAL_FIXED_PYSICAL      ((uint8)0xDA)
#define CANTP_NORMAL_FIXED_FUNCTIONAL   ((uint8)0xDB)

/* Remote Diagnostics */
#define CANTP_MIXED29_PYSICAL           ((uint8)0xCE)
#define CANTP_MIXED29_FUNCTIONAL        ((uint8)0xCD)

#define CANTP_AI(type, ta, sa) J1939_21_AI(J1939_21_DEFAULT_PRIORITY, 0x00, type, ta, sa)

#if 0
prio: immer 0b110 = > 6
dp:   immer 0
                    Normal fixed,
N_TAtype = pyhsical,      PF =
    218    0xDA    1101 1010
    Normal fixed,
N_TAtype = functional,    PF = 219    0xDB    1101 1011

                               Mixed
                               29bit, N_TAtype = pyhsical,       PF =
    206    0xCE    1100 1110
    Mixed
    29bit, N_TAtype = functional,     PF = 205    0xCD    1100 1101
#endif

#define CANTP_IS_NORMAL_ADDRESSING(id) ( (LOBYTE(HIWORD((id))) & (uint8)0x10) == (uint8)0x10 \
                                         ? (uint8)0 : (uint8)1) /* Dummy for now. todo: Besserer Name!!! */

/* Macros for Protocol-Control-Information.    */
/* todo: Address-Extension berücksichtigen!!!   */
#define CANTP_SET_SF_PCI(frame, len, ae)                                                             \
    _BEGIN_BLOCK                                                                                     \
        (frame)->SDU[0 + (ae)] = (CANTP_NPCI_TYPE_SF & (uint8)0xf0) | (LOBYTE((len)) & (uint8)0x0f); \
    _END_BLOCK

#define CANTP_SET_FF_PCI(frame, len, ae)                                                             \
    _BEGIN_BLOCK                                                                                     \
        (frame)->SDU[0 + (ae)] = (CANTP_NPCI_TYPE_FF & (uint8)0xf0) | (HIBYTE((len)) & (uint8)0x0f); \
    (frame)->SDU[1 + (ae)]     = LOBYTE((len));                                                      \
    _END_BLOCK

#define CANTP_SET_CF_PCI(frame, sn, ae)                                                             \
    _BEGIN_BLOCK                                                                                    \
        (frame)->SDU[0 + (ae)] = (CANTP_NPCI_TYPE_CF & (uint8)0xf0) | (LOBYTE((sn)) & (uint8)0x0f); \
    _END_BLOCK

#define CANTP_SET_FC_PCI(frame, fs, block_size, st_min, ae)                                         \
    _BEGIN_BLOCK                                                                                    \
        (frame)->SDU[0 + (ae)] = (CANTP_NPCI_TYPE_FC & (uint8)0xf0) | (LOBYTE((fs)) & (uint8)0x0f); \
    (frame)->SDU[1 + (ae)]     = (uint8)((block_size) & (uint8)0xff);                               \
    (frame)->SDU[2 + (ae)]     = (uint8)((st_min) & (uint8)0xff);                                   \
    _END_BLOCK

/*
**  Global Types.
*/
typedef enum tagCanTp_MessageType {
    CANTP_MESSAGE_TYPE_DIAGNOSTICS,
    CANTP_MESSAGE_TYPE_REMOTE_DIAGNOSTICS
} CanTp_MessageType;

/* Maps to the AUTOSAR-Parameter 'CAN-TP-CONNECTION-CHANNEL:AR:MULTICAST-ADDRESSING'. */
typedef enum tagCanTp_TargetAddressType {
    CANTP_TA_TYPE_PHYSICAL,     /* 1:1 comm., for all types of NL-Messages. */
                                /* Point to point communication, e.g.       */
                                /* 'ReadDataByteByIdentifier',              */
                                /* 'InputOutputControlByIdentifier'.        */
    CANTP_TA_TYPE_FUNCTIONAL    /* 1:N comm., only for SFs.                 */
                                /* Broadcast communication, phys. addr. of  */
                                /* ECU not known, e.g. 'ECU_RESET',         */
                                /* 'COMMUNICATION_CONTROL'.                 */
                                /* OBD communication will always be         */
                                /* performed as part of functional          */
                                /* addressing.                              */
} CanTp_TargetAddressType;

typedef struct tagCanTp_NetworkAddressInformationType { /* Network Address Information.     */
    uint8                   N_SA;                       /* Network Source Address.          */
    uint8                   N_TA;                       /* Network Target Address.          */
    CanTp_TargetAddressType N_TA_Type;                  /* Network Target Address Type.     */
} CanTp_NetworkAddressInformationType;

typedef enum tagCanTp_ResultUSDTType {
    CANTP_RESULT_OK,
    CANTP_RESULT_TIMEOUT_A,
    CANTP_RESULT_TIMEOUT_BS,
    CANTP_RESULT_TIMEOUT_CR,
    CANTP_RESULT_WRONG_SN,
    CANTP_RESULT_INVALID_FS,
    CANTP_RESULT_UNEXP_PDU,
    CANTP_RESULT_WFT_OVRN,
    CANTP_RESULT_BUFFER_OVFLW,
    CANTP_RESULT_ERROR
} CanTp_ResultUSDTType;

typedef enum tagCanTp_ResultChangeParameter {
    CANTP_RESULT_CP_OK,
    CANTP_RESULT_CP_RX_ON,
    CANTP_RESULT_CP_WRONG_PARAMETER,
    CANTP_RESULT_CP_WRONG_VALUE
} CanTp_ResultChangeParameter;

typedef enum tagIsoTp_FlowStatusType {
    CANTP_FC_CTS,       /* FLOW_STATE CLEAR_TO_SEND -- FS_CTS */
    CANTP_FC_WAIT,      /* WAIT     -- FS_WAIT  */
    CANTP_FC_OVFLW      /* OVERFLOW -- FS_OVFLW */
#if 0
    /* Flexray  */
    CANTP_FS_CNLDO,     /* Data Outdated - Abbruch wegen veralteter Daten.      */
    CANTP_FS_CNLNB,     /* No Buffer - Abbruch, da kein Empfangspuffer frei.    */
    CANTP_FS_CNLOR      /* Other Reason - Abbruch aus anderem Grund.            */
#endif
} IsoTp_FlowStatusType;

typedef struct tagIsoTp_FlowControlFrame {
    IsoTp_FlowStatusType    status;
    uint8                   block_size;
    uint8                   st_min;
} IsoTp_FlowControlFrame;

typedef enum tagIsoTp_NPCIType {
    CANTP_NPCI_SINGLE_FRAME,
    CANTP_NPCI_FIRST_FRAME,
    CANTP_NPCI_CONSECUTIVE_FRAME,
    CANTP_NPCI_FLOW_CONTROL
} IsoTp_NPCIType;

typedef struct tagIsoTp_NPCI {
    IsoTp_NPCIType Type;

    union {
        uint8                   DataLength;     /* Single- and First-Frame. */
        uint8                   SequenceNumber; /* Consecutive Frame.       */
        IsoTp_FlowControlFrame  FC;             /* Flow Control.            */
    };
} IsoTp_NPCI;

typedef struct tagIsoTp_PduType {
    MSCan_IdType    ID;
    uint8           Len;
    uint8 *         SDU;
} IsoTp_PduType;

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __COM_CAN_TP_H */
