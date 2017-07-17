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
#include <sstream>
#include <istream>
#include <string.h>
#include "WtpImage.h"
#include "WtpDownloadApp.h"
#include "WtpCommand.h"
#include "Tim.h"

using namespace std;

CWtpImage::CWtpImage(void)
{
	eImageType = 0;
	sImageFilename = "";
	iImageFileSize = 0;
}

CWtpImage::~CWtpImage(void){}

int CWtpImage::GetEImageType()
{
	return this->eImageType;
}

void CWtpImage::SetEImageType(int eImageType)
{
	this->eImageType = eImageType;
}

string& CWtpImage::GetSImageFileName()
{
	return sImageFilename;
}

void CWtpImage::SetSImageFileName(string& sImageFileName)
{
	this->sImageFilename = sImageFileName;
}

int CWtpImage::GetIImageFileSize()
{
	return iImageFileSize;
}

void CWtpImage::SetIImageFileSize(int iImageFileSize)
{
	this->iImageFileSize = iImageFileSize;
}

void CWtpImage::StartDataTransfer(ifstream& ifFile,int numOfBytes)
{
	unsigned int uiBufferSize = 0;
	unsigned int uiRemainingBytes = numOfBytes;
	ostringstream Message;
	CMisc misc;
	long BytesRead = 0;

	theApp.ProtocolManager.DataHeader(uiRemainingBytes);

	if ((theApp.ProtocolManager.GetStatus()->GetFlags() & 0x4) && theApp.bFastDownload )
	{
		Message.str("");
		Message << endl << "*******NOTE:: Fast Download is NOW activated!" << endl;
		misc.UserMessagePrintStr( true, Message.str());
	}
	else
		theApp.bFastDownload = false;

	int length = theApp.ProtocolManager.GetStatus()->GetDLEN();
	for (int i = 0; i < length; i++)
	{
		uiBufferSize |= theApp.ProtocolManager.GetStatus()->GetData()[i] << (8 * i);
	}

    while (uiRemainingBytes > 0)
    {
		if (uiBufferSize > uiRemainingBytes)
			uiBufferSize = uiRemainingBytes;
		uiRemainingBytes -= uiBufferSize;

		Message.str("");
		Message << "Buffer size = " << dec << uiBufferSize  << ",";
		Message << " Bytes remaining = " << uiRemainingBytes << endl;
		misc.UserMessagePrintStr( theApp.bVerbose,Message.str());

		char* buffer = new char[uiBufferSize];
		ifFile.read(buffer,uiBufferSize);

		try
		{
			if ((BytesRead = (long)ifFile.gcount()) != uiBufferSize)
			{
				Message << " should've read " << uiBufferSize;
				throw CWtpException(CWtpException::INSUFFICIENT_BYTES_READ, (int)BytesRead,(std::string)Message.str());
			}

			int totalSize = uiBufferSize;
			int mark = 0;

			//Total amount of data to send = uiBufferSize
			//Download over USB? data size to send = packetSize.
			//Download over UART? data size to send = uiBufferSize
			int packetSize = (theApp.iPortType == USB)? theApp.iUSBPacketSize : uiBufferSize;

			if(theApp.iPortType == USB && theApp.iUSBPacketSize > (int)uiBufferSize)
			{
				Message.str("");
				Message << "Packet size changed from " << packetSize << " to " << uiBufferSize << endl;
				misc.UserMessagePrintStr( theApp.bVerbose,Message.str());
				packetSize = uiBufferSize;
			}

			Message.str("");
			Message << "Packet size = " << dec << packetSize  << endl;
			misc.UserMessagePrintStr( theApp.bVerbose,Message.str());

			while(totalSize > 0)
			{
				if(totalSize <= packetSize)
				{
					packetSize = uiBufferSize;
					if(theApp.bFastDownload && uiRemainingBytes == 0 && theApp.iPortType == USB)
						theApp.ProtocolManager.isLastData(SET, true);
					packetSize = totalSize;
				}
				theApp.ProtocolManager.Data((unsigned char*)&buffer[mark],packetSize);

				mark += packetSize;
				totalSize -= packetSize;
			}
			delete [] buffer;

			if (!theApp.bFastDownload && uiRemainingBytes > 0)
			{
				theApp.ProtocolManager.DataHeader(uiRemainingBytes);
				uiBufferSize = 0;
				for (int i = 0; i < (int)theApp.ProtocolManager.GetStatus()->GetDLEN(); i++)
					uiBufferSize |= (int)theApp.ProtocolManager.GetStatus()->GetData()[i] << (8 * i);
			}
		}

		catch(CWtpException&)
		{
			delete [] buffer;
			throw;
		}
	} // End of download loop
	theApp.ProtocolManager.Done();

	if (theApp.bFastDownload)
		theApp.ProtocolManager.isLastData (SET, false);

	if(ifFile.is_open())
		ifFile.close();
}

/**
* Method:	DownloadImage
* Input:	N/A
* Output:	boolean
* Desc:		Downloads image asked by target. Protocol commands till
*			SELECTIMAGE is executed in RequestImageType method in
*			CWtpDownloadApp class since they are not related to a
*			particular image. VERIFYIMAGE command onwards is executed
*			inside this method since they are related to the image.
*/
void CWtpImage::DownloadImage()
{
	ostringstream Message;
	ifstream ifFile;
    	unsigned int uiRemainingBytes = 0;
	unsigned int uiBufferSize = 0;

	Message.str("");
	theApp.ProtocolManager.VerifyImage(ACK);

	ifFile.open(GetSImageFileName().c_str(), ios_base::in | ios_base::binary );
	if ( !ifFile.is_open() )
		throw CWtpException(CWtpException::FILE_OPEN_FAIL,0,GetSImageFileName());

    if ( ((GetEImageType() & TYPEMASK) == (TIMIDENTIFIER & TYPEMASK)) || (GetEImageType() == PARTIONIDENTIFIER))
	    ifFile.seekg(0,ios::beg); // Set position to SOF
    else
	    ifFile.seekg(4L,ios::beg); // Set position to SOF

	uiRemainingBytes = GetIImageFileSize();
	try
	{
		StartDataTransfer(ifFile,uiRemainingBytes);
	}
	catch(CWtpException&)
	{
		if(ifFile.is_open())
			ifFile.close();
		throw;
	}
}

