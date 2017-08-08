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
#include "WtpImage.h"
#include "WtpDownloadApp.h"
#include "TimParser.h"
#include <sstream>

CTimParser::CTimParser(void){}
CTimParser::~CTimParser(void){}

CTIM* CTimParser::getTimHeader()
{
	return &this->TimHeader;
}

/**
* Method:	ParseTim
* Input:	N/A
* Output:	boolean
* Desc:		Parses the TIM/NTIM input file
*/
void CTimParser::ParseTim() throw (CWtpException)
{
	CMisc misc;
	int ImageIndex = 0, j = 0;
	unsigned int nImages = 0;
	bool IsTim = false;
	unsigned int TimId = 0;
	ifstream ifsTimFile;
	IMAGE_INFO_3_5_0 ImgInf350 = {0};
	CWtpImage *TimImage = 0,*Image = 0;
	stringstream Message;

	if(!theApp.bTimFile)
		throw CWtpException(CWtpException::NO_TIMIMAGE);

    TimImage = theApp.GetImageFromImageList(TIMIDENTIFIER);
	if ( TimImage == 0 )
	{
		string message = "ParseTim failed to locate a TimImage";
		throw CWtpException(CWtpException::FILE_OPEN_FAIL,0,message);
	}

	Message << endl << "ParseTim opening TIM file:" << (TimImage)->GetSImageFileName() << " (Image ID: 0x" << hex << TimImage->GetEImageType() << ")" << endl;
	misc.UserMessagePrintStr(theApp.bVerbose,Message.str());

	ifsTimFile.open(TimImage->GetSImageFileName().c_str(), ios::binary | ios::in);

	if (!ifsTimFile.is_open())
	{
		string message = "Tim File:" + TimImage->GetSImageFileName();
		throw CWtpException(CWtpException::FILE_OPEN_FAIL,0,message);
	}

	ifsTimFile.seekg(sizeof(int), ios::beg);
	ifsTimFile.read((char *)&TimId,sizeof(int));

	if ((TimId & TYPEMASK) != (TIMIDENTIFIER & TYPEMASK))
	{
		ifsTimFile.close();
		throw CWtpException(CWtpException::INVALID_TIM_FILE,TimId,string(""));
	}

	ifsTimFile.seekg(0,ios::beg);
	ifsTimFile.read((char *)&TimHeader,sizeof(CTIM));
	nImages = TimHeader.NumImages;

	Message.str("");
	Message << endl << "Number of Images listed in TIM: " << nImages << endl;
	misc.UserMessagePrintStr(theApp.bVerbose,Message.str());

	Message.str("");
	Message << endl << "TIM image ID list:" << endl;
	misc.UserMessagePrintStr(theApp.bVerbose,Message.str());

	//ImageId = new unsigned int[nImages * sizeof(unsigned int)];

	for (int i = 0; i < (int)nImages; i++)
	{
		if ( TimHeader.VersionFlash3_6.VersionBind.Version >= TIM_3_5_00 )
		{
			// include bigger hash
			ifsTimFile.read ((char *)&ImgInf350,sizeof(IMAGE_INFO_3_5_0));//, 1, hTimFile);
		}
		else if (TimHeader.VersionFlash3_6.VersionBind.Version == TIM_3_4_00)
		{
			// include bigger hash
			ifsTimFile.read ((char *)&ImgInf350,sizeof(IMAGE_INFO_3_4_0));//, 1, hTimFile);
		}
		else if (TimHeader.VersionFlash3_6.VersionBind.Version >= TIM_3_2_00)
		{
			IMAGE_INFO_3_2_0 ImgInf320 = {0};
			// include bigger hash
			ifsTimFile.read ((char *)&ImgInf320,sizeof(IMAGE_INFO_3_2_0));//, 1, hTimFile);
			ImgInf350.ImageID = ImgInf320.ImageID;
			ImgInf350.ImageSize = ImgInf320.ImageSize;
			ImgInf350.FlashEntryAddr = ImgInf320.FlashEntryAddr;
			for(j = 0; j< 8 ; j++)
				ImgInf350.Hash[j] = ImgInf320.Hash[j];
			ImgInf350.HashAlgorithmID = ImgInf320.HashAlgorithmID;
			ImgInf350.ImageSizeToHash = ImgInf320.ImageSizeToHash;
			ImgInf350.LoadAddr = ImgInf320.LoadAddr;
			ImgInf350.NextImageID = ImgInf320.NextImageID;
			ImgInf350.PartitionNumber = ImgInf320.PartitionNumber;
        }
		else
		{
			// Remove last word which contains the partition number
			// for versions prior to 0x030202. Not supported.
			IMAGE_INFO_3_1_0 ImgInf310 = {0};
			ifsTimFile.read ((char *)&ImgInf310,sizeof(IMAGE_INFO_3_1_0));//, 1, hTimFile);

			ImgInf350.ImageID = ImgInf310.ImageID;
			ImgInf350.ImageSize = ImgInf310.ImageSize;
			ImgInf350.FlashEntryAddr = ImgInf310.FlashEntryAddr;
			for(int j = 0; j< 8 ; j++)
				ImgInf350.Hash[j] = ImgInf310.Hash[j];
			ImgInf350.HashAlgorithmID = ImgInf310.HashAlgorithmID;
			ImgInf350.ImageSizeToHash = ImgInf310.ImageSizeToHash;
			ImgInf350.LoadAddr = ImgInf310.LoadAddr;
			ImgInf350.NextImageID = ImgInf310.NextImageID;
			ImgInf350.PartitionNumber = 0;
		}

		//ImageId[i] = ImgInf350.ImageID;
		Message.str("");
		Message << "Image ID: ";
		for (j = 3; j >= 0; j--)
			Message << (char)(ImgInf350.ImageID >> (j * 8));
		Message << " (0x" << hex << ImgInf350.ImageID << ")" << endl;
		misc.UserMessagePrintStr(true,Message.str());

        Image = theApp.GetImageFromImageList(ImgInf350.ImageID);
		if(Image == 0)
		{
			Message.str("");
			Message << "0x" << hex << ImgInf350.ImageID;
            if (!theApp.bJtagFile)
			    throw CWtpException(CWtpException::NOT_ENUF_FILES,0,Message.str());
            else if (theApp.bJtagFile)
                Message << endl << "\nWarning: Image not provided on command-line. Cannot download this image. \n" << endl;
        }
	}
	ifsTimFile.close();
}
