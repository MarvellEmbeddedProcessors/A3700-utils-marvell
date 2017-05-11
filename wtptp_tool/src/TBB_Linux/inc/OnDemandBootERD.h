/******************************************************************************
 *
 *  (C)Copyright 2013 Marvell. All Rights Reserved.
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


#ifndef __ON_DEMAND_BOOT_ERD_H__
#define __ON_DEMAND_BOOT_ERD_H__

#include "ErdBase.h"

#include <string>
#include <list>
using namespace std;

class COnDemandBoot: public CErdBase {
public:
	COnDemandBoot(bool bIsBlf = false);
	virtual ~COnDemandBoot();

	// copy constructor
	COnDemandBoot(const COnDemandBoot& rhs);
	// assignment operator
	virtual COnDemandBoot& operator=(const COnDemandBoot& rhs);

	static const string Begin;	// "OnDemand Boot"
	static const string End;	// "End OnDemand Boot"
	virtual const string& PackageName() {
		return Begin;
	}

	virtual bool ToBinary(ofstream& ofs);

	virtual unsigned int PackageSize();
	virtual int AddPkgStrings(CReservedPackageData* pRPD);

	bool Parse(CTimDescriptor& TimDescriptor, CTimDescriptorLine*& pLine,
			bool bIsBlf);
	void AddRefs(COnDemandBoot& nc_rhs);
	void RemoveRefs();

	void Reset();

	// permits changing between blf and txt file format
	bool IsBlf() {
		return m_bIsBlf;
	}
	void IsBlf(bool bSet) {
		m_bIsBlf = bSet;
	}

	bool m_bIsBlf;
	enum OnDemandBootField {
		MAGIC_WORD,
		MAGIC_WORD_ADDRESS,
		MAGIC_WORD_PARTITION,
		NUM_ODBT_IMAGES,
		ODBT_IMAGE_ID,
		ONDEMAND_BOOT_MAX
	};

	t_stringVector ODBTImageVec;
};

typedef list<COnDemandBoot*> t_OnDemandBootList;
typedef list<COnDemandBoot*>::iterator t_OnDemandBootListIter;

#endif //__ON_DEMAND_BOOT_ERD_H__
