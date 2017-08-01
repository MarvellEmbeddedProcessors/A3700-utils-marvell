/******************************************************************************
 *
 *  (C)Copyright 2005 - 2013 Marvell. All Rights Reserved.
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

typedef enum MMP2_CLOCK_ID_E {
	MMP2_CLOCK_ID_MCU, MMP2_CLOCK_ID_I2C,

	MMP2_CLOCK_ID_E_MAX
} MMP2_CLOCK_ID_T;

typedef enum MMP2_FREQ_ID_E {
	MMP2_FREQ_ID_MCU,

	MMP2_FREQ_ID_E_MAX
} MMP2_FREQ_ID_T;

typedef enum MMP2_MCU_REGID_E {
	//Register IDs
	// Config Registers
	MMP2_SDRCFGREG0_ID,		//0x020
	MMP2_SDRCFGREG1_ID,		//0x030
	//Timing Registers
	MMP2_SDRTMGREG1_ID,		//0x050
	MMP2_SDRTMGREG2_ID,		//0x060
	MMP2_SDRTMGREG3_ID,		//0x190
	MMP2_SDRTMGREG4_ID,		//0x1C0
	MMP2_SDRTMGREG5_ID,		//0x650
	//Control Registers
	MMP2_SDRCTLREG1_ID,		//0x080
	MMP2_SDRCTLREG2_ID,		//0x090
	MMP2_SDRCTLREG3_ID,		//0x0f0
	MMP2_SDRCTLREG4_ID,		//0x1a0
	MMP2_SDRCTLREG5_ID,		//0x280
	MMP2_SDRCTLREG6_ID,		//0x760
	MMP2_SDRCTLREG7_ID,		//0x770
	//MCB Control Registers
	//MMP2_MCBCTLREG1_ID,	
	MMP2_MCBSLFSTCTRLREG1_ID,		//0x590
	// Phy Control Registers
	MMP2_PHYCTLREG3_ID,		//0x140
	MMP2_PHYCTLREG7_ID,		//0x1d0
	MMP2_PHYCTLREG8_ID,		//0x1e0
	MMP2_PHYCTLREG9_ID,		//0x1f0
	MMP2_PHYCTLREG11_ID,    //0x210
	MMP2_PHYCTLREG13_ID,	//0x230
	MMP2_PHYCTLREG14_ID,	//0x240
	// PHY DLL Control Registers
	MMP2_DLLCTLREG1_ID,		//0xe10
	MMP2_DLLCTLREG2_ID,		//0xe20
	MMP2_DLLCTLREG3_ID,		//0xe30
	// Memory Address Map Registers
	MMP2_ADRMAPREG0_ID,		//0x100
	MMP2_ADRMAPREG1_ID,		//0x110
	//User Command Registers
	MMP2_USRCMDREG0_ID,		//0x120
	//Added Registers
	// Existing supported registers are before this line
	// to maintain backward compatibility with the tools and released bootrom versions.
	// Newly supported registers are after this line.
	// Address Register
	MMP2_SDRADCREG_ID,		//0x010
	// Config Registers
	MMP2_SDRCFGREG2_ID,		//0x040 - not supported
	MMP2_SDRCFGREG3_ID,		//0xB300
	MMP2_SDRCFGREG4_ID,     //0xB40
	MMP2_SDRCFGREG5_ID,		//0xB50
	MMP2_SDRCFGREG6_ID,		//0xB60 - not supported
	MMP2_SDRCFGREG7_ID,		//0xB70 - not supported
	//Timing Registers
	MMP2_SDRTMGREG6_ID,		//0x660
	//Control Registers
	MMP2_SDRCTLREG8_ID,		//0x780
	MMP2_SDRCTLREG11_ID,	//0x7B0
	MMP2_SDRCTLREG13_ID,	//0x7D0
	MMP2_SDRCTLREG14_ID,	//0x7E0
	//MCB Control Registers
	MMP2_MCBCNTRLREG4_ID,	//0x540
	MMP2_MCBSLFSTSELREG_ID,	//0x570
	//MMP2_MCBCTLREG0_ID,	
	MMP2_MCBSLFSTCTRLREG0_ID,	//0x580
	//MMP2_MCBCTLREG2_ID,
	MMP2_MCBSLFSTCTRLREG2_ID,	//0x5A0
	//MMP2_MCBCTLREG3_ID,
	MMP2_MCBSLFSTCTRLREG3_ID,	//0x5B0
	// Phy Control Registers
	MMP2_PHYCTLREG10_ID,	//0x200
	MMP2_PHYCTLREG12_ID,	//0x220
	MMP2_PHYCTLREG15_ID,	//0x250 - not supported
	// PHY DLL Control Registers
	MMP2_DLLCTLREG4_ID,		//0xe40 - not supported
	MMP2_DLLCTLREG5_ID,		//0xe50 - not supported
	// Memory Address Map Registers
	MMP2_ADRMAPREG2_ID,		//0x130 - not supported
	MMP2_ADRMAPREG3_ID,		//0xA30 - not supported
	//User Command Registers
	MMP2_USRCMDREG1_ID,		//0x410
	// Misc Registers
	MMP2_CMWRPROTREG_ID,	//0x180
	MMP2_PHYCTLTEST_ID,		//0xe80
	MMP2_DRAMSTATUS_ID,		//0x1b0
	MMP2_ERRORSTATUS_ID,	//0x0d0
	MMP2_SYS_ID,			//0x2c0
	MMP2_EXCLMONCTRL_ID,	//0x380
	MMP2_MODERDDATA_ID,		//0x440
	MMP2_TESTMODE0_ID,		//0x4C0 - not supported
	MMP2_TESTMODE1_ID,		//0x4D0
	MMP2_REGTBLCTRL0_ID,	//0xc00
	MMP2_REGTBLDATA0_ID,	//0xc20
	MMP2_REGTBLDATA1_ID,	//0xc30
	MMP2_ERROR_IDREG_ID,	//0x490
	MMP2_ERROR_ADDRREG_ID,	//0x4A0
	MMP2_OPDELAY_ID,		// operation delay reg
	MMP2_OPREAD_ID,			// operation read reg

	MMP2_MCU_REGID_E_MAX
} MMP2_MCU_REGID_T;

typedef enum TRUSTZONE_REGID_E {
	// Register IDs
	MMP2_TZSELECT_ID,
	MMP2_TZRANGE0_ID,
	MMP2_TZRANGE1_ID,
	MMP2_TZPERMISSION_ID,

	MMP2_TZ_REGID_MAX
} TRUSTZONE_REGID_T;

typedef enum TRUSTZONE_SPEC_E {
	// Field IDs
	TZLOCK_ID,        // Lock Trust Zones
	TZCS0_RANGE0_ID,  // Size of Region 0 (64K chunks)
	TZCS0_RANGE1_ID,  // Size of Region 1 (64K chunks)
	TZCS0_RANGE2_ID,  // Size of Region 2 (64K chunks)
	TZCS0_RANGE3_ID,  // Size of Region 3 (64K chunks)
	TZCS0_PERMR0_ID,  // Access permission for Region 0 
	TZCS0_PERMR1_ID,  // Access permission for Region 1 
	TZCS0_PERMR2_ID,  // Access permission for Region 2
	TZCS0_PERMR3_ID,  // Access permission for Region 3 
	TZCS0_PERMRU_ID,  // Access permission for Region U

	TZ_SPEC_E_MAX
} TRUSTZONE_SPEC_T;
#endif

