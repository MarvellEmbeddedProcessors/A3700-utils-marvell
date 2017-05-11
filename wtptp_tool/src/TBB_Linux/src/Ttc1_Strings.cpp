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

#include <algorithm>

#include "Ttc1_Strings.h"
#include "Ttc1_Processor_Enums.h"	// need to change to get bootloader version

#define STRINGIZE(x) #x
#define FIELDSTRING( array, field_enum ) array[field_enum] = new string( STRINGIZE(field_enum) );

TTC1Strings::TTC1Strings(CExtendedReservedData& Ext) :
		m_Ext(Ext) {
	// make sure deallocated before allocate
	Depopulate(m_Ext.g_ClockEnableFields);
	Depopulate(m_Ext.g_FrequencyFields);
	Depopulate(m_Ext.g_DDRCustomFields);
	Depopulate(m_Ext.g_VoltagesFields);

	PopulateClockEnableFields(m_Ext.g_ClockEnableFields);
	PopulateFrequencyFields(m_Ext.g_FrequencyFields);
	PopulateDDRCustomFields(m_Ext.g_DDRCustomFields);
	PopulateVoltagesFields(m_Ext.g_VoltagesFields);
}

TTC1Strings::~TTC1Strings() {
	// do not depopulate here because the object initializes a global vector
	// Depopulate is called elsewhere before shutdown
}

// to deallocate the g_ClockEnableFields strings call this 
void TTC1Strings::Depopulate(t_stringVector& Fields) {
	if (Fields.size() > 0) {
		for_each(Fields.begin(), Fields.end(), [](string*& ps) {delete ps;});
		Fields.clear();
	}
}

// MUST call PopulateClockEnableFields to fill g_ClockEnableFields array with enum
// field strings before trying to access g_ClockEnableFields
void TTC1Strings::PopulateClockEnableFields(
		t_stringVector& g_ClockEnableFields) {
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_ClockEnableFields.resize(TTC_CLOCK_ID_E_MAX);

	// populate strings for each field
	FIELDSTRING(g_ClockEnableFields, TTC_CLOCK_ID_MCU);
	FIELDSTRING(g_ClockEnableFields, TTC_CLOCK_ID_I2C);
}

// MUST call PopulateFrequencyFields to fill g_FrequencyFields array with enum
// field strings before trying to access g_FrequencyFields
void TTC1Strings::PopulateFrequencyFields(t_stringVector& g_FrequencyFields) {
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_FrequencyFields.resize(TTC_FREQ_ID_E_MAX);

	// populate strings for each field
	FIELDSTRING(g_FrequencyFields, TTC_FREQ_ID_MCU);
}

