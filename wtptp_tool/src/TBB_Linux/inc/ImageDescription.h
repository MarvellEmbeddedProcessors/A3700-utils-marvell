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


#ifndef __IMAGE_DESCRIPTION_H__
#define __IMAGE_DESCRIPTION_H__

#include "TimLib.h"

#include <string>
#include <list>
#include <iterator>
using namespace std;

const int MAX_HASH = 256;
const int MAX_HASH_IN_BITS = 512;	// 512 bits

class CTimDescriptor;
class CTimDescriptorLine;

class CImageDescription: public CTimLib {
public:
	CImageDescription(bool bIsBlf, CTimDescriptor& rTimDescriptor);
	virtual ~CImageDescription(void);

	// copy constructor
	CImageDescription(const CImageDescription& rhs);
	// assignment operator
	CImageDescription& operator=(const CImageDescription& rhs);

	string& ImageId() {
		return m_sImageId;
	}
	void ImageId(string& sImageId);

	string& ImageIdTag() {
		return m_sImageIdTag;
	}
	void ImageIdTag(string& sTag);

	string& NextImageId() {
		return m_sNextImageId;
	}
	void NextImageId(string& sImageId);

	string& NextImageIdTag() {
		return m_sNextImageIdTag;
	}
	void NextImageIdTag(string& sImageIdTag);

	void FlashEntryAddress(string& sAddress) {
		sAddress.length() > 2 ?
				m_sFlashEntryAddress = sAddress : m_sFlashEntryAddress =
						"0x00000000";
		m_bChanged = true;
	}
	string& FlashEntryAddress() {
		return m_sFlashEntryAddress;
	}

	void LoadAddress(string& sAddress) {
		sAddress.length() > 2 ? m_sLoadAddress = sAddress : m_sLoadAddress =
										"0x00000000";
		m_bChanged = true;
	}
	string& LoadAddress() {
		return m_sLoadAddress;
	}

	void ImageSizeToCrc(unsigned int uiImageSizeToCrc) {
		m_uiImageSizeToCrc = uiImageSizeToCrc;
		m_bChanged = true;
	}
	unsigned int& ImageSizeToCrc() {
		return m_uiImageSizeToCrc;
	}

	void ImageFilePath(string& sImageFilePath) {
		m_sImageFilePath = sImageFilePath;
		m_bChanged = true;
	}
	string& ImageFilePath() {
		return m_sImageFilePath;
	}

	string& HashAlgorithmIdStr() {
		return m_sHashAlgorithmId;
	}
	void HashAlgorithmId(string& sHashAlgorithmId) {
		m_sHashAlgorithmId = sHashAlgorithmId;
		m_bChanged = true;
	}
	void HashAlgorithmId(HASHALGORITHMID_T Id);
	HASHALGORITHMID_T HashAlgorithmId();

	void PartitionNumber(unsigned int uiPartitionNumber) {
		m_uiPartitionNumber = uiPartitionNumber;
		m_bChanged = true;
	}
	unsigned int& PartitionNumber() {
		return m_uiPartitionNumber;
	}

	void ImageSizeToHash(unsigned int uiImageSizeToHash) {
		m_uiImageSizeToHash = uiImageSizeToHash;
		m_bChanged = true;
	}
	unsigned int& ImageSizeToHash() {
		return m_uiImageSizeToHash;
	}

	void ImageSize(unsigned int uiImageSize) {
		m_uiImageSize = uiImageSize;
		m_bChanged = true;
	}
	unsigned int& ImageSize() {
		return m_uiImageSize;
	}

	string& EncryptAlgorithmIdStr() {
		return m_sEncryptAlgorithmId;
	}
	void EncryptAlgorithmId(string& sEncryptAlgorithmId) {
		m_sEncryptAlgorithmId = sEncryptAlgorithmId;
		m_bChanged = true;
	}
	void EncryptAlgorithmId(ENCRYPTALGORITHMID_T Id);
	ENCRYPTALGORITHMID_T EncryptAlgorithmId();

	void EncryptStartOffset(unsigned int uiEncryptStartOffset) {
		m_uiEncryptStartOffset = uiEncryptStartOffset;
		m_bChanged = true;
	}
	unsigned int& EncryptStartOffset() {
		return m_uiEncryptStartOffset;
	}

	void EncryptSize(unsigned int uiEncryptSize) {
		m_uiEncryptSize = uiEncryptSize;
		m_bChanged = true;
	}
	unsigned int& EncryptSize() {
		return m_uiEncryptSize;
	}

	// blf image fields
	void Enable(bool bSet) {
		m_bEnable = bSet;
		m_bChanged = true;
	}
	bool& Enable() {
		return m_bEnable;
	}

	void TimIncluded(unsigned int uiTimIncluded) {
		m_uiTimIncluded = uiTimIncluded;
		m_bChanged = true;
	}
	unsigned int& TimIncluded() {
		return m_uiTimIncluded;
	}

	string TimIncludedStr();

	void Type(string& sType) {
		m_sType = sType;
		m_bChanged = true;
	}
	string& Type() {
		return m_sType;
	}

	void EraseSize(unsigned int uiEraseSize) {
		m_uiEraseSize = uiEraseSize;
		m_bChanged = true;
	}
	unsigned int& EraseSize() {
		return m_uiEraseSize;
	}

	bool ParseImageInfoTIM(CTimDescriptorLine*& pLine,
			CTimDescriptorLine*& pPrevLine);
	bool ParseImageInfoBlf(CTimDescriptorLine*& pLine,
			CTimDescriptorLine*& pPrevLine, string sNum);

	void Reset();

	bool IsChanged() {
		return m_bChanged;
	}
	void Changed(bool bSet) {
		m_bChanged = bSet;
	}

	unsigned int Hash[MAX_HASH_IN_BITS / 32];// Reserve 512 bits for the largest hash

protected:
	string m_sImageId;
	string m_sImageIdTag;
	string m_sNextImageId;
	string m_sNextImageIdTag;
	string m_sFlashEntryAddress;
	string m_sLoadAddress;
	string m_sImageFilePath; // no path, assume sTimDescriptorFilePath
	string m_sHashAlgorithmId;
	string m_sEncryptAlgorithmId;

	unsigned int m_uiImageSize;
	unsigned int m_uiImageSizeToHash;
	unsigned int m_uiImageSizeToCrc;
	unsigned int m_uiPartitionNumber;
	unsigned int m_uiEncryptStartOffset;
	unsigned int m_uiEncryptSize;

	// blf image fields
	bool m_bIsBlf;
	bool m_bEnable;
	unsigned int m_uiTimIncluded;
	string m_sType;
	unsigned int m_uiEraseSize;

	CTimDescriptor& m_TimDescriptor;

	bool m_bChanged;
};

typedef list<CImageDescription*> t_ImagesList;
typedef list<CImageDescription*>::iterator t_ImagesIter;
#endif //__IMAGE_DESCRIPTION_H__
