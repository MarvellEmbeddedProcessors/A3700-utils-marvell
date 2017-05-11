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

#include "ReservedPackageData.h"
#include "TimDescriptor.h"
#include "ExtendedReservedData.h"

#include <algorithm>

CReservedPackageData::CReservedPackageData(void) :
		CTimLib() {
	m_sPackageId = "";
	m_sPackageIdTag = "0x00000000";
	m_bChanged = false;
}

CReservedPackageData::~CReservedPackageData(void) {
	DiscardAll();
}

CReservedPackageData::CReservedPackageData(const CReservedPackageData& rhs) :
		CTimLib(rhs) {
	// copy constructor
	CReservedPackageData& nc_rhs = const_cast<CReservedPackageData&>(rhs);

	m_sPackageId = rhs.m_sPackageId;
	m_sPackageIdTag = rhs.m_sPackageIdTag;
	m_bChanged = rhs.m_bChanged;

	if (nc_rhs.m_PackageDataList.size() > 0) {
		for_each(nc_rhs.m_PackageDataList.begin(),
				nc_rhs.m_PackageDataList.end(), [this](string*& psRhsData)
				{
					string* psData = new string(*psRhsData);
					m_PackageDataList.push_back(psData);
				});
	}

	if (nc_rhs.m_PackageCommentList.size() > 0) {
		for_each(nc_rhs.m_PackageCommentList.begin(),
				nc_rhs.m_PackageCommentList.end(), [this](string*& psRhsComment)
				{
					string* psData = new string(*psRhsComment);
					m_PackageCommentList.push_back(psData);
				});
	}
}

CReservedPackageData& CReservedPackageData::operator=(
		const CReservedPackageData& rhs) {
	// assignment operator
	if (&rhs != this) {
		CTimLib::operator=(rhs);

		CReservedPackageData& nc_rhs = const_cast<CReservedPackageData&>(rhs);

		// delete the existing list and recreate a new one
		DiscardAll();

		m_sPackageId = rhs.m_sPackageId;
		m_sPackageIdTag = rhs.m_sPackageIdTag;
		m_bChanged = rhs.m_bChanged;

		// need to do a deep copy of lists to avoid dangling references
		if (nc_rhs.m_PackageDataList.size() > 0) {
			for_each(nc_rhs.m_PackageDataList.begin(),
					nc_rhs.m_PackageDataList.end(), [this](string*& psRhsData)
					{
						string* psData = new string(*psRhsData);
						m_PackageDataList.push_back(psData);
					});
		}

		if (nc_rhs.m_PackageCommentList.size() > 0) {
			for_each(nc_rhs.m_PackageCommentList.begin(),
					nc_rhs.m_PackageCommentList.end(),
					[this](string*& psRhsComment)
					{
						string* psData = new string(*psRhsComment);
						m_PackageCommentList.push_back(psData);
					});
		}
	}
	return *this;
}

void CReservedPackageData::DiscardAll() {
	DiscardDataAndComments();
}

void CReservedPackageData::DiscardDataAndComments() {
	// delete all package data strings
	if (m_PackageDataList.size() > 0) {
		for_each(m_PackageDataList.begin(), m_PackageDataList.end(),
				[](string*& psData)
				{
					delete psData;
				});
	}
	m_PackageDataList.clear();

	// delete all package data strings
	if (m_PackageDataList.size() > 0) {
		for_each(m_PackageCommentList.begin(), m_PackageCommentList.end(),
				[](string*& psComment)
				{
					delete psComment;
				});
	}
	m_PackageCommentList.clear();
}

int CReservedPackageData::Size() {
	return (int) (m_PackageDataList.size() * 4) + 8; // +4 (PID) add packageid & package size
}

void CReservedPackageData::AddData(string* psData, string *psComment) {
	m_PackageDataList.push_back(psData);
	if (*psComment == "")
		PredefinedPackageComments((int) m_PackageDataList.size(), psComment);

	m_PackageCommentList.push_back(psComment);
	m_bChanged = true;
}

void CReservedPackageData::DeleteData(string* psData) {

}

int CReservedPackageData::DataBlock(string& sDataBlock) {
	sDataBlock = "";
	// tally payload size
	if (m_PackageDataList.size() > 0) {
		for_each(m_PackageDataList.begin(), m_PackageDataList.end(),
				[&sDataBlock](string*& pData) {sDataBlock += (pData)->c_str(); sDataBlock += " ";});
	}
	return Size();
}

