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

#ifndef __TIM_DESCRIPTOR_PARSER_H__
#define __TIM_DESCRIPTOR_PARSER_H__

#include "TimDescriptor.h"
#include "ReservedPackageData.h"
#include "ImageDescription.h"
#include "Partition.h"
#include "ExtendedReservedData.h"
#include "ErdBase.h"

#include "TimLib.h"
#include "TimDescriptorLine.h"
#include "CryptoPP_L_interface.h"

// forward declarations
class CCommandLineParser;
class CDownloaderInterface;

//The following general steps will be performed in order to parse the non-trusted/trusted descriptor text file:
//1. Read version information. Store in data struct.
//2. Check to see if non-trusted or trusted.
//3. Read and store image information.
//Non-trusted only:
//4. If there is any reserved data, read, verify and store reserved data.
//Trusted only:
//5. If there are any keys, read and store key information.
//6. If there is any reserved data, read, verify and store reserved data. 
//7. Read and store DSA algorithm.

class CTimDescriptorParser: public CTimLib {
public:
	CTimDescriptorParser(CCommandLineParser& CommandLineParser);
	virtual ~CTimDescriptorParser(void);

	CTimDescriptor& TimDescriptor() {
		return m_TimDescriptor;
	}
	virtual void Reset(); // use to reset parser if tim descriptor read fails

	bool GetTimDescriptorLines(CCommandLineParser& CommandLineParser);
	bool GetTimDescriptorLines(t_stringList& Lines);
	bool GetTimDescriptorLinesFromIncludeFile(string& sIncludeFilePath,
			CTimDescriptorLine*& pIncludeLine);
	virtual bool ParseDescriptor(CCommandLineParser& CommandLineParser);
	bool FillList(ifstream& ifs, t_stringList& List, unsigned int iSize);
	bool FillList(CTimDescriptorLine*& pLine, t_stringList& List,
			unsigned int iSize);
	bool FillListBlf(CTimDescriptorLine*& pLine, t_stringList& List,
			unsigned int iSize);
	string& TimDescriptorBinFilePath() {
		return m_sTimBinFilename;
	}

protected:
	ifstream& OpenTimDescriptorTextFile(ios_base::openmode mode = ios_base::in);
	void CloseTimDescriptorTextFile();
	virtual bool ParseTrustedDescriptor(CCommandLineParser& CommandLineParser,
			CTimDescriptorLine*& pLine);
	virtual bool VerifyNumberOfKeys() {
		return false;
	}
	bool ParseNonTrustedDescriptor(CCommandLineParser& CommandLineParser,
			CTimDescriptorLine*& pLine);

	// common Functions
	bool VerifySizeOfTim(CCommandLineParser& CommandLineParser);
	bool VerifyNonTrusted();
	bool VerifyNumberOfImages();
	bool VerifyDescriptorIntegrity();
	bool VerifyReservedData();
	bool ParseReservedData();
	bool ParseImageInfo(CImageDescription& ImageDesc,
			CTimDescriptorLine*& pLine, string sNum = "");
	bool ValidFieldIdx(string& sFieldName, unsigned int& idx,
			t_stringVector& Fields);

	bool ParseERDPackage(CTimDescriptorLine*& pLine, CErdBase*& pErd,
			const string& sBegin, const string& sEnd, bool bIsBlf);

	bool ParseExtendedReservedData(CCommandLineParser& CommandLineParser);
	bool SearchFieldValue(string& sLine, unsigned int& idx,
			t_stringVector& Fields, DWORD& dwValue);
	bool ParseDDRPackage(CTimDescriptorLine*& pLine, t_PairList& Fields,
			t_stringVector& g_FieldsNames, unsigned int& idx,
			CExtendedReservedData& Ext,
			void (CExtendedReservedData::*AddField)(
					std::pair<unsigned int, unsigned int>*&),
			const string& sPackage, string& sEndField, bool& bRetry);

	bool ParseImageEncryptSize(CImageDescription* pImageDesc,
			CTimDescriptorLine* pLine);

	static CTimDescriptorLine* pPrevLine;

	CTimDescriptor m_TimDescriptor;

	string m_sTimBinFilename;
	string m_sTimFilePath;
	string m_sImageFilename;
	string m_sImageOutFilename;
	ifstream m_ifsTimDescriptorTxtFile;
	t_stringList m_RawTimTextList;

public:
	CCommandLineParser& m_CommandLineParser;
};
#endif //__TIM_DESCRIPTOR_PARSER_H__
