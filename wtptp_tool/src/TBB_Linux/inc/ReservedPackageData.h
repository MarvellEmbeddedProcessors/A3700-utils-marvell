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

#ifndef __RESERVED_PACKAGE_DATA_H__
#define __RESERVED_PACKAGE_DATA_H__

#include "TimLib.h"

#include <string>
#include <list>
#include <iterator>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

typedef list<string*> t_stringList;
typedef list<string*>::iterator t_stringListIter;

class CTimDescriptor;

class CReservedPackageData: public CTimLib {
public:
	CReservedPackageData(void);
	virtual ~CReservedPackageData(void);

	// copy constructor
	CReservedPackageData(const CReservedPackageData& rhs);
	// assignment operator
	CReservedPackageData& operator=(const CReservedPackageData& rhs);

public:
	string& PackageId() {
		return m_sPackageId;
	}
	void PackageId(const string& sPackageId) {
		m_sPackageId = sPackageId;
		m_bChanged = true;
	}

	string& PackageIdTag() {
		return m_sPackageIdTag;
	}
	void PackageIdTag(const string& sPackageIdTag);

	int Size();

	t_stringList& PackageDataList() {
		return m_PackageDataList;
	}
	t_stringList& PackageCommentList() {
		return m_PackageCommentList;
	}

	void DiscardAll();
	void DiscardDataAndComments();
	void AddData(string* psData, string* psComment);
	void DeleteData(string* psData);
	int DataBlock(string& sDataBlock);

	bool IsChanged();
	void Changed(bool bSet) {
		m_bChanged = bSet;
	}

private:
	void PredefinedPackageComments(int idx, string *psComment);

private:
	string m_sPackageId;
	string m_sPackageIdTag;

	t_stringList m_PackageDataList;
	t_stringList m_PackageCommentList;

	bool m_bChanged;
};

typedef list<CReservedPackageData*> t_ReservedDataList;
typedef list<CReservedPackageData*>::iterator t_ReservedDataListIter;

#endif //__RESERVED_PACKAGE_DATA_H__