bool CReservedPackageData::IsChanged() {
	return m_bChanged;
}

void CReservedPackageData::PackageIdTag(const string& sPackageIdTag) {
	m_sPackageIdTag = ToUpper(sPackageIdTag, true);

	// update PackageId if a predefined PackageIdTag
	// this is necessary when parsing a descriptor file since only the tag
	// is stored in the descriptor file
	if (Translate(m_sPackageIdTag) == AUTOBIND)
		m_sPackageId = "AUTOBIND";
	else if (Translate(m_sPackageIdTag) == DDRID)
		m_sPackageId = "DDRID";
	else if (Translate(m_sPackageIdTag) == GPIOID)
		m_sPackageId = "GPIOID";
	else if (Translate(m_sPackageIdTag) == RESUMEBLID)
		m_sPackageId = "RESUMEBLID";
	else if (Translate(m_sPackageIdTag) == ROMRESUMEID)
		m_sPackageId = "ROMRESUMEID";
	else if (Translate(m_sPackageIdTag) == TBR_XFER)
		m_sPackageId = "TBR_XFER";
	else if (Translate(m_sPackageIdTag) == UARTID)
		m_sPackageId = "UARTID";
	else if (Translate(m_sPackageIdTag) == USBID)
		m_sPackageId = "USBID";
	else if (Translate(m_sPackageIdTag) == USB_CONFIG_DESCRIPTOR)
		m_sPackageId = "USB_CONFIG_DESCRIPTOR";
	else if (Translate(m_sPackageIdTag) == USB_DEFAULT_STRING_DESCRIPTOR)
		m_sPackageId = "USB_DEFAULT_STRING_DESCRIPTOR";
	else if (Translate(m_sPackageIdTag) == USB_DEVICE_DESCRIPTOR)
		m_sPackageId = "USB_DEVICE_DESCRIPTOR";
	else if (Translate(m_sPackageIdTag) == USB_ENDPOINT_DESCRIPTOR)
		m_sPackageId = "USB_ENDPOINT_DESCRIPTOR";
	else if (Translate(m_sPackageIdTag) == USB_INTERFACE_DESCRIPTOR)
		m_sPackageId = "USB_INTERFACE_DESCRIPTOR";
	else if (Translate(m_sPackageIdTag) == USB_INTERFACE_STRING_DESCRIPTOR)
		m_sPackageId = "USB_INTERFACE_STRING_DESCRIPTOR";
	else if (Translate(m_sPackageIdTag) == USB_LANGUAGE_STRING_DESCRIPTOR)
		m_sPackageId = "USB_LANGUAGE_STRING_DESCRIPTOR";
	else if (Translate(m_sPackageIdTag) == USB_MANUFACTURER_STRING_DESCRIPTOR)
		m_sPackageId = "USB_MANUFACTURER_STRING_DESCRIPTOR";
	else if (Translate(m_sPackageIdTag) == USB_PRODUCT_STRING_DESCRIPTOR)
		m_sPackageId = "USB_PRODUCT_STRING_DESCRIPTOR";
	else if (Translate(m_sPackageIdTag) == USB_SERIAL_STRING_DESCRIPTOR)
		m_sPackageId = "USB_SERIAL_STRING_DESCRIPTOR";
	else if (Translate(m_sPackageIdTag) == USBVENDORREQ)
		m_sPackageId = "USB_VENDORREQ";
	// extended reserved data packages
	else if (m_sPackageIdTag == ToUpper(sCLKE, true))
		m_sPackageId = sClockEnable;
	else if (m_sPackageIdTag == ToUpper(sDDRG, true))
		m_sPackageId = sDDRGeometry;
	else if (m_sPackageIdTag == ToUpper(sDDRT, true))
		m_sPackageId = sDDRTiming;
	else if (m_sPackageIdTag == ToUpper(sDDRC, true))
		m_sPackageId = sDDRCustom;
	else if (m_sPackageIdTag == ToUpper(sFREQ, true))
		m_sPackageId = sFrequency;
	else if (m_sPackageIdTag == ToUpper(sVOLT, true))
		m_sPackageId = sVoltages;
	else if (m_sPackageIdTag == ToUpper(sCMCC, true))
		m_sPackageId = sConfigMemoryControl;
	else if (m_sPackageIdTag == ToUpper(sTZID, true))
		m_sPackageId = sTrustZone;
	else if (m_sPackageIdTag == ToUpper(sTZON, true))
		m_sPackageId = sTrustZoneRegid;
	else if (m_sPackageIdTag == ToUpper(sOPDV, true))
		m_sPackageId = sOpDiv;
	else if (m_sPackageIdTag == ToUpper(sMODE, true))
		m_sPackageId = sOpMode;
	else if (Translate(m_sPackageIdTag) == CIDPID)
		m_sPackageId = "CIDP";
	else if (Translate(m_sPackageIdTag) == ESCAPESEQID)
		m_sPackageId = "ESCAPE_SEQUENCE";
	else if (Translate(m_sPackageIdTag) == ESCSEQID_V2)
		m_sPackageId = "ESCAPE_SEQUENCE_V2";
	else if (Translate(m_sPackageIdTag) == OEMCUSTOMID)
		m_sPackageId = "CUST";
	else if (Translate(m_sPackageIdTag) == NOMONITORID)
		m_sPackageId = "NOMONITORID";
	else if (Translate(m_sPackageIdTag) == COREID)
		m_sPackageId = "COREID";
	else if (Translate(m_sPackageIdTag) == COREID_V2)
		m_sPackageId = "COREID_V2";
	else if (Translate(m_sPackageIdTag) == BBMTID)
		m_sPackageId = "BBMTID";
	else if (Translate(m_sPackageIdTag) == IMAPID)
		m_sPackageId = "IMAPID";
	else if (Translate(m_sPackageIdTag) == ONDEMANDBOOTTID)
		m_sPackageId = "ONDEMANDBOOTTID";
	else if (Translate(m_sPackageIdTag) == DTYPID)
		m_sPackageId = "DTYPID";
	else if (m_sPackageIdTag.compare(0, 8, "0x444452") == 0) // DDRx
		m_sPackageId = HexAsciiToText(m_sPackageIdTag);
	else if (m_sPackageIdTag.compare(0, 8, "0x545A52") == 0) // TZRx
		m_sPackageId = HexAsciiToText(m_sPackageIdTag);
	else if (m_sPackageIdTag.compare(0, 8, "0x475050") == 0) // GPPx
		m_sPackageId = HexAsciiToText(m_sPackageIdTag);
	else
		// totally unknown custom user defined reserved data
		m_sPackageId = HexAsciiToText(m_sPackageIdTag);

	m_bChanged = true;
}

