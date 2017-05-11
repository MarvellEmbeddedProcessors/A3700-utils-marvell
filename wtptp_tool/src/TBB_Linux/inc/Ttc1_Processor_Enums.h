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

#ifndef __PROCESSOR_ENUMS_H__
#define __PROCESSOR_ENUMS_H__

typedef enum TTC_CLOCK_ID_E {
	TTC_CLOCK_ID_MCU, TTC_CLOCK_ID_I2C,

	TTC_CLOCK_ID_E_MAX	// must be last value in TTC_CLOCK_ID_E
} TTC_CLOCK_ID_T;

typedef enum TTC_FREQ_ID_E {
	TTC_FREQ_ID_MCU,

	TTC_FREQ_ID_E_MAX	// must be last value in TTC_FREQ_ID_E
} TTC_FREQ_ID_T;

typedef enum TTC_MCU_REGID_E {
	TTC_SDRREVREG_ID,		// revision
	TTC_SDRADCREG_ID,		// address decode
	TTC_SDRCFGREG0_ID,		// sdram config reg 0
	TTC_SDRCFGREG1_ID,		// sdram config reg 1
	TTC_SDRCFGREG2_ID,		// sdram config reg 2
	TTC_SDRTMGREG1_ID,		// sdram timing reg 1
	TTC_SDRTMGREG2_ID,		// sdram timing reg 2
	TTC_SDRTMGREG3_ID,		// sdram timing reg 3
	TTC_SDRTMGREG4_ID,		// sdram timing reg 4
	TTC_SDRTMGREG5_ID,		// sdram timing reg 5
	TTC_SDRCTLREG1_ID,		// sdram control reg 1
	TTC_SDRCTLREG2_ID,		// sdram control reg 2
	TTC_SDRCTLREG3_ID,		// sdram control reg 3
	TTC_SDRCTLREG4_ID,		// sdram control reg 4
	TTC_SDRCTLREG5_ID,		// sdram control reg 5
	TTC_SDRCTLREG14_ID,		// sdram control reg 14
	TTC_SDRPADREG_ID,		// sdram pad calibration reg
	TTC_ADRMAPREG0_ID,		// address map cs0
	TTC_ADRMAPREG1_ID,		// address map cs1
	TTC_ADRMAPREG2_ID,		// address map cs2
	TTC_USRCMDREG0_ID,		// user initiated command registers
	TTC_SDRSTAREG_ID,		// sdram status register
	TTC_PHYCTLREG3_ID,		// phy control reg 3
	TTC_PHYCTLREG7_ID,		// phy control reg 7
	TTC_PHYCTLREG8_ID,		// phy control reg 8
	TTC_PHYCTLREG9_ID,		// phy control reg 9
	TTC_PHYCTLREG10_ID,		// phy control reg 10
	TTC_PHYCTLREG13_ID,		// phy control reg 13
	TTC_PHYCTLREG14_ID,		// phy control reg 14
	TTC_DLLCTLREG1_ID,		// dll control reg 1
	TTC_DLLCTLREG2_ID,		// dll control reg 2
	TTC_DLLCTLREG3_ID,		// dll control reg 3
	TTC_MCBCTLREG1_ID,		// mcb control reg 1
	TTC_MCBCTLREG2_ID,		// mcb control reg 2
	TTC_MCBCTLREG3_ID,		// mcb control reg 3
	TTC_MCBCTLREG4_ID,		// mcb control reg 4
	TTC_OPDELAY_ID,			// operation delay reg
	TTC_OPREAD_ID,			// operation read reg

	TTC_MCU_REGID_E_MAX	// must be last value in TTC_MCU_REGID_E
} TTC_MCU_REGID_T;

#endif

