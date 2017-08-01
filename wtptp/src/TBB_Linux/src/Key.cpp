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


#include "Key.h"

CKey::CKey(void) :
		CTimLib() {
	m_uiPublicKeySize = 0;
	m_uiKeySize = 0;
	m_bChanged = false;
	m_sEncryptAlgorithmId = "Marvell";
	m_sHashAlgorithmId = "SHA-160";
	m_bEncrypted = false;
}

CKey::~CKey(void) {
	CKey::DiscardAll();
}

CKey::CKey(const CKey& rhs) :
		CTimLib(rhs) {
	// copy constructor
	CKey& nc_rhs = const_cast<CKey&>(rhs);

	m_sKeyId = rhs.m_sKeyId;
	m_sKeyTag = rhs.m_sKeyTag;
	m_sHashAlgorithmId = rhs.m_sHashAlgorithmId;
	m_sEncryptAlgorithmId = rhs.m_sEncryptAlgorithmId;
	m_uiPublicKeySize = rhs.m_uiPublicKeySize;
	m_uiKeySize = rhs.m_uiKeySize;

	m_bEncrypted = rhs.m_bEncrypted;
	m_bChanged = rhs.m_bChanged;

}

CKey& CKey::operator=(const CKey& rhs) {
	// assignment operator
	if (&rhs != this) {
		CTimLib::operator=(rhs);

		CKey& nc_rhs = const_cast<CKey&>(rhs);
		CKey::DiscardAll();

		m_sKeyId = rhs.m_sKeyId;
		m_sKeyTag = rhs.m_sKeyTag;
		m_sHashAlgorithmId = rhs.m_sHashAlgorithmId;
		m_sEncryptAlgorithmId = rhs.m_sEncryptAlgorithmId;
		m_uiPublicKeySize = rhs.m_uiPublicKeySize;
		m_uiKeySize = rhs.m_uiKeySize;

		m_bEncrypted = rhs.m_bEncrypted;
		m_bChanged = rhs.m_bChanged;

	}
	return *this;
}

void CKey::DiscardAll() {

}

void CKey::KeyId(string& sKeyId) {
	sKeyId.resize(4);
	m_sKeyId = ToUpper(sKeyId, true);
	m_sKeyTag = TextToHexFormattedAscii(m_sKeyId);
	m_bChanged = true;
}

void CKey::KeyTag(string& sKeyTag) {
	m_sKeyTag = sKeyTag;
	UINT_T KeyID = Translate(sKeyTag);
	switch (KeyID) {
	case JTAGIDENTIFIER:
		m_sKeyId = "JTAG";
		break;
	case PATCHIDENTIFIER:
		m_sKeyId = "PATC";
		break;
	case TCAIDENTIFIER:
		m_sKeyId = "TCAK";
		break;
	case DSIGIDENTIFIER:
		m_sKeyId = "DSIG";
		break;
	case ENCKIDENTIFIER:
		m_sKeyId = "ENCK";
		break;
	case CSK0ID:
		m_sKeyId = "CSK0";
		break;
	case CSK1ID:
		m_sKeyId = "CSK1";
		break;
	case CSK2ID:
		m_sKeyId = "CSK2";
		break;
	case CSK3ID:
		m_sKeyId = "CSK3";
		break;
	case CSK4ID:
		m_sKeyId = "CSK4";
		break;
	case CSK5ID:
		m_sKeyId = "CSK5";
		break;
	case CSK6ID:
		m_sKeyId = "CSK6";
		break;
	case CSK7ID:
		m_sKeyId = "CSK7";
		break;
	case CSK8ID:
		m_sKeyId = "CSK8";
		break;
	case CSK9ID:
		m_sKeyId = "CSK9";
		break;
	case CSKAID:
		m_sKeyId = "CSKA";
		break;
	case CSKBID:
		m_sKeyId = "CSKB";
		break;
	case CSKCID:
		m_sKeyId = "CSKC";
		break;
	case CSKDID:
		m_sKeyId = "CSKD";
		break;
	case CSKEID:
		m_sKeyId = "CSKE";
		break;
	case CSKFID:
		m_sKeyId = "CSKF";
		break;
	default: {
		string sCvt = HexAsciiToText(m_sKeyTag);
		if (sCvt != m_sKeyTag)
			m_sKeyId = HexAsciiToText(m_sKeyTag); // custom
		else
			m_sKeyId = m_sKeyTag;
	}
	}

	m_bChanged = true;
}

