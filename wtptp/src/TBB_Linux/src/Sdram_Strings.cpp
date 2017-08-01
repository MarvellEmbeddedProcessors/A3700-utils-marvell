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

#include "Sdram_Strings.h"
#include <Sdram_Specs.h>			// need to change to get bootloader version
#include <Sdram_Support.h>

#define STRINGIZE(x) #x
#define FIELDSTRING( array, field_enum ) array[field_enum] = new string( STRINGIZE(field_enum) );

SdramStrings::SdramStrings(CExtendedReservedData& Ext) :
		m_Ext(Ext) {
	// make sure deallocated before allocate
	Depopulate(m_Ext.g_SdramSpecFields);
	Depopulate(m_Ext.g_ConfigMemoryControlFields);

	PopulateSdramSpecFields(m_Ext.g_SdramSpecFields);
	PopulateConfigMemoryControlFields(m_Ext.g_ConfigMemoryControlFields);
}

SdramStrings::~SdramStrings() {
	// do not depopulate here because the object initializes a global vector
	// Depopulate is called elsewhere before shutdown
}

// to deallocate the g_ClockEnableFields strings call this 
void SdramStrings::Depopulate(t_stringVector& Fields) {
	if (Fields.size() > 0) {
		for_each(Fields.begin(), Fields.end(), [](string*& ps) {delete ps;});
		Fields.clear();
	}
}

// MUST call PopulateSdramSpecFields to fill g_SdramSpecFields array with enum
// field strings before trying to access g_SdramSpecFields
void SdramStrings::PopulateSdramSpecFields(t_stringVector& g_SdramSpecFields) {
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_SdramSpecFields.resize(SDRAM_SPEC_E_MAX);

	// populate strings for each field

	//
	// Timing info: Timing info: Timing info: 
	//
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tCK_Hz);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_rCK_Hz);

	// misc
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_CL);

	// Clock characteristics
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tCK);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDQSCK);

	// Data strobe-out
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tRPRE);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tRPST);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tLZ1);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tLZDQS);

	// Data strobe-in
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDQSS);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDQSH);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDQSL);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDSS);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDSH);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tWPRES);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tWPRE);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tWPST);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tWCDQSDX);

	// Data-out
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tAC);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDQSQ);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tQHS);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tQH);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tHZ);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tLZ2);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tLZDQ);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDVW);

	// Data-in
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDS);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDH);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDSb);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDHb);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDSa);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDHa);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDIPW);

	// Command and Address
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tIS);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tIH);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tISb);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tIHb);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tISa);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tIHa);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tIPW);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tRC);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tRCD);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tRAS);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tRP);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tRPA);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tRRD);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tFAW);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tRTP);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tCCD);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tWR);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDAL);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tWTR);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tMRD);

	// Refresh
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tRFC);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tREFI);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tDELAY);

	// Self refresh
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tXSNR);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tXSRD);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tISXR);

	// Power down
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tXARD);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tXARDS);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tXP);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tCKE);

	// ODT
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tANPD);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tAXPD);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tAOND);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tAOFD);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tAON);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tAOF);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tAONPD);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tAOFPD);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tMOD);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_tOIT);

	//
	// Geometry info: Geometry info: Geometry info: 
	//
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_NUMBANKS);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_NUMROWS);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_NUMCOLUMNS);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_MEGABYTES);

	//
	// Misc info: Misc info: Misc info: Misc info: 
	//
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_BURSTTYPE);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_BURSTLEN);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_SDRTYPE);
	FIELDSTRING(g_SdramSpecFields, SDRAM_S_DATAWIDTH);

}

// MUST call PopulateClockEnableFields to fill g_ConfigMemoryControlFields array with enum
// field strings before trying to access g_ConfigMemoryControlFields
void SdramStrings::PopulateConfigMemoryControlFields(
		t_stringVector& g_ConfigMemoryControlFields) {
	// stringize the enums into field name strings
	// allocate the full array beased on the enum max
	g_ConfigMemoryControlFields.resize(CMCC_E_MAX);

	// populate strings for each field
	FIELDSTRING(g_ConfigMemoryControlFields, CMCC_CONFIG_ENA_ID);
	FIELDSTRING(g_ConfigMemoryControlFields, CMCC_MEMTEST_ENA_ID);
	FIELDSTRING(g_ConfigMemoryControlFields, CMCC_CONSUMER_ID);
}

