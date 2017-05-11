/******************************************************************************
 *
 *  (C)Copyright 2005 - 2009 Marvell. All Rights Reserved.
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


#ifndef __DIGITAL_SIGNATURE_H__
#define __DIGITAL_SIGNATURE_H__
#include "Key.h"
#include "ECCKey.h"
#include "RSAKey.h"

#include <string>
#include <list>
#include <iterator>
#include <cstring>

using namespace std;

class CDigitalSignature: public CTimLib {
public:
	CDigitalSignature(void);
	virtual ~CDigitalSignature(void);

	CDigitalSignature(const CDigitalSignature& rhs);
	CDigitalSignature& operator=(const CDigitalSignature& rhs);
	CDigitalSignature& operator=(const CKey& rhs);

public:
	bool IncludeInTim() {
		return m_bIncludeInTim;
	}
	void IncludeInTim(bool bIncludeInTim) {
		m_bIncludeInTim = bIncludeInTim;
		Changed(true);
	}

	virtual unsigned int KeySize() {
		return (m_pKey ? m_pKey->KeySize() : 0);
	}
	virtual void KeySize(unsigned int uiKeySize);

	t_stringList& RsaDigSList() {
		return m_RsaDigSList;
	}
	string& RsaDigS();
	string& RsaDigSPacked();
	void ResetRsaDigS();

	t_stringList& ECDigS_RList() {
		return m_ECDigS_RList;
	}
	string& ECDigS_R();
	string& ECDigS_RPacked();
	void ResetECDigS_R();

	t_stringList& ECDigS_SList() {
		return m_ECDigS_SList;
	}
	string& ECDigS_S();
	string& ECDigS_SPacked();
	void ResetECDigS_S();

	PLAT_DS toPlatDs();
	bool FromPlatDs(PLAT_DS& stDs);

	bool DigSFromArray(UINT* pDigSAry, unsigned int size);
	bool DigSToArray(UINT* pDigSAry, unsigned int size);

	void DiscardAll();

	CKey*& DSKey() {
		return m_pKey;
	}
	void DSKey(CKey* pKey) {
		delete m_pKey;
		m_pKey = pKey;
	}

	bool IsChanged();
	void Changed(bool bSet);

private:
	bool m_bIncludeInTim;
	bool m_bChanged;

	string m_sRsaDigS;         // temp - do not store
	t_stringList m_RsaDigSList;
	string m_sECDigS_R;         // temp - do not store
	t_stringList m_ECDigS_RList;
	string m_sECDigS_S;         // temp - do not store
	t_stringList m_ECDigS_SList;

	CKey* m_pKey;
};

typedef list<CDigitalSignature*> t_DigitalSignatureList;
typedef list<CDigitalSignature*>::iterator t_DigitalSignatureListIter;

#endif //__DIGITAL_SIGNATURE_H__
