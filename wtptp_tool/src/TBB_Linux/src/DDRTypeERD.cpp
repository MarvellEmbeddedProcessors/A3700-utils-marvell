/******************************************************************************
 *
 *  (C)Copyright 2015 Marvell. All Rights Reserved.
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

#include "TimDescriptorParser.h"
#include "DDRTypeERD.h"

CDDRType::CDDRType(bool bIsBlf) :
		CErdBase(DDR_TYPE_ERD, DDR_TYPE_MAX) {
	m_bIsBlf = bIsBlf;
	*m_FieldNames[DDR_TYPE] = "DDR Type";
}

CDDRType::~CDDRType() {

}

const string CDDRType::Begin("DTYP");
const string CDDRType::End("End DTYP");

// copy constructor
CDDRType::CDDRType(const CDDRType& rhs) :
		CErdBase(rhs) {
	m_bIsBlf = rhs.m_bIsBlf;
	// copy constructor
}

// assignment operator
CDDRType& CDDRType::operator=(const CDDRType& rhs) {
	// assignment operator
	if (&rhs != this) {
		CErdBase::operator=(rhs);
	}
	return *this;
}

bool CDDRType::Parse(CTimDescriptor& TimDescriptor, CTimDescriptorLine*& pLine,
		bool bIsBlf) {
	m_bChanged = false;

	while ((pLine = TimDescriptor.GetNextLineField(pLine))) {
		if (pLine->m_FieldName
				== (bIsBlf ?
						SpaceToUnderscore("End " + PackageName()) :
						("End " + PackageName()))) {
			return true;
		}

		bool bFound = false;
		for (unsigned int idx = 0; idx < m_FieldNames.size(); idx++) {
			if (pLine->m_FieldName
					== (bIsBlf ?
							SpaceToUnderscore(*m_FieldNames[idx]) :
							*m_FieldNames[idx])) {
				*m_FieldValues[idx] = pLine->m_FieldValue;
				bFound = true;
				break;
			}
		}

		if (!bFound) {
			stringstream ss;
			ss << endl << "Error: Parsing of '"
					<< (bIsBlf ?
							SpaceToUnderscore(PackageName()) : PackageName());
			TimDescriptor.ParsingError(ss.str(), true, pLine);
			break;
		}
	}

	// field not found  
	return true;
}

bool CDDRType::ToBinary(ofstream& ofs) {
	bool bRet = true;
	ofs << DTYPID;
	ofs << PackageSize();
	ofs << Translate(*m_FieldValues[DDR_TYPE]);
	return ofs.good();
}

int CDDRType::AddPkgStrings(CReservedPackageData* pRPD) {
	pRPD->PackageIdTag(HexFormattedAscii(DTYPID));
	pRPD->AddData(new string(*m_FieldValues[DDR_TYPE]), new string("DDR Type"));
	int size = PackageSize();
	return PackageSize();
}
unsigned int CDDRType::PackageSize() {
	// 8(WRAH) + 4(DDR Type) 
	unsigned int iSize = 8 + 4;
	return iSize;
}
