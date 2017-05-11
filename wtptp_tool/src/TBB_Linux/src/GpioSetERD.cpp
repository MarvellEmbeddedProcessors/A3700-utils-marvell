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

#include "GpioSetERD.h"
#include "TimDescriptorParser.h"

const string CGpio::Begin("Gpio");
const string CGpio::End("End Gpio");

CGpio::CGpio() :
		CErdBase(GPIO_ERD, GPIO_MAX) {
	*m_FieldNames[ADDRESS] = "ADDRESS";
	*m_FieldNames[VALUE] = "VALUE";
}

CGpio::~CGpio() {
}

// copy constructor
CGpio::CGpio(const CGpio& rhs) :
		CErdBase(rhs) {
	// copy constructor
}

// assignment operator
CGpio& CGpio::operator=(const CGpio& rhs) {
	// assignment operator
	if (&rhs != this) {
		CErdBase::operator=(rhs);
	}
	return *this;
}

bool CGpio::ToBinary(ofstream& ofs) {
	// validate size
	if (m_FieldValues.size() != m_iMaxFieldNum)
		return false;

	ofs << Translate(*m_FieldValues[ADDRESS]);
	ofs << Translate(*m_FieldValues[VALUE]);

	return ofs.good();
}

int CGpio::AddPkgStrings(CReservedPackageData* pRPD) {
	pRPD->AddData(new string(*m_FieldValues[ADDRESS]), new string("ADDRESS"));
	pRPD->AddData(new string(*m_FieldValues[VALUE]), new string("VALUE"));

	return PackageSize();
}

void CGpio::Assign(string& sAddress, string& sValue) {
	*m_FieldValues[ADDRESS] = sAddress;
	*m_FieldValues[VALUE] = sValue;
}

bool CGpio::Parse(CTimDescriptor& TimDescriptor, CTimDescriptorLine*& pLine,
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

const string CGpioSet::Begin("GpioSet");
const string CGpioSet::End("End GpioSet");

CGpioSet::CGpioSet() :
		CErdBase(GPIOSET_ERD, GPIOSET_MAX) {
	*m_FieldNames[NUM_GPIOS] = "NUM_GPIOS";
}

CGpioSet::~CGpioSet() {
	Reset();
}

// copy constructor
CGpioSet::CGpioSet(const CGpioSet& rhs) :
		CErdBase(rhs) {
	// need to do a deep copy of lists to avoid dangling references
	CGpioSet& nc_rhs = const_cast<CGpioSet&>(rhs);
	t_GpioListIter iter = nc_rhs.GpioList.begin();
	while (iter != nc_rhs.GpioList.end()) {
		CGpio* pGpio = new CGpio(*(*iter));
		GpioList.push_back(pGpio);
		iter++;
	}
}

// assignment operator
CGpioSet& CGpioSet::operator=(const CGpioSet& rhs) {
	// assignment operator
	if (&rhs != this) {
		CErdBase::operator=(rhs);

		Reset();

		// need to do a deep copy of lists to avoid dangling references
		CGpioSet& nc_rhs = const_cast<CGpioSet&>(rhs);
		t_GpioListIter iter = nc_rhs.GpioList.begin();
		while (iter != nc_rhs.GpioList.end()) {
			CGpio* pGpio = new CGpio(*(*iter));
			GpioList.push_back(pGpio);
			iter++;
		}
	}
	return *this;
}

void CGpioSet::Reset() {
	if (GpioList.size() > 0) {
		for_each(GpioList.begin(), GpioList.end(), [](CGpio*& ps) {delete ps;});
		GpioList.clear();
	}
}

bool CGpioSet::Parse(CTimDescriptor& TimDescriptor, CTimDescriptorLine*& pLine,
		bool bIsBlf) {
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

		stringstream ssGpioNum;
		int nGpio = GpioList.size() + 1;
		ssGpioNum.str("");
		ssGpioNum << nGpio << "_";

		CGpio* pGpio = 0;
		if (pLine->m_FieldName
				== (bIsBlf ?
						ssGpioNum.str() + SpaceToUnderscore(CGpio::Begin) :
						CGpio::Begin)) {
			pGpio = new CGpio;

			string sGpioNum = ssGpioNum.str(); // this syntax if for linux compiler
			if (pGpio
					&& !pGpio->Parse(TimDescriptor, pLine, bIsBlf, sGpioNum)) {
				stringstream ss;
				ss << endl << "Error: Parsing of '"
						<< (bIsBlf ?
								ssGpioNum.str()
										+ SpaceToUnderscore(CGpio::Begin) :
								CGpio::Begin);
				TimDescriptor.ParsingError(ss.str(), true, pLine);
				delete pGpio;
				return false;
			}
			if (pGpio
					&& pLine->m_FieldName
							== (bIsBlf ?
									ssGpioNum.str()
											+ SpaceToUnderscore(CGpio::End) :
									CGpio::End)) {
				GpioList.push_back(pGpio);
				pGpio = 0;
				continue;
			}
		}

		stringstream ss;
		ss << endl << "Error: Parsing of '"
				<< (bIsBlf ? ssGpioNum.str() + SpaceToUnderscore(End) : End);
		TimDescriptor.ParsingError(ss.str(), true, pLine);
		delete pGpio;
		return false;
	}

	if (Translate(*m_FieldValues[NUM_GPIOS]) != GpioList.size()) {
		stringstream ss;
		ss << endl
				<< "\nError: Parsing of GpioSet, NUM_GPIOS not equal to actual number of Gpios defined in the GpioSet";
		TimDescriptor.ParsingError(ss.str(), true, pLine);
		return false;
	}

	return true;
}

bool CGpioSet::ToBinary(ofstream& ofs) {
	// validate size
	if (m_FieldValues.size() != m_iMaxFieldNum)
		return false;

	bool bRet = true;
	ofs << GPIOID;
	ofs << PackageSize();
	ofs << Translate(*m_FieldValues[NUM_GPIOS]);
	t_GpioListIter iter = GpioList.begin();
	while (bRet && iter != GpioList.end())
		bRet = (*iter++)->ToBinary(ofs);

	return (ofs.good() && bRet);
}

int CGpioSet::AddPkgStrings(CReservedPackageData* pRPD) {
	pRPD->PackageIdTag(HexFormattedAscii(GPIOID));
	pRPD->AddData(new string(*m_FieldValues[NUM_GPIOS]),
			new string("NUM_GPIOS"));

	t_GpioListIter iter = GpioList.begin();
	while (iter != GpioList.end())
		(*iter++)->AddPkgStrings(pRPD);

	return PackageSize();
}
