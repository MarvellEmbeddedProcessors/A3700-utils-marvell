/******************************************************************************
 *
 *  (C)Copyright 2005 - 2009 Marvell. All Rights Reserved.
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


#include "PartitionData.h"

CPartitionData::CPartitionData(void) :
		CTimLib() {
	m_PartitionId = 0x0;
	m_PartitionAttributes = 0x0;
	m_sPartitionUsage = "BOOT";
	m_sPartitionType = "Logi";
	m_PartitionStartAddress = 0UL;
	m_PartitionEndAddress = 0UL;
	m_ReservedPoolStartAddress = 0UL;
	m_ReservedPoolSize = 0x0;
	m_sReservedPoolAlgorithm = "UPWD";
	m_sRuntimeBBTType = "MBBT";
	m_RuntimeBBTLocation = 0x0;
	m_BackupRuntimeBBTLocation = 0x0;
	m_bChanged = false;
}

CPartitionData::~CPartitionData(void) {
}

CPartitionData::CPartitionData(const CPartitionData& rhs) :
		CTimLib(rhs) {
	// copy constructor
	m_PartitionId = rhs.m_PartitionId;
	m_PartitionAttributes = rhs.m_PartitionAttributes;
	m_sPartitionUsage = rhs.m_sPartitionUsage;
	m_sPartitionType = rhs.m_sPartitionType;
	m_PartitionStartAddress = rhs.m_PartitionStartAddress;
	m_PartitionEndAddress = rhs.m_PartitionEndAddress;
	m_sRuntimeBBTType = rhs.m_sRuntimeBBTType;
	m_RuntimeBBTLocation = rhs.m_RuntimeBBTLocation;
	m_BackupRuntimeBBTLocation = rhs.m_BackupRuntimeBBTLocation;
	m_ReservedPoolStartAddress = rhs.m_ReservedPoolStartAddress;
	m_ReservedPoolSize = rhs.m_ReservedPoolSize;
	m_sReservedPoolAlgorithm = rhs.m_sReservedPoolAlgorithm;
	m_bChanged = rhs.m_bChanged;
}

CPartitionData& CPartitionData::operator=(const CPartitionData& rhs) {
	// assignment operator
	if (&rhs != this) {
		CTimLib::operator=(rhs);

		m_PartitionId = rhs.m_PartitionId;
		m_PartitionAttributes = rhs.m_PartitionAttributes;
		m_sPartitionUsage = rhs.m_sPartitionUsage;
		m_sPartitionType = rhs.m_sPartitionType;
		m_PartitionStartAddress = rhs.m_PartitionStartAddress;
		m_PartitionEndAddress = rhs.m_PartitionEndAddress;
		m_sRuntimeBBTType = rhs.m_sRuntimeBBTType;
		m_RuntimeBBTLocation = rhs.m_RuntimeBBTLocation;
		m_BackupRuntimeBBTLocation = rhs.m_BackupRuntimeBBTLocation;
		m_ReservedPoolStartAddress = rhs.m_ReservedPoolStartAddress;
		m_ReservedPoolSize = rhs.m_ReservedPoolSize;
		m_sReservedPoolAlgorithm = rhs.m_sReservedPoolAlgorithm;
		m_bChanged = rhs.m_bChanged;
	}
	return *this;
}
