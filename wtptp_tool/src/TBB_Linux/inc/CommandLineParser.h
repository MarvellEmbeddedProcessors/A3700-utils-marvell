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

#ifndef __COMMAND_LINE_PARSER_H__
#define __COMMAND_LINE_PARSER_H__

#include "Typedef.h"
#include "TimDescriptor.h"

#include <string>
using namespace std;

#define VERBOSE 'v'
#define HELP '?'
#define DESCRIPTOR_TXT 'r'
#define TIMIN 'i'
#define TIMOUT 'o'
#define HASH 'h'
#define DS 's'
#define OPTION 'm'
#define TIMVERIFY 'V'
#define CONCATENATE 'C'
#define PARTITION 'P' 
#define PROCESSOR_TYPE 'T'
#define PADDED_TIM_IMAGE 'O'
#define GENERATE_KEY 'G'
#define USE_KEY 'B'
#define INJECT_PIN 'N'
#define TIM_TXT_RELATIVE 'U'
#define KEYBYTESWAP 'a'
#define OPT_HASH_PADDING 'D'
#define PAD_IMAGE_MOD_N_LENGTH 'L'

#define MIN_ARGS 2
#define MAX_PATH 260

class CCommandLineParser {
public:

	CCommandLineParser(void);
	~CCommandLineParser(void);

	bool ParseCommandLine(int argc, char *argv[]);

	bool CheckCommandLineOptions();
	void PrintUsage();
	void Reset();

	bool bVerbose;
	bool bConcatenate;
	int iOption;
	bool bIsTimVerify;
	bool bOneNANDPadding;

	bool bIsNonTrusted;
	bool bIsTimTxtFile;
	bool bIsBlfTxtFile;
	bool bIsHashFile;
	bool bIsDsFile;
	bool bIsTimInFile;
	bool bIsTimOutFile;
	bool bIsPartitionDataFile;
	bool bGenerateKeySpecFile;
	bool bUseKeyFile;
	bool bPinFile;
	bool bTimTxtRelative;
	bool bKeyByteSwap;
	bool bOtpHashPadding;
	bool bImageLengthPadding;

	string TimTxtFilePath; // TIM Descriptor text file
	string HashFilePath;
	string DsFilePath;
	string TimInFilePath;
	string TimOutFilePath;
	string PartitionFilePath;
	string EncryptionKeyFilePath;
	string FromPublicKeyFilePath;
	string KeyGenSpecFilePath;
	string KeyFilePath;
	string InjectKeyFilePath;
	string MatchKeyHashFilePath;
	string PinFilePath;

	unsigned int uiImageModSize;

	eProcessorType iProcessorType;
	unsigned int uiPaddedSize;

};
#endif //__COMMAND_LINE_PARSER_H__