void CReservedPackageData::PredefinedPackageComments(int idx,
		string *psComment) {
	if (Translate(m_sPackageIdTag) == AUTOBIND) // AUTOBIND
	{
		switch (idx) {
		case 1:
			*psComment = "Autobind";
			return;
			break;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == DDRID) // DDRID
	{
		switch (idx) {
		case 1:
			*psComment = "ACCR_VALUE";
			return;
		case 2:
			*psComment = "MDCNFG_VALUE";
			return;
		case 3:
			*psComment = "DDR_HCAL_VALUE";
			return;
		case 4:
			*psComment = "MDREFR_VALUE";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == GPIOID) // GPIOID
	{
		switch (idx) {
		case 1:
			*psComment = "Number of GPIO Address & Value Pairs below";
			return;
		case 2:
			*psComment = "Address";
			return;
		case 3:
			*psComment = "Value";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == TBR_XFER) // TBR_XFER
	{
		switch (idx) {
		case 1:
			*psComment = "XFER_TABLE_LOCATION";
			return;
		case 2:
			*psComment = "NUM_PAIRS";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == RESUMEBLID) // RESUMEBLID
	{
		switch (idx) {
		case 1:
			*psComment = "DDR Resume Address";
			return;
		case 2:
			*psComment = "DDR Scratch Area Address";
			return;
		case 3:
			*psComment = "DDR Scratch Area Length";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == ROMRESUMEID) // ROMRESUMEID
	{
		switch (idx) {
		case 1:
			*psComment = "ROM Resume Address";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == UARTID) // UARTID
	{
		switch (idx) {
		case 1:
			*psComment = "Port";
			return;
		case 2:
			*psComment = "Enabled";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == USBID) // USBID
	{
		switch (idx) {
		case 1:
			*psComment = "Port";
			return;
		case 2:
			*psComment = "Enabled";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == IMAPID) // IMAPID
	{
		switch (idx) {
		case 1:
			*psComment = "Number of Image Maps below";
			return;
		case 2:
			*psComment = "IMAGE_ID";
			return;
		case 3:
			*psComment = "IMAGE_TYPE";
			return;
		case 4:
			*psComment = "FLASH_ADDRESS_LO";
			return;
		case 5:
			*psComment = "FLASH_ADDRESS_HI";
			return;
		case 6:
			*psComment = "PARTITION_NUM";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == USB_CONFIG_DESCRIPTOR) // USB_CONFIG_DESCRIPTOR
		return;
	else if (Translate(m_sPackageIdTag) == USB_DEFAULT_STRING_DESCRIPTOR) // USB_DEFAULT_STRING_DESCRIPTOR
		return;
	else if (Translate(m_sPackageIdTag) == USB_DEVICE_DESCRIPTOR) // USB_DEVICE_DESCRIPTOR
		return;
	else if (Translate(m_sPackageIdTag) == USB_ENDPOINT_DESCRIPTOR) // USB_ENDPOINT_DESCRIPTOR
		return;
	else if (Translate(m_sPackageIdTag) == USB_INTERFACE_DESCRIPTOR) // USB_INTERFACE_DESCRIPTOR
		return;
	else if (Translate(m_sPackageIdTag) == USB_INTERFACE_STRING_DESCRIPTOR) // USB_INTERFACE_STRING_DESCRIPTOR
		return;
	else if (Translate(m_sPackageIdTag) == USB_LANGUAGE_STRING_DESCRIPTOR) // USB_LANGUAGE_STRING_DESCRIPTOR
		return;
	else if (Translate(m_sPackageIdTag) == USB_MANUFACTURER_STRING_DESCRIPTOR) // USB_MANUFACTURER_STRING_DESCRIPTOR
		return;
	else if (Translate(m_sPackageIdTag) == USB_PRODUCT_STRING_DESCRIPTOR) // USB_PRODUCT_STRING_DESCRIPTOR
		return;
	else if (Translate(m_sPackageIdTag) == USB_SERIAL_STRING_DESCRIPTOR) // USB_SERIAL_STRING_DESCRIPTOR
		return;
	else if (Translate(m_sPackageIdTag) == USBVENDORREQ) // USB_VENDORREQ
	{
		switch (idx) {
		case 1:
			*psComment = "bm Request Type";
			return;
		case 2:
			*psComment = "bm Request";
			return;
		case 3:
			*psComment = "wValue";
			return;
		case 4:
			*psComment = "wIndex";
			return;
		case 5:
			*psComment = "wLength";
			return;
		case 6:
			*psComment = "wData";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == ESCAPESEQID) // escape sequence
	{
		switch (idx) {
		case 1:
			*psComment = "EscSeqTimeOutMS";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == ESCSEQID_V2) // escape seq v2
	{
		switch (idx) {
		case 1:
			*psComment = "GPIO to check";
			return;
		case 2:
			*psComment = "value of GPIO to trigger the escape sequence";
			return;
		case 3:
			*psComment = "Number of MFPR Pairs";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == COREID_V2) // core id package
	{
		switch (idx) {
		case 1:
			*psComment = "Number of Core ID V2 Pairs";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == COREID) // core id package
	{
		switch (idx) {
		case 1:
			*psComment = "Core ID";
			return;
		case 2:
			*psComment = "Address Mapping";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == BBMTID) // bbmt id package
	{
		switch (idx) {
		case 1:
			*psComment = "BBM Type";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == ONDEMANDBOOTTID) // on demand boot
	{
		switch (idx) {
		case 1:
			*psComment = "Magic Word";
			return;
		case 2:
			*psComment = "Magic Word Address";
			return;
		case 3:
			*psComment = "Magic Word Partition";
			return;
		case 4:
			*psComment = "Num ODBT Images";
			return;
		case 5:
			*psComment = "ODBT Image ID";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == DTYPID) // DDR Type
	{
		switch (idx) {
		case 1:
			*psComment = "DDR Type";
			return;
		}
		return;
	} else if (Translate(m_sPackageIdTag) == NOMONITORID) // NOMONITORID package
		return;
	else if (Translate(m_sPackageIdTag) == OEMCUSTOMID) // OEM CUSTOM package
		return;
}

