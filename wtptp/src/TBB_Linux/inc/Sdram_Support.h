/******************************************************************************
 *
 *  (C)Copyright 2011 Marvell. All Rights Reserved.
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
 *
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

#ifndef __SDRAM_SUPPORT_H__
#define __SDRAM_SUPPORT_H__

#include "Typedef.h"

// DDR Geometry descriptors
typedef struct DDRGSpec_S {
	unsigned long KeyId;
	unsigned long KeyValue;
} DDRGSpec_T;

typedef struct DDRGSpecList_S {
	unsigned long PID;			// package id
	int NumBytes;
	DDRGSpec_T DDRGSpecs[0];
} DDRGSpecList_T;

// DDR Timing descriptors
typedef struct DDRTSpec_S {
	unsigned long KeyId;
	unsigned long KeyValue;
} DDRTSpec_T;

typedef struct DDRTSpecList_S {
	unsigned long PID;			// package id
	int NumBytes;
	DDRTSpec_T DDRTSpecs[0];
} DDRTSpecList_T;

// DDR Custom descriptors
typedef struct DDRCSpec_S {
	unsigned long KeyId;
	unsigned long KeyValue;
} DDRCSpec_T;

typedef struct DDRCSpecList_S {
	unsigned long PID;			// package id
	int NumBytes;
	DDRCSpec_T DDRCSpecs[0];
} DDRCSpecList_T;

// Frequency descriptors
typedef struct FreqSpec_S {
	unsigned long KeyId;
	unsigned long KeyValue;
} FreqSpec_T;

typedef struct FreqSpecList_S {
	unsigned long PID;			// package id
	int NumBytes;
	FreqSpec_T FreqSpecs;
} FreqSpecList_T;

// Clock enable descriptors
typedef struct CLKESpec_S {
	unsigned long KeyId;
	unsigned long KeyValue;
} CLKESpec_T;

typedef struct CLKESpecList_S {
	unsigned long PID;			// package id
	int NumBytes;
	CLKESpec_T CLKESpecs[0];
} CLKESpecList_T;

// Voltage descriptors
typedef struct VoltSpec_S {
	unsigned long Start;
	unsigned long Stop;
	unsigned long Value;
} VoltSpec_T;

typedef struct VoltSpecList_S {
	unsigned long PID;			// package id
	int NumBytes;
	VoltSpec_T VoltSpecs[0];
} VoltSpecList_T;

// Configure Memory Controller Control descriptors
typedef struct CMCCSpec_S {
	unsigned long KeyId;
	unsigned long KeyValue;
} CMCCSpec_T;

typedef struct CMCCSpecList_S {
	unsigned long PID;			// package id
	int NumBytes;
	CMCCSpec_T CMCCSpecs[0];
} CMCCSpecList_T;

typedef enum CMCC_E {
	CMCC_CONFIG_ENA_ID, CMCC_MEMTEST_ENA_ID, CMCC_CONSUMER_ID,	// consumer id
	CMCC_E_MAX
} CMCC_T;

// Trustzone descriptors
typedef struct TZSpec_S {
	unsigned long KeyId;
	unsigned long KeyValue;
} TZSpec_T;

typedef struct TZSpecList_S {
	unsigned long PID;			// package id
	int NumBytes;
	TZSpec_T TZSpecs[0];
} TZSpecList_T;

// function prototypes
// bit field manipulation:
unsigned long SetBitField(unsigned long *pReg, unsigned long val,
		unsigned long bf_lsb, unsigned long bf_len);
unsigned long SetSplitBitField(unsigned long *pReg, unsigned long val,
		unsigned long bfs_hi, unsigned long bfl_hi, unsigned long bfs_low,
		unsigned long bfl_low);
unsigned long ReadBitField(unsigned volatile long *pReg, unsigned long bf_lsb,
		unsigned long bf_len);

// conversion routines:
unsigned long CR_banks(unsigned long * args);

unsigned long CR_rows(unsigned long * args);

unsigned long CR_columns(unsigned long * args);

unsigned long CR_megabytes(unsigned long * args);

unsigned long CR_none(unsigned long * args);

unsigned long CR_ns_to_clk(unsigned long * args);

unsigned long CR_us_to_clk(unsigned long * args);

// record parser helper routines:
int ConfigRecordCount(void *pSpecList);

// reliablity routines: read/modify/write verifier:
unsigned long CheckMemoryReliability(unsigned long startaddr,
		unsigned long length, unsigned long *faultaddr);

// Trustzone configuration
void ConfigureTrustZone(void *pTIM);
UINT_T TzGetRegion(UINT_T region, UINT_T ChipSelect, UINT_T* pBaseAddressOffset,
		UINT_T* pSize, UINT_T* pPermission);

#endif

