/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

 * (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>

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
#include "WinRand.h"
#include <stdio.h>
#include <Wincrypt.h>

static HCRYPTPROV hCryptoProvider;

__int8  WinRand_I8(void);
__int16 WinRand_I16(void);
__int32 WinRand_I32(void);
__int64 WinRand_I64(void);


BOOL WinRand_Init(void)
{
    BOOL res;

    res = CryptAcquireContext(&hCryptoProvider, NULL, NULL, PROV_RSA_FULL, 0);

    return res;
}


BOOL WinRand_RandomBytes(DWORD count, BYTE * randomBytes)
{
    BOOL res;

    res = CryptGenRandom(hCryptoProvider, count, (BYTE *)randomBytes);

    return res;
}


__int8 WinRand_I8(void)
{
    __int8 res;
    BYTE randomBytes[1];

    if (WinRand_RandomBytes(1UL, (BYTE *)randomBytes)) {
        res = randomBytes[0];
    }

    return res;
}


__int16 WinRand_I16(void)
{
    __int16 res;
    BYTE randomBytes[2];

    if (WinRand_RandomBytes(2UL, (BYTE *)randomBytes)) {
        res = *(__int16 * )randomBytes;
    }

    return res;
}


__int32 WinRand_I32(void)
{
    __int32 res;
    BYTE randomBytes[4];

    if (WinRand_RandomBytes(4UL, (BYTE *)randomBytes)) {
        res = randomBytes[0];
        res = randomBytes[1];
        res = *(__int32 * )randomBytes;
    }

    return res;
}


__int64 WinRand_I64(void)
{
    __int64 res;
    BYTE randomBytes[8];

    if (WinRand_RandomBytes(8UL, (BYTE *)randomBytes)) {
        res = randomBytes[0];
        res = randomBytes[1];
        res = *(__int64 * )randomBytes;
    }

    return res;
}


void WinRand_Test(void)
{
    int i;
    __int64 I64;

    WinRand_Init();

    for (i = 0; i < 64; ++i) {
        I64 = WinRand_I64();
        printf("%#0I64X\n", I64);
    }
    printf("\n");
}
