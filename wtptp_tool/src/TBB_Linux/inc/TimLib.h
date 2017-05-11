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

#ifndef __TIMLIB_H__
#define __TIMLIB_H__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

#include "Tim.h"

// forward declaration
class CImageDescription;
class CReservedPackageData;

typedef list<CImageDescription*> t_ImagesList;
typedef list<CImageDescription*>::iterator t_ImagesIter;

typedef list<string*> t_stringList;
typedef list<string*>::iterator t_stringListIter;
typedef list<string*>::reverse_iterator t_stringListReverseIter;

class CTimLib {
public:
	CTimLib(void);
	virtual ~CTimLib(void);

	bool GetDWord(ifstream& ifs, const char *szString, unsigned int* pValue);
	bool GetSValue(ifstream& ifs, const char *szString, string& sValue,
			bool bErrMsg = true);

	bool GetFieldValue(string& sLine, string& sField, DWORD& dwValue);
	bool GetFieldValue(string& sLine, string& sField, string& sValue);

	bool CheckGetSValue(ifstream& ifs, char *szString, string& sValue);

	void Endian_Convert(UINT_T x, UINT_T *y);
	UINT32_T CheckSum(unsigned char *Address, UINT_T length);

	bool CheckImageOverlap(t_ImagesList& Images);

	bool CreateOutputImageName(string& sImageFilename,
			string& sImageOutFilename);
	bool CreateOutputTimBinImageName(string& sImageFilename,
			string& sImageOutFilename);

	bool GetQWord(ifstream& ifs, char *szString, UINT64 *pValue);
	unsigned long long TranslateQWord(const char *pValue);

	string QuotedFilePath(string& sFilePath);
	string HexAsciiToText(const string& sHexAscii);
	string& TextToHexFormattedAscii(string& sText);

	ifstream& GetNextLine(ifstream& ifs, string& sLine,
			bool bIncludeWS = false);
	ifstream& GetNextLineNoWSSkipComments(ifstream& ifs, string& sLine);

	static string FormatWindowsGetLastErrorToString(DWORD dwErrorCode);

	static string& HexFormattedAscii(unsigned int iNum);
	static string& HexFormattedAscii64(UINT64 iNum);
	static unsigned int Translate(const char *pValue);
	static unsigned int Translate(string& sValue);
	static unsigned int HexFormattedTodayDate();

	// std::string helpers
	static bool IsAlpha(const string& instring);
	static bool IsNumeric(const string& instring);
	static bool IsHexNumeric(const string& instring);
	static bool IsAlphaNumeric(const string& instring);
	static string ToLower(const string& instring, bool bHexNum = false);
	static string ToUpper(const string& instring, bool bHexNum = false);
	static string TrimWS(string& sValue);
	static string TrimLeadingWS(string& sValue);
	static string TrimTrailingWS(string& sValue);
	static string TrimQuotes(string& sValue);
	static string TrimInternalWS(string& sValue);
	static string SpaceToUnderscore(const string& sValue);

	// array and list helpers
	static bool ToArrayFromList(t_stringList& List, unsigned int* pUintAry,
			unsigned int size);
	static bool ToListFromArray(unsigned int* pUintAry, t_stringList& List,
			unsigned int size);
	static bool ResizeList(t_stringList& List, unsigned int size,
			bool& bChanged);

	// file access helpers
	static void ToRelativePath(string& sFilePath, string& sRelativeTo);
	static bool SetPathAsCWD(string& sFilePath);
	static string MakePathAbsoluteOrRelativeToTIMTxtPath(string& sFilePath,
			string& sTIMTxtPath, bool bRelative);
	static void PrependPathIfNone(string& sFilePath);

};
#endif //__TIMLIB_H__
