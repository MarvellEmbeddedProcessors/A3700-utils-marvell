/******************************************************************************
 *
 *  (C)Copyright 2009 - 2013 Marvell. All Rights Reserved.
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

#include "ImageMapERD.h"
#include "TimDescriptorParser.h"

const string CImageMapInfo::Begin("Image Map Info");
const string CImageMapInfo::End("End Image Map Info");

CImageMapInfo::CImageMapInfo(bool bIsBlf) :
		CErdBase(IMAGE_MAP_INFO_ERD, IMAGE_MAP_MAX) {
	m_bIsBlf = bIsBlf;

	*m_FieldNames[IMAGE_ID] = "Image ID";
	*m_FieldNames[IMAGE_TYPE] = "Image Type";
	*m_FieldNames[FLASH_ADDRESS_LO] = "Flash Address Lo";
	*m_FieldNames[FLASH_ADDRESS_HI] = "Flash Address Hi";
	*m_FieldNames[PARTITION_NUM] = "Partition";
}

CImageMapInfo::~CImageMapInfo() {

}

// copy constructor
CImageMapInfo::CImageMapInfo(const CImageMapInfo& rhs) :
		CErdBase(rhs) {
	// copy constructor
	m_eErdType = rhs.m_eErdType;
	m_iMaxFieldNum = rhs.m_iMaxFieldNum;

	m_bIsBlf = rhs.m_bIsBlf;
	m_bChanged = rhs.m_bChanged;

	// need to do a deep copy of lists to avoid dangling references
	for (unsigned int i = 0; i < m_iMaxFieldNum; i++) {
		*m_FieldNames[i] = *rhs.m_FieldNames[i];
		*m_FieldValues[i] = *rhs.m_FieldValues[i];
		*m_FieldComments[i] = *rhs.m_FieldComments[i];
	}
}

// assignment operator
CImageMapInfo& CImageMapInfo::operator=(const CImageMapInfo& rhs) {
	// assignment operator
	if (&rhs != this) {
		CTimLib::operator=(rhs);
		m_bIsBlf = rhs.m_bIsBlf;
		m_eErdType = rhs.m_eErdType;
		m_iMaxFieldNum = rhs.m_iMaxFieldNum;
		m_bChanged = rhs.m_bChanged;

		// need to do a deep copy of lists to avoid dangling references
		for (unsigned int i = 0; i < m_iMaxFieldNum; i++) {
			*m_FieldNames[i] = *rhs.m_FieldNames[i];
			*m_FieldValues[i] = *rhs.m_FieldValues[i];
			*m_FieldComments[i] = *rhs.m_FieldComments[i];
		}

	}
	return *this;
}

bool CImageMapInfo::ToBinary(ofstream& ofs) {
	// validate size
	if (m_FieldValues.size() != m_iMaxFieldNum)
		return false;

	ofs << Translate(*m_FieldValues[IMAGE_ID]);
	ofs << Translate(*m_FieldValues[IMAGE_TYPE]);
	ofs << Translate(*m_FieldValues[FLASH_ADDRESS_LO]);
	ofs << Translate(*m_FieldValues[FLASH_ADDRESS_HI]);
	ofs << Translate(*m_FieldValues[PARTITION_NUM]);

	return ofs.good();
}

int CImageMapInfo::AddPkgStrings(CReservedPackageData* pRPD) {
	pRPD->AddData(new string(*m_FieldValues[IMAGE_ID]), new string("Image ID"));
	pRPD->AddData(new string(*m_FieldValues[IMAGE_TYPE]),
			new string("Type ID"));
	pRPD->AddData(new string(*m_FieldValues[FLASH_ADDRESS_LO]),
			new string("Flash Address Lo"));
	pRPD->AddData(new string(*m_FieldValues[FLASH_ADDRESS_HI]),
			new string("Flash Address Hi"));
	pRPD->AddData(new string(*m_FieldValues[PARTITION_NUM]),
			new string("Partition"));

	return PackageSize();
}

bool CImageMapInfo::Parse(CTimDescriptor& TimDescriptor,
		CTimDescriptorLine*& pLine, bool bIsBlf, string& sNum) {
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

const string CImageMaps::Begin("Image Maps");
const string CImageMaps::End("End Image Maps");

CImageMaps::CImageMaps(bool bIsBlf) :
		CErdBase(IMAGE_MAPS_ERD, IMAGE_MAPS_MAX) {
	m_bIsBlf = bIsBlf;
	*m_FieldNames[NUM_MAPS] = "NUM_MAPS";
}

CImageMaps::~CImageMaps() {
	Reset();
}

// copy constructor
CImageMaps::CImageMaps(const CImageMaps& rhs) :
		CErdBase(rhs) {
	// need to do a deep copy of lists to avoid dangling references
	CImageMaps& nc_rhs = const_cast<CImageMaps&>(rhs);
	m_bIsBlf = rhs.m_bIsBlf;
	t_ImageMapListIter iter = nc_rhs.ImageMapList.begin();
	while (iter != nc_rhs.ImageMapList.end()) {
		CImageMapInfo* pImageMap = new CImageMapInfo(*(*iter));
		ImageMapList.push_back(pImageMap);
		iter++;
	}
}

// assignment operator
CImageMaps& CImageMaps::operator=(const CImageMaps& rhs) {
	// assignment operator
	if (&rhs != this) {
		CErdBase::operator=(rhs);

		Reset();

		m_bIsBlf = rhs.m_bIsBlf;

		// need to do a deep copy of lists to avoid dangling references
		CImageMaps& nc_rhs = const_cast<CImageMaps&>(rhs);
		t_ImageMapListIter iter = nc_rhs.ImageMapList.begin();
		while (iter != nc_rhs.ImageMapList.end()) {
			CImageMapInfo* pImageMap = new CImageMapInfo(*(*iter));
			ImageMapList.push_back(pImageMap);
			iter++;
		}
	}
	return *this;
}

void CImageMaps::Reset() {
	if (ImageMapList.size() > 0) {
		for_each(ImageMapList.begin(), ImageMapList.end(),
				[](CImageMapInfo*& pIMI) {delete pIMI;});
		ImageMapList.clear();
	}
}

bool CImageMaps::Parse(CTimDescriptor& TimDescriptor,
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

		stringstream ssImageMapNum;
		int nImageMap = ImageMapList.size() + 1;
		ssImageMapNum.str("");
		ssImageMapNum << nImageMap << "_";

		CImageMapInfo* pImageMap = 0;
		if (pLine->m_FieldName
				== (bIsBlf ?
						ssImageMapNum.str()
								+ SpaceToUnderscore(CImageMapInfo::Begin) :
						CImageMapInfo::Begin)) {
			pImageMap = new CImageMapInfo(bIsBlf);
			string sImageMapNum = ssImageMapNum.str(); // this syntax if for linux compiler
			if (pImageMap
					&& !pImageMap->Parse(TimDescriptor, pLine, bIsBlf,
							sImageMapNum)) {
				stringstream ss;
				ss << endl << "Error: Parsing of '"
						<< (bIsBlf ?
								ssImageMapNum.str()
										+ SpaceToUnderscore(
												CImageMapInfo::Begin) :
								CImageMapInfo::Begin);
				TimDescriptor.ParsingError(ss.str(), true, pLine);
				delete pImageMap;
				return false;
			}

			if (pImageMap
					&& pLine->m_FieldName
							== (bIsBlf ?
									ssImageMapNum.str()
											+ SpaceToUnderscore(
													CImageMapInfo::End) :
									CImageMapInfo::End)) {
				ImageMapList.push_back(pImageMap);
				pImageMap = 0;
				continue;
			}
		}

		stringstream ss;
		ss << endl << "Error: Parsing of '"
				<< (bIsBlf ? ssImageMapNum.str() + SpaceToUnderscore(End) : End);
		TimDescriptor.ParsingError(ss.str(), true, pLine);
		delete pImageMap;
		return false;
	}

	if (Translate(*m_FieldValues[NUM_MAPS]) != ImageMapList.size()) {
		stringstream ss;
		ss << endl
				<< "\nError: Parsing of Image Maps, NUM_MAPS not equal to actual number of Image Maps defined in the ImageMaps";
		TimDescriptor.ParsingError(ss.str(), true, pLine);
		return false;
	}

	return true;
}

bool CImageMaps::ToBinary(ofstream& ofs) {
	// validate size
	if (m_FieldValues.size() != m_iMaxFieldNum)
		return false;

	bool bRet = true;
	ofs << IMAPID;
	ofs << PackageSize();
	ofs << Translate(*m_FieldValues[NUM_MAPS]);
	t_ImageMapListIter iter = ImageMapList.begin();
	while (bRet && iter != ImageMapList.end())
		bRet = (*iter++)->ToBinary(ofs);

	return (ofs.good() && bRet);
}

int CImageMaps::AddPkgStrings(CReservedPackageData* pRPD) {
	pRPD->PackageIdTag(HexFormattedAscii(IMAPID));
	pRPD->AddData(new string(*m_FieldValues[NUM_MAPS]), new string("NUM_MAPS"));

	t_ImageMapListIter iter = ImageMapList.begin();
	while (iter != ImageMapList.end())
		(*iter++)->AddPkgStrings(pRPD);

	return PackageSize();
}
