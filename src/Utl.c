/*
 * Minimal utility implementations replacing k-dk dependencies.
 */

#include "Utl.h"
#include <string.h>

const uint16 Utl_SetBitTab16[16] = {
    0x0001u, 0x0002u, 0x0004u, 0x0008u,
    0x0010u, 0x0020u, 0x0040u, 0x0080u,
    0x0100u, 0x0200u, 0x0400u, 0x0800u,
    0x1000u, 0x2000u, 0x4000u, 0x8000u
};

sint32 Utl_SetJump(Utl_JumpBufType * env)
{
    return setjmp(*env);
}

void Utl_LongJump(Utl_JumpBufType * env, sint32 value)
{
    longjmp(*env, value);
}

void Utl_MemCopy(void * dest, const void * src, SizeType len)
{
    if (len != 0u) {
        (void)memcpy(dest, src, (size_t)len);
    }
}

void Utl_MemSet(void * dest, uint8 value, SizeType len)
{
    if (len != 0u) {
        (void)memset(dest, (int)value, (size_t)len);
    }
}
