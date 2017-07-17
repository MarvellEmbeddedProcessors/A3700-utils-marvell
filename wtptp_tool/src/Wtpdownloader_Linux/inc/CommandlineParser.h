/******************************************************************************
 *
 *  (C)Copyright 2005 - 2012 Marvell. All Rights Reserved.
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
#define WTPTP_3_2_X 0
#include "WtpException.h"

using namespace std;

class CWtpException;
class CCommandlineParser
{
private:
	unsigned int FileGetImageID(string fileName) throw(CWtpException);
public:

#if WTPTP_3_2_X // previous version commandline switches now obsolete
	enum eCommandlineOption{VERBOSE			= 'V',
							TIM_BIN_FILE	= 't',
							IMAGE_FILE		= 'f',
							JTAG_KEY		= 'k',
							MESSAGE_MODE	= 'M',
							PORT_TYPE		= 'P',
							INTERFACE_NAME	= 'i',
							USB_PACKET_SIZE	= 'U',
							COMM_PORT		= 'C',
							BAUD_RATE		= 'B',
							PLATFORM_TYPE	= 'T',
							PARTITION_TABLE	= 'p',
							DISABLE_FAST_DOWNLOAD = 'N',
							LOG_FILE		= 'L',
							UPLOAD_DATA     = 'W',
							HELP			= 'h'};
#endif
						enum eCommandlineOption{
							VERBOSE = 'V',
							TIM_BIN_FILE = 'B',
							IMAGE_FILE = 'I',
							JTAG_KEY = 'J',
							MESSAGE_MODE = 'M',
							PORT_TYPE = 'P',
							INTERFACE_NAME = 'G',
							USB_PACKET_SIZE = 'U',
							COMM_PORT = 'C',
							BAUD_RATE = 'R',
							PLATFORM_TYPE = 'T',
							PARTITION_TABLE = 'Q',
							DISABLE_FAST_DOWNLOAD = 'N',
							LOG_FILE = 'L',
							UPLOAD_DATA = 'W',
							GET_PC = 'D',
							FORCE_UART_IMAGE_LOAD = 'O',
							FORCE_BOOTROM_CONSOLE_MODE = 'Y',
							SEND_FF_STREAM = 'y',
							SEND_FF_STREAM_DLOAD = 'o',
					#if USE_USB_DELAY_SWITCHES == 1
							ENUMERATE_DELAY = 'E',
							ENUMERATE_ARRIVE_WAIT = 'A',
							USB_READ_DELAY  = 'Z',
					#endif
							HELP = 'H',
							ENABLER_EXE_PATH = 'X', // overrides default
							PLAINTEXT_WTPCR = 'E'
												};

	enum eJtagEnablerCommandLineOption{
                    JTAG_CONNECT_TEST       = 'J',
                    INQUIRE_TARGET          = 'I',
                    PROC_TYPE		        = 'T',
                    FORCE_RESET				= 'X',

                    ENABLE_JTAG_CMD	        = 'E',
                    CORES_TO_ENABLE         = 'P',
                    JTAG_KEY_FILE           = 'K',
                    SIGNED_UID_KEY_FILE     = 'k',
                    GET_KEY_PROVIDER        = 'G',

                    // Password Access Control word:
                    AC_REQUEST_TYPE         = 'R',

                    // Set Parameter Command
                    SET_PARAMETERS_CMD      = 'S',
                    PARAM_BOOT_STATE        = 'B',
                    PARAM_FORCE_DOWNLOAD    = 'F',
                    PARAM_FORCE_UART_LOG    = 'L',

                    AC_PERMANENTLY_DISABLE  = 'D',
                    AC_FA_STATE             = 'A',

                    // JTAG Config
                    CHANGE_TCK_FREQ         = 'C',

                    USE_WTM_TO_RUN_PROTOCOL = 'U',
					PORTTYPE				= 'Y',
					COMMPORT				= 'N',
                    BAUDRATE                = 'M',
                    //USB_INTERFACENAME       = 'Q',

                    // SLE target controls
                    TARGET_TIMEOUT = 'O',
                    POLLING_DELAY = 'W',

                    TRACE                   = 'Z',
                    ENABLER_VERBOSE         = 'V',

                    ENABLER_HELP	        = 'H'
	};

	void Parse(int argc, char* argv[]) throw(CWtpException);
	void ParseJtagEnablerCommandLine(int argc, char* argv[]) throw (CWtpException);


	void InsufficientParameters(unsigned char eCmdLineOpt) throw(CWtpException);
	void RecalculateUSBPacketSize( );

	void AddImageToList(char* FileName, unsigned int ID) throw (CWtpException);
	void CheckYourOptions() throw(CWtpException);

	CCommandlineParser(void);
	~CCommandlineParser(void);
};
