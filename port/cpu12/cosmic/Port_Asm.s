;
;   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
;
;  (C) 2007-2011 by Christoph Schueler <chris@konnex-tools.de,
;                                      cpu12.gems@googlemail.com>
;
;   All Rights Reserved
;
;   This program is free software; you can redistribute it and/or modify
;   it under the terms of the GNU General Public License as published by
;   the Free Software Foundation; version 2 of the License.
;
;   This program is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License
;   along with this program; if not, write to the Free Software
;   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
;
;   s. FLOSS-EXCEPTION.txt
;

	xdef _Utl_SetJump
	xdef _Utl_LongJump
	xdef _Utl_Divrem
	xdef _OsMLQ_GetLowestBitNumber	
	xdef _Mem_GetStackBOS

	xref __stack
	xref __stack_size


;	switch .const
.const:	section	.data
;	switch .data
;

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
	
	switch .text

_Utl_SetJump:
    tfr d,y
    movw    0,sp,2,y+   ; Save PC   - todo: +2!!!
    sts     0,y         ; Save Stack-Pointer
    ldd     #0x0000
    rts


_Utl_LongJump:
; RegD : Startup-Context.
; 2,sp: Value
    tfr     d,y
    lds     2,y ; SP
    ldx     0,y ; PC
    ldd     2,sp
    tbne    d,__ljexit
;    addd    #1
    inx
__ljexit:
    jmp     [0,x]
    rts

_OsMLQ_GetLowestBitNumber:
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
_Utl_Divrem:
;
;
;   D       -> dividend
;   2,sp    -> divisor
;   4,sp    -> res
    pshx
    ldx	    2+2,sp
    idiv
    ldy     4+2,sp
    std     2,y+
    stx     0,y
    pulx
    rts

_Mem_GetStackBOS:
    ldd #__stack
    subd #__stack_size
    rts

    end

