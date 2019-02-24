/*
* ***************************************************************************
* Copyright (C) 2017 Marvell International Ltd.
* ***************************************************************************
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* Neither the name of Marvell nor the names of its contributors may be used
* to endorse or promote products derived from this software without specific
* prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************
*/
/*-------------------------------------------------------------------------------
*  $Revision:
*
*  Description:	DDR initialization
*
*
*---------------------------------------------------------------------------*/

#include "../sys.h"
#include "ddr.h"
#include "ddr_support.h"

#define DLL_PHSEL_START		0x00
#define DLL_PHSEL_END		0x3F
#define DLL_PHSEL_STEP		0x1

struct dll_tuning_info {
	unsigned short left;
	unsigned short right;
	unsigned short medium;
};

static const unsigned int tune_patterns[] =
{
	0x5555aaaa,
	0xaaaa5555,
	0x0000ffff,
	0xf0f0f0f0,
	0x0f0f0f0f,
	0xf0f0f0f0,
	0x0f0f0f0f,
	0xabadbeef,
	0xbeefabad,
	0x0000000f,
	0x000000f0,
	0x00000f00,
	0x0000f000,
	0x000f0000,
	0x00f00000,
	0x0f000000,
	0xf0000000,
	0x0000000f,
	0x55555555,
	0xaaaaaaaa,
	0x00000000,
	0xffffffff,
	0x00000000,
	0xffffffff,
	0x00000000,
	0x807D5E19,
	0xA52D0F2D,
	0xB68A62AF,
	0x44F4BF0C,
	0x0
};

static int static_pattern(unsigned int wdata, unsigned int start, unsigned int end)
{
	volatile unsigned int *l_waddr;
	unsigned int l_rdata;

	for (l_waddr = (volatile unsigned int *)start; 
		 l_waddr < (volatile unsigned int *)end;
		 l_waddr++) {
		*l_waddr = wdata;// write data in
		l_rdata = *l_waddr;// read data back

		if (l_rdata != wdata)
			return 1;
	}

	return 0;
}

static int walking1_pattern(unsigned int start,unsigned int end)
{
	volatile unsigned int *waddr;// a pointer to a short( 16 bit)
	unsigned int wdata, rdata;
	int i;

	wdata = 0x8000;//original data 16 bits

	for (waddr = (volatile unsigned int *)start; waddr < (volatile unsigned int *)end; waddr++, wdata = 0x8000)
	{
		for (i = 0; i < 16; i++, wdata = wdata >> 1) {

			*waddr = wdata;// write data in
			rdata = *waddr;// read data back

			if ( wdata != rdata)
				return 1;
		}
	}

	return 0;
}

static int DDR_WR_Test(unsigned int start, unsigned int size)
{
	unsigned int end;
	int i;

	end = start + size;

	for (i = 0; i < sizeof(tune_patterns) / sizeof(tune_patterns[0]); i++) {
		if (static_pattern(tune_patterns[i], start, end))
			return 1;
	}

	if (walking1_pattern(start, end))
		return 1;

	return 0;	//pass
}

void reset_dll_phy(void)
{
	// reset PHY DLL. Dll_reset_timer ([31:24] of PHY Control Register 8, Offset 0x41C/0xC1C) is set to 0x10, reset DLL for 128*32=4096 MCLK cycles.
	// updates DLL master. Block read/MMR for 4096 MCLK cycles to guarantee DLL lock. Either wait 4096 MCLK (memPll/4) cycles, or check DLL lock status
	ll_write32(PHY_CONTROL_9, 0x20000000);	//DLL reset
	//TODO: optimize the delays for specific CPU frequency
	wait_ns(640);							//delay(512nCK);Assuming 800MHz CPU frequency
	// update Dll delay_l. When Dll_update_timer ([20:16] of PHY Control Register 8, Offset 0x41C/0xC1C) is 0x11, assert DLL_UPDATE_EN pin for 0x11*16 = 272 MCLK cycles.
	// copy DLL master to DLL slave. Slave controls the actual delay_l. Both DLL bypass and DLL needs 'update'.
	ll_write32(PHY_CONTROL_9, 0x40000000);	//Update  DLL
	//TODO: optimize the delays for specific CPU frequency
	wait_ns(640);                   		//delay(512nCK);Assuming 800MHz CPU frequency
	ll_write32(PHY_CONTROL_9, 0x0);			//Finish DLL update
	//TODO: optimize the delays for specific CPU frequency
	wait_ns(640);                   		//delay(512nCK);Assuming 800MHz CPU frequency
}

static void set_DLL(unsigned short dll_phsel, unsigned short dll_phsel1)
{
	replace_val((PHY_DLL_CONTROL_BASE + 0), dll_phsel, 16, 0x003F0000);
	replace_val((PHY_DLL_CONTROL_BASE + 0), dll_phsel1, 24, 0x3F000000);

	replace_val((PHY_DLL_CONTROL_BASE + 4), dll_phsel, 16, 0x003F0000);
	replace_val((PHY_DLL_CONTROL_BASE + 4), dll_phsel1, 24, 0x3F000000);

	replace_val((PHY_DLL_CONTROL_BASE + 8), dll_phsel, 16, 0x003F0000);
	replace_val((PHY_DLL_CONTROL_BASE + 8), dll_phsel1, 24, 0x3F000000);

	replace_val((PHY_DLL_CONTROL_BASE + 12), dll_phsel, 16, 0x003F0000);
	replace_val((PHY_DLL_CONTROL_BASE + 12), dll_phsel1, 24, 0x3F000000);

	reset_dll_phy();
}