// MUST call PopulateDDRCustomFields to fill g_DDRCustomFields array with enum
// field strings before trying to access g_DDRCustomFields
void TTC1Strings::PopulateDDRCustomFields(t_stringVector& g_DDRCustomFields) {
	//
	// TTC custom fields
	//
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max

	g_DDRCustomFields.resize(TTC_MCU_REGID_E_MAX);

	// populate strings for each field
	FIELDSTRING(g_DDRCustomFields, TTC_SDRREVREG_ID);		// revision
	FIELDSTRING(g_DDRCustomFields, TTC_SDRADCREG_ID);		// address decode
	FIELDSTRING(g_DDRCustomFields, TTC_SDRCFGREG0_ID);	// sdram config reg 0
	FIELDSTRING(g_DDRCustomFields, TTC_SDRCFGREG1_ID);	// sdram config reg 1
	FIELDSTRING(g_DDRCustomFields, TTC_SDRCFGREG2_ID);	// sdram config reg 2
	FIELDSTRING(g_DDRCustomFields, TTC_SDRTMGREG1_ID);	// sdram timing reg 1
	FIELDSTRING(g_DDRCustomFields, TTC_SDRTMGREG2_ID);	// sdram timing reg 2
	FIELDSTRING(g_DDRCustomFields, TTC_SDRTMGREG3_ID);	// sdram timing reg 3
	FIELDSTRING(g_DDRCustomFields, TTC_SDRTMGREG4_ID);	// sdram timing reg 4
	FIELDSTRING(g_DDRCustomFields, TTC_SDRTMGREG5_ID);	// sdram timing reg 5
	FIELDSTRING(g_DDRCustomFields, TTC_SDRCTLREG1_ID);	// sdram control reg 1
	FIELDSTRING(g_DDRCustomFields, TTC_SDRCTLREG2_ID);	// sdram control reg 2
	FIELDSTRING(g_DDRCustomFields, TTC_SDRCTLREG3_ID);	// sdram control reg 3
	FIELDSTRING(g_DDRCustomFields, TTC_SDRCTLREG4_ID);	// sdram control reg 4
	FIELDSTRING(g_DDRCustomFields, TTC_SDRCTLREG5_ID);	// sdram control reg 5
	FIELDSTRING(g_DDRCustomFields, TTC_SDRCTLREG14_ID);	// sdram control reg 14
	FIELDSTRING(g_DDRCustomFields, TTC_SDRPADREG_ID);// sdram pad calibration reg
	FIELDSTRING(g_DDRCustomFields, TTC_ADRMAPREG0_ID);	// address map cs0
	FIELDSTRING(g_DDRCustomFields, TTC_ADRMAPREG1_ID);	// address map cs1
	FIELDSTRING(g_DDRCustomFields, TTC_ADRMAPREG2_ID);	// address map cs2
	FIELDSTRING(g_DDRCustomFields, TTC_USRCMDREG0_ID);// user initiated command registers
	FIELDSTRING(g_DDRCustomFields, TTC_SDRSTAREG_ID);	// sdram status register
	FIELDSTRING(g_DDRCustomFields, TTC_PHYCTLREG3_ID);	// phy control reg 3
	FIELDSTRING(g_DDRCustomFields, TTC_PHYCTLREG7_ID);	// phy control reg 7
	FIELDSTRING(g_DDRCustomFields, TTC_PHYCTLREG8_ID);	// phy control reg 8
	FIELDSTRING(g_DDRCustomFields, TTC_PHYCTLREG9_ID);	// phy control reg 9
	FIELDSTRING(g_DDRCustomFields, TTC_PHYCTLREG10_ID);	// phy control reg 10
	FIELDSTRING(g_DDRCustomFields, TTC_PHYCTLREG13_ID);	// phy control reg 13
	FIELDSTRING(g_DDRCustomFields, TTC_PHYCTLREG14_ID);	// phy control reg 14
	FIELDSTRING(g_DDRCustomFields, TTC_DLLCTLREG1_ID);	// dll control reg 1
	FIELDSTRING(g_DDRCustomFields, TTC_DLLCTLREG2_ID);	// dll control reg 2
	FIELDSTRING(g_DDRCustomFields, TTC_DLLCTLREG3_ID);	// dll control reg 3
	FIELDSTRING(g_DDRCustomFields, TTC_MCBCTLREG1_ID);	// mcb control reg 1
	FIELDSTRING(g_DDRCustomFields, TTC_MCBCTLREG2_ID);	// mcb control reg 2
	FIELDSTRING(g_DDRCustomFields, TTC_MCBCTLREG3_ID);	// mcb control reg 3
	FIELDSTRING(g_DDRCustomFields, TTC_MCBCTLREG4_ID);	// mcb control reg 4
	FIELDSTRING(g_DDRCustomFields, TTC_OPDELAY_ID);		// operation delay reg
	FIELDSTRING(g_DDRCustomFields, TTC_OPREAD_ID);		// operation read reg
}

// MUST call PopulateVoltagesFields to fill g_VoltagesFields array with enum
// field strings before trying to access g_VoltagesFields
void TTC1Strings::PopulateVoltagesFields(t_stringVector& g_VoltagesFields) {
	// TBD

	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
//	g_VoltagesFields.resize(MMP2_VOLTAGES_E_MAX);

	// populate strings for each field
//	FIELDSTRING( g_VoltagesFields,  );
}

