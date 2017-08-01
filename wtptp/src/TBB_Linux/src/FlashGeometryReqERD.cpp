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

#include "FlashGeometryReqERD.h"
#include "TimDescriptorParser.h"

#include <algorithm>

const string CFlashGeometryReq::Begin("Flash Geometry Package");
const string CFlashGeometryReq::End("End Flash Geometry Package");

CFlashGeometryReq::CFlashGeometryReq(bool bIsBlf) :
		CErdBase(FLASH_GEOMETRY_REQ_ERD, FLASH_GEOMETRY_REQ_MAX) {
	m_bIsBlf = bIsBlf;

	*m_FieldNames[PAGE_SIZE] = "Page Size";
	*m_FieldNames[BLOCK_SIZE] = "Block Size";
	*m_FieldNames[NUM_BLOCKS] = "Num Blocks";
	*m_FieldNames[RESERVED1] = "Reserved1";
	*m_FieldNames[RESERVED2] = "Reserved2";
	*m_FieldNames[RESERVED3] = "Reserved3";
	*m_FieldValues[RESERVED1] = "0x00000000";
	*m_FieldValues[RESERVED2] = "0x00000000";
	*m_FieldValues[RESERVED3] = "0x00000000";
}

CFlashGeometryReq::~CFlashGeometryReq() {
	Reset();
}

// copy constructor
CFlashGeometryReq::CFlashGeometryReq(const CFlashGeometryReq& rhs) :
		CErdBase(rhs) {
	// copy constructor
	m_eErdType = rhs.m_eErdType;
	m_iMaxFieldNum = rhs.m_iMaxFieldNum;

	m_bIsBlf = rhs.m_bIsBlf;
	m_bChanged = rhs.m_bChanged;

	CFlashGeometryReq& nc_rhs = const_cast<CFlashGeometryReq&>(rhs);

	// need to do a deep copy of lists to avoid dangling references
	for (unsigned int i = 0; i < m_iMaxFieldNum - 3; i++) {
		*m_FieldNames[i] = *rhs.m_FieldNames[i];
		*m_FieldValues[i] = *rhs.m_FieldValues[i];
		*m_FieldComments[i] = *rhs.m_FieldComments[i];
	}
}

// assignment operator
CFlashGeometryReq& CFlashGeometryReq::operator=(const CFlashGeometryReq& rhs) {
	// assignment operator
	if (&rhs != this) {
		CTimLib::operator=(rhs);

		//#if TOOLS_GUI == 1
		Reset();
		//#endif

		m_bIsBlf = rhs.m_bIsBlf;
		m_eErdType = rhs.m_eErdType;
		m_iMaxFieldNum = rhs.m_iMaxFieldNum;
		m_bChanged = rhs.m_bChanged;

		CFlashGeometryReq& nc_rhs = const_cast<CFlashGeometryReq&>(rhs);

		// need to do a deep copy of lists to avoid dangling references
		for (unsigned int i = 0; i < m_iMaxFieldNum - 3; i++) {
			*m_FieldNames[i] = *rhs.m_FieldNames[i];
			*m_FieldValues[i] = *rhs.m_FieldValues[i];
			*m_FieldComments[i] = *rhs.m_FieldComments[i];
		}
	}
	return *this;
}

void CFlashGeometryReq::Reset() {

}

bool CFlashGeometryReq::ToBinary(ofstream& ofs) {
	// validate size
	if (m_FieldValues.size() != m_iMaxFieldNum)
		return false;

	ofs << FLASHGEOPKGID;
	ofs << PackageSize();

	ofs << Translate(*m_FieldValues[PAGE_SIZE]);
	ofs << Translate(*m_FieldValues[BLOCK_SIZE]);
	ofs << Translate(*m_FieldValues[NUM_BLOCKS]);
	ofs << Translate(*m_FieldValues[RESERVED1]);
	ofs << Translate(*m_FieldValues[RESERVED2]);
	ofs << Translate(*m_FieldValues[RESERVED3]);

	return ofs.good();
}

int CFlashGeometryReq::AddPkgStrings(CReservedPackageData* pRPD) {
	pRPD->PackageIdTag(HexFormattedAscii(FLASHGEOPKGID));
	pRPD->AddData(new string(*m_FieldValues[PAGE_SIZE]),
			new string("Page Size"));
	pRPD->AddData(new string(*m_FieldValues[BLOCK_SIZE]),
			new string("Block Size"));
	pRPD->AddData(new string(*m_FieldValues[NUM_BLOCKS]),
			new string("Num Blocks"));
	pRPD->AddData(new string(*m_FieldValues[RESERVED1]),
			new string("Reserved1"));
	pRPD->AddData(new string(*m_FieldValues[RESERVED2]),
			new string("Reserved2"));
	pRPD->AddData(new string(*m_FieldValues[RESERVED3]),
			new string("Reserved3"));
	m_bChanged = true;

	return PackageSize();
}

bool CFlashGeometryReq::Parse(CTimDescriptor& TimDescriptor,
		CTimDescriptorLine*& pLine, bool bIsBlf) {
	m_bChanged = false;

	if (pLine->m_FieldName != (bIsBlf ? SpaceToUnderscore(Begin) : Begin)) {
		stringstream ss;
		ss << endl << "Error: Parsing of '"
				<< (bIsBlf ? SpaceToUnderscore(Begin) : Begin);
		TimDescriptor.ParsingError(ss.str(), true, pLine);
		return false;
	}

	while ((pLine = TimDescriptor.GetNextLineField(pLine))) {
		if (pLine->m_FieldName == (bIsBlf ? SpaceToUnderscore(End) : End)) {
			break;
		}

		bool bFound = false;
		for (unsigned int idx = 0; idx < m_FieldNames.size() - 3; idx++) {
			if (pLine->m_FieldName == *m_FieldNames[idx]) {
				*m_FieldValues[idx] = pLine->m_FieldValue;
				bFound = true;
				break;
			}
		}

		if (!bFound) {
			stringstream ss;
			ss << endl << "Error: Parsing of '"
					<< (bIsBlf ? SpaceToUnderscore(Begin) : Begin);
			TimDescriptor.ParsingError(ss.str(), true, pLine);
		}
	}

	return true;
}

unsigned int CFlashGeometryReq::PackageSize() {
	// 8 (WRAH) + 12 (fields) + 12 (Reserved Fields)
	unsigned int iSize = sizeof(FLASH_GEOMETRY_REQ);
	return iSize;
}

