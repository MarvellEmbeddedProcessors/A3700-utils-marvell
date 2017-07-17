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
#pragma once
#include <list>
#include "Port.h"
#include "Misc.h"
#include "WtpException.h"
#if WINDOWS
#include <windows.h>
#include "USB_INTERFACE_API.h"
#endif
using namespace std;



class CPort;
class CWtpCommand;
class CWtpException;
class CUARTPort;

class CProtocolManager
{
private:
	CPort* Port;
	CUARTPort* SerialPort;
	void PublicKey () throw(CWtpException);
	void Password () throw(CWtpException);
	void SignedPassword () throw(CWtpException);
	void UploadData() throw(CWtpException);
	void UploadDataHeader (UPLOAD_DATA_PARAMS* pUploadDataParams) throw (CWtpException);
	void HandlePendingMessages();

#if WINDOWS
	//Methods for pipe setup and handling
	void createJtagEnablerChildProcess(string args) throw(CWtpException);

	void writeToPipe(string message) throw(CWtpException);

#endif
public:

	PROTOCOL_VERSION TargetPV;
	TARGET_PARAMS TargetParams;
	list<UPLOAD_DATA_SPEC*> lstUploadSpecs;
	int UploadPacketLength;
	CMisc misc;

#if WINDOWS
	//Members related to pipes set up for JtagEnabler
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;

	//Pipe handles for JtagEnabler (Child process)
	//and WtpDownload (Parent process)
	HANDLE h_Child_STDIN_Read;
	HANDLE h_Child_STDIN_Write;
	HANDLE h_Child_STDOUT_Read;
	HANDLE h_Child_STDOUT_Write;
	HANDLE m_DownloadThreadHandle;
	void JtagOverMedia (string jtagArgsString) throw(CWtpException);
#endif

	void SendPlainTextWtpCR();
	void SendContinuousForceUARTMode();
	void SendContinuousFFStream();
	void SendContinuousFFStream_DLoad();
	void SendContinuousForceConsoleMode();
	void SendPreamble () throw(CWtpException);
	void GetVersion () throw(CWtpException);
	//void SelectImage (unsigned long *pImageType) throw(CWtpException);
	void SelectImage (unsigned int *pImageType) throw(CWtpException);
	void VerifyImage (unsigned char AckOrNack) throw(CWtpException);
	void DataHeader (unsigned int uiRemainingData) throw(CWtpException);
	void Data (unsigned char *pData,int Length) throw(CWtpException);
	void GetWtpMessage ();
	void GetTargetMessage ();
	//bool DebugBoot ();
	//bool OtpView ();
	void Done () throw(CWtpException);
	void Disconnect () throw(CWtpException);
	void JtagReEnable () throw(CWtpException);

	void GetPC() throw (CWtpException);

	void GetTargetProtocolVersion() throw(CWtpException);
	void GetParameters() throw(CWtpException);
	void DoUploads() throw(CWtpException);
	bool isLastData(bool mode, bool value);
	void ParseUploadSpecFile() throw(CWtpException);

	void readFromPipe() throw(CWtpException);

	//helper functions
	CPort* GetPort();
	void SetWtpPort(CPort* Port);
	CWtpStatus* GetStatus();


	CProtocolManager(void);
	~CProtocolManager(void);
#if WINDOWS
    CUSB_INTERFACE_DLL m_USBInterfaceAPI;
#endif
};

