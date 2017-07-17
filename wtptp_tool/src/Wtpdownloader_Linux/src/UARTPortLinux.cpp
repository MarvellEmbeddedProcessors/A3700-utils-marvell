/******************************************************************************
 *
 *  (C)Copyright 2011 - 2012 Marvell. All Rights Reserved.
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
#if LINUX
#include <errno.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include "UARTPortLinux.h"
#include "WtpDownloadApp.h"
#include "WtpCommand.h"
#include "WTPDEF.h"
#include <iostream>
#include <sstream>
#include <string.h>
#include <unistd.h>
using namespace std;

CUARTPortLinux::~CUARTPortLinux(void){}
CUARTPortLinux::CUARTPortLinux(void){}

/**
* Method:	OpenPort
* Input:	N/A
* Output:	void
* Desc:		This is a overriden method from Port class.
*			Opens UART port.
*/
void CUARTPortLinux::OpenPort() throw (CWtpException)
{
	struct termios options;
	stringstream portNumber;
	portNumber << theApp.iCommPort;
	UARTDeviceName ="/dev/ttyUSB" ;
	UARTDeviceName.append(portNumber.str());
	cout << endl << UARTDeviceName << endl;
	uartLinuxFileDesc = open(UARTDeviceName.c_str(),O_RDWR|O_NOCTTY);
	if(uartLinuxFileDesc < 0)
	{
		cout << endl << " ERROR: Open UART driver failed:" << endl;
		throw CWtpException(CWtpException::OPENUARTPORT,0,UARTDeviceName);
	}
	fcntl(uartLinuxFileDesc,F_SETFL,0);
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	options.c_iflag     &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	options.c_lflag     &= ~(ICANON|ECHO|ECHOE|ISIG|ECHONL|IEXTEN);
	options.c_oflag     &= ~OPOST;
	options.c_cc[VMIN]   = 0;
	options.c_cc[VTIME]  = 0;
	options.c_cflag &= ~(CSIZE|PARENB);
	options.c_cflag |= CS8;
	tcsetattr(uartLinuxFileDesc, TCSANOW, &options);
}



void CUARTPortLinux::SetCommReadTimeout(DWORD msecs) {
//	CommTimeOuts.ReadTotalTimeoutMultiplier = msecs;

}


/**
* Method:	ReadPort
* Input:	CWtpStatus, DWORD
* Output:	void
* Desc:		This is a overriden method from Port class.
*			Retrieves CWtpStatus message sent on UART port by target.
*/
void CUARTPortLinux::ReadPort(UINT32_T dwStatusSize)throw (CWtpException)
{
	int result = 0;
	int BytesReceived = 0, BytesRemaining = dwStatusSize;
    char* buffer = new char[dwStatusSize];
    memset( buffer, 0, dwStatusSize );
    //cout << "CUARTPortLinux::ReadPort(UINT_T dwSize)" << endl;

	// Read the initial 6 bytes of the target's command response.
	while(BytesRemaining > 0)
	{

		result = read(uartLinuxFileDesc, buffer + BytesReceived, BytesRemaining);
		if(result == -1)
		{
			delete[] buffer;
			throw CWtpException(CWtpException::READUARTPORT);
		}
		BytesRemaining -= result;
		BytesReceived += result;
	}

	// Now check Length payload for this command and prime the host.
	memcpy(&WtpStatus.GetWtpStatus(),buffer,BytesReceived);
	if(WtpStatus.GetDLEN() > 0)
		BytesRemaining = WtpStatus.GetDLEN();
	else if(WtpStatus.GetCMD() == UPLOADDATA)
		BytesRemaining = theApp.ProtocolManager.UploadPacketLength;

	// Read additional data specified by LEN field
	//cout << "ReadPort remaining bytes after 6 bytes of response in is: " << BytesRemaining << endl;
	if(BytesRemaining > 0)
	{

		result = 0;
		BytesReceived = 0;
		delete[] buffer;
		buffer = new char[WtpStatus.GetDLEN()];
		while (BytesRemaining > 0)
		{
			result = read(uartLinuxFileDesc, buffer + BytesReceived, BytesRemaining);
			if(result == -1)
			{
				delete[] buffer;
				cout << endl << "Error: ReadUart returned " <<  result << endl;
				throw CWtpException(CWtpException::READUARTPORT);
			}
			BytesReceived += result;
			BytesRemaining -= result;
		}
		memcpy(WtpStatus.GetData(),buffer,WtpStatus.GetDLEN());
	}
	delete[] buffer;
}


void CUARTPortLinux::WriteRaw(char * buffer, DWORD count) throw (CWtpException)
{
	DWORD bytestransmitted;
	if (write(uartLinuxFileDesc, buffer, count) == 0)
	{
		stringstream ss;
		ss << "Error: " << "WriteRaw failed.. " << endl;
		throw CWtpException(CWtpException::WRITEUARTPORT);
	}
}

int CUARTPortLinux::ReadRaw(char * buffer, DWORD count)
{
	int BytesRead, i = 0;
	BytesRead = read(uartLinuxFileDesc, buffer, count);
#if DEV_DEBUG
	if (BytesRead > 0) {
		cout << "Bytes Read: ";
		for (i = 0; i < BytesRead; i++) {
			cout  << hex << (unsigned int) buffer[i] << " " << endl;
		}
		cout << endl;
	}
#endif
	return BytesRead;
}



/**
* Method:	WriteUart
* Input:	UINT_T
* Output:	void
* Desc:		Send command on UART
*/
void CUARTPortLinux::WritePort (UINT32_T dwCmdSize)throw (CWtpException)
{
	int result = 0;
	WTPCOMMAND WtpCmdStruct = WtpCmd->GetWtpCommand();
	result = write( uartLinuxFileDesc,(char *)&WtpCmdStruct,dwCmdSize);
	if (result < 0)
		throw CWtpException(CWtpException::WRITEUARTPORT);
}
/**
* Method:	WritePort
* Input:
* Output:	void
* Desc:		This is a overriden method from Port class.
*			Send command on UART and get response in CWtpstatus object.
*/
void CUARTPortLinux::WritePort() throw (CWtpException)
{
	bool IsOk = true;
	int CmdSize = 8 + WtpCmd->GetLEN(), StatusSize = 6;
	stringstream Message;

	if(WtpCmd->GetCMD() == PREAMBLE)
	{
		CmdSize = 4; StatusSize = 4;
	}
	else if (WtpCmd->GetCMD() == PLAINTEXTWTPCR)
	{
		CmdSize = 4;
		StatusSize = 5;
	}

	WritePort (CmdSize);
	ReadPort(StatusSize);

	if(WtpStatus.GetCMD() == MESSAGE)
	{
		if(!WtpStatus.GetDLEN() > 0)
			WtpStatus.SetStatus(NACK);
		else
			GetWtpRemainingMessage();
	}
}
/**
* Method:	ClosePort
* Input:	N/A
* Output:	void
* Desc:		This is a overriden method from Port class.
*			Closes UART port.
*/
void CUARTPortLinux::ClosePort ()
{
	close (uartLinuxFileDesc);
}
#endif
