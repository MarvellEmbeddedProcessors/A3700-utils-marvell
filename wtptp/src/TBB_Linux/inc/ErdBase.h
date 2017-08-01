/******************************************************************************
 *
 *  (C)Copyright 2009 Marvell. All Rights Reserved.
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


#ifndef ERDBASE_H
#define ERDBASE_H

#include "ReservedPackageData.h"
#include "TimLib.h"
#include "TimDescriptorLine.h"

#include <string>
#include <vector>
using namespace std;

typedef vector<string*> t_stringVector;
typedef vector<string*>::iterator t_stringVectorIter;

// forward declaration
class CTimDescriptor;

class CErdBase: public CTimLib {
public:

	enum ERD_PKG_TYPE {
		UNKNOWN_ERD = 0,
		AUTOBIND_ERD = 100,
		ESCAPESEQ_ERD,
		GPIOSET_ERD,
		GPIO_ERD,
		RESUME_ERD, // obsolete but kept to maintain pkg type id
		RESUME_DDR_ERD,
		TBRXFER_ERD,
		XFER_ERD,
		UART_ERD,
		USB_ERD,
		USBVENDORREQ_ERD,
		CONSUMER_ID_ERD,
		DDR_INITIALIZATION_ERD,
		INSTRUCTIONS_ERD,
		DDR_OPERATIONS_ERD,
		CORE_ID_ERD,
		TZ_INITIALIZATION_ERD,
		TZ_OPERATIONS_ERD,
		BBMT_ERD,
		GPP_ERD,
		GPP_OPERATIONS_ERD,
		ROM_RESUME_ERD,
		PIN_ERD,
		IMAGE_MAPS_ERD,
		IMAGE_MAP_INFO_ERD,
		ESC_SEQ_V2_ERD,
		CORE_PAIR_ERD,
		CORE_RELEASE_ERD,
		MFPR_PAIR_ERD,
		ONDEMAND_BOOT_ERD,
		FUSE_ERD,
		FUSE_SOC_CONFIG_ERD,
		FUSE_APCP_CONFIG_ERD,
		FUSE_MP_CONFIG_ERD,
		FUSE_RKEK_ERD,
		FUSE_OEM_PLATFORM_KEY_ERD,
		FUSE_JTAG_ERD,
		FUSE_UID_ERD,
		FUSE_USBID_ERD,
		FUSE_SECURITY_CONFIG_ERD,
		FUSE_LCS_STATE_ERD,
		FUSE_LOCK_BLOCK_ERD,
		FUSE_PIN_ERD,
		FUSE_SW_VERSION_ERD,
		FUSE_MARVELL_CONFIG_BITS_ERD,
		FUSE_DRM_TRANSPORT_KEY_ERD,
		FUSE_BOOLEAN_FUSE_BITS_ERD,
		FUSE_BLOCK_ERD,
		MV_IMAGE_HEADERS_PKG_ERD,
		MV_IMAGE_HEADER_ERD,
		MV_EXT_RSA_KEY_ERD,
		MV_CUST_KEY_ERD,
		DDR_TYPE_ERD,
		FLASH_GEOMETRY_REQ_ERD,
		MAX_ERD_PACKAGES = (MV_CUST_KEY_ERD - 100 + 2)
	};

	CErdBase(ERD_PKG_TYPE ErdType, int iMaxFieldNum);
	virtual ~CErdBase(void);

	// copy constructor
	CErdBase(const CErdBase& rhs);
	// assignment operator
	virtual CErdBase& operator=(const CErdBase& rhs);

	virtual void Reset();

	const ERD_PKG_TYPE ErdPkgType() {
		return m_eErdType;
	}
	static CErdBase* Create(ERD_PKG_TYPE ErdPkgType);
	static CErdBase* Create(CErdBase& src);
	static CErdBase* Create(string& sPackageName, bool bIsBlf = false);

	virtual const string& PackageName() = 0;

	virtual bool Parse(CTimDescriptor& TimDescriptor,
			CTimDescriptorLine*& pLine, bool bIsBlf);

	virtual bool ToBinary(ofstream& ofs) = 0;
	virtual unsigned int PackageSize() {
		return 0;
	}
	virtual int AddPkgStrings(CReservedPackageData* pRPD) = 0;

	unsigned int MaxFieldNum() {
		return m_iMaxFieldNum;
	}
	t_stringVector& FieldNames() {
		return m_FieldNames;
	}
	t_stringVector& FieldValues() {
		return m_FieldValues;
	}
	t_stringVector& FieldComments() {
		return m_FieldComments;
	}

	bool IsChanged() {
		return m_bChanged;
	}
	void Changed(bool bSet) {
		m_bChanged = bSet;
	}

	unsigned int m_iMaxFieldNum;
	t_stringVector m_FieldNames;
	t_stringVector m_FieldValues;
	t_stringVector m_FieldComments;

protected:
	bool m_bChanged;
	ERD_PKG_TYPE m_eErdType;

private:
	void Init();
};

typedef list<CErdBase*> t_ErdBaseVector;
typedef list<CErdBase*>::iterator t_ErdBaseVectorIter;

#endif //ERDBASE_H
