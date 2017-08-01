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

#include "ImageDescription.h"
#include "TimDescriptor.h"
#include "TimDescriptorLine.h"

#include <cstring>

CImageDescription::CImageDescription(bool bIsBlf,
		CTimDescriptor& rTimDescriptor) :
		m_TimDescriptor(rTimDescriptor), CTimLib() {
	m_bIsBlf = bIsBlf;
	Reset();
	m_bChanged = false;
}

CImageDescription::~CImageDescription(void) {

}

CImageDescription::CImageDescription(const CImageDescription& rhs) :
		m_TimDescriptor(rhs.m_TimDescriptor), CTimLib(rhs) {
	// copy constructor
	memcpy(Hash, rhs.Hash, sizeof(Hash));

	m_sImageId = rhs.m_sImageId;
	m_sImageIdTag = rhs.m_sImageIdTag;
	m_sNextImageId = rhs.m_sNextImageId;
	m_sNextImageIdTag = rhs.m_sNextImageIdTag;
	m_sFlashEntryAddress = rhs.m_sFlashEntryAddress;
	m_sLoadAddress = rhs.m_sLoadAddress;
	m_uiImageSizeToCrc = rhs.m_uiImageSizeToCrc;
	m_sImageFilePath = rhs.m_sImageFilePath;
	m_sHashAlgorithmId = rhs.m_sHashAlgorithmId;
	m_uiImageSize = rhs.m_uiImageSize;
	m_uiImageSizeToHash = rhs.m_uiImageSizeToHash;
	m_uiPartitionNumber = rhs.m_uiPartitionNumber;
	m_sEncryptAlgorithmId = rhs.m_sEncryptAlgorithmId;
	m_uiEncryptStartOffset = rhs.m_uiEncryptStartOffset;
	m_uiEncryptSize = rhs.m_uiEncryptSize;
	m_bChanged = rhs.m_bChanged;

	// blf image fields
	m_bIsBlf = rhs.m_bIsBlf;
	m_bEnable = rhs.m_bEnable;
	m_uiTimIncluded = rhs.m_uiTimIncluded;
	m_sType = rhs.m_sType;
	m_uiEraseSize = rhs.m_uiEraseSize;
}

// assignment operator
CImageDescription& CImageDescription::operator=(const CImageDescription& rhs) {
	// assignment operator
	if (&rhs != this) {
		CTimLib::operator=(rhs);

		memcpy(Hash, rhs.Hash, sizeof(Hash));

		m_sImageId = rhs.m_sImageId;
		m_sImageIdTag = rhs.m_sImageIdTag;
		m_sNextImageId = rhs.m_sNextImageId;
		m_sNextImageIdTag = rhs.m_sNextImageIdTag;
		m_sFlashEntryAddress = rhs.m_sFlashEntryAddress;
		m_sLoadAddress = rhs.m_sLoadAddress;
		m_uiImageSizeToCrc = rhs.m_uiImageSizeToCrc;
		m_sImageFilePath = rhs.m_sImageFilePath;
		m_sHashAlgorithmId = rhs.m_sHashAlgorithmId;
		m_uiImageSize = rhs.m_uiImageSize;
		m_uiImageSizeToHash = rhs.m_uiImageSizeToHash;
		m_uiPartitionNumber = rhs.m_uiPartitionNumber;
		m_sEncryptAlgorithmId = rhs.m_sEncryptAlgorithmId;
		m_uiEncryptStartOffset = rhs.m_uiEncryptStartOffset;
		m_uiEncryptSize = rhs.m_uiEncryptSize;
		m_bChanged = rhs.m_bChanged;

		// blf image fields
		m_bIsBlf = rhs.m_bIsBlf;
		m_bEnable = rhs.m_bEnable;
		m_uiTimIncluded = rhs.m_uiTimIncluded;
		m_sType = rhs.m_sType;
		m_uiEraseSize = rhs.m_uiEraseSize;
	}

	return *this;
}

