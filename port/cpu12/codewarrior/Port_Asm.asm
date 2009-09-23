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
 
  PUBLIC Cpu_InterruptsDisabled
  PUBLIC Utl_SetJump  
  PUBLIC Utl_LongJump
  PUBLIC OsMLQ_GetLowestBitNumber
  PUBLIC Utl_Divrem
  PUBLIC Mem_GetStackPointer
  PUBLIC Mem_GetFreePointer
  
  XREF  OsCurrentTCB,__OS_TOS_ISRStack

MY_ROM: SECTION

ReversedLog2Tab:   
    dc.b    $00,$08,$07,$00,$06,$00,$00,$00,$05,$00,$00,$00,$00,$00,$00,$00
    dc.b    $04,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    dc.b    $03,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    dc.b    $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    dc.b    $02,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    dc.b    $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    dc.b    $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    dc.b    $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
    dc.b    $01


;.text



Cpu_InterruptsDisabled:
    tfr     ccr,b
    bitb    #$10
    bne     __id_l10
    clrb
    bra     __id_exit
__id_l10:    
    ldab    #$01    
__id_exit:    
    rts


Utl_SetJump:
    tfr     d,y
    movw    0,sp,2,y+
    sts     0,y
    ldd     #$0000
    rts
    
Utl_LongJump:
; 2,sp -> Pointer to Context.
; D -> Value.
    ldy     2,sp
    lds     2,y ; SP
    ldx     0,y ; PC
    tbne    d,__ljexit
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
    clra                        ; 1
    rts                         ; 5

;
;   void Utl_Divrem(uint16 dividend,uint16 rhs,Utl_DivremType *res);
;
Utl_Divrem:
;
;
;   4,SP	-> dividend
;   2,SP	-> divisor
;   D	 	-> res
    pshx
    tfr		d,y
    ldd		4,sp
    ldx		2,sp
    idiv
    std		2,y+
    stx		0,y
    pulx
    rts

Mem_GetStackPointer:
    tfr     sp,d
    rts


Mem_GetFreePointer:

    rts

    END
