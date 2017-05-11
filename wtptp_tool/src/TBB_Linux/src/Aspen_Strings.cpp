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


#include "Aspen_Strings.h"			// pull in function prototypes
#include "Aspen_Processor_Enums.h"	// need to change to get bootloader version

#define STRINGIZE(x) #x
#define FIELDSTRING( array, field_enum ) array[field_enum] = new string( STRINGIZE(field_enum) );

AspenStrings::AspenStrings(CExtendedReservedData& Ext) :
		m_Ext(Ext) {
	// make sure deallocated before allocate
	Depopulate(m_Ext.g_ClockEnableFields);
	Depopulate(m_Ext.g_FrequencyFields);
	Depopulate(m_Ext.g_DDRCustomFields);
	Depopulate(m_Ext.g_VoltagesFields);
	Depopulate(m_Ext.g_OpDivFields);
	Depopulate(m_Ext.g_OpModeFields);

	PopulateClockEnableFields(m_Ext.g_ClockEnableFields);
	PopulateFrequencyFields(m_Ext.g_FrequencyFields);
	PopulateDDRCustomFields(m_Ext.g_DDRCustomFields);
	PopulateVoltagesFields(m_Ext.g_VoltagesFields);
	PopulateOpDivFields(m_Ext.g_OpDivFields);
	PopulateOpModeFields(m_Ext.g_OpModeFields);
}

AspenStrings::~AspenStrings() {
	// do not depopulate here because the object initializes a global vector
	// Depopulate is called elsewhere before shutdown
}

// to deallocate the g_ClockEnableFields strings call this 
void AspenStrings::Depopulate(t_stringVector& Fields) {
	for (unsigned int i = 0; i < Fields.size(); i++)
		delete Fields[i];
	Fields.clear();
}

// MUST call PopulateClockEnableFields to fill g_ClockEnableFields array with enum
// field strings before trying to access g_ClockEnableFields
void AspenStrings::PopulateClockEnableFields(
		t_stringVector& g_ClockEnableFields) {
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_ClockEnableFields.resize(ASPEN_CLOCK_ID_E_MAX);

	// populate strings for each field
	FIELDSTRING(g_ClockEnableFields, ASPEN_CLOCK_ID_MCU);
	FIELDSTRING(g_ClockEnableFields, ASPEN_CLOCK_ID_I2C);
}

// MUST call PopulateFrequencyFields to fill g_FrequencyFields array with enum
// field strings before trying to access g_FrequencyFields
void AspenStrings::PopulateFrequencyFields(t_stringVector& g_FrequencyFields) {
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_FrequencyFields.resize(ASPEN_FREQ_ID_E_MAX);

	// populate strings for each field
	FIELDSTRING(g_FrequencyFields, ASPEN_FREQ_ID_MCU);
}

