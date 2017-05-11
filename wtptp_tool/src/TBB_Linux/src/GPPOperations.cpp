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

#include <algorithm>

#include "GPPOperations.h"
#include "TimDescriptorParser.h"

CGPPOperation::CGPPOperation() :
		CTimLib() {
	m_OperationId = GPP_NOP;
	m_sOpIdText = "NOP";
	m_Value = 0;
}

CGPPOperation::CGPPOperation(GPP_OPERATION_SPEC_T OpId,
		const string & sOpIdText) :
		m_OperationId(OpId), m_sOpIdText(sOpIdText), CTimLib() {
	m_Value = 0;
}

CGPPOperation::~CGPPOperation(void) {

}

CGPPOperation::CGPPOperation(const CGPPOperation& rhs) :
		CTimLib(rhs) {
	// copy constructor
	m_OperationId = rhs.m_OperationId;
	m_sOpIdText = rhs.m_sOpIdText;
	m_Value = rhs.m_Value;
}

CGPPOperation& CGPPOperation::operator=(const CGPPOperation& rhs) {
	// assignment operator
	if (&rhs != this) {
		CTimLib::operator=(rhs);

		m_OperationId = rhs.m_OperationId;
		m_sOpIdText = rhs.m_sOpIdText;
		m_Value = rhs.m_Value;
	}
	return *this;
}

bool CGPPOperation::SetOperationID(GPP_OPERATION_SPEC_T eOpId) {
	t_GPPOperationListIter Iter =
			CGPPOperations::s_DefinedGPPOperations.begin();
	while (Iter != CGPPOperations::s_DefinedGPPOperations.end()) {
		if ((*Iter)->m_OperationId == eOpId) {
			*this = *(*Iter);
			break;
		}
		Iter++;
	}
	return true;
}

bool CGPPOperation::SetOperationID(string& sOpIdText) {
	t_GPPOperationListIter Iter =
			CGPPOperations::s_DefinedGPPOperations.begin();
	while (Iter != CGPPOperations::s_DefinedGPPOperations.end()) {
		if ((*Iter)->m_sOpIdText == sOpIdText) {
			*this = *(*Iter);
			break;
		}
		Iter++;
	}
	return true;
}

bool CGPPOperation::ToBinary(ofstream& ofs) {
	return ofs.good();
}

t_GPPOperationList CGPPOperations::s_DefinedGPPOperations;
static CGPPOperation NOOP_OP(GPP_NOP, "NOP");
static CGPPOperation GPP_IGNORE_INST_TO_OP(GPP_IGNORE_INST_TO,
		"GPP_IGNORE_INST_TO");

int CGPPOperations::s_GPPOpCount = 0;

const string CGPPOperations::Begin("GPP Operations");
const string CGPPOperations::End("End GPP Operations");
const string CGPPOperations::BlfBegin("GPPOperations");
const string CGPPOperations::BlfEnd("End_GPPOperations");

CGPPOperations::CGPPOperations() :
		CErdBase(GPP_OPERATIONS_ERD, OPERATIONS_MAX) {
	s_GPPOpCount++;

	if (s_GPPOpCount == 1) {
		s_DefinedGPPOperations.clear();
		s_DefinedGPPOperations.push_back(&NOOP_OP);
		s_DefinedGPPOperations.push_back(&GPP_IGNORE_INST_TO_OP);
	}
}

CGPPOperations::~CGPPOperations(void) {
	Reset();

	if (--s_GPPOpCount == 0) {
		if (s_DefinedGPPOperations.size() > 0)
			s_DefinedGPPOperations.clear();
	}
}

// copy constructor
CGPPOperations::CGPPOperations(const CGPPOperations& rhs) :
		CErdBase(rhs) {
	s_GPPOpCount++;

	// need to do a deep copy of lists to avoid dangling references
	CGPPOperations& nc_rhs = const_cast<CGPPOperations&>(rhs);

	t_GPPOperationListIter iter = nc_rhs.m_GPPOperationsList.begin();
	while (iter != nc_rhs.m_GPPOperationsList.end()) {
		CGPPOperation* pOp = new CGPPOperation(*(*iter));
		m_GPPOperationsList.push_back(pOp);
		iter++;
	}
}

// assignment operator
CGPPOperations& CGPPOperations::operator=(const CGPPOperations& rhs) {
	// assignment operator
	if (&rhs != this) {
		Reset();

		CErdBase::operator=(rhs);

		// need to do a deep copy of lists to avoid dangling references
		CGPPOperations& nc_rhs = const_cast<CGPPOperations&>(rhs);
		t_GPPOperationListIter iter = nc_rhs.m_GPPOperationsList.begin();
		while (iter != nc_rhs.m_GPPOperationsList.end()) {
			CGPPOperation* pOp = new CGPPOperation(*(*iter));
			m_GPPOperationsList.push_back(pOp);
			iter++;
		}
	}
	return *this;
}

void CGPPOperations::Reset() {
	if (m_GPPOperationsList.size() > 0) {
		for_each(m_GPPOperationsList.begin(), m_GPPOperationsList.end(),
				[](CGPPOperation*& pOp) {delete pOp;});
		m_GPPOperationsList.clear();
	}
}

unsigned int CGPPOperations::PackageSize() {
	unsigned int iSize = (unsigned int) m_GPPOperationsList.size() * 8;

	return iSize;
}

bool CGPPOperations::Parse(CTimDescriptor& TimDescriptor,
		CTimDescriptorLine*& pLine, bool bIsBlf) {
	m_bChanged = false;

	if (pLine->m_FieldName != (bIsBlf ? SpaceToUnderscore(BlfBegin) : Begin)) {
		stringstream ss;
		ss << endl << "Error: Parsing of '"
				<< (bIsBlf ? SpaceToUnderscore(BlfBegin) : Begin);
		TimDescriptor.ParsingError(ss.str(), true, pLine);
		return false;
	}

	while ((pLine = TimDescriptor.GetNextLineField(pLine))) {
		if (pLine->m_FieldName == (bIsBlf ? SpaceToUnderscore(BlfEnd) : End)) {
			break;
		}

		bool bFound = false;
		t_GPPOperationListIter iter = s_DefinedGPPOperations.begin();
		while (iter != s_DefinedGPPOperations.end()) {
			if (TrimWS(pLine->m_FieldName) == (*iter)->m_sOpIdText) {
				CGPPOperation* pOp = new CGPPOperation(*(*iter));
				pOp->m_Value = Translate(pLine->m_FieldValue);
				m_GPPOperationsList.push_back(pOp);
				bFound = true;
				break;
			}
			iter++;
		}

		if (!bFound) {
			stringstream ss;
			ss << endl << "Error: Parsing of '"
					<< (bIsBlf ? SpaceToUnderscore(BlfBegin) : Begin);
			TimDescriptor.ParsingError(ss.str(), true, pLine);
			break;
		}
	}

	// field not found
	return true;
}

bool CGPPOperations::ToBinary(ofstream& ofs) {
	bool bRet = true;
	return (ofs.good() && bRet);
}

int CGPPOperations::AddPkgStrings(CReservedPackageData* pRPD) {
	t_GPPOperationListIter iter = m_GPPOperationsList.begin();
	while (iter != m_GPPOperationsList.end()) {
		pRPD->AddData(new string(HexFormattedAscii((*iter)->m_OperationId)),
				new string((*iter)->m_sOpIdText));
		pRPD->AddData(new string(HexFormattedAscii((*iter)->m_Value)),
				new string("Value"));
		iter++;
	}

	return PackageSize();
}
