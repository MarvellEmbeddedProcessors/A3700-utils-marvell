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


#ifndef __PARTITION_TABLE_H__
#define __PARTITION_TABLE_H__

#include "PartitionData.h"
#include "TimLib.h"
#include "Partition.h"

#include <string>
#include <list>
#include <iterator>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

class CTimDescriptorParser;

class CPartitionTable: public CTimLib {
public:
	CPartitionTable(void);
	virtual ~CPartitionTable(void);

	// copy constructor
	CPartitionTable(const CPartitionTable& rhs);
	// assignment operator
	CPartitionTable& operator=(const CPartitionTable& rhs);

	void PartitionFilePath(string& sFilePath) {
		m_PartitionFilePath = sFilePath;
		m_bNotWritten = m_bChanged = true;
	}
	string& PartitionFilePath() {
		return m_PartitionFilePath;
	}

	void PartitionTableVersion(unsigned int uiVer) {
		m_PartitionTableVersion = uiVer;
		m_bNotWritten = m_bChanged = true;
	}
	unsigned int PartitionTableVersion() {
		return m_PartitionTableVersion;
	}

	PartitionTable_T& PartitionTable() {
		return m_PartitionHeader;
	}
	P_PartitionInfo_T& PartitionInfo() {
		return m_pPartitionInfo;
	}

	t_PartitionList& Partitions() {
		return m_Partitions;
	}

	void DiscardAll();
	void Reset();

	bool IsChanged();
	void Changed(bool bSet) {
		m_bChanged = bSet;
	}
	bool IsNotWritten() {
		return m_bNotWritten;
	}
	;
	void NotWritten(bool bSet) {
		m_bNotWritten = bSet;
	}

	string GetText();
	bool ParsePartition();
	bool GeneratePartitionBinary(string& sPartitionBinPath);

private:
	bool ParsePartitionTextFile(ifstream& ifsPartitionFile,
			PartitionTable_T& PartitionHeader,
			P_PartitionInfo_T& pPartitionInfo);

private:
	string m_PartitionFilePath;
	unsigned int m_PartitionTableVersion;
	t_PartitionList m_Partitions;

	PartitionTable_T m_PartitionHeader;
	P_PartitionInfo_T m_pPartitionInfo;

	bool m_bChanged;
	bool m_bNotWritten;
};
#endif //__PARTITION_TABLE_H__