// MUST call PopulateDDRCustomFields to fill g_DDRCustomFields array with enum
// field strings before trying to access g_DDRCustomFields
void AspenStrings::PopulateDDRCustomFields(t_stringVector& g_DDRCustomFields) {
	//
	// Aspen custom fields
	//
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_DDRCustomFields.resize(ASPEN_MCU_REGID_E_MAX);

	// populate strings for each field
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRREVREG_ID);		// revision
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRADCREG_ID);		// address decode
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRCFGREG0_ID);// sdram config reg 0
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRCFGREG1_ID);// sdram config reg 1
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRTMGREG1_ID);// sdram timing reg 1
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRTMGREG2_ID);// sdram timing reg 2
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRTMGREG3_ID);// sdram timing reg 3
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRTMGREG4_ID);// sdram timing reg 4
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRTMGREG5_ID);// sdram timing reg 5
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRCTLREG1_ID);// sdram control reg 1
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRCTLREG2_ID);// sdram control reg 2
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRCTLREG3_ID);// sdram control reg 3
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRCTLREG4_ID);// sdram control reg 4
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRCTLREG5_ID);// sdram control reg 5
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRCTLREG6_ID);// sdram control reg 6
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRCTLREG7_ID);// sdram control reg 7
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRCTLREG13_ID);// sdram control reg 13
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRCTLREG14_ID);// sdram control reg 14
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRERRREG_ID);	// sdram error status
	FIELDSTRING(g_DDRCustomFields, ASPEN_ADRMAPREG0_ID);	// address map cs0
	FIELDSTRING(g_DDRCustomFields, ASPEN_ADRMAPREG1_ID);	// address map cs1
	FIELDSTRING(g_DDRCustomFields, ASPEN_USRCMDREG0_ID);// user initiated command registers
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRSTAREG_ID);	// sdram status register
	FIELDSTRING(g_DDRCustomFields, ASPEN_PHYCTLREG3_ID);	// phy control reg 3
	FIELDSTRING(g_DDRCustomFields, ASPEN_PHYCTLREG7_ID);	// phy control reg 7
	FIELDSTRING(g_DDRCustomFields, ASPEN_PHYCTLREG8_ID);	// phy control reg 8
	FIELDSTRING(g_DDRCustomFields, ASPEN_PHYCTLREG9_ID);	// phy control reg 9
	FIELDSTRING(g_DDRCustomFields, ASPEN_PHYCTLREG10_ID);// phy control reg 10
	FIELDSTRING(g_DDRCustomFields, ASPEN_PHYCTLREG11_ID);// phy control reg 11
	FIELDSTRING(g_DDRCustomFields, ASPEN_PHYCTLREG12_ID);// phy control reg 12
	FIELDSTRING(g_DDRCustomFields, ASPEN_PHYCTLREG13_ID);// phy control reg 13
	FIELDSTRING(g_DDRCustomFields, ASPEN_PHYCTLREG14_ID);// phy control reg 14
	FIELDSTRING(g_DDRCustomFields, ASPEN_DLLCTLREG1_ID);	// dll control reg 1
	FIELDSTRING(g_DDRCustomFields, ASPEN_TSTMODREG0_ID);	// test mode reg 0
	FIELDSTRING(g_DDRCustomFields, ASPEN_TSTMODREG1_ID);	// test mode reg 1
	FIELDSTRING(g_DDRCustomFields, ASPEN_MCBCTLREG1_ID);	// mcb control reg 1
	FIELDSTRING(g_DDRCustomFields, ASPEN_MCBCTLREG2_ID);	// mcb control reg 2
	FIELDSTRING(g_DDRCustomFields, ASPEN_MCBCTLREG3_ID);	// mcb control reg 3
	FIELDSTRING(g_DDRCustomFields, ASPEN_MCBCTLREG4_ID);	// mcb control reg 4
	FIELDSTRING(g_DDRCustomFields, ASPEN_PRFCTLREG0_ID);// perf control reg 0
	FIELDSTRING(g_DDRCustomFields, ASPEN_PRFCTLREG1_ID);// perf control reg 1
	FIELDSTRING(g_DDRCustomFields, ASPEN_PRFSTAREG_ID);		// perf status reg
	FIELDSTRING(g_DDRCustomFields, ASPEN_PRFSELREG_ID);		// perf select reg
	FIELDSTRING(g_DDRCustomFields, ASPEN_PRFCNTREG_ID);		// perf count reg
	FIELDSTRING(g_DDRCustomFields, ASPEN_SDRTMGREG6_ID);// sdram timing reg 6
	FIELDSTRING(g_DDRCustomFields, ASPEN_PHYCTLREGTST_ID);// phy control reg test mode
	FIELDSTRING(g_DDRCustomFields, ASPEN_PRF_COUNTER_ID);// performance counter reg
	FIELDSTRING(g_DDRCustomFields, ASPEN_OPDELAY_ID);	// operation delay reg
	FIELDSTRING(g_DDRCustomFields, ASPEN_OPREAD_ID);	// operation read reg
}

// MUST call PopulateVoltagesFields to fill g_VoltagesFields array with enum
// field strings before trying to access g_VoltagesFields
void AspenStrings::PopulateVoltagesFields(t_stringVector& g_VoltagesFields) {
	// TBD

	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	//	g_VoltagesFields.resize(ASPEN_VOLTAGES_E_MAX);

	// populate strings for each field
	//	FIELDSTRING( g_VoltagesFields,  );
}

void AspenStrings::PopulateOpDivFields(t_stringVector& g_OpDivFields) {
	//
	// Aspen custom fields
	//
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_OpDivFields.resize(ASPEN_OPDIV_MAX);

	// populate strings for each field
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_CONSUMER_ID);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_PLLSEL);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_AXI_PLLSEL);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_PLL2_REFDIV);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_PLL2_FBDIV);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_PCLK_DIV);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_DCLK_DIV);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_XPCLK_DIV);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_BACLK_DIV);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_ACLK_DIV);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_ACLK2_DIV);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_PLL2_REG1);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_MV);
	FIELDSTRING(g_OpDivFields, ASPEN_OPDIV_GO);
}

void AspenStrings::PopulateOpModeFields(t_stringVector& g_OpModeFields) {
	//
	// Aspen custom fields
	//
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_OpModeFields.resize(ASPEN_OPMODE_MAX);

	// populate strings for each field
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE_CONSUMER_ID);// will be BootROM or BootLoader. If omitted, processed by both.
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE0_ID);			// 156,  PLL1 based
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE1_ID);			// 400,  PLL1 based
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE2_ID);// 624,  PLL2 based - requires higher voltage
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE3_ID);// 800,  PLL2 based - requires higher voltage
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE4_ID);// 1066, PLL2 based - requires higher voltage
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE5_ID);// 1200, PLL2 based - requires higher voltage
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE6_ID);// 1600, PLL2 based - requires higher voltage

	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE2_1_ID);		// 624, PLL1 based 
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE2_2_ID);		// 624, PLL1 based 
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE2_3_ID);		// 624, PLL1 based 

	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE3_1_ID);		// 800, PLL2 based 
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE3_2_ID);		// 800, PLL2 based 
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE3_3_ID);		// 800, PLL2 based 

	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE0_3_ID);// 156,  PLL2 based, tight voltage tolerance 
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE2_4_ID);		// 624,  PLL2 based 
	FIELDSTRING(g_OpModeFields, ASPEN_OPMODE4_1_ID);		// 1066, PLL2 based
}
