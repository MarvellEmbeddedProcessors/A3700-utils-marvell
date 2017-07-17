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
#include "WtpStatus.h"
//#include <sstream>
#include <iomanip>
#include <iostream>
#if LINUX
#include <cstring>
#else
#include <string.h>
#endif
using namespace std;

CWtpStatus::CWtpStatus(void)
{
	ResetWtpStatus();
}

CWtpStatus::~CWtpStatus(void)
{
}

WTPSTATUS& CWtpStatus::GetWtpStatus()
{
	return WtpStatus;
}

void CWtpStatus::SetWtpStatus(WTPSTATUS* WtpStatus)
{
	this->WtpStatus.CMD		= WtpStatus->CMD;
	this->WtpStatus.SEQ		= WtpStatus->SEQ;
	this->WtpStatus.CID		= WtpStatus->CID;
	this->WtpStatus.Status	= WtpStatus->Status;
	this->WtpStatus.DLEN	= WtpStatus->DLEN;
	this->WtpStatus.Flags	= WtpStatus->Flags;
	SetData(WtpStatus->Data,RETURNDATALENGTH);
}

void CWtpStatus::print()
{
	cout << "TARGET::" << endl;
	cout << "CMD\tSEQ\tCID\tnACK\tFLAGS\tDLEN\t\tDATA" << endl;
	cout << hex << setfill('0') << setw(2) << "0x" << (unsigned int)WtpStatus.CMD;
	cout << hex << setfill('0') << setw(2) << "\t0x" << (unsigned int)WtpStatus.SEQ;
	cout << hex << setfill('0') << setw(2) << "\t0x" << (unsigned int)WtpStatus.CID;
	cout << hex << setfill('0') << setw(2) << "\t0x" << (unsigned int)WtpStatus.Status;
	cout << hex << setfill('0') << setw(2) << "\t0x" << (unsigned int)WtpStatus.Flags;
	cout << hex << setfill('0') << setw(2) << "\t0x" << (unsigned int)WtpStatus.DLEN;

	BYTE buffer[RETURNDATALENGTH];
	memset(buffer,0,RETURNDATALENGTH);
	cout << "\t";
	if(memcmp(WtpStatus.Data,buffer,RETURNDATALENGTH) != 0)
		for(int i = 1 ; i <= WtpStatus.DLEN ; i++)
		{
			if(i % 6 == 0)
				cout << endl << "\t\t\t\t\t\t";
			cout << hex << setw(2) << setfill('0')  << "0x" << (unsigned int)WtpStatus.Data[i-1] << " ";
		}
	cout << endl << endl;
}

BYTE CWtpStatus::GetCMD()
{
	return WtpStatus.CMD;
}
BYTE CWtpStatus::GetSEQ()
{
	return this->WtpStatus.SEQ;
}
BYTE CWtpStatus::GetCID()
{
	return this->WtpStatus.CID;
}
BYTE CWtpStatus::GetStatus()
{
	return this->WtpStatus.Status;
}
BYTE CWtpStatus::GetDLEN()
{
	return this->WtpStatus.DLEN;
}
BYTE CWtpStatus::GetFlags()
{
	return this->WtpStatus.Flags;
}
BYTE* CWtpStatus::GetData()
{
	return (this->WtpStatus.Data);
}

void CWtpStatus::SetCMD(BYTE CMD)
{
	this->WtpStatus.CMD = CMD;
}
void CWtpStatus::SetSEQ(BYTE SEQ)
{
	this->WtpStatus.SEQ = SEQ;
}
void CWtpStatus::SetCID(BYTE CID)
{
	this->WtpStatus.CID = CID;
}
void CWtpStatus::SetStatus(BYTE Status)
{
	this->WtpStatus.Status = Status;
}
void CWtpStatus::SetDLEN(BYTE DLEN)
{
	this->WtpStatus.DLEN = DLEN;
}
void CWtpStatus::SetFlags(BYTE Flags)
{
	this->WtpStatus.Flags = Flags;
}
void CWtpStatus::SetData(BYTE Data,int pos)
{
	this->WtpStatus.Data[pos] = Data;
}
void CWtpStatus::SetData(BYTE* Data,int length)
{
	for(int i = 0; i < length ; i++)
		this->WtpStatus.Data[i] = Data[i];
}

void CWtpStatus::ResetWtpStatus()
{
	memset(&WtpStatus,0,sizeof(WTPSTATUS));
}
