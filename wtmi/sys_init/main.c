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
#include "apctl.h"
#include "clock.h"
#include "avs.h"
#include "ddr/ddrcore.h"
#include <string.h>

#if DEBUG
#define ddr_debug printf
#else
#define ddr_debug(...)
#endif

/* remap 0xa0000000 to first 512M of CS1 */
static void cm3_win1_remap(u32 win1_base){
	writel(0xFFFFFFFE, 0xC000C710);
	writel(0x1FFF0000, 0xC000C710);
	writel(0xa0000000, 0xC000C714);
	writel(win1_base, 0xC000C718);
	writel(0x1FFF0001, 0xC000C710);
}

static u32 do_checksum32(u32 *start, u32 len)
{
	u32 sum = 0;
	u32 *startp = start;

	do {
		sum += *startp;
		startp++;
		len -= 4;
	} while (len > 0);

	return sum;
}

static int sys_check_warm_boot(void)
{
	/* warm boot bit is stored in BIT0 of 0xC001404C */
	if (readl(0xC001404C) & BIT0)
		return 1;

	return 0;
}

int sys_init_main(void)
{
	struct ddr_topology map;
	struct ddr_init_para ddr_para;
	struct ddr_init_result *result_in_dram, result_in_sram;
	u32 ddr_type = DDR3, chksum_in_dram = 0;

	printf("ENTER init_ddrgen\n");

	result_in_dram = (struct ddr_init_result *)(DDR_TUNE_RESULT_MEM_BASE);

	ddr_para.warm_boot = sys_check_warm_boot();
	if (ddr_para.warm_boot) {
		chksum_in_dram = readl(DDR_TUNE_RESULT_MEM_BASE + sizeof(struct ddr_init_result));
		if (chksum_in_dram != do_checksum32((u32 *)result_in_dram, sizeof(struct ddr_init_result)))
			printf("DDR tuning result checksum ERROR!\n");
	}
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

	/*
	* Use reserved settings if warm boot is found, otherwise, because ddr init process
	* may access DRAM memory, store the result in sram first, and copy to reserved dram
	* after init_ddr function
	*/
	if (ddr_para.warm_boot)
		init_ddr(ddr_para, result_in_dram);
	else
		init_ddr(ddr_para, &result_in_sram);

	/* Copy tuning result to reserved memory */
	if (!ddr_para.warm_boot) {
		memcpy(result_in_dram, &result_in_sram, sizeof(struct ddr_init_result));
		writel(do_checksum32((u32 *)&result_in_sram, sizeof(struct ddr_init_result)),
			DDR_TUNE_RESULT_MEM_BASE + sizeof(struct ddr_init_result));
	}

	kick_ap();

	return 0;
}

/***************************************************************************************************
  * exception_handler
  *
  * return: None
 ***************************************************************************************************/
static void exception_handler(int exception)
{
	return;
}

int main(int exception, char **dummy)
{
	u32     status;

	if (exception != 0) {
		exception_handler(exception);
		return NO_ERROR;
	}

	/* Initialization stuff */
	status = clock_init();
	if (status)
		return status;

	status = uart_init(115200);
	if (status)
		return status;
	init_printf(NULL, uart_putc);
	printf("WTMI%s\n", VERSION);

	/* Call ddrgen library */
	sys_init_main();

	/*
	* Now WTMI running image(fuse.bin or dummy.bin or freeRTOS image) and
	* system initialization image(sys_init.bin) are merged into one image -
	* "wtmi.bin", WTMI running image is aligned up to 16 bytes to be the
	* first part while sys_init.bin is the second part; wtmi.bin is loaded
	* to SRAM start address 0x1fff0000. CM3 executing start address is
	* sys_init.bin start address; after sys_init finishes the
	* initialization work, PC address will jump back to WTMI runing image
	* start address 0x1fff0000.
	*/
	__asm__ volatile("bl 0x1fff0000\n");

	return NO_ERROR;
}

