; 
;  k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
;  
;  (C) 2007-2009 by Christoph Schueler <chris@konnex-tools.de>
;  
;  All Rights Reserved
; 
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2 of the License, or
;  (at your option) any later version.
; 
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License along
;  with this program; if not, write to the Free Software Foundation, Inc.,
;  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
;


    NAME port_iar

MEM_STACK_OK=0
MEM_STACK_OVERFLOW=1
MEM_STACK_UNDERFLOW=2

    PUBLIC  OS_START_CURRENT_TASK
    PUBLIC  OS_SAVE_CONTEXT
    PUBLIC  OS_RESTORE_CONTEXT
    PUBLIC  OS_ISR_CONTEXT
    PUBLIC  Utl_SetJump
    PUBLIC  Utl_LongJump
    PUBLIC  OsMLQ_GetLowestBitNumber

    PUBLIC  Utl_Divrem

    PUBLIC  Mem_GetStackPointer
    PUBLIC  Mem_GetFreePointer
    PUBLIC  Mem_GetStackTOS
    PUBLIC  Mem_GetStackBOS
    PUBLIC  Mem_StackCheck
    
    PUBLIC  MemoryStuff

    EXTERN  OsCurrentTCB
    EXTERN  OS_TOS_ISR

    rseg    CSTACK:DATA:REORDER:NOROOT(0)   ; ForwardDBG_CODE Declarations.    
    rseg    DATA16_Z:DATA:REORDER:NOROOT(0)        

    rseg    DBG_CODE:DATA:REORDER:NOROOT(0)

    RSEG DATA16_C:CONST:REORDER:NOROOT(0)   ; Constant-Segment.

ReversedLog2Tab:
    DC.B    0x00,0x08,0x07,0x00,0x06,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    DC.B    0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    DC.B    0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    DC.B    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    DC.B    0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    DC.B    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    DC.B    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    DC.B    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    DC.B    0x01

    RSEG CODE:CODE:REORDER:NOROOT(0)        ; Code-Segment.

OS_START_CURRENT_TASK:
    ldy     OsCurrentTCB
    lds     0,y
    rti
  

OS_SAVE_CONTEXT:
    ldy     OsCurrentTCB
    tfr     sp,d
    addd    #2
    std     0,y
    rts
  

OS_RESTORE_CONTEXT:
    ldy     OsCurrentTCB
    lds     0,y
    rti


OS_ISR_CONTEXT:
    ldd     0,sp
    ldy     OS_TOS_ISR
    std     0,y
    tfr     y,sp
    rts  


Utl_SetJump:
    movw    0,sp,2,y+
    sts     0,y
    ldd     #0x0000
    rts
    

Utl_LongJump:
; Y -> Pointer to Context.
; D -> Value.
    lds     2,y ; SP
    ldx     0,y ; PC
    tbne    d,__ljexit
;    addd    #1
    inx 
__ljexit:    
    jmp     [0,x]
    rts

 
OsMLQ_GetLowestBitNumber:
    ldy     #ReversedLog2Tab    ; 2
    tbeq    a,L10               ; 3
    ldab    a,y                 ; 3
    bra     L20                 ; 3
L10:
    ldab    b,y                 ; 3
    addb    #8                  ; 1
    nop                         ; 1 - remove jitter.
    nop                         ; 1
L20:
    rts                         ; 5

;
;   void Utl_Divrem(uint16 dividend,uint16 divisor,Utl_DivremType *res);
;
Utl_Divrem:
;
;
;   D       -> dividend
;   Y       -> divisor
;   2,sp    -> res
    pshx
    tfr     y,x
    idiv
    ldy     2+2,sp
    std     2,y+
    stx     0,y
    pulx
    rts

Mem_GetStackPointer:
    tfr     sp,d
    rts

Mem_GetStackBOS:
    ldd     #SFB(CSTACK)
    rts

Mem_GetStackTOS:
    ldd     #SFE(CSTACK)
    rts
    
Mem_StackCheck:
    movw    #SFE(CSTACK),2,-sp
    movw    #SFB(CSTACK),2,-sp
    tfr     sp,d
    cpd     0,sp
    bhs     st_cont1
    ldab    #MEM_STACK_OVERFLOW
    bra     st_exit
st_cont1:   
    cpd     2,sp
    bls     st_cont2
    ldab    #MEM_STACK_UNDERFLOW
    bra     st_exit
st_cont2:
    ldab    #MEM_STACK_OK
st_exit:
    leas    4,sp
    rts

Mem_GetFreePointer:
    ldy     #SFE(DATA16_Z)
    rts

MemoryStuff:
    ldd     #SFB(CSTACK)
    ldd     #SFE(CSTACK)
    ldd     #SIZEOF(CSTACK)    

    ldd     #SFB(DATA16_Z)
    ldd     #SFE(DATA16_Z)
    ldd     #SIZEOF(DATA16_Z)
    
    ldd     #SIZEOF(DBG_CODE)

    rts

    END
    