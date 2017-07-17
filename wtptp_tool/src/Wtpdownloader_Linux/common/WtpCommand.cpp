/******************************************************************************
 *
 *  (C)Copyright 2012 Marvell. All Rights Reserved.
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
#include <cstring>
//#include <sstream>
#include <iomanip>
#include "WtpCommand.h"
#include <iostream>

using namespace std;


int CWtpCommand::iSequence = 0x00;

CWtpCommand::CWtpCommand(void)
{
	ResetWtpCommand();
}

CWtpCommand::~CWtpCommand(void){}

WTPCOMMAND CWtpCommand::GetWtpCommand()
{
	return this->WtpCmd;
}

void CWtpCommand::print()
{
	cout << "HOST::" << endl;
	cout << "CMD\tSEQ\tCID\tFLAGS\tLEN\t\t\tDATA" << endl;
	cout << hex <<  setfill('0') << setw(2) << "0x" << (unsigned int)WtpCmd.CMD;
	cout << hex <<  setfill('0') << setw(2) << "\t0x" << (unsigned int)WtpCmd.SEQ;
	cout << hex <<  setfill('0') << setw(2) << "\t0x" << (unsigned int)WtpCmd.CID;
	cout << hex <<  setfill('0') << setw(2) << "\t0x" << (unsigned int)WtpCmd.Flags;
	BYTE lenBuf[4];
	lenBuf[0] = (int)((WtpCmd.LEN & 0xFF000000) >> 24 );
	lenBuf[1] = (int)((WtpCmd.LEN & 0x00FF0000) >> 16 );
	lenBuf[2] = (int)((WtpCmd.LEN & 0x0000FF00) >> 8 );
	lenBuf[3] = (int)((WtpCmd.LEN & 0X000000FF));
	cout << "\t";
	for(int i = 0;i < 4 ; i++)
		cout << hex <<  setfill('0') << setw(2) << "0x" << (unsigned int)lenBuf[i] << " ";

	BYTE buffer[DATALENGTH];
	memset(buffer,0,DATALENGTH);

	if(memcmp(WtpCmd.Data,buffer,DATALENGTH) != 0)
		for(int i = 1 ; i <= WtpCmd.LEN ; i++)
		{
			if(i % 6 == 0)
				cout << endl << "\t\t\t\t\t\t";
			cout << hex << setfill('0') << setw(2) << "0x" << (unsigned int)WtpCmd.Data[i-1] << " ";
		}
	cout << endl << endl;
}

void CWtpCommand::ResetWtpCommand()
{
	memset(&(this->WtpCmd),0,sizeof(WTPCOMMAND));
}

unsigned char CWtpCommand::GetCMD()
{
	return this->WtpCmd.CMD;
}

unsigned char CWtpCommand::GetCID()
{
	return this->WtpCmd.CID;
}

unsigned char CWtpCommand::GetSEQ()
{
	return this->WtpCmd.SEQ;
}

unsigned char CWtpCommand::GetFlags()
{
	return this->WtpCmd.Flags;
}

long CWtpCommand::GetLEN()
{
	return this->WtpCmd.LEN;
}

unsigned char* CWtpCommand::GetData()
{
	return this->WtpCmd.Data;
}

void CWtpCommand::SetCMD(unsigned char CMD)
{
	this->WtpCmd.CMD = CMD;
}

void CWtpCommand::SetCID(unsigned char CID)
{
	this->WtpCmd.CID = CID;
}

void CWtpCommand::SetSEQ(unsigned char SEQ)
{
	this->WtpCmd.SEQ = SEQ;
}

void CWtpCommand::SetFlags(unsigned char Flags)
{
	this->WtpCmd.Flags = Flags;
}

void CWtpCommand::SetData(unsigned char Data,int pos)
{
	this->WtpCmd.Data[pos] = Data;
}
void CWtpCommand::SetData(unsigned char *Data, int length)
{
	for(int i = 0; i < length ; i++)
		SetData(Data[i],i);
}

CPlainTextWTPCR::CPlainTextWTPCR(void)
{
	ResetWtpCommand();
	WtpCmd.CMD = PLAINTEXTWTPCR;
	WtpCmd.SEQ = 0x74;
	WtpCmd.CID = 0x70;
	WtpCmd.Flags = 0xD;
}

CPreambleCommand::CPreambleCommand(void)
{
	ResetWtpCommand();
	WtpCmd.CMD = PREAMBLE;
	WtpCmd.SEQ = 0xD3;
	WtpCmd.CID = 0x02;
	WtpCmd.Flags = 0x2B;
//	WtpCmd.LEN = 0x00D3022B;
//	WtpCmd.LEN = 0x00000000;
}

CGetVersionCommand::CGetVersionCommand(void)
{
	ResetWtpCommand();
	CWtpCommand::iSequence = 0x00;
	WtpCmd.CMD = GETVERSION;
	WtpCmd.SEQ = 0x00;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x8;
	WtpCmd.LEN = 0x00000000;
}

CSelectImageCommand::CSelectImageCommand()
{
	ResetWtpCommand();
	WtpCmd.CMD = SELECTIMAGE;
	WtpCmd.SEQ = 0x00;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000000;
}

CVerifyImageCommand::CVerifyImageCommand(unsigned char AckOrNoAck)
{
	ResetWtpCommand();
	iSequence = 0x00;
	WtpCmd.CMD = VERIFYIMAGE;
	WtpCmd.SEQ = 0x00;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000001;
	WtpCmd.Data[0] = AckOrNoAck;
}

CDataHeaderCommand::CDataHeaderCommand(bool bFastDownload)
{
	ResetWtpCommand();
	iSequence++;
	WtpCmd.CMD = DATAHEADER;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000004;
	if ( bFastDownload)
		WtpCmd.Flags = 0x4;
}

CDataCommand::CDataCommand(unsigned char *pData, int Length)
{
	ResetWtpCommand();
	iSequence++;
	WtpCmd.CMD = DATA;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = Length;
	SetData(pData,Length);
}

CDoneCommand::CDoneCommand()
{
	ResetWtpCommand();
	iSequence = 0x00;
	WtpCmd.CMD = DONE;
	WtpCmd.SEQ = 0x00;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000000;
}


CDisconnectCommand::CDisconnectCommand()
{
	ResetWtpCommand();
	iSequence = 0x00;
	WtpCmd.CMD = DISCONNECT;
	WtpCmd.SEQ = 0x00;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000000;
}

CPasswordCommand::CPasswordCommand()
{
	ResetWtpCommand();
	iSequence = 0x00;
	WtpCmd.CMD = PASSWORD;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000000;
}

CPublicKeyCommand::CPublicKeyCommand()
{
	ResetWtpCommand();
	iSequence = 0x00;
	WtpCmd.CMD = PUBLICKEY;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000000;
}

CSignedPasswordCommand::CSignedPasswordCommand()
{
	ResetWtpCommand();
	iSequence = 0x00;
	WtpCmd.CMD = SIGNEDPASSWORD;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000000;
}

CMessageCommand::CMessageCommand()
{
	ResetWtpCommand();
	iSequence = 0x00;
	WtpCmd.CMD = MESSAGE;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000000;
}

CTargetProtocolVersionCommand::CTargetProtocolVersionCommand()
{
	ResetWtpCommand();
	WtpCmd.CMD = PROTOCOLVERSION;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000000;
}

CGetParametersCommand::CGetParametersCommand()
{
	ResetWtpCommand();
	WtpCmd.CMD = GETPARAMETERS;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000000;
}

CUploadDataHeaderCommand::CUploadDataHeaderCommand()
{
	iSequence = 0x00;
	ResetWtpCommand();
	WtpCmd.CMD = UPLOADDATAHEADER;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = sizeof(UPLOAD_DATA_PARAMS);
}

CUploadDataCommand::CUploadDataCommand()
{
	ResetWtpCommand();
	WtpCmd.CMD = UPLOADDATA;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000000;
}

CGetPCCommand::CGetPCCommand()
{
	ResetWtpCommand();
	WtpCmd.CMD = GETPC;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = 0x00000004;
}

CJtagOverMediaDataCommand::CJtagOverMediaDataCommand(unsigned char *pData, int Length)
{
	iSequence++;
	ResetWtpCommand();
	WtpCmd.CMD = JTAGOVERMEDIA;
	WtpCmd.SEQ = iSequence;
	WtpCmd.CID = 0x00;
	WtpCmd.Flags = 0x00;
	WtpCmd.LEN = Length;
	SetData(pData,Length);
}

CPlainTextWTPCR::~CPlainTextWTPCR(){}
CPreambleCommand::~CPreambleCommand(){}
CGetVersionCommand::~CGetVersionCommand(){}
CSelectImageCommand::~CSelectImageCommand(){}
CVerifyImageCommand::~CVerifyImageCommand(){}
CDataHeaderCommand::~CDataHeaderCommand(){}
CDataCommand::~CDataCommand(){}
CDoneCommand::~CDoneCommand(){}
CDisconnectCommand::~CDisconnectCommand(){}
CPasswordCommand::~CPasswordCommand(){}
CPublicKeyCommand::~CPublicKeyCommand(){}
CSignedPasswordCommand::~CSignedPasswordCommand(){}
CMessageCommand::~CMessageCommand(){}
CTargetProtocolVersionCommand::~CTargetProtocolVersionCommand(){}
CGetParametersCommand::~CGetParametersCommand(){}
CUploadDataHeaderCommand::~CUploadDataHeaderCommand(){}
CUploadDataCommand::~CUploadDataCommand(){}
CJtagOverMediaDataCommand::~CJtagOverMediaDataCommand(){}
CGetPCCommand::~CGetPCCommand(){}

//CDebugBootCommand::CDebugBootCommand()
//{
//	ResetWtpCommand();
//	WtpCmd.CMD = DEBUGBOOT;
//	WtpCmd.SEQ = 0x00;
//	WtpCmd.CID = 0x00;
//	WtpCmd.Flags = 0x00;
//	WtpCmd.LEN = 0x00000001;
//}

//COtpViewCommand::COtpViewCommand()
//{
//	ResetWtpCommand();
//	iSequence = 0x00;
//	WtpCmd.CMD = OTPVIEW;
//	WtpCmd.SEQ = iSequence;
//	WtpCmd.CID = 0x00;
//	WtpCmd.Flags = 0x00;
//	WtpCmd.LEN = 0x00000001;
//	WtpCmd.Data[0] = theApp.iOtpFlashType;
//}
//CDebugBootCommand::~CDebugBootCommand(){}
//COtpViewCommand::~COtpViewCommand(){}

