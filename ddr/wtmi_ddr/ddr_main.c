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

#include "sys.h"
#include "export.h"
#include "apctl.h"
#include "clock.h"
#include "avs.h"
#include "ddrcore/ddrcore.h"

#ifdef DEBUG
#define ddr_debug printf
#else
#define ddr_debug(..)
#endif

/* remap 0xa0000000 to first 512M of CS1 */
static void cm3_win1_remap(u32 win1_base){
	writel(0xFFFFFFFE, 0xC000C710);
	writel(0x1FFF0000, 0xC000C710);
	writel(0xa0000000, 0xC000C714);
	writel(win1_base, 0xC000C718);
	writel(0x1FFF0001, 0xC000C710);
}

int wtmi_ddr_main(void)
{
	struct ddr_topology map;
	struct ddr_init_para ddr_para;
	struct ddr_init_result tune_result;
	u32 ddr_type = DDR3;

	printf("ENTER init_ddrgen\n");

	ddr_para.log_level = LOG_LEVEL_NONE;
	ddr_para.flags = FLAG_REGS_DUMP_NONE;

	ddr_debug("DDR_TOPOLOGY is %d :", DDR_TOPOLOGY);
	/* TOPOLOGY was set in the compile parametr */
	switch(DDR_TOPOLOGY){
		case 0:
			ddr_debug("	DDR3, 1CS 512M\n");
			ddr_type = DDR3;
			map.bus_width = 16;
			map.cs_num = 1;
			map.cs[0].group_num = 0;
			map.cs[0].bank_num = 8;
			map.cs[0].capacity = 512;
			ddr_para.cs_wins[0].base = 0x60000000;
			ddr_para.cs_wins[0].size = 512;
			break;

		case 1:
			ddr_debug("	DDR4, 1CS 512M\n");
			ddr_type = DDR4;
			map.bus_width = 16;
			map.cs_num = 1;
			map.cs[0].group_num = 0;
			map.cs[0].bank_num = 8;
			map.cs[0].capacity = 512;
			ddr_para.cs_wins[0].base = 0x60000000;
			ddr_para.cs_wins[0].size = 512;
			break;

		case 2:
			ddr_debug("	DDR3, 2CS 512M + 512M\n");
			ddr_type = DDR3;
			map.bus_width = 16;
			map.cs_num = 2;
			map.cs[0].group_num = 0;
			map.cs[0].bank_num = 8;
			map.cs[0].capacity = 512;
			ddr_para.cs_wins[0].base = 0x60000000;
			ddr_para.cs_wins[0].size = 512;

			map.cs[1].group_num = 0;
			map.cs[1].bank_num = 8;
			map.cs[1].capacity = 512;
			ddr_para.cs_wins[1].base = 0x80000000;
			ddr_para.cs_wins[1].size = 512;
			break;

		case 3:
			ddr_debug("	DDR4, 2CS 2G + 2G\n");
			ddr_type = DDR4;
			map.bus_width = 16;
			map.cs_num = 2;
			map.cs[0].group_num = 0;
			map.cs[0].bank_num = 8;
			map.cs[0].capacity = 2048;
			ddr_para.cs_wins[0].base = 0x60000000;
			ddr_para.cs_wins[0].size = 1024;

			/* do remap for DDR4 CS1 */
			map.cs[1].group_num = 0;
			map.cs[1].bank_num = 8;
			map.cs[1].capacity = 2048;
			cm3_win1_remap(0x80000000);/* base of CS1, remap to 0xa0000000 */
			ddr_para.cs_wins[1].base = 0xa0000000;
			ddr_para.cs_wins[1].size = 512;
			break;

		case 4:
			ddr_debug("	DDR3, 1CS 1G\n");
			ddr_type = DDR3;
			map.bus_width = 16;
			map.cs_num = 1;
			map.cs[0].group_num = 0;
			map.cs[0].bank_num = 8;
			map.cs[0].capacity = 1024;
			ddr_para.cs_wins[0].base = 0x60000000;
			ddr_para.cs_wins[0].size = 1024;
			break;

		default:
			printf("ERROR: unsupported DDR_TOPOLOGY\n");
			break;
	}

	ddr_debug("WTMI_CLOCK=%d\n", WTMI_CLOCK);
	set_clock_preset(WTMI_CLOCK);/* WTMI_CLOCK was set in the compile parametr */

	init_avs(get_cpu_clock());

	set_ddr_type(ddr_type);

	set_ddr_topology_parameters(map);

	ddr_para.clock_init = setup_clock_tree;
	ddr_para.speed = get_ddr_clock();
	init_ddr(ddr_para, &tune_result);

	kick_ap();

	return 0;
}
