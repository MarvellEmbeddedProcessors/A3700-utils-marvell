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
#if WINDOWS
#include <WINDOWS.H>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include <sstream>
#include "Port.h"
#include "WtpCommand.h"
#include "ErrorCodes.h"
#include "Misc.h"


CPort::CPort(){}
CPort::~CPort(){}

int CPort::getTickCount()
{
#if WINDOWS
	return (int)GetTickCount();
#else
	struct timeval tv;
	struct timezone tz;
	struct tm *tm;
	gettimeofday(&tv,&tz);
	tm = localtime(&tv.tv_sec);
	return ((tm->tm_hour * 3600 + tm->tm_min * 60 + tm->tm_sec) * 1000 + tv.tv_usec/1000);
#endif
}

void CPort::sleep(int time)
{
#if WINDOWS
	Sleep((DWORD)time);
#else
	sleep(time);
#endif
}

CWtpStatus* CPort::GetWtpStatus()
{
	return &WtpStatus;
}
/**
* Method:	GetCommandName
* Input:
* Output:	string
* Desc:		Returns the string value of command type
*/
string CPort::GetCommandName()
{
	string cmdName = "UNKNOWNCOMMAND";
	switch(WtpCmd->GetCMD())
	{
		case DATA: cmdName = "DATA"; break;
		case DATAHEADER: cmdName = "DATAHEADER";break;
		case DISCONNECT:cmdName = "DISCONNECT";break;
		case GETVERSION: cmdName = "GETVERSION";break;
		case MESSAGE:cmdName = "MESSAGE"; break;
		case OTPVIEW:cmdName = "OPTVIEW";break;
		case PASSWORD:cmdName = "PASSWORD";break;
		case PREAMBLE:cmdName = "PREAMBLE";break;
		case PUBLICKEY:cmdName = "PUBLICKEY";break;
		case SELECTIMAGE:cmdName = "SELECTIMAGE";break;
		case SIGNEDPASSWORD:cmdName = "SIGNEDPASSWORD";break;
		case VERIFYIMAGE:cmdName = "VERIFYIMAGE";break;
		case DONE:cmdName = "DONE";break;
		case PROTOCOLVERSION: cmdName = "PROTOCOLVERSION";break;
		case UPLOADDATA: cmdName = "UPLOADDATA";break;
		case GETPARAMETERS: cmdName = "GETPARAMETERS";break;
		case UPLOADDATAHEADER: cmdName = "UPLOADDATAHEADER";break;
	}
	return cmdName;
}

/**
* Method:	GetWtpRemainingMessage
* Input:
* Output:	void
* Desc:		Prints message returned by target in WtpStatus object
*/
void CPort::GetWtpRemainingMessage()
{
	int StatusLength = 0;
	CMisc misc;
	stringstream Message;
	if (((WtpStatus.GetFlags() & 0x02) >> 1) == MESSAGESTRING)
    {
		StatusLength = WtpStatus.GetDLEN();
		Message << endl << "---" << endl;

		for (int i = 0; i < StatusLength; i++)
			Message << WtpStatus.GetData()[i];

		Message << endl<< "---" << endl;
	}
	else if (WtpStatus.GetDLEN() == 1)
    {
		int byteErrorCode = (int)WtpStatus.GetData()[0];
		if (byteErrorCode < MAXERRORCODES)
		{
			if (byteErrorCode == 7 || byteErrorCode == 8)
				Message << "Status:";
			else
				Message << "Error:";
			Message << " 0x" << hex << byteErrorCode << ":";
			Message << ErrorCodes[byteErrorCode].Description << endl;
		}
		else
		{
			Message << "Unknown Error! Error code:" << byteErrorCode << endl;
		}
    }
	else
	{
		Message << endl << "Hex format of message:";
		for (int i = 0; i < WtpStatus.GetDLEN(); i++)
		{
			Message << hex << "0x" << (int)WtpStatus.GetData()[i] << " ";
		}

		Message << endl << "ASCII format of message:";
		for (int i = 0; i < WtpStatus.GetDLEN(); i++)
		{
			Message << WtpStatus.GetData()[i] << " ";
		}
	}
	misc.UserMessagePrintStr( true, Message.str());
}

