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


#include "BlfHeader.h"
#include "TimDescriptor.h"

// static object to hold all the *.blf only parsed lines and object references
//t_TimDescriptorLines CBlfHeader::g_BlfHeaderLines;
CTimDescriptorLine* CBlfHeader::pPrevLine = 0;

CBlfHeader::CBlfHeader() :
		CTimLib() {
	Reset();
}

CBlfHeader::~CBlfHeader(void) {

}

CBlfHeader::CBlfHeader(const CBlfHeader& rhs) :
		CTimLib(rhs) {
	// need to do a deep copy of lists to avoid dangling references
	CBlfHeader& nc_rhs = const_cast<CBlfHeader&>(rhs);

	m_sBlfVersion = rhs.m_sBlfVersion;

	// [UE_Options]
	m_uiUEBootOption = rhs.m_uiUEBootOption;

	// [Flash_Properties]
	m_uiMaxUploadSplitSize = rhs.m_uiMaxUploadSplitSize;
	m_uiMaxFBFSplitSize = rhs.m_uiMaxFBFSplitSize;
	m_sFlashFamily = rhs.m_sFlashFamily;
	m_uiSpareAreaSize = rhs.m_uiSpareAreaSize;
	m_uiDataAreaSize = rhs.m_uiDataAreaSize;
	m_uiFBFSectorSize = rhs.m_uiFBFSectorSize;

	// [Flash_Options]
	m_uiProductionMode = rhs.m_uiProductionMode;
	m_uiSkipBlocksNumber = rhs.m_uiSkipBlocksNumber;
	m_bEraseAllFlash = rhs.m_bEraseAllFlash;
	m_bResetBBT = rhs.m_bResetBBT;
	m_pTimDescriptor = rhs.m_pTimDescriptor;
	m_bChanged = rhs.m_bChanged;
}

CBlfHeader& CBlfHeader::operator=(const CBlfHeader& rhs) {
	// assignment operator
	if (&rhs != this) {
		CTimLib::operator=(rhs);

		// need to do a deep copy of lists to avoid dangling references
		CBlfHeader& nc_rhs = const_cast<CBlfHeader&>(rhs);

		m_sBlfVersion = rhs.m_sBlfVersion;

		// [UE_Options]
		m_uiUEBootOption = rhs.m_uiUEBootOption;

		// [Flash_Properties]
		m_uiMaxUploadSplitSize = rhs.m_uiMaxUploadSplitSize;
		m_uiMaxFBFSplitSize = rhs.m_uiMaxFBFSplitSize;
		m_sFlashFamily = rhs.m_sFlashFamily;
		m_uiSpareAreaSize = rhs.m_uiSpareAreaSize;
		m_uiDataAreaSize = rhs.m_uiDataAreaSize;
		m_uiFBFSectorSize = rhs.m_uiFBFSectorSize;

		// [Flash_Options]
		m_uiProductionMode = rhs.m_uiProductionMode;
		m_uiSkipBlocksNumber = rhs.m_uiSkipBlocksNumber;
		m_bEraseAllFlash = rhs.m_bEraseAllFlash;
		m_bResetBBT = rhs.m_bResetBBT;

		m_bChanged = rhs.m_bChanged;
	}
	return *this;
}

void CBlfHeader::Reset() {
	m_sBlfVersion = "V2.1.0";

	// [UE_Options]
	m_uiUEBootOption = 0;

	// [Flash_Properties]
	m_uiMaxUploadSplitSize = 0;
	m_uiMaxFBFSplitSize = 0;
	m_sFlashFamily = "eMMC";
	m_uiSpareAreaSize = 0;
	m_uiDataAreaSize = 0;
	m_uiFBFSectorSize = 4096;

	// [Flash_Options]
	m_uiProductionMode = 0;
	m_uiSkipBlocksNumber = 0;
	m_bEraseAllFlash = false;
	m_bResetBBT = false;

	m_bChanged = false;
}

bool CBlfHeader::ParseBlfHeader(CTimDescriptorLine*& pLine) {
	bool bRet = true;
	pLine = m_pTimDescriptor->GetLineField("[BLF_Version]", true);
	if (pLine != 0) {
		pPrevLine = pLine;
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("Blf_Version_Number", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_sBlfVersion = pLine->m_FieldValue;
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("[UE_Options]", true);
	if (pLine != 0) {
		pPrevLine = pLine;
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("UE_Boot_Option", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_uiUEBootOption = Translate(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("[Flash_Properties]", true);
	if (pLine != 0) {
		pPrevLine = pLine;
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("Max_Upload_Split_Size", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_uiMaxUploadSplitSize = Translate(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("Max_FBF_Split_Size", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_uiMaxFBFSplitSize = Translate(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("Flash_Family", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_sFlashFamily = pLine->m_FieldValue;
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("Spare_Area_Size", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_uiSpareAreaSize = Translate(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("Data_Area_Size", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_uiDataAreaSize = Translate(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("FBF_Sector_Size", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_uiFBFSectorSize = Translate(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("[Flash_Options]", true);
	if (pLine != 0) {
		pPrevLine = pLine;
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("ProductionMode", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_uiProductionMode = Translate(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("Skip_Blocks_Number", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_uiSkipBlocksNumber = Translate(pLine->m_FieldValue);
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("Erase_All_Flash", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_bEraseAllFlash = (Translate(pLine->m_FieldValue) == 0 ? false : true);
	} else {
		pLine = pPrevLine;
	}

	pLine = m_pTimDescriptor->GetLineField("Reset_BBT", true);
	if (pLine != 0) {
		pPrevLine = pLine;
		m_bResetBBT = (Translate(pLine->m_FieldValue) == 0 ? false : true);
	} else {
		pLine = pPrevLine;
	}

	return bRet;
}
