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
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include "USBPortLinux.h"
#include "WtpDownloadApp.h"
#include "WtpCommand.h"
#include "WTPDEF.h"
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#if LINUX
#include <iomanip>
#endif
using namespace std;

CUSBPortLinux::~CUSBPortLinux(void){}
CUSBPortLinux::CUSBPortLinux(void){}

/**
* Method:	GetDeviceName
* Input:	N/A
* Output:	void
* Desc:		Identify MARVELL USB device and intialize for communication
*/
void CUSBPortLinux::getDeviceName() throw (CWtpException)
{
	FILE* fp;
	char* buffer = new char[256];
	string cmd = "ls /dev/MARVELL:*";
	fp = popen(cmd.c_str(),"r");

	while(fgets(buffer,256,fp) != NULL)
	{
		string temp = buffer;
		lVidList.push_back(temp.substr(0,temp.length() -1));
	}
	pclose(fp);
	if(lVidList.empty())
		throw CWtpException(CWtpException::OPENUSBPORT,0,"Marvell device(s) not found!");

	if(theApp.bUSBInterface)
	{
		bool deviceFound = false;
		VidIter = lVidList.begin();
		while(!lVidList.empty())
		{
			string device = *VidIter;
			if(device.find(theApp.sUSBInterface))
			{
				deviceFound = true;break;
			}
			VidIter++;
		}
		if(deviceFound)
			UsbDeviceName.append("/dev/" + theApp.sUSBInterface);
		else
			throw CWtpException(CWtpException::OPENUSBPORT,0,UsbDeviceName + " not found!");
	}
	else
	{
		if(lVidList.size() > 1)
		{
			cout << "Note: There is more than one MARVELL device connected to system" << endl;
			cout << "If you want to specify which device to download, please use the -G option" << endl;
			cout << "For current download the first device in the list is considered:" << endl;
		}
		theApp.sUSBInterface = lVidList.front();
		UsbDeviceName.append(lVidList.front());
	}
}

/**
* Method:	OpenPort
* Input:	N/A
* Output:	void
* Desc:		Overriden method of Port class.
*			Opens USB port
*/
void CUSBPortLinux::OpenPort () throw (CWtpException)
{
	struct termios options, old_options;
	getDeviceName();
	cout << "Opened Port to device: " <<  UsbDeviceName << endl;
	usbLinuxFileDesc = open(UsbDeviceName.c_str(), O_RDWR | O_NOCTTY);

	if (usbLinuxFileDesc < 0)
		throw CWtpException(CWtpException::OPENUSBPORT,0,UsbDeviceName);

	fcntl(usbLinuxFileDesc, F_SETFL, 0);
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	options.c_lflag     &= ~(ICANON|ECHO|ECHOE|ISIG|ECHONL|IEXTEN);
	options.c_oflag     &= ~OPOST;
	options.c_iflag	    &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	options.c_cc[VMIN]   = 0;
	options.c_cc[VTIME]  = 10;
	options.c_cflag &= ~(CSIZE|PARENB);
	options.c_cflag |= CS8;
	tcsetattr(usbLinuxFileDesc, TCSANOW, &options);
}

void CUSBPortLinux::WriteRaw(char * buffer, DWORD count) throw(CWtpException)
{
}

int CUSBPortLinux::ReadRaw(char * buffer, DWORD count)
{
	return 0;
}

void CUSBPortLinux::SetCommReadTimeout(DWORD msecs)
{
//	CommTimeOuts.ReadTotalTimeoutMultiplier = msecs;

}


/**
* Method:	WritePort
* Input:	DWORD
* Output:	void
* Desc:		Writes CWtpCommand message to USB port
*/
void CUSBPortLinux::WritePort (UINT32_T dwCmdSize)throw (CWtpException)
{
	int result = 0;
	WTPCOMMAND WtpCmdStruct = WtpCmd->GetWtpCommand();
	if(theApp.bFastDownload && WtpCmdStruct.CMD == DATA)
	{
		//cout << "Bytes to write:" << endl;
		for (int i= 1 ; i <= WtpCmdStruct.LEN; i++)
		{
			cout << hex << setfill('0') << setw(2) << "0x" << (unsigned int)WtpCmdStruct.Data[i-1] << " ";
		}
		result = write( usbLinuxFileDesc,(char *)(WtpCmdStruct.Data), WtpCmdStruct.LEN);

	}
	else
	{
		result = write( usbLinuxFileDesc,(char *)(&WtpCmdStruct),dwCmdSize);
		//WtpCmd->print();
	}
	//cout << "Number of bytes written:" << result << endl;
	if (result < 0)
		throw CWtpException(CWtpException::WRITEUSBPORT);
}

/**
* Method:	WritePort
* Input:	CWtpCommand, CWtpStatus
* Output:	void
* Desc:		Overriden method of Port class.
*			Writes CWtpCommand message to port and
*			gets response in CWtpStatus object
*/
void CUSBPortLinux::WritePort() throw (CWtpException)
{
	bool IsOk = true;
	int CmdSize = 8 + WtpCmd->GetLEN(), StatusSize = 6;
	if(WtpCmd->GetCMD() == PREAMBLE)
	{
		CmdSize = 4; StatusSize = 4;
	}
	WritePort (CmdSize);
	if(theApp.bFastDownload && WtpCmd->GetCMD() == DATA && !theApp.ProtocolManager.isLastData(GET,FDMODE));
	else
		ReadPort(StatusSize);
	if(WtpCmd->GetCMD() == MESSAGE)
	{
		if (!(WtpStatus.GetDLEN() > 0))
			WtpStatus.SetStatus(NACK);
		else
			GetWtpRemainingMessage();
	}
}
/**
* Method:	ReadPort
* Input:	CWtpStatus, DWORD
* Output:	void
* Desc:		Overriden method of Port class.
*			Reads CWtpStatus message sent by target
*/
void CUSBPortLinux::ReadPort(UINT32_T dwSize)throw (CWtpException)
{
	int result = 0;
	int recievedBytes = 0, remainingBytes = dwSize;
	char* buffer = new char[dwSize];
	memset(buffer,0,dwSize);
	while(remainingBytes > 0)
	{
		result = read(usbLinuxFileDesc, buffer + recievedBytes, remainingBytes);
		//cout << "Number of bytes read:" << result << endl;
		if(result == 0)
			throw CWtpException(CWtpException::READUSBPORT);
		remainingBytes -= result;
		recievedBytes += result;
	}
	memcpy(&(WtpStatus.GetWtpStatus()),buffer,recievedBytes);
	if(WtpStatus.GetDLEN() > 0)
		remainingBytes = WtpStatus.GetDLEN();
	else
	if(WtpStatus.GetCMD() == UPLOADDATA)
		remainingBytes = theApp.ProtocolManager.UploadPacketLength;

	if(remainingBytes > 0)
	{
		delete[] buffer;
		buffer = new char[remainingBytes];
		result = 0;
		recievedBytes = 0;
		while(remainingBytes > 0)
		{
			result = read(usbLinuxFileDesc,buffer + recievedBytes,remainingBytes);
			//cout << "Number of bytes read:" << result << endl;
			if(result == -1)
				throw CWtpException(CWtpException::READUSBPORT);
			remainingBytes -= result;
			recievedBytes += result;
		}
		memcpy(WtpStatus.GetData(),buffer,recievedBytes);
	}
	delete[] buffer;
}

/**
* Method:	ClosePort
* Input:	N/A
* Output:	void
* Desc:		Overriden method of Port class.
*		Closes USB port
*/
void CUSBPortLinux::ClosePort ()
{
	close (usbLinuxFileDesc);
}
#endif
