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

#ifndef __RSA_KEY_H__
#define __RSA_KEY_H__

#include "Key.h"
#include <cstdio>

typedef enum RSA_KEY_TYPE {
	RSA1024 = 1, RSA2048,
} RSA_KEY_TYPE;

class CRSAKey: public CKey {
public:
	CRSAKey();
	virtual ~CRSAKey();

	CRSAKey(const CRSAKey& rhs);
	CRSAKey& operator=(const CRSAKey& rhs);
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

	t_stringList& PublicKeyExponentList() {
		return m_PublicKeyExponentList;
	}
	t_stringList& RsaSystemModulusList() {
		return m_RsaSystemModulusList;
	}
	t_stringList& RsaPrivateKeyList() {
		return m_RsaPrivateKeyList;
	}

	void ResetExponent();
	void ResetModulus();

	int RSAKeyGen(RSA_KEY_TYPE RSAtype, unsigned int*& pBufN, int *szN,
			unsigned int*& pBufE, int *szE,
			unsigned int*& pBufD, int *szD, unsigned int*& pBufP, int *szP, unsigned int*& pBufQ,
			int *szQ);

	string& PublicKeyExponent(bool bIsBlf = false);
	string& PublicKeyExponentPacked(bool bIsBlf = false);
	string& RsaSystemModulus(bool bIsBlf = false);
	string& RsaSystemModulusPacked(bool bIsBlf = false);
	string& RsaPrivateKey(bool bIsBlf = false);
	string& RsaPrivateKeyPacked(bool bIsBlf = false);

protected:
	string m_sPublicKeyExponent;   // temp - do not store
	string m_sRsaSystemModulus;    // temp - do not store
	string m_sRsaPrivateKey;       // temp - do not store

	t_stringList m_PublicKeyExponentList;
	t_stringList m_RsaSystemModulusList;
	t_stringList m_RsaPrivateKeyList;
};

typedef list<CRSAKey*> t_RSAKeyList;
typedef list<CRSAKey*>::iterator t_RSAKeyListIter;

#endif //__RSA_KEY_H__
