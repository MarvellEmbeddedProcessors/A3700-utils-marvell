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

#ifndef __KEY_H__
#define __KEY_H__

#include "TimLib.h"
#include "CryptoPP_L_interface.h"

#include <string>
#include <list>
#include <iterator>
#include <cstring>

using namespace std;

class CKeyGenSpec;

class CKey: public CTimLib {
	// pure virtual base class
public:
	virtual void PublicKeySize(unsigned int uiPublicKeySize) {
		m_uiPublicKeySize = uiPublicKeySize;
	}
	virtual void KeySize(unsigned int uiKeySize) {
		m_uiKeySize = uiKeySize;
	}

	virtual void ResetPrivateKey() = 0;
	virtual void DiscardAll() = 0;
	virtual bool FromKeyInfo_3_4(KEY_MOD_3_4_0& stKey) = 0;
	virtual KEY_MOD_3_4_0 ToKeyInfo_3_4() = 0;
	virtual bool FromPlatDs(PLAT_DS& stDs) = 0;
	virtual PLAT_DS toPlatDs() = 0;
	virtual bool PrivateFromArray(UINT* pPrivateAry, unsigned int size) = 0;
	virtual bool PrivateToArray(UINT* pPrivateAry, unsigned int size) = 0;
	virtual bool PubCompDFromArray(UINT* pCompDAry, unsigned int size) = 0;
	virtual bool PubCompDToArray(UINT* pCompDAry, unsigned int size) = 0;
	virtual bool PubCompEFromArray(UINT* pCompEAry, unsigned int size) = 0;
	virtual bool PubCompEToArray(UINT* pCompEAry, unsigned int size) = 0;

	virtual CKey* newCopy() = 0;

public:
	CKey(void);
	virtual ~CKey(void);

	// copy constructor
	CKey(const CKey& rhs);
	// assignment operator
	CKey& operator=(const CKey& rhs);

public:
	string& KeyId() {
		return m_sKeyId;
	}
	void KeyId(string& sKeyId);

	string& KeyTag() {
		return m_sKeyTag;
	}
	void KeyTag(string& sKeyTag);

	string& HashAlgorithmIdStr() {
		return m_sHashAlgorithmId;
	}
	void HashAlgorithmId(string& sHashAlgorithmId) {
		m_sHashAlgorithmId = sHashAlgorithmId;
		m_bChanged = true;
	}
	void HashAlgorithmId(HASHALGORITHMID_T Id);
	HASHALGORITHMID_T HashAlgorithmId();

	string& EncryptAlgorithmIdStr(bool bIsBlf = false);
	void EncryptAlgorithmId(string& sEncryptAlgorithmId) {
		m_sEncryptAlgorithmId = sEncryptAlgorithmId;
		m_bChanged = true;
	}
	void EncryptAlgorithmId(ENCRYPTALGORITHMID_T Id, bool bIsBlf = false);
	ENCRYPTALGORITHMID_T EncryptAlgorithmId();

	unsigned int ActiveSizeOfList(t_stringList& List);
	unsigned int &PublicKeySize() {
		return m_uiPublicKeySize;
	}
	unsigned int& KeySize() {
		return m_uiKeySize;
	}

	virtual bool IsChanged();
	virtual void Changed(bool bSet) {
		m_bChanged = bSet;
	}

	virtual bool Encrypted() {
		return m_bEncrypted;
	}
	virtual void Encrypted(bool bEnc) {
		m_bEncrypted = bEnc;
	}

protected:
	bool m_bChanged;
	string m_sEncryptAlgorithmId;
	string m_sKeyId;
	string m_sKeyTag;
	string m_sHashAlgorithmId;

	unsigned int m_uiPublicKeySize;
	unsigned int m_uiKeySize;
	bool m_bEncrypted;           // temp - do not store
};

typedef list<CKey*> t_KeyList;
typedef list<CKey*>::iterator t_KeyListIter;

#endif //__KEY_H__
