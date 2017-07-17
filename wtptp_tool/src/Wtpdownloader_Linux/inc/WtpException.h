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
#include <string>
using namespace std;

class CWtpException
{
private:
	int ErrorNumber;
	unsigned int intArgs;
	string stringArgs;

public:
	enum errorList{
		INVALID_TIM_FILE,
		COMMANDLINE_PARSE_ERROR,
		INVALID_COMMANDLINE_COMBINATION,
		OPENLOGFILE,
		NOT_ENUF_FILES,
		NO_MIN_ARGS,
		NO_TIMIMAGE,
		ZEROFILESDOWNLOAD,
		NO_PARTITION_TABLE,
		NO_IMAGE,
		ABORTED_DOWNLOAD_FILE,
		FILE_OPEN_FAIL,
		INSUFFICIENT_BYTES_READ,
		ABORTED_MASTERBLOCKHEADER,
		READ_MASTERBLOCKHEADER,
		READ_DEVICEHEADERS,
		UNKNOWN_DEVICENUMBER,
		UNKNOWN_IMAGENUMBER,
		NO_KEYFILE,
		READ_KEYFILE,
		INVALID_COMMANDLINE_OPTION,
		INVALID_PORTTYPE,
		INVALID_MESSAGEMODE,
		PORTTYPE_WITH_MESSAGE_ONLY,
		NO_FEATURE_IN_DISABLED_MSG_MODE,
		INVALID_DECLARATION_STATEMENT,
		UPLOAD,
		//UPLOAD_DATA_EMPTY,
		OPEN_UPLOADSPECFILE,
		//WRITE_BIN_OUTPUTFILE,
		//OPEN_BINOUTPUTFILE,
		//OPEN_BINCOMPAREFILE,
		//ACCESS_BINCOMPAREFILE,
		//READ_BINCOMPAREFILE,
		RSAKEYLENGTH,
		RSAFLASHTYPE,
		RSAISSUEDATE,
		RSAOEMUNIQUEID,
		RSAHASHVALUELENGTH,
		RSAEXPONENT,
		RSAPRIVATEKEY,
		RSAPUBLICKEY,
		RSAMODULUS,
		IPPSRSAGETSIZE,
		IPPSRSAINIT,
		IPPSBIGNUMGETSIZE,
		IPPSBIGNUMINIT,
		IPPSRSASETKEY,
		IPPSRSAENCRYPT,
		IPPSGET_BN,
		IPPSSET_BN,
		CREATEFILE,
		SETUPDIGETCLASSDEVS,
		SETUPDIENUMDEVICEINTERFACES,
		MEM_ALLOC_PSP_DEVICE_INTERFACE_DETAIL_DATA,
		SETUPDIDEVICEINTERFACEDETAIL,
		OPENUSBPORT,
		READUSBPORT,
		WRITEUSBPORT,
		OPENUARTPORT,
		READUARTPORT,
		WRITEUARTPORT,
		GETCOMMSTATE,
		GETCOMMCONFIG,
		SETCOMMSTATE,
		SETCOMMMASK,
		SETUPCOMM,
		SETCOMMTIMEOUTS,
		CLEARCOMMERROR,
		NACK_WTPCR,
		NACK_FORCEUART,
		NACK_FORCECONSOLE,
		NACK_PREAMBLE,
		NACK_GETVERSION,
		NACK_SELECTIMAGE,
		NACK_VERIFYIMAGE,
		NACK_DATAHEADER,
		NACK_DATA,
		NACK_DONE,
		NACK_DISCONNECT,
		PLATFORM_DISCONNECT,
		NACK_PUBLICKEY,
		NACK_PASSWORD,
		NACK_SIGNEDPASSWORD,
	/*	NACK_OTPVIEW,
		NACK_DEBUGBOOT,*/
		NACK_GETPROTOCOLVERSION,
		NACK_GETPARAMETERS,
		NACK_UPLOADDATAHEADER,
		NACK_UPLOADDATA,
		NACK_GETPC,
		//JtagEnabler lauch errors
		JTAGENABLER_FAILED,
		CREATEPIPE,
		SETHANDLEINFORMATION,
		CREATEPROCESS,
		NOTSUPPORTED
	};

	void setErrorNumber(int ErrorNumber);
	void setIntArgs(int intArgs);
	void setStringArgs(string& stringArgs);

	int getErrorNumber();
	int getIntArgs();
	string& getStringArgs();

	void printError();

	CWtpException(void);
	CWtpException(errorList ErrorNumber);
	CWtpException(errorList ErrorNumber,int intArgs, string stringArgs);
	~CWtpException(void);
};
