/*
** Minimal utility macros previously provided by k-dk.
*/

#if !defined(__UTL_H)
#define __UTL_H

#include "Std_Types.h"
#include <setjmp.h>

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(x) ((void)(x))
#endif

#define UTL_BIT_SET8(val, bit)    ((val) = (uint8)((val) |  (uint8)(1u << (bit))))
#define UTL_BIT_RESET8(val, bit)  ((val) = (uint8)((val) & (uint8)(~(uint8)(1u << (bit)))))
#define UTL_BIT_GET8(val, bit)    (((uint8)(val) >> (bit)) & 0x1u)

#define UTL_BIT_SET16(val, bit)   ((val) = (uint16)((val) |  (uint16)(1u << (bit))))
#define UTL_BIT_RESET16(val, bit) ((val) = (uint16)((val) & (uint16)(~(uint16)(1u << (bit)))))
#define UTL_BIT_GET16(val, bit)   (((uint16)(val) >> (bit)) & 0x1u)

#ifndef BIT0
#define BIT0 ((uint16)0x0001u)
#endif

typedef jmp_buf Utl_JumpBufType;

sint32 Utl_SetJump(Utl_JumpBufType * env);
void Utl_LongJump(Utl_JumpBufType * env, sint32 value);

void Utl_MemCopy(void * dest, const void * src, SizeType len);
void Utl_MemSet(void * dest, uint8 value, SizeType len);

extern const uint16 Utl_SetBitTab16[16];

#endif /* __UTL_H */
