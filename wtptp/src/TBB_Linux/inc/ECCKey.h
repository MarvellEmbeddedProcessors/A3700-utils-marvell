/******************************************************************************
 *
 *  (C)Copyright 2011 Marvell. All Rights Reserved.
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

#ifndef __ECC_KEY_H__
#define __ECC_KEY_H__

#include "Key.h"

#include <cstdio>
#include <algorithm>

typedef enum ECC_KEY_TYPE {
	ECC256 = 1, ECC521,
} ECC_KEY_TYPE;

class CECCKey: public CKey {
public:
	CECCKey();
	virtual ~CECCKey();

	CECCKey(const CECCKey& rhs);
	CECCKey& operator=(const CECCKey& rhs);
	virtual CKey* newCopy();

	virtual void PublicKeySize(unsigned int uiPublicKeySize);
	virtual void KeySize(unsigned int uiKeySize);

	virtual void ResetPrivateKey();
	virtual void DiscardAll();

	virtual bool FromKeyInfo_3_4(KEY_MOD_3_4_0& stKey);
	virtual KEY_MOD_3_4_0 ToKeyInfo_3_4();
	virtual bool FromPlatDs(PLAT_DS& stDs);
	virtual PLAT_DS toPlatDs();

	virtual bool PrivateFromArray(UINT* pPrivateAry, unsigned int size);
	virtual bool PrivateToArray(UINT* pPrivateAry, unsigned int size);
	virtual bool PubCompDFromArray(UINT* pCompDAry, unsigned int size);
	virtual bool PubCompDToArray(UINT* pCompDAry, unsigned int size);
	virtual bool PubCompEFromArray(UINT* pCompEAry, unsigned int size);
	virtual bool PubCompEToArray(UINT* pCompEAry, unsigned int size);

	t_stringList& ECDSAPublicKeyCompXList() {
		return m_ECDSAPublicKeyCompXList;
	}
	t_stringList& ECDSAPublicKeyCompYList() {
		return m_ECDSAPublicKeyCompYList;
	}
	t_stringList& ECDSAPrivateKeyList() {
		return m_ECDSAPrivateKeyList;
	}

	void ResetCompX();
	void ResetCompY();

	bool ECCKeyGen(ECC_KEY_TYPE ECCtype);

	string& ECDSAPublicKeyCompX(bool bIsBlf = false);
	string& ECDSAPublicKeyCompY(bool bIsBlf = false);
	string& ECDSAPublicKeyCompXPacked(bool bIsBlf = false);
	string& ECDSAPublicKeyCompYPacked(bool bIsBlf = false);
	string& ECDSAPrivateKey(bool bIsBlf = false);
	string& ECDSAPrivateKeyPacked(bool bIsBlf = false);

	t_stringList m_ECDSAPublicKeyCompXList;
	t_stringList m_ECDSAPublicKeyCompYList;
	t_stringList m_ECDSAPrivateKeyList;

	unsigned int ECCKeyPrivate[MAXECCKEYSIZEWORDS];
	unsigned int ECCKeyPublicX[MAXECCKEYSIZEWORDS];
	unsigned int ECCKeyPublicY[MAXECCKEYSIZEWORDS];

protected:
	string m_sECDSAPublicKeyCompX; // temp - do not store
	string m_sECDSAPublicKeyCompY; // temp - do not store
	string m_sECDSAPrivateKey;     // temp - do not store
};

typedef list<CECCKey*> t_ECCKeyList;
typedef list<CECCKey*>::iterator t_ECCKeyListIter;

#endif //__ECC_KEY_H__