void CImageDescription::Reset() {
	m_sImageId = "";
	m_sImageIdTag = "";
	m_sNextImageId = "<None>";
	m_sNextImageIdTag = "0xFFFFFFFF";
	m_sFlashEntryAddress = "0x00000000";
	m_sLoadAddress = "0x00000000";
	m_sImageFilePath = "";
	HashAlgorithmId(SHA160);
	m_uiImageSizeToCrc = 0;	 // now used for image hash in ntim descriptor files
	m_uiImageSizeToHash = 0xFFFFFFFF;
	m_uiImageSize = 0;
	m_uiPartitionNumber = 0;
	EncryptAlgorithmId(NO_ENCRYPTION);
	m_uiEncryptStartOffset = 0;
	m_uiEncryptSize = 0;

	// blf image fields
	m_bEnable = false;
	m_uiTimIncluded = 0;
	m_sType = "RAW";
	m_uiEraseSize = 0;

	memset(Hash, 0, sizeof(Hash));
}

void CImageDescription::ImageId(string& sImageId) {
	m_sImageId = sImageId;

	if (m_sImageId == "<None>" || m_sImageId.length() == 0)
		m_sImageIdTag = "0xFFFFFFFF";
	else {
		m_sImageId = ToUpper(sImageId, true);
		// turn first 4 ascii chars of Id into a hex encoded ascii tag
		string sIdReversed;
		size_t i = min((int) m_sImageId.length(), 4);
		for (; i > 0; i--)
			sIdReversed += m_sImageId[i - 1];
		while (sIdReversed.length() < 4)
			sIdReversed += '\0'; // nulls reserve bytes for int* dereference

		m_sImageIdTag = HexFormattedAscii(*((int*) sIdReversed.c_str()));
	}
	m_bChanged = true;
}

void CImageDescription::ImageIdTag(string& sTag) {
	m_sImageIdTag = sTag;

	int iId = Translate(m_sImageIdTag);
	char szId[5] = { 0 };
	*((int*) (szId)) = iId;

	string sIdReversed;
	size_t i = 4;
	for (; i > 0; i--)
		sIdReversed += szId[i - 1];
	m_sImageId = sIdReversed;
	m_bChanged = true;
}

void CImageDescription::NextImageId(string& sImageId) {
	m_sNextImageId = sImageId;

	if (m_sNextImageId == "0xFFFFFFFF")
		m_sNextImageId = "<None>";

	if (m_sNextImageId == "<None>" || m_sNextImageId.length() == 0)
		m_sNextImageIdTag = "0xFFFFFFFF";
	else {
		m_sNextImageId = ToUpper(sImageId, true);

		// turn first 4 ascii chars of Id into a hex encoded ascii tag
		string sIdReversed;
		size_t i = min((int) m_sNextImageId.length(), 4);
		for (; i > 0; i--)
			sIdReversed += m_sNextImageId[i - 1];
		while (sIdReversed.length() < 4)
			sIdReversed += '\0'; // nulls reserve bytes for int* dereference

		m_sNextImageIdTag = HexFormattedAscii(*((int*) sIdReversed.c_str()));
	}
	m_bChanged = true;
}

void CImageDescription::NextImageIdTag(string& sTag) {
	m_sNextImageIdTag = sTag;

	int iId = Translate(m_sNextImageIdTag);
	char szId[5] = { 0 };
	*((int*) (szId)) = iId;

	string sIdReversed;
	size_t i = 4;
	for (; i > 0; i--)
		sIdReversed += szId[i - 1];
	m_sNextImageId = sIdReversed;
	m_bChanged = true;
}

