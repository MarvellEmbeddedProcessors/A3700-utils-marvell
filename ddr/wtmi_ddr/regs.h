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

#ifndef __REGS_H_
#define __REGS_H_

#define APPLICATION_CPU_REGS_BASE	(0xC0000000)
#define SECURE_CPU_REGS_BASE		(0x40000000)
#define SECURE_CPU_DDR_BASE		(0x60000000)

/* SYS */
/*
* This memory region is allocated as part of ATF PSCI domain, which
* is used for the reserved data through system suspend cycle.
* DDR tuning uses part of the memory to store tuning results
* at DDR_TUNE_RESULT_MEM_BASE
*/
#define SYS_RSRV_MAILBOX_BASE (SECURE_CPU_DDR_BASE + 0x04000400)

/* DDR */
#define DDR_TUNE_RESULT_MEM_BASE (SYS_RSRV_MAILBOX_BASE + 0x32)

#endif /* __REGS_H_ */
