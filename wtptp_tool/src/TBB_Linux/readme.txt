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

Requirements for building TBBV4 for Linux:
1.	The code has been built and tested on x86_64 architecture machine running Ubuntu 14.04.
2.	The compiler used for biulding this code was g++ 4.8.2
3.	The tool requires Crypto++ security library installed. It was built and tested using CryptoPP version 5.6.5. It can be downloaded at:
		https://www.cryptopp.com/release565.html

Building instructions:
1.	The tool requires the CryptoPP library to be built first. The current version can be downloaded at:
		https://www.cryptopp.com/release565.html
	To build CryptoPP, the following command can be used when in the CryptoPP source directory:
		make -f GNUmakefile
		
2.	To build the TBB tool, the following command can be used when in TBB_linux direcrory:
		make -f TBB_linux.mak LIBDIR=<path_to_Cryptopp_library>
	In case LIBDIR variable is not specified while building the TBB tool, the makefile will look in "lib/cryptpp565" for cryptopp library by default.

3.	For testing purposes, the CryptoPP sources were downloaded in the folder "<TBB_Linux directory>/lib/cryptpp565", and was then built using the command:
		make -f GNUmakefile
	The tool was then built using the command:
		make -f TBB_linux.mak