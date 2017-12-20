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

#include "../sys.h"
#include "ddr.h"
#include "ddr_support.h"

#define DDR3_QSGATING
#define DDR4_VREF_TRAINING

enum ddr_type tc_ddr_type;
unsigned int tc_cs_num;

int set_ddr_type(enum ddr_type type){
	if(type >= DDR_TYPE_MAX)
		return -1;
	else
	{
		tc_ddr_type = type;
		return 0;
	}
}

int set_ddr_topology_parameters(struct ddr_topology top_map){
	tc_cs_num = top_map.cs_num;
	return 0;
}

int init_ddr(struct ddr_init_para init_para,
	struct ddr_init_result *result){

	unsigned int cs;
	unsigned int zp_zn_trm = 0x0;
	unsigned int orig_log_level=0;
	unsigned int dll_res;
	int ret_val=0;
#ifdef DDR3_QSGATING
	unsigned int qs_res[MAX_CS_NUM];
	unsigned int disable_rl;
#endif
#ifdef DDR4_VREF_TRAINING
	unsigned int vdac_value=0;
	unsigned int vref_value=0;
#endif

	/* Write patterns at slow speed before going into Self Refresh */
	//TODO: ddr_test_dma(0x1000) - size, base addr - Is it needed if I do self_refresh_test(0)

	/*
	 * The self-refresh test should applied to the code boot only.
	 * In a warm boot, DRAM holds the user data and Linux content
	 * of the previous boot. The test may corrupt the existing
	 * data unexpectly.
	 */
	if (!init_para.warm_boot)
		/*
		 * The CM3's address map to DRAM is supplied, which is
		 * available for memory test. But the boot image is pre-
		 * loaded and located at 0x0041.0000. Avoiding wripping
		 * out the image data, only fill the test pattern to the
		 * first 1KB memory per each chip select.
		 */
		for(cs=0; cs<tc_cs_num; cs++)
			self_refresh_test(0, init_para.cs_wins[cs].base, 1024);

	/* 1. enter self refresh */
	self_refresh_entry();

	/* 2. setup clock */
	init_para.clock_init();

	/* 3. DDRPHY sync2 and DLL reset */
	phyinit_sequence_sync2(1, 3, 2, 0);

	/* 4. update CL/CWL *nd other timing parameters as per lookup table */
	mc6_init_timing_selfrefresh(tc_ddr_type, init_para.speed);

	/* 5. enable DDRPHY termination */
	if(tc_ddr_type == DDR3)
	{
		zp_zn_trm = 0xC4477889 & 0x0FF00000;	//PHY_Control_2[0xC0001004]: copied from TIM
		set_clear_trm(1, zp_zn_trm);
	}

	/* Step 6 and 7 - Not needed as per Suresh */
	/* 6. enable DRAM termination , set DRAM DLL */
	//dll_on_dram();
	/* 7. enable DLL on for ddrphy and DLL reset */
	//dll_on_ddrphy();

	/* 8. exit self refresh */
	self_refresh_exit();

	/* 9. do MR command */
	send_mr_commands(tc_ddr_type);

	if (!init_para.warm_boot)
		for(cs=0; cs<tc_cs_num; cs++)
			self_refresh_test(1, init_para.cs_wins[cs].base, 1024);

	if(init_para.warm_boot)
        {
                printf("\nWarm boot is set");

                if(tc_ddr_type == DDR3)
                {
#ifdef DDR3_QSGATING
                        //qs gate
                        ll_write32(CH0_PHY_WL_RL_Control, result->ddr3.wl_rl_ctl);
                        ll_write32(CH0_PHY_RL_Control_CS0_B0, result->ddr3.cs0_b0);
                        ll_write32(CH0_PHY_RL_Control_CS0_B1, result->ddr3.cs0_b1);
                        ll_write32(CH0_PHY_RL_Control_CS1_B0, result->ddr3.cs1_b0);
                        ll_write32(CH0_PHY_RL_Control_CS1_B1, result->ddr3.cs1_b1);
			disable_rl = result->ddr3.wl_rl_ctl & (~0x3);
			ll_write32(CH0_PHY_WL_RL_Control, disable_rl);
#endif
                }
                else if(tc_ddr_type == DDR4)
                {
#ifdef DDR4_VREF_TRAINING
                        //vref read
                        ll_write32(PHY_Control_15, result->ddr4.vref_read);
			wait_ns(1000);

                        //vref write
                        en_dis_write_vref(1);
			ll_write32(CH0_DRAM_Config_4, result->ddr4.vref_write);
                        ll_write32(USER_COMMAND_2, 0x13004000);
                        wait_ns(1000);
                        en_dis_write_vref(0);
#endif
                }
                //dll tuning
                ll_write32(CH0_PHY_DLL_control_B0, result->dll_tune.dll_ctrl_b0);
                ll_write32(CH0_PHY_DLL_control_B1, result->dll_tune.dll_ctrl_b1);
                ll_write32(CH0_PHY_DLL_control_ADCM, result->dll_tune.dll_ctrl_adcm);

                printf("\nWarm boot completed");
                return 0;
        }
	
	/* QS Gate Training/ Read Leveling - for DDR3 only*/
	//Capture settings from TIM, update only if training passes.
	result->ddr3.wl_rl_ctl = ll_read32(CH0_PHY_WL_RL_Control);
	result->ddr3.cs0_b0 = ll_read32(CH0_PHY_RL_Control_CS0_B0);
        result->ddr3.cs0_b1 = ll_read32(CH0_PHY_RL_Control_CS0_B1);
        result->ddr3.cs1_b0 = ll_read32(CH0_PHY_RL_Control_CS1_B0);
        result->ddr3.cs1_b1 = ll_read32(CH0_PHY_RL_Control_CS1_B1);
#ifdef DDR3_QSGATING
	if(tc_ddr_type == DDR3)
	{
		for(cs=0; cs<tc_cs_num; cs++)
		{
			qs_res[cs] = qs_gating(init_para.cs_wins[cs].base, cs, init_para.log_level, result);
			if(qs_res[cs])		//if qs gate training passed, update res and dump register
			{
				printf("CH0_PHY_RL_Control_CS%d_B0[0x%08X]: 0x%08X\n", cs, (CH0_PHY_RL_Control_CS0_B0 + cs*0x24), ll_read32(CH0_PHY_RL_Control_CS0_B0 + cs*0x24));
				printf("CH0_PHY_RL_Control_CS%d_B1[0x%08X]: 0x%08X\n", cs, (CH0_PHY_RL_Control_CS0_B1 + cs*0x24), ll_read32(CH0_PHY_RL_Control_CS0_B1 + cs*0x24));
			}
			else			//qs gating fails
			{
				printf("\nCS%d: QS GATE TRAINING FAILED\n", cs);
				ret_val = -3;
			}
		}
	}
#endif

#ifdef DDR4_VREF_TRAINING
	/* Read PHY Vref Training - only for DDR4 */
	//Capture settings from TIM, update only if training passes.
	result->ddr4.vref_read = ll_read32(PHY_Control_15);
	if(tc_ddr_type == DDR4)
	{
		printf("\nVref read training\n===================");
		vdac_value = vref_read_training(tc_cs_num, init_para);
		if(vdac_value != 0)				//training passed
		{
			printf("\nFinal vdac_value 0x%08X\n", vdac_value);
			vdac_set(1, vdac_value);                //Set the tuned vdac value
                        result->ddr4.vref_read = ll_read32(PHY_Control_15);
		}
		else
		{
			printf("\nVREF READ TRAINING FAILED\n");
			ret_val = -3;
		}
	}

	/* Write DRAM Vref Training - only for DDR4 */
	//Capture settings from TIM, update only if training passes.
	result->ddr4.vref_write = ll_read32(CH0_DRAM_Config_4);
	if(tc_ddr_type == DDR4)
        {
                printf("\nVref write training\n===================");
                vref_value = vref_write_training(tc_cs_num, init_para);
                if(vref_value != 0)				//training passed
		{
                        printf("\nFinal vref_value 0x%08X\n", vref_value);
			en_dis_write_vref(1);
	                vref_set(1, vref_value);                //Set the tuned vref value
	                en_dis_write_vref(0);
			result->ddr4.vref_write = ll_read32(CH0_DRAM_Config_4);
                }
		else
                {
			printf("\nVREF WRITE TRAINING FAILED\n");
			ret_val = -3;
		}
        }
#endif
	/* 10. DLL tuning */
	//enable logs for DLL tuning
	orig_log_level = init_para.log_level;
	init_para.log_level = 1;
	result->dll_tune.dll_ctrl_b0 = ll_read32(CH0_PHY_DLL_control_B0);
	result->dll_tune.dll_ctrl_b1 = ll_read32(CH0_PHY_DLL_control_B1);
        result->dll_tune.dll_ctrl_adcm = ll_read32(CH0_PHY_DLL_control_ADCM);
	dll_res = DLL_tuning(2, tc_cs_num, init_para, 0, 0);	//use long DLL method, mpr mode disabled
	init_para.log_level = orig_log_level;
	if(dll_res)						//training passed
	{
		result->dll_tune.dll_ctrl_b0 = ll_read32(CH0_PHY_DLL_control_B0);
	        result->dll_tune.dll_ctrl_b1 = ll_read32(CH0_PHY_DLL_control_B1);
        	result->dll_tune.dll_ctrl_adcm = ll_read32(CH0_PHY_DLL_control_ADCM);
	}
	else
	{
		printf("\nDLL TUNING FAILED\n");
		ret_val = -3;
	}

	/* Test DRAM */
	//ddr_test_dma(0x1000); 	//TODO: size, base addr

	return ret_val;
}