void CKey::HashAlgorithmId(HASHALGORITHMID_T Id) {
	switch ((int) Id) {
	case 2:
	case 32:
	case 256:
		Id = HASHALGORITHMID_T::SHA256;
		break;

	case 64:
	case 512:
		Id = HASHALGORITHMID_T::SHA512;
		break;

	case 1:
	case 160:
	case 20:
		Id = HASHALGORITHMID_T::SHA160;
		break;

	default:
		printf("\nUnrecognized Hash Algorithm Id. Defaulting to SHA-160.\n");
		Id = SHA160;
		break;
	}

	switch (Id) {
	case HASHALGORITHMID_T::SHA256:
		m_sHashAlgorithmId = "SHA-256";
		break;

	case HASHALGORITHMID_T::SHA512:
		m_sHashAlgorithmId = "SHA-512";
		break;

	case HASHALGORITHMID_T::SHA160:
	default:
		m_sHashAlgorithmId = "SHA-160";
		break;
	}
}

void CKey::EncryptAlgorithmId(ENCRYPTALGORITHMID_T Id, bool bIsBlf) {
	m_bEncrypted = false;
	if ((Id & EncryptedBitMask) > 0) {
		m_bEncrypted = true;
		Id = (ENCRYPTALGORITHMID_T) (Id & ~EncryptedBitMask);
	}

	switch (Id) {
	case PKCS1_v1_5_Caddo:
		m_sEncryptAlgorithmId =
				bIsBlf ? "PKCS1_v1_5_Caddo" : "PKCS1_v1_5 (Caddo)";
		break;

	case PKCS1_v1_5_Ippcp:
		m_sEncryptAlgorithmId =
				bIsBlf ? "PKCS1_v1_5_Ippcp" : "PKCS1_v1_5 (Ippcp)";
		break;

	case PKCS_v2_2_Ippcp:
		m_sEncryptAlgorithmId =
				bIsBlf ? "PKCS_v2_2_Ippcp" : "PKCS_v2_2 (Ippcp)";
		break;

	case ECDSA_256:
		m_sEncryptAlgorithmId = "ECDSA_256";
		break;

	case ECDSA_521:
		m_sEncryptAlgorithmId = "ECDSA_521";
		break;

	case Marvell_DS:
		m_sEncryptAlgorithmId = "Marvell";
		break;

	default:
		m_sEncryptAlgorithmId = "";
		printf("\nError: Unrecognized Encryption Algorithm ID.\n");
		break;
	}
}

string& CKey::EncryptAlgorithmIdStr(bool bIsBlf) {
	// make sure m_sEncryptAlgorithmId is updated in case bIsBlf has changed
	EncryptAlgorithmId(EncryptAlgorithmId(), bIsBlf);
	// return the correct string
	return m_sEncryptAlgorithmId;
}

unsigned int CKey::ActiveSizeOfList(t_stringList& List) {
	// returns a size excluding all trailing 0x00000000 values

	// get bit size of entire list
	unsigned int uiSize = ((int) List.size()) * 32;
	string sItem;

	if (uiSize > 0) {
		t_stringListIter iter = List.end();
		while (--iter != List.begin()) {
			sItem = *(*iter);
			if (ToUpper(sItem, true) == ToUpper("0x00000000"))
				uiSize -= 32;
			else
				break;
		}
	}
	return uiSize;
}

bool CKey::IsChanged() {
	return m_bChanged;
}

ENCRYPTALGORITHMID_T CKey::EncryptAlgorithmId() {
	ENCRYPTALGORITHMID_T eEncryptAlgoId = DUMMY_ENALG;
	if (m_sEncryptAlgorithmId == "PKCS1_v1_5 (Caddo)"
			|| m_sEncryptAlgorithmId == "PKCS1_v1_5_Caddo")
		eEncryptAlgoId = PKCS1_v1_5_Caddo;
	else if (m_sEncryptAlgorithmId == "PKCS1_v1_5 (Ippcp)"
			|| m_sEncryptAlgorithmId == "PKCS1_v1_5_Ippcp")
		eEncryptAlgoId = PKCS1_v1_5_Ippcp;
	else if (m_sEncryptAlgorithmId == "PKCS_v2_2 (Ippcp)"
			|| m_sEncryptAlgorithmId == "PKCS_v2_2_Ippcp")
		eEncryptAlgoId = PKCS_v2_2_Ippcp;
	else if (m_sEncryptAlgorithmId == "ECDSA_256")
		eEncryptAlgoId = ECDSA_256;
	else if (m_sEncryptAlgorithmId == "ECDSA_521")
		eEncryptAlgoId = ECDSA_521;

	return eEncryptAlgoId;
}

HASHALGORITHMID_T CKey::HashAlgorithmId() {
	HASHALGORITHMID_T eHashAlgoId = HASHALGORITHMID_T::SHA160;
	if (HashAlgorithmIdStr() == "SHA-160")
		eHashAlgoId = HASHALGORITHMID_T::SHA160;
	else if (HashAlgorithmIdStr() == "SHA-256")
		eHashAlgoId = HASHALGORITHMID_T::SHA256;
	else if (HashAlgorithmIdStr() == "SHA-512")
		eHashAlgoId = HASHALGORITHMID_T::SHA512;

	return eHashAlgoId;
}

