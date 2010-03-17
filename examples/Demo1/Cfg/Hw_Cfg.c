/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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
#include "Hw_Cfg.h"

/*
**  ATD
*/
const S12Atd_ConfigType S12Atd_Configuration[2]={
    {
        BASE_ADDR_ATD0,
        ((uint8)16),                /* Prescaler                    */
        TRUE,                       /* TenBit                       */
        S12ATD_EXT_TRIG_DISABLED,   /* ExternalTrigger              */
        FALSE,                      /* EnableCompletionInterrupt    */
        TRUE,                       /* ContinuousConversion         */
        S12ATD_CONV_TIME_SLOW       /* ConversionTime               */
    },
    {
        BASE_ADDR_ATD1,
        ((uint8)16),                /* Prescaler                    */
        TRUE,                       /* TenBit                       */
        S12ATD_EXT_TRIG_DISABLED,   /* ExternalTrigger              */
        FALSE,                      /* EnableCompletionInterrupt    */
        TRUE,                       /* ContinuousConversion         */
        S12ATD_CONV_TIME_SLOW       /* ConversionTime               */
    }
};


/*
**  BKP
*/
const S12Bkp_ConfigType BKP={
    BASE_ADDR_BKP
};


/*
**  CAN
*/
const S12Can_ConfigType S12Can_Configuration[5]={
    {
        BASE_ADDR_CAN0,
        {
            S12CAN_ACC_FLT_FOUR_16,
            S12CAN_4_16_FILTERS(0x0100U,0x0200U,0x0000U,0x0000U),
            S12CAN_4_16_FILTERS(0x0003U,0x0003U,0xffffU,0xffffU)
        }
    },
    {
        BASE_ADDR_CAN1,
        {
            S12CAN_ACC_FLT_FOUR_16,
            S12CAN_4_16_FILTERS(0x0000U,0x0000U,0x0000U,0x0000U),
            S12CAN_4_16_FILTERS(0xffffU,0xffffU,0xffffU,0xffffU)
        }
    },
    {
        BASE_ADDR_CAN2,
        {
            S12CAN_ACC_FLT_FOUR_16,
            S12CAN_4_16_FILTERS(0x0000U,0x0000U,0x0000U,0x0000U),
            S12CAN_4_16_FILTERS(0xffffU,0xffffU,0xffffU,0xffffU)
        }
    },
    {
        BASE_ADDR_CAN3,
        {
            S12CAN_ACC_FLT_FOUR_16,
            S12CAN_4_16_FILTERS(0x0000U,0x0000U,0x0000U,0x0000U),
            S12CAN_4_16_FILTERS(0xffffU,0xffffU,0xffffU,0xffffU)
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
#if defined(VM_USE_WATCHDOG)
    TRUE,           /* Enable Watchdog?     */
    CR2|CR1|CR0,    /* Watchdog Prescaler   */
#else
    FALSE,          /* Enable Watchdog?     */
    ((uint8)0),     /* Watchdog Prescaler   */
#endif /* VM_USE_WATCHDOG */

    FALSE,          /* Enable RTI?          */
    ((uint8)5),     /* RTI Prescaler        */
    ((uint8)14)     /* RTI Modulo           */
};


/*
**  ECT
*/
const S12Ect_ConfigType ECT={
    BASE_ADDR_ECT,  /* Base-Address         */

    (uint16)1000,           /* cycle - (od. Period/TimeBase???) in Nano-Secs. */
    ((uint8)0x00),          /* TIos     */
    TEN|TSWAI|TSFRZ,        /* TScr1    */
    TOI|PR0|PR1,            /* TScr2    */
    ((uint8)0x00),          /* TCtl1    */
    ((uint8)0x00),          /* TCtl2    */
    ((uint8)0x00),          /* TCtl3    */
    ((uint8)0x00),          /* TCtl4    */
    ((uint8)0x00),          /* TIe Interrupt-Freigabe je nach Cfg. */
    PAEN|PAOVI|PEDGE,       /* PActl    */
    PBEN|PBOVI,             /* PBctl    */
    ((uint8)0x00),          /* ICpar    */
    MCZI|MODMC|MCEN|MCPR1,  /* MCctl    */
    VM_SYSTEM_TIMER_FREQ,   /* MCcnt    */
    ((uint8)0x00),          /* DLyct    */
    ((uint8)0x00),          /* ICovw    */
    ((uint8)0x00),          /* ICsys    */
};

/*
**  FLASH
*/
const S12Fls_ConfigType FLS={
    BASE_ADDR_FLS
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
**  INT
*/
const S12Int_ConfigType INT={
    BASE_ADDR_INT
};


/*
**  MEBI
*/
const S12Mebi_ConfigType MEBI={
    BASE_ADDR_MEBI,

    PUPKE|PUPEE|PUPBE|PUPAE,    /* Pucr     */
    ((uint8)0x00),              /* Rdriv    */
    ((uint8)0x00),              /* IrqCr    */
    ((uint8)0x00),              /* DdrA     */
    ((uint8)0x00),              /* PortA    */
    ((uint8)0x00),              /* DdrB     */
    ((uint8)0x00),              /* PortB    */
    ((uint8)0x00),              /* DdrE     */
    ((uint8)0x00),              /* PortE    */
    ((uint8)0x00),              /* DdrK     */
    ((uint8)0x00),              /* PortK    */
};


/*
**  MMC
*/
const S12Mmc_ConfigType MMC={
    BASE_ADDR_MMC,
    ((uint8)0x09),  /*  InitRM  */
    ((uint8)0x00),  /*  InitRG  */
    ((uint8)0x01),  /*  InitEE  */
    ((uint8)0x0d),  /*  Misc    */
};


/*
**  PIM
*/
const S12Pim_ConfigType PIM={
    BASE_ADDR_PIM,
    ((uint8)0x00),  /* Modrr    */

    /* PortT */
    ((uint8)0x00),  /* DdrT     */
    ((uint8)0x00),  /* PtT      */
    ((uint8)0x00),  /* RdrT     */
    ((uint8)0xff),  /* PerT     */
    ((uint8)0x00),  /* PpsT     */

    /* PortS */
    ((uint8)0x00),  /* DdrS     */
    ((uint8)0x00),  /* PtS      */
    ((uint8)0x00),  /* RdrS     */
    ((uint8)0xff),  /* PerS     */
    ((uint8)0x00),  /* PpsS     */
    ((uint8)0xf0),  /* WomS     */

    /* PortM */
    ((uint8)0x00),  /* DdrM     */
    ((uint8)0x00),  /* PtM      */
    ((uint8)0x00),  /* RdrM     */
    ((uint8)0xff),  /* PerM     */
    ((uint8)0x00),  /* PpsM     */
    ((uint8)0xf0),  /* WomM     */

    /* PortP */
    ((uint8)0x00),  /* DdrP     */
    ((uint8)0x00),  /* PtP      */
    ((uint8)0x00),  /* RdrP     */
    ((uint8)0xff),  /* PerP     */
    ((uint8)0x00),  /* PpsP     */
    ((uint8)0x00),  /* PieP     */

    /* PortH */
    ((uint8)0x00),  /* DdrH     */
    ((uint8)0x00),  /* PtH      */
    ((uint8)0x00),  /* RdrH     */
    ((uint8)0xff),  /* PerH     */
    ((uint8)0x00),  /* PpsH     */
    ((uint8)0x00),  /* PieH     */

    /* PortJ */
    ((uint8)0x00),  /* DdrJ     */
    ((uint8)0x00),  /* PtJ      */
    ((uint8)0x00),  /* RdrJ     */
    ((uint8)0xff),  /* PerJ     */
    ((uint8)0x00),  /* PpsJ     */
    ((uint8)0x00),  /* PieJ     */
};


/*
**  PWM
*/
const S12Pwm_ConfigType PWM={
    BASE_ADDR_PWM,

    ((uint8)0xaa),                      /* PwmE                             */
    CON67|CON45|CON23|CON01|PSWAI|PFRZ, /* PwmCtl - 16Bit channels.         */
    ((uint8)0xaa),                      /* PwmPol                           */
    ((uint8)0xaa),                      /* PwmClk - use scaled clocks.      */
    ((uint8)0x00),                      /* PwmPrClk                         */
    ((uint8)0x00),                      /* PwmCae - left aligned channels.  */
    ((uint8)0x05),                      /* PwmSclA                          */
    ((uint8)0x05),                      /* PwmSclB                          */
    ((uint8)0x00),                      /* PwmSdn                           */

    ((uint16)0xffffU),                  /* PwmPer01                         */
    ((uint16)0xffffU),                  /* PwmPer23                         */
    ((uint16)0xffffU),                  /* PwmPer45                         */
    ((uint16)0xffffU),                  /* PwmPer67                         */

    ((uint16)0x0000U),                  /* PwmDty01                         */
    ((uint16)0x0000U),                  /* PwmDty23                         */
    ((uint16)0x0000U),                  /* PwmDty45                         */
    ((uint16)0x0000U),                  /* PwmDty67                         */
};


/*
**  SCI
*/
static S12Sci_VariablesType ComPort[2];
static uint8 SCI0_RxBuffer[SCI0_RX_BUF_LEN],SCI1_RxBuffer[SCI1_RX_BUF_LEN];

const S12Sci_ConfigType S12Sci_Configuration[2]={
    {
        BASE_ADDR_SCI0,
        ((uint32)19200),
        SCI_PARITY_NONE,
        ((uint8)8),
        SCI0_RxBuffer,
        SCI0_RX_BUF_LEN,
        &ComPort[0],
        (HW_EventNotifyFunc)NULL,
        (HW_EventNotifyFunc)NULL
    },
    {
        BASE_ADDR_SCI1,
        ((uint32)19200),
        SCI_PARITY_NONE,
        ((uint8)8),
        SCI1_RxBuffer,
        SCI1_RX_BUF_LEN,
        &ComPort[1],
        (HW_EventNotifyFunc)NULL,
        (HW_EventNotifyFunc)NULL
    }
};


/*
**  SPI
*/

static S12Spi_VariablesType SPIVars[3];

const S12Spi_ConfigType S12Spi_Configuration[3]={
    {
        BASE_ADDR_SPI0,
        (uint32)100000,
        &SPIVars[0]
    }
};
