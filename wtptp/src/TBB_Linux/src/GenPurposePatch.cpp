/******************************************************************************
 *
 *  (C)Copyright 2011 - 2013 Marvell. All Rights Reserved.
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


#include "GenPurposePatch.h"
#include "TimDescriptorParser.h"

const string CGenPurposePatch::Begin("GPP");
const string CGenPurposePatch::End("End GPP");

CGenPurposePatch::CGenPurposePatch() :
		CErdBase(GPP_ERD, GPP_FIELDS_MAX) {
	*m_FieldNames[GPP_PID] = "GPP_PID";
	m_sGPP_PID = "";
}

CGenPurposePatch::~CGenPurposePatch(void) {

}

// copy constructor
CGenPurposePatch::CGenPurposePatch(const CGenPurposePatch& rhs) :
		CErdBase(rhs) {
	m_sGPP_PID = rhs.m_sGPP_PID;

	m_GPPOperations = rhs.m_GPPOperations;
	m_GPPInstructions = rhs.m_GPPInstructions;
}

// assignment operator
CGenPurposePatch& CGenPurposePatch::operator=(const CGenPurposePatch& rhs) {
	// assignment operator
	if (&rhs != this) {
		CErdBase::operator=(rhs);
		m_sGPP_PID = rhs.m_sGPP_PID;
		m_GPPOperations = rhs.m_GPPOperations;
		m_GPPInstructions = rhs.m_GPPInstructions;
	}
	return *this;
}

unsigned int CGenPurposePatch::PackageSize() {
	// 8(WRAH) + 8(NumOps & NumInst) + instructions actual size + ops actual size
	return 8 + 8 + m_GPPInstructions.PackageSize()
			+ m_GPPOperations.PackageSize();
}

bool CGenPurposePatch::Parse(CTimDescriptor& TimDescriptor,
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
		CErdBase* pErd = 0;

		if (pLine->m_FieldName == (bIsBlf ? SpaceToUnderscore(End) : End)) {
			break;
		}

		bool bFound = false;
		for (unsigned int idx = 0; idx < m_FieldNames.size(); idx++) {
			if (TrimWS(pLine->m_FieldName) == *m_FieldNames[idx]) {
				if (idx == GPP_PID)
					m_sGPP_PID = TrimWS(pLine->m_FieldValue);
				else
					*m_FieldValues[idx] = TrimWS(pLine->m_FieldValue);

				bFound = true;
				break;
			}
		}

		if (bFound)
			continue;

		if (pLine->m_FieldName.find(
				bIsBlf ?
						SpaceToUnderscore(CGPPOperations::BlfBegin) :
						CGPPOperations::Begin) != string::npos) {
			pErd = &m_GPPOperations;
			ParseERDPackage(TimDescriptor, pLine, pErd,
					(bIsBlf ?
							SpaceToUnderscore(CGPPOperations::BlfBegin) :
							CGPPOperations::Begin),
					(bIsBlf ?
							SpaceToUnderscore(CGPPOperations::BlfEnd) :
							CGPPOperations::End), bIsBlf);

			bFound = true;
			continue;
		} else if (pLine->m_FieldName.find(
				bIsBlf ?
						SpaceToUnderscore(CInstructions::Begin) :
						CInstructions::Begin) != string::npos) {
			pErd = &m_GPPInstructions;
			ParseERDPackage(TimDescriptor, pLine, pErd,
					(bIsBlf ?
							SpaceToUnderscore(CInstructions::Begin) :
							CInstructions::Begin),
					(bIsBlf ?
							SpaceToUnderscore(CInstructions::End) :
							CInstructions::End), bIsBlf);

			bFound = true;
			continue;
		}

		if (!bFound) {
			stringstream ss;
			ss << endl << "Error: Parsing of '"
					<< (bIsBlf ? SpaceToUnderscore(Begin) : Begin);
			TimDescriptor.ParsingError(ss.str(), true, pLine);
			break;
		}
	}

	// field not found
	return true;
}

bool CGenPurposePatch::ParseERDPackage(CTimDescriptor& TimDescriptor,
		CTimDescriptorLine*& pLine, CErdBase*& pErd, const string& sBegin,
		const string& sEnd, bool bIsBlf) {

	if ((!pLine) || !pErd->Parse(TimDescriptor, pLine, bIsBlf)
			|| (pLine->m_FieldName.find(bIsBlf ? SpaceToUnderscore(sEnd) : sEnd)
					== string::npos)) {
		stringstream ss;
		ss << endl << "Error: Parsing of '"
				<< (bIsBlf ? SpaceToUnderscore(sBegin) : sBegin);
		TimDescriptor.ParsingError(ss.str(), true, pLine);
		return false;
	}
	return true;
}

bool CGenPurposePatch::ToBinary(ofstream& ofs) {
	bool bRet = true;
	return (ofs.good() && bRet);
}

int CGenPurposePatch::AddPkgStrings(CReservedPackageData* pRPD) {
	string sData;
	sData = TextToHexFormattedAscii(m_sGPP_PID);
	pRPD->PackageIdTag(sData);

	string* pData = new string;
	*pData = HexFormattedAscii(m_GPPOperations.NumOps());
	pRPD->AddData(pData, new string("NumOps"));

	pData = new string;
	*pData = HexFormattedAscii(m_GPPInstructions.NumInst());
	pRPD->AddData(pData, new string("NumInst"));

	m_GPPOperations.AddPkgStrings(pRPD);
	m_GPPInstructions.AddPkgStrings(pRPD);

	return PackageSize();
}
