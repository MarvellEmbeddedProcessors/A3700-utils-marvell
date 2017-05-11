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

#include <algorithm>

#include "OnDemandBootERD.h"
#include "TimDescriptorParser.h"

const string COnDemandBoot::Begin("OnDemand Boot");
const string COnDemandBoot::End("End OnDemand Boot");

COnDemandBoot::COnDemandBoot(bool bIsBlf) :
		CErdBase(ONDEMAND_BOOT_ERD, ONDEMAND_BOOT_MAX) {
	m_bIsBlf = bIsBlf;

	*m_FieldNames[MAGIC_WORD] = "Magic Word";
	*m_FieldNames[MAGIC_WORD_ADDRESS] = "Magic Word Address";
	*m_FieldNames[MAGIC_WORD_PARTITION] = "Magic Word Partition";
	*m_FieldNames[NUM_ODBT_IMAGES] = "Num ODBT Images";
	*m_FieldNames[ODBT_IMAGE_ID] = "ODBT Image ID";
}

COnDemandBoot::~COnDemandBoot() {
	Reset();
}

// copy constructor
COnDemandBoot::COnDemandBoot(const COnDemandBoot& rhs) :
		CErdBase(rhs) {
	// copy constructor
	m_eErdType = rhs.m_eErdType;
	m_iMaxFieldNum = rhs.m_iMaxFieldNum;

	m_bIsBlf = rhs.m_bIsBlf;
	m_bChanged = rhs.m_bChanged;

	COnDemandBoot& nc_rhs = const_cast<COnDemandBoot&>(rhs);

	// need to do a deep copy of lists to avoid dangling references
	for (unsigned int i = 0; i < m_iMaxFieldNum; i++) {
		*m_FieldNames[i] = *rhs.m_FieldNames[i];
		*m_FieldValues[i] = *rhs.m_FieldValues[i];
		*m_FieldComments[i] = *rhs.m_FieldComments[i];
	}

	if (nc_rhs.ODBTImageVec.size() > 0) {
		for_each(nc_rhs.ODBTImageVec.begin(), nc_rhs.ODBTImageVec.end(),
				[this](string*& psRhsImageID)
				{
					string* psImageID = new string(*psRhsImageID);
					ODBTImageVec.push_back(psImageID);
				});
	}
}

// assignment operator
COnDemandBoot& COnDemandBoot::operator=(const COnDemandBoot& rhs) {
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

		COnDemandBoot& nc_rhs = const_cast<COnDemandBoot&>(rhs);

		// need to do a deep copy of lists to avoid dangling references
		for (unsigned int i = 0; i < m_iMaxFieldNum; i++) {
			*m_FieldNames[i] = *rhs.m_FieldNames[i];
			*m_FieldValues[i] = *rhs.m_FieldValues[i];
			*m_FieldComments[i] = *rhs.m_FieldComments[i];
		}

		if (nc_rhs.ODBTImageVec.size() > 0) {
			for_each(nc_rhs.ODBTImageVec.begin(), nc_rhs.ODBTImageVec.end(),
					[this](string*& psRhsImageID)
					{
						string* psImageID = new string(*psRhsImageID);
						ODBTImageVec.push_back(psImageID);
					});
		}
	}
	return *this;
}

void COnDemandBoot::Reset() {
	if (ODBTImageVec.size() > 0) {
		for_each(ODBTImageVec.begin(), ODBTImageVec.end(),
				[](string*& ps) {delete ps;});
		ODBTImageVec.clear();
	}
}

bool COnDemandBoot::ToBinary(ofstream& ofs) {
	// validate size
	if (m_FieldValues.size() != m_iMaxFieldNum)
		return false;

	ofs << ONDEMANDBOOTTID;
	ofs << PackageSize();

	ofs << Translate(*m_FieldValues[MAGIC_WORD]);
	ofs << Translate(*m_FieldValues[MAGIC_WORD_ADDRESS]);
	ofs << Translate(*m_FieldValues[MAGIC_WORD_PARTITION]);

	*m_FieldValues[NUM_ODBT_IMAGES] = HexFormattedAscii(ODBTImageVec.size());
	ofs << Translate(*m_FieldValues[NUM_ODBT_IMAGES]);

	if (ODBTImageVec.size() > 0) {
		for_each(ODBTImageVec.begin(), ODBTImageVec.end(),
				[&ofs, this](string*& psRhsImageID) {ofs << Translate(*psRhsImageID);});
	}

	return ofs.good();
}

int COnDemandBoot::AddPkgStrings(CReservedPackageData* pRPD) {
	pRPD->PackageIdTag(HexFormattedAscii(ONDEMANDBOOTTID));
	pRPD->AddData(new string(*m_FieldValues[MAGIC_WORD]),
			new string("Magic Word"));
	pRPD->AddData(new string(*m_FieldValues[MAGIC_WORD_ADDRESS]),
			new string("Magic Word Address"));
	pRPD->AddData(new string(*m_FieldValues[MAGIC_WORD_PARTITION]),
			new string("Magic Word Partition"));

	*m_FieldValues[NUM_ODBT_IMAGES] = HexFormattedAscii(ODBTImageVec.size());
	pRPD->AddData(new string(*m_FieldValues[NUM_ODBT_IMAGES]),
			new string("Num ODBT Images"));

	t_stringVectorIter iter = ODBTImageVec.begin();
	if (ODBTImageVec.size() > 0) {
		string* pODBTImage = 0;
		for_each(ODBTImageVec.begin(), ODBTImageVec.end(),
				[&pRPD, &pODBTImage, this](string*& psRhsImageID)
				{
					pODBTImage = new string;
					*pODBTImage = TextToHexFormattedAscii(*psRhsImageID);
					pRPD->AddData(pODBTImage, new string(*psRhsImageID + ("  ODBT Image ID")));
				});
	}

	m_bChanged = true;

	return PackageSize();
}

bool COnDemandBoot::Parse(CTimDescriptor& TimDescriptor,
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
//            if ( TrimWS( pLine->m_FieldName ) != *m_FieldNames[ ODBT_IMAGE_ID ] )
			if (TrimWS(pLine->m_FieldName)
					!= (bIsBlf ?
							SpaceToUnderscore(*m_FieldNames[ODBT_IMAGE_ID]) :
							*m_FieldNames[ODBT_IMAGE_ID])) {
				if (TrimWS(pLine->m_FieldName)
						== (bIsBlf ?
								SpaceToUnderscore(*m_FieldNames[idx]) :
								*m_FieldNames[idx])) {
					*m_FieldValues[idx] = TrimWS(pLine->m_FieldValue);
					bFound = true;
					break;
				}
			} else {
				string* pODBTImage = new string(
						HexAsciiToText(TrimWS(pLine->m_FieldValue)));
				ODBTImageVec.push_back(pODBTImage);
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

	if (Translate(*m_FieldValues[NUM_ODBT_IMAGES]) != ODBTImageVec.size()) {
		stringstream ss;
		ss << endl
				<< "\nError: Parsing of OnDemandBoot ERD, NUM_ODBT_IMAGES not equal to actual number of ODBT Image IDs defined in the ODBTImageList";
		TimDescriptor.ParsingError(ss.str(), true, pLine);
		return false;
	}

	return true;
}

unsigned int COnDemandBoot::PackageSize() {
	// 8 (WRAH) + 12 (fields) + 4(num ODBTImage) + actual size of ODBTImageVec
	unsigned int iSize = 8 + 12 + 4 + ((unsigned int) ODBTImageVec.size() * 4);
	return iSize;
}
