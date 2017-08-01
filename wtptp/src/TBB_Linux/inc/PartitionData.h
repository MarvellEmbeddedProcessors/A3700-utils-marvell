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


#ifndef __PARTITION_DATA_H__
#define __PARTITION_DATA_H__

#include "TimLib.h"

#include <string>
#include <list>
#include <iterator>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
using namespace std;

class CPartitionData: public CTimLib {
public:
	CPartitionData(void);
	virtual ~CPartitionData(void);

	// copy constructor
	CPartitionData(const CPartitionData& rhs);
	// assignment operator
	CPartitionData& operator=(const CPartitionData& rhs);

	void PartitionId(const unsigned int Id) {
		m_PartitionId = Id;
		m_bChanged = true;
	}
	unsigned int PartitionId() {
		return m_PartitionId;
	}

	void PartitionAttributes(const unsigned int Attributes) {
		m_PartitionAttributes = Attributes;
		m_bChanged = true;
	}
	unsigned int PartitionAttributes() {
		return m_PartitionAttributes;
	}

	void PartitionUsage(const string& sUsage) {
		m_sPartitionUsage = sUsage;
		m_bChanged = true;
	}
	string& PartitionUsage() {
		return m_sPartitionUsage;
	}

	void PartitionType(const string& sType) {
		m_sPartitionType = sType;
		m_bChanged = true;
	}
	string& PartitionType() {
		return m_sPartitionType;
	}

	void PartitionStartAddress(const u_int64_t Address) {
		m_PartitionStartAddress = Address;
		m_bChanged = true;
	}
	u_int64_t PartitionStartAddress() {
		return m_PartitionStartAddress;
	}

	void PartitionEndAddress(const u_int64_t Address) {
		m_PartitionEndAddress = Address;
		m_bChanged = true;
	}
	u_int64_t PartitionEndAddress() {
		return m_PartitionEndAddress;
	}

	void ReservedPoolStartAddress(const u_int64_t Address) {
		m_ReservedPoolStartAddress = Address;
		m_bChanged = true;
	}
	u_int64_t ReservedPoolStartAddress() {
		return m_ReservedPoolStartAddress;
	}

	void RuntimeBBTType(const string& sType) {
		m_sRuntimeBBTType = sType;
		m_bChanged = true;
	}
	string& RuntimeBBTType() {
		return m_sRuntimeBBTType;
	}

	void RuntimeBBTLocation(const unsigned int Location) {
		m_RuntimeBBTLocation = Location;
		m_bChanged = true;
	}
	unsigned int RuntimeBBTLocation() {
		return m_RuntimeBBTLocation;
	}

	void BackupRuntimeBBTLocation(const unsigned int Location) {
		m_BackupRuntimeBBTLocation = Location;
		m_bChanged = true;
	}
	unsigned int BackupRuntimeBBTLocation() {
		return m_BackupRuntimeBBTLocation;
	}

	void ReservedPoolSize(const unsigned int Size) {
		m_ReservedPoolSize = Size;
		m_bChanged = true;
	}
	unsigned int ReservedPoolSize() {
		return m_ReservedPoolSize;
	}

	void ReservedPoolAlgorithm(const string& Algorithm) {
		m_sReservedPoolAlgorithm = Algorithm;
		m_bChanged = true;
	}
	string& ReservedPoolAlgorithm() {
		return m_sReservedPoolAlgorithm;
	}

	bool IsChanged() {
		return m_bChanged;
	}
	void Changed(bool bSet) {
		m_bChanged = bSet;
	}

private:
	unsigned int m_PartitionId;
	unsigned int m_PartitionAttributes;
	string m_sPartitionUsage;
	string m_sPartitionType;

	u_int64_t m_PartitionStartAddress;
	u_int64_t m_PartitionEndAddress;
	u_int64_t m_ReservedPoolStartAddress;

	string m_sRuntimeBBTType;
	unsigned int m_RuntimeBBTLocation;
	unsigned int m_BackupRuntimeBBTLocation;
	unsigned int m_ReservedPoolSize;
	string m_sReservedPoolAlgorithm;

	bool m_bChanged;
};

typedef list<CPartitionData*> t_PartitionList;
typedef list<CPartitionData*>::iterator t_PartitionListIter;

#endif //__PARTITION_DATA_H__
