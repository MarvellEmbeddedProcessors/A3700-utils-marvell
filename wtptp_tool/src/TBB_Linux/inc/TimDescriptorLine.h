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

#ifndef __TIM_DESCRIPTOR_LINE_H__
#define __TIM_DESCRIPTOR_LINE_H__

#include "TimLib.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

typedef list<string*> t_stringList;
typedef list<string*>::iterator t_stringListIter;

typedef vector<string*> t_stringVector;
typedef vector<string*>::iterator t_stringVectorIter;

typedef vector<void*> t_ObjectVector;
typedef vector<void*>::iterator t_ObjectVectorIter;

class CTimDescriptorParser;

class CTimDescriptorLine: public CTimLib {
public:
	CTimDescriptorLine(void);
	virtual ~CTimDescriptorLine(void);

	// copy constructor
	CTimDescriptorLine(const CTimDescriptorLine& rhs);
	// assignment operator
	CTimDescriptorLine& operator=(const CTimDescriptorLine& rhs);

	bool ParseLine(ifstream& ifs, CTimDescriptorParser& Parser, bool bIsBlf);
	bool ParseLine(t_stringList& Lines, CTimDescriptorParser& Parser,
			bool bIsBlf);

	bool IsObject(void* pTestObj) {
		return false;
	}

	bool ParsingError() {
		return m_bParsingError;
	}
	string ParsingErrorStr() {
		return m_sParsingError;
	}
	void ParsingError(string sErr) {
		m_sParsingError = sErr;
		m_bParsingError = m_sParsingError.length() > 0;
	}

	bool ParsingWarning() {
		return m_bParsingWarning;
	}
	string ParsingWarningStr() {
		return m_sParsingWarning;
	}
	void ParsingWarning(string sWarn) {
		m_sParsingWarning = sWarn;
		m_bParsingError = m_sParsingWarning.length() > 0;
	}

	void DiscardAll();

	string m_FieldName;
	string m_FieldValue;

private:
	bool m_bParsingError;
	string m_sParsingError;
	bool m_bParsingWarning;
	string m_sParsingWarning;
};

typedef list<CTimDescriptorLine*> t_TimDescriptorLines;
typedef list<CTimDescriptorLine*>::iterator t_TimDescriptorLinesIter;


#endif //__TIM_DESCRIPTOR_LINE_H__
