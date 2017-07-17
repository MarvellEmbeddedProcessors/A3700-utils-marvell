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
#include "WtpException.h"
class CRSAKeys
{
private:
	unsigned int PublicKey[64];
	unsigned int Modulus[64];
	unsigned int PrivateKey[64];

	int KeyLength;

	int RSA_p;
	int RSA_n;
	int IppRSAContextSize;

	int GetRSA_p();
	int GetRSA_n();
	int GetIppRSAContextSize();
	unsigned int* GetPrivateKey();

	void SetRSA_p(int RSA_p);
	void SetRSA_n(int RSA_n);
	void SetIppRSAContextSize(int IppRSAContextSize);
	bool checkKeyLength(unsigned int Key[]);

public:
	unsigned int* GetPublicKey();
	unsigned int* GetModulus();

	int GetKeyLength();


	void SetPublicKey(unsigned int PublicKey[]);
	void SetModulus(unsigned int  Modulus[]);
	void SetPrivateKey(unsigned int  PrivateKeY[]);
	void SetKeyLength(int KeyLength);

	void PrivateKeySignPassword(unsigned int* RSA_M,unsigned int*& RSA_C) throw(CWtpException);

	void printPrivateKey();
	void printPublicKey();
	void printModulus();
	void printKeyLength();

	CRSAKeys(void);
	~CRSAKeys(void);
};
