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

	.file	"Port_Asm.s"
	.mode mshort
	.sect	.text
	.globl	SetJump
	.type	SetJump,@function  
	.globl	LongJump
	.type	LongJump,@function  
	.globl  InterruptsDisabled
	.type   InterruptsDisabled,@function
	.globl  OsUtilLog2O1
	.type   OsUtilLog2O1,@function

SetJump:
; D -> Pointer to Context.
    tfr d,y
    movw    0,sp,2,y+ 
    sts     0,y
    ldd     #0x0000
    rts
    
LongJump:
; D -> Pointer to Context.
; 0,sp -> Value.
    tfr d,y
    lds     2,y ; SP
    ldx     0,y ; PC
    ldd 0,sp
    tbne    d,__ljexit
;    addd    #1
    inx 
__ljexit:    
    jmp     [0,x]
    rts
    
InterruptsDisabled:
    tfr ccr,b
    andb #0x10
    rts
    
OsUtilLog2O1:   ; SelectHighestPriority
    ldy     #Log2Table  ; 2
    tbeq    a,L10       ; 3

    ldab    a,y         ; 3
    bra     L20         ; 3
L10:
    ldab    b,y         ; 3
    addb    #8          ; 1
    nop                 ; 1
    nop                 ; 1
L20:
    clra                ; 1
    rts                 ; 5
    