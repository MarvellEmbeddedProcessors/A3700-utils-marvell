/******************************************************************************
 *
 *  (C)Copyright 2006 - 2012 Marvell. All Rights Reserved.
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
#include <fstream>
#include <time.h>
#include "ProtocolManager.h"
#include "Misc.h"
#include "WtpException.h"
class CWtpImage;
class CCommandlineParser;
class CUARTPort;

using namespace std;

const string g_Version = "3.3.11.15";
const string g_Date    = "04.May.2017";


class CWtpDownloadApp
{
private:
	PROTOCOL_VERSION TARGET_PROTOCOL_VERSION;
	typedef list<CWtpImage*> t_WtpImageList;
	typedef list<CWtpImage*>::iterator t_WtpImageIter;

public:

	enum EPlatformType {PXAMIN_PT = -1,
						PXA168 = 0,           // ASPEN A0
						PXA30x = 1,           // MH-L
						PXA31x = 2,           // MH-LV
						PXA32x = 3,           // MH-P
						ARMADA610 = 4,        // MMP2
						PXA91x = 5,           // TTC
						PXA92x = 6,           // TAVOR TD
						PXA93x = 7,           // TAVOR P/PV
						PXA94x = 8,           // TAVOR PV (RIM ONLY)
						PXA95x = 9,           // TAVOR PV2
						ARMADA16x = 10,       // Aspen family
						PXA955 = 12,          // TAVOR MG1
						PXA968 = 13,          // TAVOR MG2
						PXA1701 = 14,         // CP888/ESHEL
						PXA978 = 15,          // NEVO
						PXA2128 = 16,         // MMP3 Family
						ARMADA622 = 18,       // MMP2X
						PXA1202 = 19,         // WUKONG
						PXA1801 = 20,         // ESHEL2
                        P88PA62_70 = 21,      // GRANITE2          *
						PXA988 = 22,          // EMEI
						PXA1920 = 23,         // HELN LTE
						PXA2101 = 24,         // BIRCH-40
						PXA192 = 25,          // BIRCH-28
						PXA1928 = 26,         // EDEN
						PXA1986 = 27,         // ADIR
						PXA1802 = 28,         // NEZHA
						PXA986 = 29,          // KUNLUN
						PXA1206 = 30,         // WUJIN
						PXA888 = 31,          // WUTAI
						PXA1088 = 32,         // HELN
						PXA1812 = 33,         // ESHEL3
						PXA1822 = 34,         // NEZHA2
						PXA1U88 = 35,		  // HELAN2
                        PXA1908 = 36,         // ULC1
                        PXA1826 = 37,         // NEZHA3
                        P88PA62_20 = 38,      // GEMSTONE2          *
                        P88PA62_10 = 39,      // BASALT2       *
                        PXA1956 = 40,		  // HELAN4
						VEGA	= 41,
						BOWIE	= 42,
						ULC2	= 43,
						PXAMAX_PT
    };

	bool bDownload;
	bool bJtagMedia;
	bool bPlainTextWtpCR;
	bool bVerbose;
	bool bTimFile;
	bool bImageFile;
	bool bJtagFile;
	bool bMessageMode;
	bool bPartitionFile;
	bool bUSBInterface;
	bool bUSBPacketSize;
	bool bUARTCommPort;
	bool bUARTBaudRate;
	bool bLogFile;
	bool bPlatform;
	bool bFastDownload;
	bool bUploadSpecFile;
	bool bGetPC;
    bool bEnumerateDelay;
    bool bEnumerateArriveWait;
    bool bUSBReadDelay;
    bool bHelp;
	bool bForceUARTImageLoad;
	bool bForceBootROMConsoleMode;
	bool bSendFFStream;
	bool bSendFFStream_DLoad;

	int inumImagesinCLtoDL;
	int iDownloadBaudRate;
	int iUSBPacketSize;
	int iPortType;
	int iCommPort;
    int iEnumerateDelay;
    int iEnumerateArriveWait;
    int iUSBReadDelay;

    bool bEnablerExePath;
	string sEnableExePath;

	string exePath;

	t_WtpImageList lImageFiles;
	CCommandlineParser* Parser;
	CProtocolManager ProtocolManager;


	EPlatformType ePlatformType;
	eMessageMode eCurrentMessageMode;
	string sLogFilename;
	string sUploadSpecFileName;
	string sUSBInterface;//used in multi-target download
	string jtagArgsString;
	ofstream hLog;

	void ListenForMessages();
    void RequestImageType(CWtpImage*& Image) throw (CWtpException);
    void DownloadImages(int numImages) throw (CWtpException);

	void Upload() throw (CWtpException);
	void GetPC() throw (CWtpException);

	string ConvertSecondsToTimeString(double seconds);

    CWtpImage* GetImageFromImageList(unsigned int ImageType);

	time_t downloadStartTime;
	time_t downloadEndTime;

	time_t imgDwnldStartTime;
	time_t imgDwnldEndTime;

	CWtpDownloadApp(void);
	~CWtpDownloadApp(void);
};

extern CWtpDownloadApp theApp;

