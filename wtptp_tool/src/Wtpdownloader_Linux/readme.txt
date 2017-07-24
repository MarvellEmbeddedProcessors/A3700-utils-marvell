/******************************************************************************
 *
 *  (C)Copyright 2005 - 2017 Marvell. All Rights Reserved.
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
 *
 *  Marvell BSD License Option
 *
 *  If you received this File from Marvell, you may opt to use, redistribute and/or
 *  modify this File under the following licensing terms.
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *	* Redistributions of source code must retain the above copyright notice,
 *	  this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the distribution.
 *
 *	* Neither the name of Marvell nor the names of its contributors may be
 *	  used to endorse or promote products derived from this software without
 *	  specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************/

HOW-TO Build
============
o Requirements for building WTP download tool for Linux:
1. The code has been built and tested on x86_64 architecture machine running Ubuntu 14.04.
2. The compiler used for biulding this code was g++ 4.8.2.

o Building instructions:
The tool was built using the command:
   make -f makefile.mk


HOW-TO Use
============
o UART downloading instructions:
The UART downloading procedure requires BootROM enters into UART R/W mode.
(See section - 'Escape sequence instructions' for details.)

1. UART downloading for non-secure boot
   UART image components:
	* TIM_ATF.bin
	* boot-image_h.bin
	* wtmi_h.bin
   UART download command:
	sudo stty -F /dev/ttyUSB<port #> clocal
	sudo ./WtpDownload_linux -P UART -C <port #> -R 115200 -B TIM_ATF.bin -I wtmi_h.bin -I boot-image_h.bin -E

2. Trusted Images
   UART image components:
	* TIM_ATF_TRUSTED.bin
	* TIMN_ATF_TRUSTED.bin
	* boot-image_h.bin
	* wtmi_h.bin
   UART download command:
	sudo stty -F /dev/ttyUSB<port #> clocal
	sudo ./WtpDownload_linux -P UART -C <port #> -R 115200 -B TIM_ATF_TRUSTED.bin -B TIMN_ATF_TRUSTED.bin -I wtmi_h.bin -I boot-image_h.bin -E

Notes:
1. WtpDownload application requires to assign the userid to 'dailout' group access.
   (need re-login to take effect)
   sudo usermod -a -G dialout <username>
2. Please refer to ATF - doc/mvebu/build.txt for the build command of UART images.
3. The UART image components are archieved in ATF - build/a3700/[release|debug]/uart-images.tgz.

o Escape sequence instrustions:
When working with a mal-functional bootloader in flash, WTP download tool can be used to
trap BootROM into UART R/W mode via UART port.

1. Press the 'RESET' button on your system and hold on.
2. Run the following command on your host machine and releaese the 'RESET' button in the meanwhile.
   sudo stty -F /dev/ttyUSB<port #> clocal
   sudo ./WtpDownload_linux -P UART -C <port #> -R 115200 -Y


