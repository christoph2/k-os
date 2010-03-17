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
#if !defined(__MEMMAP_H)
#define	__MEMMAP_H

/*
**  Plain Memory-Mapping for IAR-HCS12 - we need a _LINKER-FILE_-Configuration Tool !!!
*/
/* This File is Configuration-Specific. */

#define	MEMMAP_VENDOR_ID           ((uint16)4711)
#define	MEMMAP_AR_MAJOR_VERSION	((uint8)1)
#define	MEMMAP_AR_MINOR_VERSION	((uint8)0)
#define	MEMMAP_AR_PATCH_VERSION	((uint8)0)
#define	MEMMAP_SW_MAJOR_VERSION	((uint8)0)
#define	MEMMAP_SW_MINOR_VERSION	((uint8)1)
#define	MEMMAP_SW_PATCH_VERSION	((uint8)0)

#define MEMMAP_ERROR

/*
**
**  Section-Start Keywords.
**
*/



/*
**	OS: OS.
*/
#if defined(OS_START_SEC_CODE)
	#undef OS_START_SEC_CODE
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_NOINIT_BOOLEAN)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_NOINIT_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_NOINIT_8BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_NOINIT_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_NOINIT_16BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_NOINIT_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_NOINIT_32BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_NOINIT_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_NOINIT_UNSPECIFIED)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_NOINIT_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_POWER_ON_INIT_BOOLEAN)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_POWER_ON_INIT_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_POWER_ON_INIT_8BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_POWER_ON_INIT_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_POWER_ON_INIT_16BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_POWER_ON_INIT_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_POWER_ON_INIT_32BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_POWER_ON_INIT_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_FAST_BOOLEAN)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_FAST_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_FAST_8BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_FAST_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_FAST_16BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_FAST_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_FAST_32BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_FAST_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_FAST_UNSPECIFIED)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_FAST_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_INTERNAL_VAR_BOOLEAN)
	#pragma dataseg=default
	#undef OS_START_SEC_INTERNAL_VAR_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_INTERNAL_VAR_8BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_INTERNAL_VAR_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_INTERNAL_VAR_16BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_INTERNAL_VAR_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_INTERNAL_VAR_32BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_INTERNAL_VAR_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_INTERNAL_VAR_UNSPECIFIED)
	#pragma dataseg=default
	#undef OS_START_SEC_INTERNAL_VAR_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_BOOLEAN)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_8BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_16BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_32BIT)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_VAR_UNSPECIFIED)
	#pragma dataseg=default
	#undef OS_START_SEC_VAR_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CONST_BOOLEAN)
	#pragma constseg=default
	#undef OS_START_SEC_CONST_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CONST_8BIT)
	#pragma constseg=default
	#undef OS_START_SEC_CONST_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CONST_16BIT)
	#pragma constseg=default
	#undef OS_START_SEC_CONST_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CONST_32BIT)
	#pragma constseg=default
	#undef OS_START_SEC_CONST_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CONST_UNSPECIFIED)
	#pragma constseg=default
	#undef OS_START_SEC_CONST_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CONFIG_DATA_BOOLEAN)
	#pragma constseg=default
	#undef OS_START_SEC_CONFIG_DATA_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CONFIG_DATA_8BIT)
	#pragma constseg=default
	#undef OS_START_SEC_CONFIG_DATA_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CONFIG_DATA_16BIT)
	#pragma constseg=default
	#undef OS_START_SEC_CONFIG_DATA_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CONFIG_DATA_32BIT)
	#pragma constseg=default
	#undef OS_START_SEC_CONFIG_DATA_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CONFIG_DATA_UNSPECIFIED)
	#pragma constseg=default
	#undef OS_START_SEC_CONFIG_DATA_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CODE_SLOW)
	#undef OS_START_SEC_CODE_SLOW
	#undef MEMMAP_ERROR
#elif defined(OS_START_SEC_CODE_LIB)
	#undef OS_START_SEC_CODE_LIB
	#undef MEMMAP_ERROR
#endif

/*
**
**  Section-Stop Keywords.
**
*/



/*
**	OS: OS.
*/
#if defined(OS_STOP_SEC_CODE)
	#undef OS_STOP_SEC_CODE
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_NOINIT_BOOLEAN)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_NOINIT_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_NOINIT_8BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_NOINIT_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_NOINIT_16BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_NOINIT_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_NOINIT_32BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_NOINIT_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_NOINIT_UNSPECIFIED)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_POWER_ON_INIT_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_POWER_ON_INIT_8BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_POWER_ON_INIT_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_POWER_ON_INIT_16BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_POWER_ON_INIT_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_POWER_ON_INIT_32BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_POWER_ON_INIT_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_FAST_BOOLEAN)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_FAST_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_FAST_8BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_FAST_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_FAST_16BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_FAST_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_FAST_32BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_FAST_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_FAST_UNSPECIFIED)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_FAST_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_INTERNAL_VAR_BOOLEAN)
	#pragma dataseg=default
	#undef OS_STOP_SEC_INTERNAL_VAR_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_INTERNAL_VAR_8BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_INTERNAL_VAR_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_INTERNAL_VAR_16BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_INTERNAL_VAR_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_INTERNAL_VAR_32BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_INTERNAL_VAR_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_INTERNAL_VAR_UNSPECIFIED)
	#pragma dataseg=default
	#undef OS_STOP_SEC_INTERNAL_VAR_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_BOOLEAN)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_8BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_16BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_32BIT)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_VAR_UNSPECIFIED)
	#pragma dataseg=default
	#undef OS_STOP_SEC_VAR_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CONST_BOOLEAN)
	#pragma constseg=default
	#undef OS_STOP_SEC_CONST_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CONST_8BIT)
	#pragma constseg=default
	#undef OS_STOP_SEC_CONST_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CONST_16BIT)
	#pragma constseg=default
	#undef OS_STOP_SEC_CONST_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CONST_32BIT)
	#pragma constseg=default
	#undef OS_STOP_SEC_CONST_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CONST_UNSPECIFIED)
	#pragma constseg=default
	#undef OS_STOP_SEC_CONST_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CONFIG_DATA_BOOLEAN)
	#pragma constseg=default
	#undef OS_STOP_SEC_CONFIG_DATA_BOOLEAN
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CONFIG_DATA_8BIT)
	#pragma constseg=default
	#undef OS_STOP_SEC_CONFIG_DATA_8BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CONFIG_DATA_16BIT)
	#pragma constseg=default
	#undef OS_STOP_SEC_CONFIG_DATA_16BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CONFIG_DATA_32BIT)
	#pragma constseg=default
	#undef OS_STOP_SEC_CONFIG_DATA_32BIT
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
	#pragma constseg=default
	#undef OS_STOP_SEC_CONFIG_DATA_UNSPECIFIED
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CODE_SLOW)
	#undef OS_STOP_SEC_CODE_SLOW
	#undef MEMMAP_ERROR
#elif defined(OS_STOP_SEC_CODE_LIB)
	#undef OS_STOP_SEC_CODE_LIB
	#undef MEMMAP_ERROR
#endif


/*
**
**  Error-Check.
**
*/
#if defined(MEMMAP_ERROR)
    #error No valid memory mapping symbol defined.
#endif

#endif	/* __MEMMAP_H */

