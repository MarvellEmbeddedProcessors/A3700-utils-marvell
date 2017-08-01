/******************************************************************************
 *
 *  (C)Copyright 2009 Marvell. All Rights Reserved.
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


#ifndef __Partition_H__
#define __Partition_H__

#include "Typedef.h"
#include "BBM.h"

//Marvell Master Partition Table ID: "MRVL MPT"
#define MARVELL_PARTITION_TABLE_ID0	0x4D52564C
#define MARVELL_PARTITION_TABLE_ID1	0x204D5054

/* Partition Info values */
//Type
#define PI_TYPE_PHYSICAL	0x50687973		//"Phys"
#define PI_TYPE_LOGICAL		0x4C6F6769		//"Logi"

//Usage
#define PI_USAGE_BOOT		0x424F4F54		//"Boot" Boot Partition
#define PI_USAGE_OSLD		0x4F534C44		//"OSLD" OS Loader Partition
#define PI_USAGE_KRNL		0x4B524E4C		//"KRNL" Kernal Partition
#define PI_USAGE_FFOS		0x46464F53		//"FFOS" FF OS Partition
#define PI_USAGE_FSYS		0x46535953		//"FSYS" File System Partition

//Partition Attributes
typedef struct {
	UINT ReadOnly :1;
	UINT Writable :1;
	UINT Executable :1;
	UINT Compressed :1;
	UINT CompressType :4;
	UINT Reserved :24;
} PartAttributes_T;

//Reserved Pool Algorithm
#define PI_RP_ALGO_NA		0x00000000		//Reserved Pool Not Used
#define PI_RP_ALGO_UPWD		0x55505744		//"UPWD" Reserved Pool grows Upwards
#define PI_RP_ALGO_DNWD		0x444E5744		//"DNWD" Reserved Pool grows Downwards

/*  End of Partition Info values */

// Indicates the state of the PT in volatile memory
typedef enum {
	PT_INVALID = 1, PT_VALID = 2, PT_NEW = 3//this means that either the PT was loaded externally, OR block 0 was erased
} PTState_T;

//Partition table structure:
typedef struct {
	UINT_T Type;				//Partition Type
	UINT_T Usage;				//Partition Usage
	UINT_T Indentifier;		//Partition ID
	PartAttributes_T Attributes;			//Attributes for this partition
	UINT_T StartAddr;			//Absolute address of start of partition
	UINT_T Reserved1;//for now, the upper 32bits of 64bit address are reserved
	UINT_T EndAddr;			//Absolute address of end of partition
	UINT_T Reserved2;//for now, the upper 32bits of 64bit address are reserved
	UINT_T ReserveStartAddr;	//Absolute address of start of reserved pool
	UINT_T Reserved3;//for now, the upper 32bits of 64bit address are reserved
	UINT_T ReserveSize;		//Size of the reserved pool
	UINT_T Reserved4;//for now, the upper 32bits of 64bit address are reserved
	UINT_T ReserveAlgorithm;	//Reserved pool algorithm
	BBT_TYPE BBT_Type; 			//runtime BBT type
	UINT_T RBBT_Location;		//Absolute address of runtime BBT
	UINT_T Reserved5;//for now, the upper 32bits of 64bit address are reserved
	UINT_T BackupRBBT_Location;			//Absoulte address of backup runtime BBT
	UINT_T Reserved6;//for now, the upper 32bits of 64bit address are reserved
	UINT_T Reserved[2];
} PartitionInfo_T, *P_PartitionInfo_T;

typedef struct {
	union {
		UINT64 Identifier;		//Partiton Table ID
		struct {
			UINT_T Identifier0;	// low half - for easier code access
			UINT_T Identifier1;	// hi half
		};
	};

	UINT_T Version;		//Partition Version
	UINT_T NumPartitions;	//Number of partitions
	UINT_T Reserved[2];
} PartitionTable_T, *P_PartitionTable_T;

#endif

