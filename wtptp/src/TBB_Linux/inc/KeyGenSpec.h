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

#ifndef __KEY_GEN_SPEC_H__
#define __KEY_GEN_SPEC_H__
#include "TimLib.h"
#include "RSAKey.h"
#include "ECCKey.h"

#include <cstdlib>
#include <cstring>
#include <string>
#include <list>
#include <iterator>
using namespace std;

typedef list<string*> t_stringList;
typedef list<string*>::iterator t_stringListIter;

typedef struct {
	UINT_T KeyID;			// Associate an ID with this key
							// this value is the ASCII encoded hex value of the Key 
							// TAG <DSIG, JTAG, PTAC, TCAK, any 4-char>
	UINT_T HashAlgorithmID;	// 20=SHA160, 32=SHA256, 64=SHA512
	UINT_T EncryptAlgorithmID;// 0=Marvell_DS, 1=PKCS_1_5(caddo), (removed)2=PKCS_2_1(caddo)
							  // 3=PKCS_1_5(ippcp), (removed)4=PKCS_2_1(ippcp), 5=ECC_256, 6=ECC_521,7=PKCS_2_2(ippcp)
	UINT_T KeySize;	          	// Specified in bits
	union {
		struct {
			UINT_T RSAPublicExponent[MAXRSAKEYSIZEWORDS];
			UINT_T RSAModulus[MAXRSAKEYSIZEWORDS];         // Up to 2K bits
			UINT_T RSAPrivateKey[MAXRSAKEYSIZEWORDS];
		} Rsa;

		struct {
			UINT_T ECDSAPublicKeyCompX[MAXECCKEYSIZEWORDS];
			UINT_T ECDSAPublicKeyCompY[MAXECCKEYSIZEWORDS];
			UINT_T ECDSAPrivateKey[MAXECCKEYSIZEWORDS];
			// Pad this struct so it remains consistent with RSA struct
			UINT_T Reserved[(MAXRSAKEYSIZEWORDS * 3) - (MAXECCKEYSIZEWORDS * 3)];
		} Ecdsa;
	};
} GENERATED_KEY;

class CKeyGenSpec: public CTimLib {
public:
	CKeyGenSpec(void);
	virtual ~CKeyGenSpec(void);

	// copy constructor
	CKeyGenSpec(const CKeyGenSpec& rhs);
	// assignment operator
	CKeyGenSpec& operator=(const CKeyGenSpec& rhs);

	bool CheckKeySize(unsigned int keySize);
public:
	string& KeyGenSpecFilename() {
		return m_sKeyGenSpecFilename;
	}
	void KeyGenSpecFilename(string& sKeyGenSpecFilename);

	string& Seed() {
		return m_sSeed;
	}
	void Seed(string& sSeed);

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

	string& EncryptAlgorithmIdStr() {
		return m_sEncryptAlgorithmId;
	}
	void EncryptAlgorithmId(string& sEncryptAlgorithmId) {
		m_sEncryptAlgorithmId = sEncryptAlgorithmId;
		m_bChanged = true;
	}
	void EncryptAlgorithmId(ENCRYPTALGORITHMID_T Id);
	ENCRYPTALGORITHMID_T EncryptAlgorithmId();

	unsigned int& KeySize() {
		return m_uiKeySize;
	}
	virtual void KeySize(unsigned int uiKeySize);

	bool isValidKeySize(ENCRYPTALGORITHMID_T Id);

	string& OutputBinaryFilename() {
		return m_sOutputBinaryFilename;
	}
	void OutputBinaryFilename(string& sOutputBinaryFilename);

	string& OutputTextFilename() {
		return m_sOutputTextFilename;
	}
	void OutputTextFilename(string& sOutputTextFilename);

	string& OutputHashBinaryFilename() {
		return m_sOutputHashBinaryFilename;
	}
	void OutputHashBinaryFilename(string& sOutputHashBinaryFilename);

	bool GenerateKey(CKey& Key);
	bool OutputKeyText(string& sOutputTextFilename, CKey& Key);
	bool OutputKeyBinary(string& sOutputBinaryFilename, CKey& Key);
	bool GenerateHash(string& sOutputHashBinaryFilename, CKey& Key);

	bool ParseKeyGenSpecFile();

	bool IsChanged();
	void Changed(bool bSet) {
		m_bChanged = bSet;
	}

protected:
	bool m_bChanged;
	string m_sKeyGenSpecFilename;
	string m_sSeed;
	string m_sKeyId;
	string m_sKeyTag;
	string m_sEncryptAlgorithmId;
	string m_sHashAlgorithmId;
	unsigned int m_uiKeySize;
	string m_sOutputBinaryFilename;
	string m_sOutputTextFilename;
	string m_sOutputHashBinaryFilename;
};

#endif //__KEY_GEN_SPEC_H__
