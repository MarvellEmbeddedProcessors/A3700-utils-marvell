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

#ifndef __IMAGE_BUILDER_H
#define __IMAGE_BUILDER_H

#include "Tim.h"
#include "TimLib.h"
#include "Partition.h"
#include "TrustedTimDescriptorParser.h"
#include "CommandLineParser.h"
#include "KeyGenSpec.h"
#include "CryptoPP_L_interface.h"


using namespace std;

class CImageDescription;

class CImageBuilder: public CTimLib {
public:
	CImageBuilder(CCommandLineParser& rCommandLineParser,
			CTimDescriptorParser& rTimDescriptorParser);

	virtual ~CImageBuilder(void);

	// build the non-trusted images
	virtual bool BuildDescriptorFile();

	CKeyGenSpec& KeyGenSpec() {return m_KeyGenSpec;}

protected:
	virtual bool GenerateHash(ifstream& ifsImage, CImageDescription& ImageDesc);
	virtual bool GenerateTIMHash(char *TIMBuffer, long lFileSize,
			unsigned char *pTIMHash, unsigned int TimSizeToHash);

	fstream& OpenTimBinFile(ios_base::openmode mode);
	void CloseTimBinFile();

	int GetTIMHashSize();
	bool ProcessImages(fstream& fsTimBinFile, unsigned int* pTIMHashSize);
	bool ProcessReservedData(fstream& fsTimBinFile);
	bool ProcessConcatenatedImages(fstream& fsTimBinFile);
	bool PrependID(ifstream& ifsImage, CImageDescription& ImageDesc);
	bool UpdateTIMHashInTimBin(fstream& fsTimBinFile, unsigned char *pTIMHash);
	bool LoadTimBinFileToTimBuffer(fstream& fsTimBinFile, char*& pTIMBuffer,
			long* plFileSize);
	bool PadImageToModNLength(ofstream& ofsImageOutFile,
			string& sImageOutFilename, std::streamsize iBlockSize,
			unsigned int uiModSize);

	CCommandLineParser& CommandLineParser;
	CTimDescriptorParser& TimDescriptorParser;
	fstream m_fsTimBinFile;  // in/out stream

protected:
	bool VerifyTIMHash(fstream& fsTimBinFile);
	bool ParseTimHeader(unsigned int& iPos, char* pTimBuffer);
	void ParseImages(unsigned int& iPos, char* pTimBuffer,
			CImageDescription& TimImage);

	CTimDescriptor TimDescriptor; // for parsing TIM binary

	CKeyGenSpec m_KeyGenSpec;

};
#endif //__IMAGE_BUILDER_H
