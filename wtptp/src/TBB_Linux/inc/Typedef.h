/******************************************************************************
 *
 *  (C)Copyright 2005 - 2011 Marvell. All Rights Reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MARVELL.
 *  The copyright notice above does not evidence any actual or intended
 *  publication of such source code.
 *  This Module contains Proprietary Information of Marvell and should be
 *  treated as Confidential.
 *  The information in this file is provided for the exclusive use of the
 *  licensees of Marvell.
 *  Such users have the right to use, modify, and incorporate this code into
 *  products for purposes authorized by the license agreement provided they
 *  include this notice and the associated copyright notice with any such
 *  product.
 *  The information in this file is provided "AS IS" without warranty.
 
 ******************************************************************************/
 /********************************************************************************
Marvell BSD License Option

If you received this File from Marvell, you may opt to use, redistribute and/or
modify this File under the following licensing terms.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice,
	  this list of conditions and the following disclaimer.

	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.

	* Neither the name of Marvell nor the names of its contributors may be
	  used to endorse or promote products derived from this software without
	  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************/


#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

/**
 * Useful constants
 **/
#define BIT0        0x00000001
#define BIT1        0x00000002
#define BIT2        0x00000004
#define BIT3        0x00000008
#define BIT4        0x00000010
#define BIT5        0x00000020
#define BIT6        0x00000040
#define BIT7        0x00000080
#define BIT8        0x00000100
#define BIT9        0x00000200
#define BIT10       0x00000400
#define BIT11       0x00000800
#define BIT12       0x00001000
#define BIT13       0x00002000
#define BIT14       0x00004000
#define BIT15       0x00008000
#define BIT16       0x00010000
#define BIT17       0x00020000
#define BIT18       0x00040000
#define BIT19       0x00080000
#define BIT20       0x00100000
#define BIT21       0x00200000
#define BIT22       0x00400000
#define BIT23       0x00800000
#define BIT24       0x01000000
#define BIT25       0x02000000
#define BIT26       0x04000000
#define BIT27       0x08000000
#define BIT28       0x10000000
#define BIT29       0x20000000
#define BIT30       0x40000000
#define BIT31       0x80000000
#define SET32       0xffffffff
#define CLEAR32     0x00000000

#define BU_U32          unsigned int
#define BU_U16          unsigned short
#define BU_U8           unsigned char

typedef volatile unsigned int VUINT32_T;
typedef unsigned int UINT32_T;
typedef unsigned long long UINT64_T;
typedef volatile unsigned int VUINT_T;
typedef unsigned int UINT_T;
typedef int INT_T;
typedef unsigned short UINT16_T, USHORT;
typedef volatile unsigned short VUINT16_T;
typedef unsigned char UINT8_T;
typedef char INT8_T;

#define TRUE						1
#define FALSE						0
#define LOCKED						1
#define UNLOCKED					0

#ifndef NULL
#define NULL						0
#endif

typedef void (*FnPVOID)(void);

typedef unsigned int UINT, *PUINT;    // The size is not important
typedef unsigned long long UINT64, *PUINT64;
typedef unsigned int UINT32, *PUINT32;
typedef unsigned short UINT16, *PUINT16;
typedef unsigned char UINT8, *PUINT8;
typedef unsigned char UCHAR, BYTE, *PUCHAR;

typedef int INT, *PINT;    // The size is not important
typedef long long INT64, *PINT64;
typedef int INT32, *PINT32;
typedef short INT16, *PINT16;
typedef char CHAR, *PCHAR;

typedef volatile UINT VUINT, *PVUINT;    // The size is not important
typedef volatile UINT64 VUINT64, *PVUINT64;
typedef volatile UINT32 VUINT32, *PVUINT32;
typedef volatile UINT16 VUINT16, *PVUINT16;
typedef volatile UINT8 VUINT8, *PVUINT8;
typedef volatile UCHAR VUCHAR, *PVUCHAR;

typedef volatile INT VINT, *PVINT;    // The size is not important
typedef volatile INT64 VINT64, *PVINT64;
typedef volatile INT32 VINT32, *PVINT32;
typedef volatile INT16 VINT16, *PVINT16;
typedef volatile CHAR VCHAR, *PVCHAR;

typedef UINT16 WORD; /* Unsigned 16-bit quantity       */
typedef UINT32 DWORD; /* Unsigned 32-bit quantity       */
typedef void* VOID_PTR;

typedef struct {
	UINT_T ErrorCode;
	UINT_T StatusCode;
	UINT_T PCRCode;
} FUNC_STATUS, *pFUNC_STATUS;

//some simple defines to clean up code a touch
#define reg_write(reg, val) *(volatile unsigned int *)reg = val; *(volatile unsigned int *)reg;
#define reg_bit_set(reg, val) *(volatile unsigned int *)reg |= val; *(volatile unsigned int *)reg;
#define reg_bit_clr(reg, val) *(volatile unsigned int *)reg &= ~val; *(volatile unsigned int *)reg;

#define BU_REG_READ(x) (*(volatile UINT32 *)(x))
#define BU_REG_WRITE(x,y) ((*(volatile UINT32 *)(x)) = y )

#define BU_REG_READ16(x) (*(volatile UINT16 *)(x) & 0xffff)
#define BU_REG_WRITE16(x,y) ((*(volatile UINT16 *)(x)) = y & 0xffff )

#define BU_REG_READ8(x) (*(volatile UINT8 *)(x) & 0xff)
#define BU_REG_WRITE8(x,y) ((*(volatile UINT8 *)(x)) = y & 0xff )

#define BU_REG_RDMDFYWR(x,y)  (BU_REG_WRITE(x,((BU_REG_READ(x))|y)))

#undef DBGPRINT

// Operating Mode Type for BootLoader
typedef enum {
	UPGRADESW = 1, DOWNLOAD, SINGLE_TIM_BOOT, DUAL_TIM_BOOT
} OPERATING_MODE_T;

#endif //_TYPEDEF_H_
