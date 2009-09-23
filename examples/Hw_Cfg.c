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

#include "Hw_Cfg.h"


/*
**  ATD
*/
const S12Atd_ConfigType S12Atd_Configuration[2]={
    {
        BASE_ADDR_ATD0,
        ((uint8)16),
        TRUE,
        S12ATD_EXT_TRIG_DISABLED,
        TRUE,   /* Intr. */
        TRUE,
        S12ATD_CONV_TIME_SLOW
    },
    {
        BASE_ADDR_ATD1,
        ((uint8)16),
        TRUE,
        S12ATD_EXT_TRIG_DISABLED,
        FALSE,    
        TRUE,
        S12ATD_CONV_TIME_SLOW
    }
};


/*
**  CAN
*/
const S12Can_ConfigType S12Can_Configuration[5]={
    {
        BASE_ADDR_CAN0,
        {
            S12CAN_ACC_FLT_FOUR_16,
            S12CAN_4_16_FILTERS(0x0100,0x0200,0x0000,0x0000),
            S12CAN_4_16_FILTERS(0x0003,0x0003,0xffff,0xffff)
        }
    },
    {
        BASE_ADDR_CAN1,
        {
            S12CAN_ACC_FLT_FOUR_16,
            S12CAN_4_16_FILTERS(0x0000,0x0000,0x0000,0x0000),
            S12CAN_4_16_FILTERS(0xffff,0xffff,0xffff,0xffff)
        }
    },
    {
        BASE_ADDR_CAN2,
        {
            S12CAN_ACC_FLT_FOUR_16,
            S12CAN_4_16_FILTERS(0x0000,0x0000,0x0000,0x0000),
            S12CAN_4_16_FILTERS(0xffff,0xffff,0xffff,0xffff)
        }
    },
    {
        BASE_ADDR_CAN3,
        {
            S12CAN_ACC_FLT_FOUR_16,
            S12CAN_4_16_FILTERS(0x0000,0x0000,0x0000,0x0000),
            S12CAN_4_16_FILTERS(0xffff,0xffff,0xffff,0xffff)
        }
    },
    {
        BASE_ADDR_CAN4,
        {
            S12CAN_ACC_FLT_FOUR_16,
            S12CAN_4_16_FILTERS(0x0000,0x0000,0x0000,0x0000),
            S12CAN_4_16_FILTERS(0xffff,0xffff,0xffff,0xffff)
        }
    }
};


/*
**  CRG
*/
const S12Crg_ConfigType CRG={
    BASE_ADDR_CRG,  /* Base-Address         */
    ((uint8)16),    /* OSC-Freq.            */
    ((uint8)25),    /* Max. Bus Freq.       */
    FALSE,          /* Enable Watchdog?     */
    ((uint8)0),     /* Watchdog Prescaler   */
    TRUE,           /* Enable RTI?          */
    ((uint8)5),     /* RTI Prescaler        */
    ((uint8)14)     /* RTI Modulo           */
};


/*
**  ECT
*/
const S12Ect_ConfigType ECT={
    BASE_ADDR_ECT,  /* Base-Address         */
};


/*
**  IIC
*/
const S12Iic_ConfigType IIC0={
    BASE_ADDR_IIC0,
    ((uint8)0x18),   /* 25MHz: 0x23 */
    /* Fastmode
       ========
       8MHz    IBCR=0x00   // DIV=20 SDA_Hold=7
       25MHz   IBCR=0x49   // Div=64 SDA_Hold=14
    */
};


/*
**  MEBI
*/
const S12Mebi_ConfigType MEBI={
    BASE_ADDR_MEBI  
};


/*
**  SCI
*/
static S12Sci_VariablesType ComPort0,ComPort1;
static uint8 SCI0_RxBuffer[SCI0_RX_BUF_LEN],SCI1_RxBuffer[SCI1_RX_BUF_LEN];

const S12Sci_ConfigType S12Sci_Configuration[2]={
    {
        BASE_ADDR_SCI0,
        ((uint16)38400),
        SCI_PARITY_NONE,
        ((uint8)8),
        SCI0_RxBuffer,
        SCI0_RX_BUF_LEN,
        &ComPort0,
        (HW_EventNotifyFunc)NULL,
        (HW_EventNotifyFunc)NULL
    },
    {
        BASE_ADDR_SCI1,
        ((uint16)38400),
        SCI_PARITY_NONE,
        ((uint8)8),
        SCI1_RxBuffer,
        SCI1_RX_BUF_LEN,        
        &ComPort1,
        (HW_EventNotifyFunc)NULL,
        (HW_EventNotifyFunc)NULL
    }
};