static void set_dll_phsel(unsigned short offset, unsigned short bit_offset, unsigned int value)
{
	replace_val((PHY_DLL_CONTROL_BASE + offset), value, bit_offset, (0x3F << bit_offset));
	reset_dll_phy();
}

unsigned short DLL_fine_tune(unsigned int ratio, struct ddr_init_para init_para, unsigned int num_of_cs, unsigned short medium[])
{
	unsigned short offset[] = {0, 0, 4, 4, 36, 36};  //GT
	unsigned short bit_offset[] = {16, 24, 16, 24, 16, 24}; //GT
	unsigned short left[MAX_CS_NUM], right[MAX_CS_NUM], m[MAX_CS_NUM], i;
	unsigned short l,r,med;
	unsigned int regval;
	unsigned int cs;

	ll_write32(PHY_CONTROL_9, 0x0);

	//Automatically update PHY DLL with interval time set in Dll_auto_update_interval ([15:8] of PHY Control Register 13, Offset 0x248)
	regval = ll_read32(PHY_CONTROL_8);

	//turn off Dll_auto_manual_update & Dll_auto_update_en
	// DLL_auto_update_en has a known bug. Don't use.
	regval &= ~0xC;
	// change Dll_reset_timer to 128*32 cycles
	regval |= 0x80000000;
	ll_write32(PHY_CONTROL_8, regval);  // Write R41C

	LogMsg(LOG_LEVEL_DEBUG, FLAG_REGS_DLL_TUNE, "\n\nPerform fine DLL tuning:");
	for(i=0; i<sizeof(offset)/sizeof(offset[0]); ++i)
	{
		LogMsg(LOG_LEVEL_DEBUG, FLAG_REGS_DLL_TUNE, "\n\n\tDLL 0x%8x[%2d:%2d]: [l, r, m]", PHY_DLL_CONTROL_BASE+offset[i], bit_offset[i]+5, bit_offset[i]);
		for(cs=0; cs<num_of_cs; cs++)
		{
			left[cs] = medium[cs];
			right[cs] = medium[cs];
			do{
				if(left[cs]>DLL_PHSEL_START)
					left[cs]-=DLL_PHSEL_STEP;
				set_dll_phsel(offset[i], bit_offset[i], left[cs]);
			}while(!DDR_WR_Test(init_para.cs_wins[cs].base, 100*2) && left[cs]>DLL_PHSEL_START);

			do{
				if(right[cs]<DLL_PHSEL_END)
					right[cs]+=DLL_PHSEL_STEP;
				set_dll_phsel(offset[i],  bit_offset[i], right[cs]);
			}while(!DDR_WR_Test(init_para.cs_wins[cs].base, 100*2) && right[cs]<DLL_PHSEL_END);

			m[cs] = left[cs] + (right[cs] -left[cs])/ratio;
			LogMsg(LOG_LEVEL_DEBUG, FLAG_REGS_DLL_TUNE, "\n\t\tCS%d: [%x, %x, %x]",cs, left[cs], right[cs],m[cs]);
			set_dll_phsel(offset[i], bit_offset[i], m[cs]);
		}

		//pick the window common to all CS, if none exists default to CS0
		LogMsg(LOG_LEVEL_DEBUG, FLAG_REGS_DLL_TUNE, "\n\tPick the window common to all CS, if none exists default to CS0");
		l=left[0]; r=right[0];
		for(cs=1;cs<num_of_cs; cs++)
		{
			if( (left[cs] > l) && (left[cs] < r) )
				l = left[cs];
			if( (right[cs] < r) && (right[cs] > l) )
				r = right[cs];
		}

		med = l + (r - l)/ratio;
		LogMsg(LOG_LEVEL_DEBUG, FLAG_REGS_DLL_TUNE, "\n\tDLL 0x%8x[%2d:%2d]: [%x,%x,%x]",
				PHY_DLL_CONTROL_BASE+offset[i], bit_offset[i]+5, bit_offset[i], l, r, med);
		set_dll_phsel(offset[i], bit_offset[i], med);
	}
	return 1;	//DLL passed
}

static unsigned int mpr_read_Test(unsigned int start, unsigned int ddr_size)
{
        volatile unsigned int *l_waddr;
        unsigned int l_rdata, l_pattern_ddr4;

        l_pattern_ddr4 = (unsigned int)0xFFFF0000;

        for (l_waddr = (volatile unsigned int*)start; l_waddr < (volatile unsigned int*)(start + ddr_size); l_waddr++)
        {
                l_rdata = *l_waddr;

                //printf("\nRead data 0x%08X : 0x%08x Pattern: 0x%08x", l_waddr, l_rdata, l_pattern_ddr4);
                if (l_rdata != l_pattern_ddr4)
                {
                        return 1;               //1 => fail
                }
        }
        return 0;
}


