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
#include "WTPDEF.h"

class CWtpCommand
{
protected:
	static int iSequence;
	WTPCOMMAND WtpCmd;
	void ResetWtpCommand();

public:
	CWtpCommand(void);
	virtual ~CWtpCommand(void);


	WTPCOMMAND GetWtpCommand();
	void print();

	unsigned char GetCMD();
	unsigned char GetSEQ();
	unsigned char GetCID();
	unsigned char GetFlags();
	long GetLEN();
	unsigned char* GetData();

	void SetCMD(unsigned char CMD);
	void SetSEQ(unsigned char SEQ);
	void SetCID(unsigned char CID);
	void SetFlags(unsigned char Flags);
	void SetData(unsigned char Data,int pos);
	void SetData(unsigned char *Data,int length);
};

class CPlainTextWTPCR: public CWtpCommand{
public:
	CPlainTextWTPCR();
	~CPlainTextWTPCR();
};

class CPreambleCommand: public CWtpCommand{
public:
	CPreambleCommand();
	~CPreambleCommand();
};

class CGetVersionCommand:public CWtpCommand{
public:
	CGetVersionCommand();
	~CGetVersionCommand();
};

class CSelectImageCommand:public CWtpCommand{
public:
	CSelectImageCommand();
	~CSelectImageCommand();
};

class CVerifyImageCommand:public CWtpCommand{
public:
	CVerifyImageCommand(BYTE AckNoAck);
	~CVerifyImageCommand();
};

class CDataHeaderCommand:public CWtpCommand{
public:
	CDataHeaderCommand(bool bFastDownload);
	~CDataHeaderCommand();
};

class CDataCommand:public CWtpCommand{
public:
	CDataCommand(unsigned char* pData, int Length);
	~CDataCommand();
};

class CDoneCommand:public CWtpCommand{
public:
	CDoneCommand();
	~CDoneCommand();
};

class CDisconnectCommand:public CWtpCommand{
public:
	CDisconnectCommand();
	~CDisconnectCommand();
};

class CPasswordCommand:public CWtpCommand{
public:
	CPasswordCommand();
	~CPasswordCommand();
};

class CPublicKeyCommand:public CWtpCommand{
public:
	CPublicKeyCommand();
	~CPublicKeyCommand();
};

class CSignedPasswordCommand:public CWtpCommand{
public:
	CSignedPasswordCommand();
	virtual ~CSignedPasswordCommand();


};
class CMessageCommand:public CWtpCommand{
public:
	CMessageCommand();
	~CMessageCommand();
};

class CTargetProtocolVersionCommand:public CWtpCommand{
public:
	CTargetProtocolVersionCommand();
	~CTargetProtocolVersionCommand();
};

class CGetParametersCommand:public CWtpCommand{
public:
	CGetParametersCommand();
	~CGetParametersCommand();
};

class CUploadDataHeaderCommand:public CWtpCommand{
public:
	CUploadDataHeaderCommand();
	~CUploadDataHeaderCommand();
};

class CUploadDataCommand:public CWtpCommand{
public:
	CUploadDataCommand();
	~CUploadDataCommand();
};

class CJtagOverMediaDataCommand:public CWtpCommand{
public:
	CJtagOverMediaDataCommand(unsigned char *pData, int Length);
	~CJtagOverMediaDataCommand();
};

class CGetPCCommand:public CWtpCommand{
public:
	CGetPCCommand();
	~CGetPCCommand();
};

//class CDebugBootCommand:public CWtpCommand{
//public:
//	CDebugBootCommand();
//	virtual ~CDebugBootCommand();
//};
//
//class COtpViewCommand:public CWtpCommand{
//public:
//	COtpViewCommand();
//	virtual ~COtpViewCommand();
//};