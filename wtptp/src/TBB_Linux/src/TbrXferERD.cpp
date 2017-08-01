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


#include <algorithm>

#include "TbrXferERD.h"
#include "TimDescriptorParser.h"

const string CXfer::Begin("Xfer");
const string CXfer::End("End Xfer");

CXfer::CXfer() :
		CErdBase(XFER_ERD, XFER_MAX) {
	*m_FieldNames[DATA_ID] = "DATA_ID";
	*m_FieldNames[LOCATION] = "LOCATION";
}

CXfer::~CXfer() {
}

// copy constructor
CXfer::CXfer(const CXfer& rhs) :
		CErdBase(rhs) {
	// copy constructor
}

// assignment operator
CXfer& CXfer::operator=(const CXfer& rhs) {
	// assignment operator
	if (&rhs != this) {
		CErdBase::operator=(rhs);
	}
	return *this;
}

bool CXfer::ToBinary(ofstream& ofs) {
	// validate size
	if (m_FieldValues.size() != m_iMaxFieldNum)
		return false;

	ofs << Translate(*m_FieldValues[DATA_ID]);
	ofs << Translate(*m_FieldValues[LOCATION]);

	return ofs.good();
}

int CXfer::AddPkgStrings(CReservedPackageData* pRPD) {
	pRPD->AddData(new string(*m_FieldValues[DATA_ID]), new string("DATA_ID"));
	pRPD->AddData(new string(*m_FieldValues[LOCATION]), new string("LOCATION"));

	return PackageSize();
}