void CImageDescription::HashAlgorithmId(HASHALGORITHMID_T Id) {
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
		Id = HASHALGORITHMID_T::SHA160;
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

HASHALGORITHMID_T CImageDescription::HashAlgorithmId() {
	// default
	HASHALGORITHMID_T eHashAlgoId = HASHALGORITHMID_T::SHA160;
	if (HashAlgorithmIdStr() == "")
		HashAlgorithmId(eHashAlgoId);

	if (HashAlgorithmIdStr() == "SHA-160")
		eHashAlgoId = HASHALGORITHMID_T::SHA160;
	else if (HashAlgorithmIdStr() == "SHA-256")
		eHashAlgoId = HASHALGORITHMID_T::SHA256;
	else if (HashAlgorithmIdStr() == "SHA-512")
		eHashAlgoId = HASHALGORITHMID_T::SHA512;

	return eHashAlgoId;

}

void CImageDescription::EncryptAlgorithmId(ENCRYPTALGORITHMID_T Id) {
	switch (Id) {
	case PKCS1_v1_5_Caddo:
		m_sEncryptAlgorithmId = "PKCS1_v1_5_Caddo";
		break;

	case PKCS1_v1_5_Ippcp:
		m_sEncryptAlgorithmId = "PKCS1_v1_5_Ippcp";
		break;

	case PKCS_v2_2_Ippcp:
		m_sEncryptAlgorithmId = "PKCS_v2_2_Ippcp";
		break;

	case ECDSA_256:
		m_sEncryptAlgorithmId = "ECDSA_256";
		break;

	case ECDSA_521:
		m_sEncryptAlgorithmId = "ECDSA_521";
		break;

	case AES_CTS_ECB128:
		m_sEncryptAlgorithmId = "AES_CTS_ECB128";
		break;

	case AES_CTS_ECB256:
		m_sEncryptAlgorithmId = "AES_CTS_ECB256";
		break;

	case AES_CTS_ECB192:
		m_sEncryptAlgorithmId = "AES_CTS_ECB192";
		break;

	case AES_CTS_CBC128:
		m_sEncryptAlgorithmId = "AES_CTS_CBC128";
		break;

	case AES_CTS_CBC256:
		m_sEncryptAlgorithmId = "AES_CTS_CBC256";
		break;

	case AES_CTS_CBC192:
		m_sEncryptAlgorithmId = "AES_CTS_CBC192";
		break;

	case AES_TB_CTS_ECB128:
		m_sEncryptAlgorithmId = "AES_TB_CTS_ECB128";
		break;

	case AES_TB_CTS_ECB256:
		m_sEncryptAlgorithmId = "AES_TB_CTS_ECB256";
		break;

	case AES_TB_CTS_ECB192:
		m_sEncryptAlgorithmId = "AES_TB_CTS_ECB192";
		break;

	case AES_TB_CTS_CBC128:
		m_sEncryptAlgorithmId = "AES_TB_CTS_CBC128";
		break;

	case AES_TB_CTS_CBC256:
		m_sEncryptAlgorithmId = "AES_TB_CTS_CBC256";
		break;

	case AES_TB_CTS_CBC192:
		m_sEncryptAlgorithmId = "AES_TB_CTS_CBC192";
		break;

	case DUMMY_ENALG:
		m_sEncryptAlgorithmId = "DUMMY_ENALG";
		break;

	case NO_ENCRYPTION:
	default:
		m_sEncryptAlgorithmId = "NO_ENCRYPTION";
		break;
	}
}

ENCRYPTALGORITHMID_T CImageDescription::EncryptAlgorithmId() {
	// default
	ENCRYPTALGORITHMID_T eEncryptAlgoId = NO_ENCRYPTION;
	if (EncryptAlgorithmIdStr() == "")
		EncryptAlgorithmId(NO_ENCRYPTION);

	if (EncryptAlgorithmIdStr() == "PKCS1_v1_5_Caddo")
		eEncryptAlgoId = PKCS1_v1_5_Caddo;
	else if (EncryptAlgorithmIdStr() == "PKCS1_v1_5_Ippcp")
		eEncryptAlgoId = PKCS1_v1_5_Ippcp;
	else if (EncryptAlgorithmIdStr() == "PKCS_v2_2_Ippcp")
		eEncryptAlgoId = PKCS_v2_2_Ippcp;
	else if (EncryptAlgorithmIdStr() == "ECDSA_256")
		eEncryptAlgoId = ECDSA_256;
	else if (EncryptAlgorithmIdStr() == "ECDSA_521")
		eEncryptAlgoId = ECDSA_521;
	else if (EncryptAlgorithmIdStr() == "AES_TB_CTS_ECB128")
		eEncryptAlgoId = AES_TB_CTS_ECB128;
	else if (EncryptAlgorithmIdStr() == "AES_TB_CTS_ECB256")
		eEncryptAlgoId = AES_TB_CTS_ECB256;
	else if (EncryptAlgorithmIdStr() == "AES_TB_CTS_ECB192")
		eEncryptAlgoId = AES_TB_CTS_ECB192;
	else if (EncryptAlgorithmIdStr() == "AES_TB_CTS_CBC128")
		eEncryptAlgoId = AES_TB_CTS_CBC128;
	else if (EncryptAlgorithmIdStr() == "AES_TB_CTS_CBC256")
		eEncryptAlgoId = AES_TB_CTS_CBC256;
	else if (EncryptAlgorithmIdStr() == "AES_TB_CTS_CBC192")
		eEncryptAlgoId = AES_TB_CTS_CBC192;
	else if (EncryptAlgorithmIdStr() == "NO_ENCRYPTION")
		eEncryptAlgoId = NO_ENCRYPTION;
	else if (EncryptAlgorithmIdStr() == "DUMMY_ENALG")
		eEncryptAlgoId = DUMMY_ENALG;
	else if (EncryptAlgorithmIdStr() == "")
		eEncryptAlgoId = NO_ENCRYPTION;

	return eEncryptAlgoId;

}

string CImageDescription::TimIncludedStr() {
	string sTimIncluded;

	switch (m_uiTimIncluded) {
	case 0:
		sTimIncluded = "Tim Not Included";
		break;
	case 1:
		sTimIncluded = "Tim Included";
		break;
	case 2:
		sTimIncluded = "DTIM Primary";
		break;
	case 3:
		sTimIncluded = "DTIM Recovery";
		break;
	case 4:
		sTimIncluded = "DTIM CP";
		break;

	default:
		sTimIncluded = "Tim Not Included";
		break;
	}
	return sTimIncluded;
}

bool CImageDescription::ParseImageInfoTIM(CTimDescriptorLine*& pLine,
		CTimDescriptorLine*& pPrevLine) {
	bool bRet = true;
	// This is a helper function to parse image info data from the descriptor
	// text file supplied by the user.
	if ((pLine = m_TimDescriptor.GetNextLineField("Image ID", pLine, true, 3))) {
		pPrevLine = pLine;
		ImageIdTag(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	if ((pLine = m_TimDescriptor.GetNextLineField("Next Image ID", pLine, true,
			1))) {
		pPrevLine = pLine;
		NextImageIdTag(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	if ((pLine = m_TimDescriptor.GetNextLineField("Flash Entry Address", pLine,
			true, 1))) {
		pPrevLine = pLine;
		FlashEntryAddress(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	if ((pLine = m_TimDescriptor.GetNextLineField("Load Address", pLine, true,
			1))) {
		pPrevLine = pLine;
		LoadAddress(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	if (m_TimDescriptor.getTimHeader().getTrusted()) {
		if ((pLine = m_TimDescriptor.GetNextLineField(
				"Image Size To Hash in bytes", pLine, true, 1))) {
			pPrevLine = pLine;
			ImageSizeToHash(Translate(pLine->m_FieldValue));
		} else {
			pLine = pPrevLine;
			bRet = false;
		}

		if ((pLine = m_TimDescriptor.GetNextLineField("Hash Algorithm ID",
				pLine, true, 1))) {
			pPrevLine = pLine;
			HashAlgorithmId((HASHALGORITHMID_T) Translate(pLine->m_FieldValue));
		} else {
			pLine = pPrevLine;
			bRet = false;
		}
	} else {
		/*if (m_TimDescriptor.getTimHeader().getVersion() >= TIM_3_5_00)
		 {*/
		//}
		CTimDescriptorLine* pLastSuccessfulFindLine;
		if ((pLine = m_TimDescriptor.GetNextLineField(
				"Image Size To CRC in bytes", pLine, true, 1))) {
			pLastSuccessfulFindLine = pLine;
			pPrevLine = pLine;
			ImageSizeToCrc(Translate(pLine->m_FieldValue));
			// use size to crc to actually do image hash
			ImageSizeToHash(Translate(pLine->m_FieldValue));
			//HashAlgorithmId( SHA160 );
		} else {
			pLine = pPrevLine;
			bRet = false;
		}

		if (m_TimDescriptor.IsFieldPresent("Hash Algorithm ID", pLine,
				1) != NULL) {
			pLine = m_TimDescriptor.GetNextLineField("Hash Algorithm ID", pLine,
					true, 1);
			pPrevLine = pLine;
			HashAlgorithmId((HASHALGORITHMID_T) Translate(pLine->m_FieldValue));
		} else {
			//Did not find Hash Algorithm ID field hence default to SHA1 and re-load pLine and pPrevLine
			// by doing a GetNextLineField
			HashAlgorithmId(SHA160);
			//pLine = pLastSuccessfulFindLine;
			pPrevLine = pLine;
		}
	}

	if (m_TimDescriptor.getTimHeader().getVersion() >= TIM_3_2_00) {
		if ((pLine = m_TimDescriptor.GetNextLineField("Partition Number", pLine,
				true, 1))) {
			pPrevLine = pLine;
			PartitionNumber(Translate(pLine->m_FieldValue));
		} else {
			pLine = pPrevLine;
			bRet = false;
		}
	}

	if (m_TimDescriptor.getTimHeader().getTrusted()) {
		if (m_TimDescriptor.getTimHeader().getVersion() >= TIM_3_5_00) {
			if ((pLine = m_TimDescriptor.GetNextLineField(
					"Encrypt Algorithm ID", pLine, true, 1))) {
				pPrevLine = pLine;
				EncryptAlgorithmId(
						(ENCRYPTALGORITHMID_T) Translate(pLine->m_FieldValue));
			} else {
				pLine = pPrevLine;
				bRet = false;
			}

			if ((pLine = m_TimDescriptor.GetNextLineField(
					"Encrypt Start Offset", pLine, true, 1))) {
				pPrevLine = pLine;
				EncryptStartOffset(Translate(pLine->m_FieldValue));
			} else {
				pLine = pPrevLine;
				bRet = false;
			}

			if ((pLine = m_TimDescriptor.GetNextLineField("Encrypt Size", pLine,
					true, 1))) {
				pPrevLine = pLine;
				EncryptSize(Translate(pLine->m_FieldValue));
			} else {
				pLine = pPrevLine;
				bRet = false;
			}
		}
	}

	pLine = m_TimDescriptor.GetNextLineField("Image Filename", pLine, true, 1);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_sImageFilePath = pLine->m_FieldValue;

		// if the image is not the TIM.bin, then see if it exists and get the size.
		// TIM.bin may not exist yet so we can't try to open it
		if (ImageId().compare(0, 4, "TIMH") != 0
				&& ImageId().compare(0, 4, "TIMD") != 0
				&& ImageId().compare(0, 4, "TIMN") != 0) {
			ifstream ifsImage;
			ifsImage.open(ImageFilePath().c_str(),
					ios_base::in | ios_base::binary);
			if (ifsImage.bad() || ifsImage.fail() || !ifsImage.is_open()) {
				stringstream ss;
				ss << endl << "  Error: Cannot open image file name <"
						<< ImageFilePath().c_str() << ">" << endl;
				m_TimDescriptor.ParsingError(ss.str(), true, pLine);
				ImageSize(0);
				bRet = false;
			} else {
				// Get position of file, thus the file size.
				ifsImage.seekg(0, ios_base::end);
				ifsImage.clear();

				// note that the (unsigned int) cast is dangerous for very large files 
				// but should not be an issue here
				ImageSize((unsigned int) ifsImage.tellg());
				//printf("Size: %d\n", m_pImageInfo[i].ImageSize);
			}
			ifsImage.close();
		}
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	return bRet;
}

bool CImageDescription::ParseImageInfoBlf(CTimDescriptorLine*& pLine,
		CTimDescriptorLine*& pPrevLine, string sNum) {
	bool bRet = true;
	if ((pLine = m_TimDescriptor.GetLineField(sNum + "Image_Enable", true))) {
		pPrevLine = pLine;
		Enable(Translate(pLine->m_FieldValue) == 0 ? false : true);
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	if ((pLine = m_TimDescriptor.GetNextLineField(sNum + "Image_Tim_Included",
			pLine, true, 1))) {
		pPrevLine = pLine;
		TimIncluded(Translate(pLine->m_FieldValue));
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	// This is a helper function to parse image info data from the descriptor
	// text file supplied by the user.
	pLine = m_TimDescriptor.GetNextLineField(sNum + "Image_Image_ID", pLine,
			true);
	if (pLine != 0) {
		pPrevLine = pLine;
		ImageIdTag(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	pLine = m_TimDescriptor.GetNextLineField(sNum + "Image_Next_Image_ID",
			pLine, true, 1);
	if (pLine != 0) {
		pPrevLine = pLine;
		NextImageIdTag(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	pLine = m_TimDescriptor.GetNextLineField(sNum + "Image_Path", pLine, true,
			1);
	if (pLine != 0) {
		pPrevLine = pLine;
		ImageFilePath(pLine->m_FieldValue);

		// if the image is not the TIM.bin, then see it it exists and get the size.
		// TIM.bin may not exist yet so we can't try to open it
		if (ImageId().compare(0, 4, "TIMH") != 0
				&& ImageId().compare(0, 4, "TIMD") != 0
				&& ImageId().compare(0, 4, "TIMN") != 0) {
			ifstream ifsImage;
			ifsImage.open(ImageFilePath().c_str(),
					ios_base::in | ios_base::binary);
			if (ifsImage.bad() || ifsImage.fail() || !ifsImage.is_open()) {
				stringstream ss;
				ss << endl << "  Error: Cannot open image file name <"
						<< ImageFilePath().c_str() << ">" << endl;
				m_TimDescriptor.ParsingError(ss.str(), true, pLine);
				ImageSize(0);
				bRet = false;
			} else {
				// Get position of file, thus the file size.
				ifsImage.seekg(0, ios_base::end);
				ifsImage.clear();

				// note that the (unsigned int) cast is dangerous for very large files 
				// but should not be an issue here
				ImageSize((unsigned int) ifsImage.tellg());
				//printf("Size: %d\n", m_pImageInfo[i].ImageSize);
			}
			ifsImage.close();
		}
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	pLine = m_TimDescriptor.GetNextLineField(sNum + "Image_Flash_Entry_Address",
			pLine, true, 1);
	if (pLine != 0) {
		pPrevLine = pLine;
		FlashEntryAddress(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	pLine = m_TimDescriptor.GetNextLineField(sNum + "Image_Load_Address", pLine,
			true, 1);
	if (pLine != 0) {
		pPrevLine = pLine;
		LoadAddress(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	pLine = m_TimDescriptor.GetNextLineField(sNum + "Image_Type", pLine, true,
			1);
	if (pLine != 0) {
		pPrevLine = pLine;
		Type(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	// skip Image_ID_Name in blf - 2 in last arguments 

	pLine = m_TimDescriptor.GetNextLineField(sNum + "Image_Erase_Size", pLine,
			true, 2);
	if (pLine != 0) {
		pPrevLine = pLine;
		EraseSize(Translate(pLine->m_FieldValue));
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	pLine = m_TimDescriptor.GetNextLineField(sNum + "Image_Partition_Number",
			pLine, true, 1);
	if (pLine != 0) {
		pPrevLine = pLine;
		PartitionNumber(Translate(pLine->m_FieldValue));
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	if (!m_TimDescriptor.getTimHeader().getTrusted()) {
		pLine = m_TimDescriptor.GetNextLineField(
				sNum + "Image_Size_To_CRC_in_bytes", pLine, true, 1);
		if (pLine != 0) {
			pPrevLine = pLine;
			ImageSizeToCrc(Translate(pLine->m_FieldValue));
			// use size to crc to actually do image hash
			ImageSizeToHash(Translate(pLine->m_FieldValue));
			HashAlgorithmId(SHA160);

		} else {
			pLine = pPrevLine;
			bRet = false;
		}
	}

	// this should not be in non-trusted blf but it is
	pLine = m_TimDescriptor.GetNextLineField(sNum + "Image_Hash_Algorithm_ID",
			pLine, true, 1);
	if (pLine != 0) {
		pPrevLine = pLine;
		HashAlgorithmId(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
		bRet = false;
	}

	// this should not be in non-trusted blf but it is
	pLine = m_TimDescriptor.GetNextLineField(
			sNum + "Image_Image_Size_To_Hash_in_bytes", pLine, true, 1);
	if (pLine != 0) {
		pPrevLine = pLine;
		ImageSizeToHash(Translate(pLine->m_FieldValue));
	} else {
		pLine = pPrevLine;
		bRet = false;
	}
	// blf does not support this yet
	if (m_TimDescriptor.getTimHeader().getTrusted() && m_TimDescriptor.getTimHeader().getVersion() >= TIM_3_5_00) {
		pLine = m_TimDescriptor.GetNextLineField(
				sNum + "Image_Encrypt_Algorithm_ID", pLine, true, 1);
		if (pLine != 0) {
			pPrevLine = pLine;
			EncryptAlgorithmId(pLine->m_FieldValue);
		} else {
			pLine = pPrevLine;
			bRet = false;
		}

		pLine = m_TimDescriptor.GetNextLineField(
				sNum + "Image_Encrypt_Start_Offset", pLine, true, 1);
		if (pLine != 0) {
			pPrevLine = pLine;
			EncryptStartOffset(Translate(pLine->m_FieldValue));
		} else {
			pLine = pPrevLine;
			bRet = false;
		}

		pLine = m_TimDescriptor.GetNextLineField(sNum + "Image_Encrypt_Size",
				pLine, true, 1);
		if (pLine != 0) {
			pPrevLine = pLine;
			EncryptSize(Translate(pLine->m_FieldValue));
		} else {
			pLine = pPrevLine;
			bRet = false;
		}
	}
	return bRet;

}
