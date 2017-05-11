/******************************************************************************
 *
 *  (C)Copyright 2013 Marvell. All Rights Reserved.
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

#ifndef __BLF_HEADER_H__
#define __BLF_HEADER_H__

#include "TimLib.h"
#include "TimDescriptorLine.h"

#include <string>
#include <list>
#include <iterator>
using namespace std;

// predeclarations
class CTimDescriptor;

class CBlfHeader: public CTimLib {
public:
	CBlfHeader();
	virtual ~CBlfHeader(void);

	// copy constructor
	CBlfHeader(const CBlfHeader& rhs);
	// assignment operator
	CBlfHeader& operator=(const CBlfHeader& rhs);

	void BlfVersion(string& sVersion) {
		m_sBlfVersion = sVersion;
		m_bChanged = true;
	}
	string& BlfVersion() {
		return m_sBlfVersion;
	}

	void UEBootOption(unsigned int uiUEBootOption) {
		m_uiUEBootOption = uiUEBootOption;
		m_bChanged = true;
	}
	unsigned int& UEBootOption() {
		return m_uiUEBootOption;
	}

	void MaxUploadSplitSize(unsigned int uiMaxUploadSplitSize) {
		m_uiMaxUploadSplitSize = uiMaxUploadSplitSize;
		m_bChanged = true;
	}
	unsigned int& MaxUploadSplitSize() {
		return m_uiMaxUploadSplitSize;
	}

	void MaxFBFSplitSize(unsigned int uiMaxFBFSplitSize) {
		m_uiMaxFBFSplitSize = uiMaxFBFSplitSize;
		m_bChanged = true;
	}
	unsigned int& MaxFBFSplitSize() {
		return m_uiMaxFBFSplitSize;
	}

	void FlashFamily(string& sFlashFamily) {
		m_sFlashFamily = sFlashFamily;
		m_bChanged = true;
	}
	string& FlashFamily() {
		return m_sFlashFamily;
	}

	void SpareAreaSize(unsigned int uiSpareAreaSize) {
		m_uiSpareAreaSize = uiSpareAreaSize;
		m_bChanged = true;
	}
	unsigned int& SpareAreaSize() {
		return m_uiSpareAreaSize;
	}

	void DataAreaSize(unsigned int uiDataAreaSize) {
		m_uiDataAreaSize = uiDataAreaSize;
		m_bChanged = true;
	}
	unsigned int& DataAreaSize() {
		return m_uiDataAreaSize;
	}

	void FBFSectorSize(unsigned int uiFBFSectorSize) {
		m_uiFBFSectorSize = uiFBFSectorSize;
		m_bChanged = true;
	}
	unsigned int& FBFSectorSize() {
		return m_uiFBFSectorSize;
	}

	void ProductionMode(unsigned int uiProductionMode) {
		m_uiProductionMode = uiProductionMode;
		m_bChanged = true;
	}
	unsigned int& ProductionMode() {
		return m_uiProductionMode;
	}

	void SkipBlocksNumber(unsigned int uiSkipBlocksNumber) {
		m_uiSkipBlocksNumber = uiSkipBlocksNumber;
		m_bChanged = true;
	}
	unsigned int& SkipBlocksNumber() {
		return m_uiSkipBlocksNumber;
	}

	void EraseAllFlash(bool bSet) {
		m_bEraseAllFlash = bSet;
		m_bChanged = true;
	}
	bool& EraseAllFlash() {
		return m_bEraseAllFlash;
	}

	void ResetBBT(bool bSet) {
		m_bResetBBT = bSet;
		m_bChanged = true;
	}
	bool& ResetBBT() {
		return m_bResetBBT;
	}

	bool ParseBlfHeader(CTimDescriptorLine*& pLine);

	void Reset();

	bool IsChanged() {
		return m_bChanged;
	}
	void Changed(bool bSet) {
		m_bChanged = bSet;
	}

	static CTimDescriptorLine* pPrevLine;

	CTimDescriptor* m_pTimDescriptor;

protected:

	// [BLF_Version]
	string m_sBlfVersion;

	// [UE_Options]
	unsigned int m_uiUEBootOption;

	// [Flash_Properties]
	unsigned int m_uiMaxUploadSplitSize;
	unsigned int m_uiMaxFBFSplitSize;
	string m_sFlashFamily;
	unsigned int m_uiSpareAreaSize;
	unsigned int m_uiDataAreaSize;
	unsigned int m_uiFBFSectorSize;

	// [Flash_Options]
	unsigned int m_uiProductionMode;
	unsigned int m_uiSkipBlocksNumber;
	bool m_bEraseAllFlash;
	bool m_bResetBBT;

	bool m_bChanged;
};

#endif //__BLF_HEADER_H__
