/******************************************************************************
 *
 *  (C)Copyright 2005 - 2011 Marvell. All Rights Reserved.
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

#include <algorithm>

#include "Mmp3_Strings.h"
#include "Mmp3_Processor_Enums.h"

#define STRINGIZE(x) #x
#define FIELDSTRING( array, field_enum ) array[field_enum] = new string( STRINGIZE(field_enum) );

MMP3Strings::MMP3Strings(CExtendedReservedData& Ext) :
		m_Ext(Ext) {
	// make sure deallocated before allocate
	Depopulate(m_Ext.g_ClockEnableFields);
	Depopulate(m_Ext.g_FrequencyFields);
	Depopulate(m_Ext.g_DDRCustomFields);
	Depopulate(m_Ext.g_VoltagesFields);
	Depopulate(m_Ext.g_TrustZoneFields);
	Depopulate(m_Ext.g_TrustZoneRegidFields);

	PopulateClockEnableFields(m_Ext.g_ClockEnableFields);
	PopulateFrequencyFields(m_Ext.g_FrequencyFields);
	PopulateDDRCustomFields(m_Ext.g_DDRCustomFields);
	PopulateVoltagesFields(m_Ext.g_VoltagesFields);
	PopulateTrustZoneFields(m_Ext.g_TrustZoneFields);
	PopulateTrustZoneRegidFields(m_Ext.g_TrustZoneRegidFields);
}

MMP3Strings::~MMP3Strings() {
	// do not depopulate here because the object initializes a global vector
	// Depopulate is called elsewhere before shutdown
}

// to deallocate the g_ClockEnableFields strings call this 
void MMP3Strings::Depopulate(t_stringVector& Fields) {
	if (Fields.size() > 0) {
		for_each(Fields.begin(), Fields.end(), [](string*& ps) {delete ps;});
		Fields.clear();
	}
}

// MUST call PopulateClockEnableFields to fill g_ClockEnableFields array with enum
// field strings before trying to access g_ClockEnableFields
void MMP3Strings::PopulateClockEnableFields(
		t_stringVector& g_ClockEnableFields) {
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_ClockEnableFields.resize(MMP2_CLOCK_ID_E_MAX);

	// populate strings for each field
	FIELDSTRING(g_ClockEnableFields, MMP2_CLOCK_ID_MCU);
	FIELDSTRING(g_ClockEnableFields, MMP2_CLOCK_ID_I2C);
}

// MUST call PopulateFrequencyFields to fill g_FrequencyFields array with enum
// field strings before trying to access g_FrequencyFields
void MMP3Strings::PopulateFrequencyFields(t_stringVector& g_FrequencyFields) {
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_FrequencyFields.resize(MMP2_FREQ_ID_E_MAX);

	// populate strings for each field
	FIELDSTRING(g_FrequencyFields, MMP2_FREQ_ID_MCU);
}

// MUST call PopulateDDRCustomFields to fill g_DDRCustomFields array with enum
// field strings before trying to access g_DDRCustomFields
void MMP3Strings::PopulateDDRCustomFields(t_stringVector& g_DDRCustomFields) {
	//
	// MMP2 custom fields
	//
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max

	g_DDRCustomFields.resize(MMP2_MCU_REGID_E_MAX);

	// populate strings for each field
	// Address Register

	//Config Register IDs
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCFGREG0_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCFGREG1_ID);

	//Timing Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRTMGREG1_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRTMGREG2_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRTMGREG3_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRTMGREG4_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRTMGREG5_ID);

	//Control Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCTLREG1_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCTLREG2_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCTLREG3_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCTLREG4_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCTLREG5_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCTLREG6_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCTLREG7_ID);

	//MCB Control Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_MCBSLFSTCTRLREG1_ID);

	// Phy Control Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_PHYCTLREG3_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_PHYCTLREG7_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_PHYCTLREG8_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_PHYCTLREG9_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_PHYCTLREG11_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_PHYCTLREG13_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_PHYCTLREG14_ID);

	// PHY DLL Control Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_DLLCTLREG1_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_DLLCTLREG2_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_DLLCTLREG3_ID);

	// Memory Address Map Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_ADRMAPREG0_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_ADRMAPREG1_ID);

	//User Command Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_USRCMDREG0_ID);

	//Added Registers
	// Existing supported registers are before this line
	// to maintain backward compatibility with the tools and released bootrom versions.
	// Newly supported registers are after this line.
	// Address Register
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRADCREG_ID);

	// Config Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCFGREG2_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCFGREG3_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCFGREG4_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCFGREG5_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCFGREG6_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCFGREG7_ID);

	//Timing Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRTMGREG6_ID);

	//Control Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCTLREG8_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCTLREG11_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCTLREG13_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SDRCTLREG14_ID);

	//MCB Control Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_MCBCNTRLREG4_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_MCBSLFSTSELREG_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_MCBSLFSTCTRLREG0_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_MCBSLFSTCTRLREG2_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_MCBSLFSTCTRLREG3_ID);

	// Phy Control Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_PHYCTLREG10_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_PHYCTLREG12_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_PHYCTLREG15_ID);

	// PHY DLL Control Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_DLLCTLREG4_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_DLLCTLREG5_ID);

	// Memory Address Map Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_ADRMAPREG2_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_ADRMAPREG3_ID);

	//User Command Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_USRCMDREG1_ID);

	// Misc Registers
	FIELDSTRING(g_DDRCustomFields, MMP2_CMWRPROTREG_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_PHYCTLTEST_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_DRAMSTATUS_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_ERRORSTATUS_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_SYS_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_EXCLMONCTRL_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_MODERDDATA_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_TESTMODE0_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_TESTMODE1_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_REGTBLCTRL0_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_REGTBLDATA0_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_REGTBLDATA1_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_ERROR_IDREG_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_ERROR_ADDRREG_ID);
	FIELDSTRING(g_DDRCustomFields, MMP2_OPDELAY_ID);	// operation delay reg
	FIELDSTRING(g_DDRCustomFields, MMP2_OPREAD_ID);		// operation read reg
}

// MUST call PopulateVoltagesFields to fill g_VoltagesFields array with enum
// field strings before trying to access g_VoltagesFields
void MMP3Strings::PopulateVoltagesFields(t_stringVector& g_VoltagesFields) {
	// TBD

	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
//	g_VoltagesFields.resize(MMP2_VOLTAGES_E_MAX);

	// populate strings for each field
//	FIELDSTRING( g_VoltagesFields,  );
}

// MUST call PopulateTrustZoneRegidFields to fill g_TrustZoneRegidFields array with enum
// field strings before trying to access g_TrustZoneRegidFields
void MMP3Strings::PopulateTrustZoneRegidFields(
		t_stringVector& g_TrustZoneRegidFields) {
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_TrustZoneRegidFields.resize(MMP3_TZ_REGID_MAX);

	// populate strings for each field
	FIELDSTRING(g_TrustZoneRegidFields, MMP3_TZSELECT_ID);
	FIELDSTRING(g_TrustZoneRegidFields, MMP3_TZRANGE0_ID);
	FIELDSTRING(g_TrustZoneRegidFields, MMP3_TZRANGE1_ID);
	FIELDSTRING(g_TrustZoneRegidFields, MMP3_TZPERMISSION_ID);
}

// MUST call PopulateTrustZoneFields to fill g_TrustZoneFields array with enum
// field strings before trying to access g_TrustZoneFields
void MMP3Strings::PopulateTrustZoneFields(t_stringVector& g_TrustZoneFields) {
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_TrustZoneFields.resize(TZ_SPEC_E_MAX);

	// populate strings for each field
	FIELDSTRING(g_TrustZoneFields, TZLOCK_ID);
	FIELDSTRING(g_TrustZoneFields, TZCS0_RANGE0_ID);
	FIELDSTRING(g_TrustZoneFields, TZCS0_RANGE1_ID);
	FIELDSTRING(g_TrustZoneFields, TZCS0_RANGE2_ID);
	FIELDSTRING(g_TrustZoneFields, TZCS0_RANGE3_ID);
	FIELDSTRING(g_TrustZoneFields, TZCS0_PERMR0_ID);
	FIELDSTRING(g_TrustZoneFields, TZCS0_PERMR1_ID);
	FIELDSTRING(g_TrustZoneFields, TZCS0_PERMR2_ID);
	FIELDSTRING(g_TrustZoneFields, TZCS0_PERMR3_ID);
	FIELDSTRING(g_TrustZoneFields, TZCS0_PERMRU_ID);
}

