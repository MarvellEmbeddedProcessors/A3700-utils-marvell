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


#ifndef __EXTENDED_RESERVED_DATA_H__
#define __EXTENDED_RESERVED_DATA_H__

#include "ReservedPackageData.h"
#include "ErdBase.h"

#include "DDRInitialization.h"
#include "ConsumerID.h"
#include "TzInitialization.h"
#include "Instructions.h"
#include "DDROperations.h"
#include "TzOperations.h"
#include "GenPurposePatch.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>
#include <vector>
using namespace std;

typedef vector<string*> t_stringVector;
typedef vector<string*>::iterator t_stringVectorIter;

typedef list<pair<unsigned int, unsigned int>*> t_PairList;
typedef list<pair<unsigned int, unsigned int>*>::iterator t_PairListIter;

// Block labels to be recognized:
// Descriptor file     binary file
const string sExtendedReservedData = "Extended Reserved Data";

// Clock Enable  => (0x434c4b45) (CLKE),
const string sClockEnable = "Clock Enable";
const string sCLKE = "0x434c4b45";
// DDR Geometry  => (0x44445247) (DDRG),
const string sDDRGeometry = "DDR Geometry";
const string sDDRG = "0x44445247";
// DDR Timing    => (0x44445254) (DDRT),
const string sDDRTiming = "DDR Timing";
const string sDDRT = "0x44445254";
// DDR Custom    => (0x44445243) (DDRC),
const string sDDRCustom = "DDR Custom";
const string sDDRC = "0x44445243";
// Frequency     => (0x46524551) (FREQ),
const string sFrequency = "Frequency";
const string sFREQ = "0x46524551";
// Voltages      => (0x564f4c54) (VOLT)
const string sVoltages = "Voltages";
const string sVOLT = "0x564f4c54";
// "Configure Memory Control  => (0x434d4343) (CCMC)
const string sConfigMemoryControl = "Configure Memory Control";
const string sCMCC = "0x434d4343";
// "Trust Zone => (0x545A4944) (TZID)
const string sTrustZone = "Trust Zone";
const string sTZID = "0x545A4944";
// "Trust Zone Regid=> (0x545A4F4E) (TZON)
const string sTrustZoneRegid = "Trust Zone Regid";
const string sTZON = "0x545A4F4E";
// "OP Div ==> (0x4F504456) (OPDV)
const string sOpDiv = "OpDiv";
const string sOPDV = "0x4F504456";
// "OP Mode ==> (0x4D4F4445) (MODE)
const string sOpMode = "OpMode";
const string sMODE = "0x4D4F4445";

class CTimDescriptor;

class CExtendedReservedData: public CTimLib {
public:

	t_stringVector g_ClockEnableFields;	// CLKE ids: See *_CLOCK_ID_T for a list of ids.
	t_stringVector g_DDRCustomFields;// DDRC ids: See *_MCU_REGID_T for a list of ids.
	t_stringVector g_FrequencyFields;// FREQ ids: See *_FREQ_ID_T for a list of ids.

	t_stringVector g_SdramSpecFields;// DDRG, DDRT ids: See SDRAM_SPEC_T for a list of ids.  
	t_stringVector g_VoltagesFields;	// TBD

	t_stringVector g_ConfigMemoryControlFields; // CCMC ids
	t_stringVector g_TrustZoneFields; // TZID ids
	t_stringVector g_TrustZoneRegidFields; // TZON ids

	t_stringVector g_OpDivFields;  // Opdv ids
	t_stringVector g_OpModeFields; // Mode ids

	void DepopulateExtendedReservedDataFields();
	void Depopulate(t_stringVector& Fields);

public:
	CExtendedReservedData(const string& sProcessorType);
	virtual ~CExtendedReservedData(void);

	CExtendedReservedData(const CExtendedReservedData& rhs);
	CExtendedReservedData& operator=(const CExtendedReservedData& rhs);

	void Reset(void);

	bool IsChanged();
	void Changed(bool bSet) {
		m_bChanged = bSet;
	}

	void AddClockEnableField(pair<unsigned int, unsigned int>*& field) {
		ClockEnableFields.push_back(field);
		Changed(true);
	}

	void AddDDRGeometryField(pair<unsigned int, unsigned int>*& field) {
		DDRGeometryFields.push_back(field);
		Changed(true);
	}

	void AddDDRTimingField(pair<unsigned int, unsigned int>*& field) {
		DDRTimingFields.push_back(field);
		Changed(true);
	}

	void AddDDRCustomField(pair<unsigned int, unsigned int>*& field) {
		DDRCustomFields.push_back(field);
		Changed(true);
	}

	void AddFrequencyField(pair<unsigned int, unsigned int>*& field) {
		FrequencyFields.push_back(field);
		Changed(true);
	}

	void AddVoltagesField(pair<unsigned int, unsigned int>*& field) {
		VoltagesFields.push_back(field);
		Changed(true);
	}

	void AddConfigMemoryControlField(pair<unsigned int, unsigned int>*& field) {
		ConfigMemoryControlFields.push_back(field);
		Changed(true);
	}

	void AddTrustZoneField(pair<unsigned int, unsigned int>*& field) {
		TrustZoneFields.push_back(field);
		Changed(true);
	}

	void AddTrustZoneRegidField(pair<unsigned int, unsigned int>*& field) {
		TrustZoneRegidFields.push_back(field);
		Changed(true);
	}

	void AddOpDivField(pair<unsigned int, unsigned int>*& field) {
		OpDivFields.push_back(field);
		Changed(true);
	}

	void AddOpModeField(pair<unsigned int, unsigned int>*& field) {
		OpModeFields.push_back(field);
		Changed(true);
	}

	int Combine(t_ReservedDataList& ReservedDataList);

	int Size();

	int AddPkg(const string& sSectionTag, t_PairList& Fields,
			t_ReservedDataList& ReservedDataList);

	bool ProcessorTypeStr(const string& sProcessorType) {
		m_sProcessorType = sProcessorType;
		return ProcessorSpecificFields(sProcessorType);
	}
	string& ProcessorTypeStr() {
		return m_sProcessorType;
	}

	t_PairList ClockEnableFields;
	t_PairList DDRGeometryFields;
	t_PairList DDRTimingFields;
	t_PairList DDRCustomFields;
	t_PairList FrequencyFields;
	t_PairList VoltagesFields;
	t_PairList ConfigMemoryControlFields;
	t_PairList TrustZoneFields;
	t_PairList TrustZoneRegidFields;
	t_PairList OpDivFields;
	t_PairList OpModeFields;

	t_ErdBaseVector ErdVec;

	t_ConsumerIDVec m_Consumers;

private:
	bool ProcessorSpecificFields(const string& sProcessor);
	void ERDPackageText(CTimDescriptor& TimDescriptor, stringstream& ss,
			t_PairList& Fields, const string& sPkgName,
			t_stringVector& g_Fields, bool bIsBlf);
	void CopyFields(t_PairList& Fields, t_PairList& nc_rhsFields);
	void RemoveFieldRefs(t_PairList& Fields);

	string m_sProcessorType;
	bool m_bChanged;
};

#endif //__EXTENDED_RESERVED_DATA_H__