bool CXfer::Parse(CTimDescriptor& TimDescriptor, CTimDescriptorLine*& pLine,
		bool bIsBlf, string& sNum) {
	m_bChanged = false;

	while ((pLine = TimDescriptor.GetNextLineField(pLine))) {
		if (pLine->m_FieldName
				== (bIsBlf ?
						sNum + SpaceToUnderscore("End " + PackageName()) :
						("End " + PackageName()))) {
			return true;
		}

		bool bFound = false;
		for (unsigned int idx = 0; idx < m_FieldNames.size(); idx++) {
			if (pLine->m_FieldName
					== (bIsBlf ?
							sNum + SpaceToUnderscore(*m_FieldNames[idx]) :
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
							sNum + SpaceToUnderscore(PackageName()) :
							PackageName());
			TimDescriptor.ParsingError(ss.str(), true, pLine);
			break;
		}
	}

	// field not found  
	return true;
}

const string CTBRXferSet::Begin("TBR Xfer Set");
const string CTBRXferSet::End("End TBR Xfer Set");

CTBRXferSet::CTBRXferSet() :
		CErdBase(TBRXFER_ERD, TBR_XFER_SET_MAX) {
	*m_FieldNames[XFER_LOC] = "XFER_LOC";
	*m_FieldNames[NUM_DATA_PAIRS] = "NUM_DATA_PAIRS";
}

CTBRXferSet::~CTBRXferSet() {
	Reset();
}

// copy constructor
CTBRXferSet::CTBRXferSet(const CTBRXferSet& rhs) :
		CErdBase(rhs) {
	// need to do a deep copy of lists to avoid dangling references
	CTBRXferSet& nc_rhs = const_cast<CTBRXferSet&>(rhs);

	t_XferListIter iter = nc_rhs.Xfers.begin();
	while (iter != nc_rhs.Xfers.end()) {
		CXfer* pXfer = new CXfer(*(*iter));
		Xfers.push_back(pXfer);

		iter++;
	}
}

// assignment operator
CTBRXferSet& CTBRXferSet::operator=(const CTBRXferSet& rhs) {
	// assignment operator
	if (&rhs != this) {
		CErdBase::operator=(rhs);

		Reset();

		// need to do a deep copy of lists to avoid dangling references
		CTBRXferSet& nc_rhs = const_cast<CTBRXferSet&>(rhs);

		t_XferListIter iter = nc_rhs.Xfers.begin();
		while (iter != nc_rhs.Xfers.end()) {
			CXfer* pXfer = new CXfer(*(*iter));
			Xfers.push_back(pXfer);

			iter++;
		}
	}
	return *this;
}

void CTBRXferSet::Reset() {

	if (Xfers.size() > 0) {
		for_each(Xfers.begin(), Xfers.end(), [](CXfer*& ps) {delete ps;});
		Xfers.clear();
	}
}

bool CTBRXferSet::Parse(CTimDescriptor& TimDescriptor,
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
		for (unsigned int idx = 0; idx < m_FieldNames.size(); idx++) {
			if (pLine->m_FieldName == *m_FieldNames[idx]) {
				*m_FieldValues[idx] = pLine->m_FieldValue;
				bFound = true;
				break;
			}
		}

		if (bFound)
			continue;

		stringstream ssXferNum;
		int nXfer = Xfers.size() + 1;
		ssXferNum.str("");
		ssXferNum << nXfer << "_";

		CXfer* pXfer = 0;
		if (pLine->m_FieldName
				== (bIsBlf ?
						ssXferNum.str() + SpaceToUnderscore(CXfer::Begin) :
						CXfer::Begin)) {
			pXfer = new CXfer;
			if (pXfer) {
				string sXferNum = ssXferNum.str(); // this syntax if for linux compiler
				if (!pXfer->Parse(TimDescriptor, pLine, bIsBlf, sXferNum)) {
					stringstream ss;
					ss << endl << "Error: Parsing of '"
							<< (bIsBlf ?
									ssXferNum.str()
											+ SpaceToUnderscore(CXfer::Begin) :
									CXfer::Begin);
					TimDescriptor.ParsingError(ss.str(), true, pLine);
					delete pXfer;
					pXfer = 0;
					return false;
				}
				if (pXfer
						&& pLine->m_FieldName
								== (bIsBlf ?
										ssXferNum.str()
												+ SpaceToUnderscore(
														CXfer::End) :
										CXfer::End)) {
					Xfers.push_back(pXfer);
					pXfer = 0;
					continue;
				}
			}
		}

		stringstream ss;
		ss << endl << "Error: Parsing of '"
				<< (bIsBlf ? ssXferNum.str() + SpaceToUnderscore(End) : End);
		TimDescriptor.ParsingError(ss.str(), true, pLine);
		delete pXfer;
		return false;
	}

//    *m_FieldValues[ NUM_DATA_PAIRS ] = HexFormattedAscii( Xfers.size() );
	if (Translate(*m_FieldValues[NUM_DATA_PAIRS]) != Xfers.size()) {
		stringstream ss;
		ss << endl << "Error: Parsing of '"
				<< (bIsBlf ? SpaceToUnderscore(Begin) : Begin)
				<< ", NUM_DATA_PAIRS not equal to actual number of Xfers defined in the TBR Xfer Set";
		TimDescriptor.ParsingError(ss.str(), true, pLine);
		return false;
	}

	return true;
}

bool CTBRXferSet::ToBinary(ofstream& ofs) {
	// validate size
	if (m_FieldValues.size() != m_iMaxFieldNum)
		return false;

	bool bRet = true;
	ofs << TBR_XFER;
	ofs << PackageSize();

	ofs << Translate(*m_FieldNames[XFER_LOC]);
	ofs << Translate(*m_FieldNames[NUM_DATA_PAIRS]);

	t_XferListIter iter = Xfers.begin();
	while (bRet && iter != Xfers.end())
		bRet = (*iter++)->ToBinary(ofs);

	return (ofs.good() && bRet);
}

int CTBRXferSet::AddPkgStrings(CReservedPackageData* pRPD) {
	pRPD->PackageIdTag(HexFormattedAscii(TBR_XFER));
	pRPD->AddData(new string(*m_FieldValues[XFER_LOC]), new string("XFER_LOC"));
	pRPD->AddData(new string(*m_FieldValues[NUM_DATA_PAIRS]),
			new string("NUM_DATA_PAIRS"));

	t_XferListIter iter = Xfers.begin();
	while (iter != Xfers.end())
		(*iter++)->AddPkgStrings(pRPD);

	return PackageSize();
}

unsigned int CTBRXferSet::PackageSize() {
	return (unsigned int) (8 + // package tag id + size
			(m_FieldValues.size() * 4) // all fields
			+ Xfers.size() * 8);		 // all data in data list
}