unsigned int short_DLL_tune(unsigned int ratio, unsigned int cs,
			    unsigned int cs_base, unsigned int mpr_en,
			    struct dll_tuning_info *ret)
{
        unsigned short left, right, i;
        unsigned short medium;
        unsigned int regval, res;

	ll_write32(PHY_CONTROL_9, 0x0);

	//Automatically update PHY DLL with interval time set in Dll_auto_update_interval ([15:8] of PHY Control Register 13, Offset 0x248)
        regval = ll_read32(PHY_CONTROL_8);

        //turn off Dll_auto_manual_update & Dll_auto_update_en
        // DLL_auto_update_en has a known bug. Don't use.
        regval &= ~0xC;
        // change Dll_reset_timer to 128*32 cycles
        regval |= 0x80000000;
        ll_write32(PHY_CONTROL_8, regval);  // Write R41C

	LogMsg(LOG_LEVEL_DEBUG, FLAG_REGS_DLL_TUNE, "\n\tCS%d: Increment dll_phsel0 and dll_phsel1 equally by 1 and find the passing window", cs);
	//enable mpr mode
	if(mpr_en)
	{
		ll_write32(CH0_DRAM_Config_3, (ll_read32(CH0_DRAM_Config_3) | 0x00000040));
		ll_write32(USER_COMMAND_2, 0x13000800);
	}
        left = DLL_PHSEL_END;
        right = DLL_PHSEL_START;
        for(i=DLL_PHSEL_START; i<=DLL_PHSEL_END; ++i){
                set_DLL(i, i);
                wait_ns(100);
		if(mpr_en)
			res = mpr_read_Test(cs_base, 100*2);
		else
			res = DDR_WR_Test(cs_base, 32);

		if(!res) {	//PASS
			if( i<left) left = i;
			if( i>right) right = i;
			LogMsg(LOG_LEVEL_DEBUG, FLAG_REGS_DLL_TUNE, "\n\t\tdll_phsel_0 = dll_phsel_1 = 0x%02X left = 0x%02X, right = 0x%02X", i, left, right);
		}
	}
	if(left>right){
                LogMsg(LOG_LEVEL_DEBUG, FLAG_REGS_DLL_TUNE, "\n\t\tNo passing window");
                return 0;
        }else{
                medium = left + ((right-left)/ratio);
                set_DLL(medium, medium);
		LogMsg(LOG_LEVEL_DEBUG, FLAG_REGS_DLL_TUNE, "\n\t\tCS%d: Passing window: 0x%02X-0x%02X \t\tMedium = 0x%02X", cs, left, right, medium);
        }
	ret->left = left;
	ret->right = right;
	ret->medium = medium;
	//disable mpr mode
	if(mpr_en)
	{
		ll_write32(CH0_DRAM_Config_3, (ll_read32(CH0_DRAM_Config_3) & (~0x00000040)));
		ll_write32(USER_COMMAND_2, 0x13000800);
	}
	return 1;
}

/*function will return the dll range for that verf.
 *range <=0 means range is zero length
 */
int get_dll_range(unsigned int num_of_cs, struct ddr_init_para init_para)
{
	unsigned int cs;
	/*common init with maximum range*/
	struct dll_tuning_info common_info = {.left = DLL_PHSEL_START,
					      .right = DLL_PHSEL_END,
					      .medium = 0};
	struct dll_tuning_info dll_info;

	LogMsg(LOG_LEVEL_DEBUG, FLAG_REGS_DLL_TUNE,
	       "\nPerform coarse DLL tuning:");
	/*select the intersection of both CS*/
	for (cs = 0; cs < num_of_cs; cs++) {
		short_DLL_tune(2, cs, init_para.cs_wins[cs].base,
			       0, &dll_info);
		if (dll_info.left > common_info.left)
			common_info.left = dll_info.left;
		if (dll_info.right < common_info.right)
			common_info.right = dll_info.right;
	}
	return common_info.right - common_info.left;
}

unsigned int DLL_tuning(unsigned int ratio, unsigned int num_of_cs, struct ddr_init_para init_para, unsigned int short_DLL, unsigned int mpr_mode)
{
	unsigned int cs = 0, res = 1;
	unsigned short optimal[MAX_CS_NUM], medium[MAX_CS_NUM];
	struct dll_tuning_info dll_info;

	LogMsg(LOG_LEVEL_DEBUG, FLAG_REGS_DLL_TUNE, "\nPerform coarse DLL tuning:");
	for (cs=0; cs<num_of_cs; cs++)
	{
		optimal[cs] = short_DLL_tune(ratio, cs,
					     init_para.cs_wins[cs].base,
					     mpr_mode, &dll_info);
		medium[cs] = dll_info.medium;
		res &= optimal[cs];
	}

	if(short_DLL)
		return res;		//DLL tuning P/F for vref trainings
	else
	{
		if(res)
			DLL_fine_tune(ratio, init_para, num_of_cs, medium);
		else
			return 0;	//DLL tuning FAIL
	}

	return 1;			//DLL tuning PASS
}

