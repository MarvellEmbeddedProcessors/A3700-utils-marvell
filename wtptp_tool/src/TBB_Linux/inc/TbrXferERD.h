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

#ifndef __TBR_XFER_ERD_H__
#define __TBR_XFER_ERD_H__

#include "ErdBase.h"

#include <list>
using namespace std;

class CXfer: public CErdBase {
public:
	CXfer();
	virtual ~CXfer();

	// copy constructor
	CXfer(const CXfer& rhs);
	// assignment operator
	virtual CXfer& operator=(const CXfer& rhs);

	static const string Begin;	// "Xfer"
	static const string End;	// "End Xfer"
	virtual const string& PackageName() {
		return Begin;
	}

	virtual bool ToBinary(ofstream& ofs);

	virtual unsigned int PackageSize() {
		return (unsigned int) (m_FieldValues.size() * 4);
	}
	virtual int AddPkgStrings(CReservedPackageData* pRPD);

	bool Parse(CTimDescriptor& TimDescriptor, CTimDescriptorLine*& pLine,
			bool bIsBlf, string& sNum);

	enum Xfer {
		DATA_ID, LOCATION, XFER_MAX
	};

};

typedef list<CXfer*> t_XferList;
typedef list<CXfer*>::iterator t_XferListIter;

class CTBRXferSet: public CErdBase {
public:
	CTBRXferSet();
	virtual ~CTBRXferSet();

	// copy constructor
	CTBRXferSet(const CTBRXferSet& rhs);
	// assignment operator
	virtual CTBRXferSet& operator=(const CTBRXferSet& rhs);

	static const string Begin;	// "TBR Xfer Set"
	static const string End;	// "End TBR Xfer Set"
	virtual const string& PackageName() {
		return Begin;
	}

	virtual void Reset();

	virtual bool Parse(CTimDescriptor& TimDescriptor,
			CTimDescriptorLine*& pLine, bool bIsBlf);

	virtual bool ToBinary(ofstream& ofs);

	virtual unsigned int PackageSize();
	virtual int AddPkgStrings(CReservedPackageData* pRPD);

	t_XferList& XfersList() {
		return Xfers;
	}

	enum TBRXferSet {
		XFER_LOC, NUM_DATA_PAIRS, TBR_XFER_SET_MAX
	};

private:
	t_XferList Xfers;
};

typedef list<CTBRXferSet*> t_TBRXferSetList;
typedef list<CTBRXferSet*>::iterator t_TBRXferSetListIter;

#endif //__TBR_XFER_ERD_H__
