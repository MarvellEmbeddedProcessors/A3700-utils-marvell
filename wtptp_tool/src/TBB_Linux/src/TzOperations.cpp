/******************************************************************************
 *
 *  (C)Copyright 2005 - 2013 Marvell. All Rights Reserved.
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

#include "TzOperations.h"
#include "TimDescriptorParser.h"

CTzOperation::CTzOperation() :
		CTimLib() {
	m_OperationId = TZ_NOP;
	m_sOpIdText = "NOP";
	m_Value = 0;
}

CTzOperation::CTzOperation(TZ_OPERATION_SPEC_T OpId, const string & sOpIdText) :
		m_OperationId(OpId), m_sOpIdText(sOpIdText), CTimLib() {
	m_Value = 0;
}

CTzOperation::~CTzOperation(void) {

}

CTzOperation::CTzOperation(const CTzOperation& rhs) :
		CTimLib(rhs) {
	// copy constructor
	m_OperationId = rhs.m_OperationId;
	m_sOpIdText = rhs.m_sOpIdText;
	m_Value = rhs.m_Value;

}

CTzOperation& CTzOperation::operator=(const CTzOperation& rhs) {
	// assignment operator
	if (&rhs != this) {
		CTimLib::operator=(rhs);

		m_OperationId = rhs.m_OperationId;
		m_sOpIdText = rhs.m_sOpIdText;
		m_Value = rhs.m_Value;

	}
	return *this;
}

bool CTzOperation::SetOperationID(TZ_OPERATION_SPEC_T eOpId) {
	t_TzOperationListIter Iter = CTzOperations::s_DefinedTzOperations.begin();
	while (Iter != CTzOperations::s_DefinedTzOperations.end()) {
		if ((*Iter)->m_OperationId == eOpId) {
			*this = *(*Iter);
			break;
		}
		Iter++;
	}
	return true;
}

bool CTzOperation::SetOperationID(string& sOpIdText) {
	t_TzOperationListIter Iter = CTzOperations::s_DefinedTzOperations.begin();
	while (Iter != CTzOperations::s_DefinedTzOperations.end()) {
		if ((*Iter)->m_sOpIdText == sOpIdText) {
			*this = *(*Iter);
			break;
		}
		Iter++;
	}
	return true;
}

bool CTzOperation::ToBinary(ofstream& ofs) {
	return ofs.good();
}

t_TzOperationList CTzOperations::s_DefinedTzOperations;
static CTzOperation NOOP_OP(TZ_NOP, "NOP");
static CTzOperation TZ_CONFIG_ENABLE_OP(TZ_CONFIG_ENABLE, "TZ_CONFIG_ENABLE");
static CTzOperation TZ_IGNORE_INST_TO_OP(TZ_IGNORE_INST_TO,
		"TZ_IGNORE_INST_TO");

int CTzOperations::s_TzOpCount = 0;

const string CTzOperations::Begin("TZ Operations");
const string CTzOperations::End("End TZ Operations");

CTzOperations::CTzOperations() :
		CErdBase(TZ_OPERATIONS_ERD, OPERATIONS_MAX) {
	s_TzOpCount++;

	if (s_TzOpCount == 1) {
		s_DefinedTzOperations.clear();
		s_DefinedTzOperations.push_back(&NOOP_OP);
		s_DefinedTzOperations.push_back(&TZ_CONFIG_ENABLE_OP);
		s_DefinedTzOperations.push_back(&TZ_IGNORE_INST_TO_OP);
	}
}

CTzOperations::~CTzOperations(void) {
	Reset();
	if (--s_TzOpCount == 0) {
		if (s_DefinedTzOperations.size() > 0)
			s_DefinedTzOperations.clear();
	}
}

// copy constructor
CTzOperations::CTzOperations(const CTzOperations& rhs) :
		CErdBase(rhs) {
	s_TzOpCount++;

	// need to do a deep copy of lists to avoid dangling references
	CTzOperations& nc_rhs = const_cast<CTzOperations&>(rhs);

	t_TzOperationListIter iter = nc_rhs.m_TzOperationsList.begin();
	while (iter != nc_rhs.m_TzOperationsList.end()) {
		CTzOperation* pOp = new CTzOperation(*(*iter));
		m_TzOperationsList.push_back(pOp);
		iter++;
	}
}

// assignment operator
CTzOperations& CTzOperations::operator=(const CTzOperations& rhs) {
	// assignment operator
	if (&rhs != this) {
		Reset();

		CErdBase::operator=(rhs);

		// need to do a deep copy of lists to avoid dangling references
		CTzOperations& nc_rhs = const_cast<CTzOperations&>(rhs);

		t_TzOperationListIter iter = nc_rhs.m_TzOperationsList.begin();
		while (iter != nc_rhs.m_TzOperationsList.end()) {
			CTzOperation* pOp = new CTzOperation(*(*iter));
			m_TzOperationsList.push_back(pOp);
			iter++;
		}
	}
	return *this;
}

void CTzOperations::Reset() {

	if (m_TzOperationsList.size() > 0) {
		for_each(m_TzOperationsList.begin(), m_TzOperationsList.end(),
				[](CTzOperation*& ps) {delete ps;});
		m_TzOperationsList.clear();
	}
}

unsigned int CTzOperations::PackageSize() {
	unsigned int iSize = (unsigned int) m_TzOperationsList.size() * 8;

	return iSize;
}

bool CTzOperations::Parse(CTimDescriptor& TimDescriptor,
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
		t_TzOperationListIter iter = s_DefinedTzOperations.begin();
		while (iter != s_DefinedTzOperations.end()) {
			if (TrimWS(pLine->m_FieldName) == (*iter)->m_sOpIdText) {
				CTzOperation* pOp = new CTzOperation(*(*iter));
				pOp->m_Value = Translate(pLine->m_FieldValue);
				m_TzOperationsList.push_back(pOp);

				bFound = true;
				break;
			}
			iter++;
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

bool CTzOperations::ToBinary(ofstream& ofs) {
	bool bRet = true;
	return (ofs.good() && bRet);
}

int CTzOperations::AddPkgStrings(CReservedPackageData* pRPD) {
	t_TzOperationListIter iter = m_TzOperationsList.begin();
	while (iter != m_TzOperationsList.end()) {
		pRPD->AddData(new string(HexFormattedAscii((*iter)->m_OperationId)),
				new string((*iter)->m_sOpIdText));
		pRPD->AddData(new string(HexFormattedAscii((*iter)->m_Value)),
				new string("Value"));
		iter++;
	}

	return PackageSize();
}

