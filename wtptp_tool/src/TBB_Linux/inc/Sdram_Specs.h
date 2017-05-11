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

#ifndef __SDRAM_SPECS_H__
#define __SDRAM_SPECS_H__

// Identifiers for fields that control the timing of the following signals or operations:
//		DQM
//		DQS
//		DQ
//		DM
//		bank
//		activate
//		read
//		write
//		precharge
//		precharge-all
//		mode
//		refresh

typedef enum SDRAM_UNITS_E {
	SDRAM_U_CK,			// tCK
	SDRAM_U_PS,			// pico
	SDRAM_U_NS,			// nano
	SDRAM_U_US,			// micro
	SDRAM_U_MS			// milli
} SDRAM_UNITS_T;

// All values of interest will have a entry in the list below.
// Include categories for:
//	Geometry (bank, row, column, etc.)
//	Timing
//	Misc (burst capability, etc.)

typedef enum SDRAM_SPEC_E {
	//
	// Timing info: Timing info: Timing info: 
	//
	SDRAM_S_tCK_Hz,		// DCLK Frequency. 
	SDRAM_S_rCK_Hz,		// Refresh Clock frequency.

	// misc
	SDRAM_S_CL,			// tCK, CAS latency

	// Clock characteristics
	SDRAM_S_tCK,		// ns,  clock, rising edge to rising edge time,.
	SDRAM_S_tDQSCK,		// ps,  DQS output access time from CK or CK,#

	// Data strobe-out
	SDRAM_S_tRPRE,		// tCK, DQS read preamble time
	SDRAM_S_tRPST,		// tCK, DQS read postamble
	SDRAM_S_tLZ1,		// ps,  CK/CK# to DQS Low-Z (micron uses 1 postfix)
	SDRAM_S_tLZDQS,		// ps,  CK/CK# to DQS Low-Z (elpida uses DQS postfix)

	// Data strobe-in
	SDRAM_S_tDQSS,		// tCK, DQS rising edge to CK rising edge
	SDRAM_S_tDQSH,		// tCK, DQS input-high pulse width
	SDRAM_S_tDQSL,		// tCK, DQS input-low pulse width
	SDRAM_S_tDSS,		// tCK, DQS falling to CK rising: setup time
	SDRAM_S_tDSH,		// tCK, DQS falling from CK rising, hold time
	SDRAM_S_tWPRES,		// ps,  Write preamble setup time
	SDRAM_S_tWPRE,		// tCK, DQS write preamble
	SDRAM_S_tWPST,		// tCK, DQS write postamble
	SDRAM_S_tWCDQSDX,// tCK, WRITE command to first DQS transition (symbol invented)

	// Data-out
	SDRAM_S_tAC,		// ps,  DQ output access time from CK/CK#
	SDRAM_S_tDQSQ,// ps,  DQS-DQ skew, DQS to last DQ valid, per group, per access
	SDRAM_S_tQHS,		// ps,  DQ hold from next DQS strobe
	SDRAM_S_tQH,		// ps,  DQ-DQS hold, DQS to first DQ not valid
	SDRAM_S_tHZ,        // ps,  CK/CK# to DQ, DQS high-Z
	SDRAM_S_tLZ2,       // ps,  CK/CK# to DQ low-z (micron uses 2 postfix)
	SDRAM_S_tLZDQ,      // ps,  CK/CK# to DQ low-z (elpida uses DQ postfix)
	SDRAM_S_tDVW,// ns,  data valid output window (symbol modified: t prepended)

	// Data-in
	SDRAM_S_tDS,// ps,  DQ and DM input setup time to DQS (elpida symbol not qualified with a or b)
	SDRAM_S_tDH,// ps,  DQ and DM input hold time to DQS  (elpida symbol not qualified with a or b)
	SDRAM_S_tDSb,// ps,  DQ and DM input setup time to DQS (micron symbol qualified with a or b)
	SDRAM_S_tDHb,// ps,  DQ and DM input hold time to DQS  (micron symbol qualified with a or b)
	SDRAM_S_tDSa, // ps,  DQ and DM input setup time to DQS (micron symbol qualified with a or b)
	SDRAM_S_tDHa,// ps,  DQ and DM input hold time to DQS  (micron symbol qualified with a or b)
	SDRAM_S_tDIPW,		// tCK, DQ and DM input pulse width

	// Command and Address
	SDRAM_S_tIS,// ps,  Input setup time (elpida symbol not qualified with a or b)
	SDRAM_S_tIH,// ps,  Input hold time	 (elpida symbol not qualified with a or b)
	SDRAM_S_tISb,// ps,  Input setup time (micron symbol qualified with a or b)
	SDRAM_S_tIHb,// ps,  Input hold time	 (micron symbol qualified with a or b)
	SDRAM_S_tISa,// ps,  Input setup time (micron symbol qualified with a or b)
	SDRAM_S_tIHa,// ps,  Input hold time	 (micron symbol qualified with a or b)
	SDRAM_S_tIPW,		// tCK, Input pulse width
	SDRAM_S_tRC,		// ns,  Activate-to-Activate delay, same bank
	SDRAM_S_tRCD,		// ns,  Activate-to-Read or Write delay
	SDRAM_S_tRAS,		// ns,  Activate-to-Precharege delay
	SDRAM_S_tRP,		// ns,  Precharge period
	SDRAM_S_tRPA,		// ns,  Precharge-all period
	SDRAM_S_tRRD,		// ns,  Activate-to-activate delay, different bank
	SDRAM_S_tFAW,		// ns,  4 bank activate period
	SDRAM_S_tRTP,		// ns,  Internal read-to-precharge delay
	SDRAM_S_tCCD,		// tCK, CAS# to CAS# delay
	SDRAM_S_tWR,		// ns,  write recovery time
	SDRAM_S_tDAL,		// ns,  Write AP recovery + precharge time
	SDRAM_S_tWTR,		// ns,  Internal write-to-read delay 
	SDRAM_S_tMRD,		// tCK, LOAD MODE cycle time

	// Refresh
	SDRAM_S_tRFC,		// ns,  Refresh-to-activate or to refresh interval
	SDRAM_S_tREFI,		// us,  average periodic refresh
	SDRAM_S_tDELAY,		// ns,  CKE low to CK, CK# uncertainty

	// Self refresh
	SDRAM_S_tXSNR,		// ns,  Exit self refresh to non-read command
	SDRAM_S_tXSRD,		// tCK, Exit self refresh to read command
	SDRAM_S_tISXR,		// ps,  exit self refresh timing reference

	// Power down
	SDRAM_S_tXARD,		// tCK, exit active power down to read command
	SDRAM_S_tXARDS,	// tCK, exit active power down to read command, slow exit/low power mode (elpida)
	SDRAM_S_tXP,	// tCK, exit precharge power down to any non-read command
	SDRAM_S_tCKE,		// tCK, CKE min high/low time

	// ODT
	SDRAM_S_tANPD,		// tCK, ODT to power down entry latency
	SDRAM_S_tAXPD,		// tCK, ODT power down exit latency
	SDRAM_S_tAOND,		// tCK, ODT turn on delay
	SDRAM_S_tAOFD,		// tCK, ODT turn off delay
	SDRAM_S_tAON,		// ps,  ODT turn on 
	SDRAM_S_tAOF,		// ps,  ODT turn off
	SDRAM_S_tAONPD,		// ps,  ODT turn on (power down mode)
	SDRAM_S_tAOFPD,		// ps,  ODT turn off (power down mode)
	SDRAM_S_tMOD,		// ns,  ODT enable from MRS command
	SDRAM_S_tOIT,		// ns,  Output impedance test driver delay (elpida)

	//
	// Geometry info: Geometry info: Geometry info: 
	//
	SDRAM_S_NUMBANKS,
	SDRAM_S_NUMROWS,
	SDRAM_S_NUMCOLUMNS,
	SDRAM_S_MEGABYTES,

	//
	// Misc info: Misc info: Misc info: Misc info: 
	//
	SDRAM_S_BURSTTYPE,	// interleaved vs. sequential
	SDRAM_S_BURSTLEN,	// 
	SDRAM_S_SDRTYPE,	// SDRAM type (DDR1,DDR2,DDR3,LPDDR1,LPDDR2)
	SDRAM_S_DATAWIDTH,	// x16, x32, x62

	// New values go above here
	SDRAM_SPEC_E_MAX
} SDRAM_SPEC_T;

#endif

