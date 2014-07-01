;
;  k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
;
;  (C) 2007-2013 by Christoph Schueler <chris@konnex-tools.de,
;                                      cpu12.gems@googlemail.com>
;
;  All Rights Reserved
;
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; version 2 of the License.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with this program; if not, write to the Free Software
;  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
;
;  s. FLOSS-EXCEPTION.txt
;

  PUBLIC CPU_INTERRUPTS_DISABLED
  PUBLIC Utl_SetJump
  PUBLIC Utl_LongJump
  PUBLIC OsMLQ_GetLowestBitNumber
  PUBLIC Utl_Divrem
  PUBLIC Mem_GetStackPointer
  PUBLIC Mem_GetFreePointer
  PUBLIC  OsPort_TaskStackInit

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


RETURN_FROM_SUBROUTINE:  MACRO
  rtc
  ENDM

CPU_INTERRUPTS_DISABLED:
    tfr     ccr,b
    bitb    #$10
    bne     __id_l10
    clrb
    bra     __id_exit
__id_l10:
    ldab    #$01
__id_exit:
    RETURN_FROM_SUBROUTINE


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
    RETURN_FROM_SUBROUTINE


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
    RETURN_FROM_SUBROUTINE

OsPort_TaskStackInit:
; TODO: Calling conventions!!!
;   D -> StackPointer
;   X   -> TaskFunc
    pshy
    tfr     d,  y
    ldx     0,  x
    stx     1, -y

;    movb    #$c0, 1 ,-y
    leay     -6, y
    movw     #$00c0, 2, -y

    tfr     y,  d
    puly
    RETURN_FROM_SUBROUTINE



;
;   void Utl_Divrem(uint16 dividend,uint16 rhs,Utl_DivremType *res);
;
Utl_Divrem:
;
;
;   4,SP        -> dividend
;   2,SP        -> divisor
;   D           -> res
    pshx
    tfr         d,y
    ldd         4,sp
    ldx         2,sp
    idiv
    std         2,y+
    stx         0,y
    pulx
    RETURN_FROM_SUBROUTINE

Mem_GetStackPointer:
    tfr     sp,d
    RETURN_FROM_SUBROUTINE

Fhokker dc.b 1

Spl_Get:
  btas        Fhokker, #1
  rts

Spl_TryGet:
  rts

Spl_Release:
  rts


Mem_GetFreePointer:

    RETURN_FROM_SUBROUTINE

    END

