/******************************************************************************
 *
 *  (C)Copyright 2005 - 2013 Marvell. All Rights Reserved.
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



#include "CryptoPP_L_interface.h"
namespace CryptoPP {

size_t PK_Signer_Hash::SignHash(CryptoPP::RandomNumberGenerator &rng, const byte *hashMessage, byte *signature) const
{
	std::auto_ptr<PK_MessageAccumulator> m(NewSignatureAccumulator(rng));
	return SignHashAndRestart(rng, *m,(byte *) hashMessage, signature, false);
}

size_t TF_SignerBase_Hash::SignHashAndRestart(RandomNumberGenerator &rng, PK_MessageAccumulator &messageAccumulator,byte* hash, byte *signature, bool restart) const
{
	PK_MessageAccumulatorBase &ma = static_cast<PK_MessageAccumulatorBase &>(messageAccumulator);
	HashIdentifier id = GetHashIdentifier();
	const MessageEncodingInterface &encoding = GetMessageEncodingInterface();

	if (MessageRepresentativeBitLength() < encoding.MinRepresentativeBitLength(id.second, ma.AccessHash().DigestSize()))
		throw PK_SignatureScheme::KeyTooShort();

	SecByteBlock representative(MessageRepresentativeLength());
	if (ma.m_recoverableMessage.size() > 0)
		throw NotImplemented("CryptoPP_L_interface: Use native CryptoPP classes for recoverable message size > 0");
	
	encoding.ComputeMessageRepresentative(rng,
			hash, ma.m_recoverableMessage.size(),
			ma.AccessHash(), id, ma.m_empty,
			representative, MessageRepresentativeBitLength());
	ma.m_empty = true;

	Integer r(representative, representative.size());
	size_t signatureLength = SignatureLength();
	GetTrapdoorFunctionInterface().CalculateRandomizedInverse(rng, r).Encode(signature, signatureLength);
	return signatureLength;
}

// Function computing message representative for PKCS1v15
void PKCS1v15_SignatureMessageEncodingMethod_Hash::ComputeMessageRepresentative(RandomNumberGenerator &rng,
		const byte *hashMsg, size_t recoverableMessageLength,
		HashTransformation &hash, HashIdentifier hashIdentifier, bool messageEmpty,
		byte *representative, size_t representativeBitLength) const
{

	size_t pkcsBlockLen = representativeBitLength;
	// convert from bit length to byte length
	if (pkcsBlockLen % 8 != 0)
	{
		representative[0] = 0;
		representative++;
	}
	pkcsBlockLen /= 8;

	representative[0] = 1;   // block type 1

	unsigned int digestSize = hash.DigestSize();
	byte *pPadding = representative + 1;
	byte *pDigest = representative + pkcsBlockLen - digestSize;
	byte *pHashId = pDigest - hashIdentifier.second;
	byte *pSeparator = pHashId - 1;

	// pad with 0xff
	memset(pPadding, 0xff, pSeparator - pPadding);
	*pSeparator = 0;
	memcpy(pHashId, hashIdentifier.first, hashIdentifier.second);
	memcpy(pDigest,hashMsg,digestSize);
}

// Function computing message representative for PKCSv22
template<>
void PSSR_MEM_Hash<false>::ComputeMessageRepresentative(RandomNumberGenerator &rng,
		const byte *hashMsg, size_t recoverableMessageLength,
		HashTransformation &hash, HashIdentifier hashIdentifier, bool messageEmpty,
		byte *representative, size_t representativeBitLength) const
{

	const size_t u = hashIdentifier.second + 1;
	const size_t representativeByteLength = BitsToBytes(representativeBitLength);
	const size_t digestSize = hash.DigestSize();
	const size_t saltSize = SaltLen(digestSize);
	byte *const h = representative + representativeByteLength - u - digestSize;

	SecByteBlock digest(digestSize), salt(saltSize);

	digest.Assign(hashMsg, digestSize);
	rng.GenerateBlock(salt, saltSize);
	// compute H = hash of M'
	byte c[8];
	PutWord(false, BIG_ENDIAN_ORDER, c, (word32)SafeRightShift<29>(recoverableMessageLength));
	PutWord(false, BIG_ENDIAN_ORDER, c + 4, word32(recoverableMessageLength << 3));
	hash.Update(c, 8);
	hash.Update(digest, digestSize);
	hash.Update(salt, saltSize);
	hash.Final(h);

	// compute representative
	GetMGF().GenerateAndMask(hash, representative, representativeByteLength - u - digestSize, h, digestSize, false);
	byte *xorStart = representative + representativeByteLength - u - digestSize - salt.size() - recoverableMessageLength - 1;
	xorStart[0] ^= 1;
	xorbuf(xorStart + 1 + recoverableMessageLength, salt, salt.size());
	memcpy(representative + representativeByteLength - u, hashIdentifier.first, hashIdentifier.second);
	representative[representativeByteLength - 1] = hashIdentifier.second ? 0xcc : 0xbc;
	if (representativeBitLength % 8 != 0)
		representative[0] = (byte)Crop(representative[0], representativeBitLength % 8);
}
	


// Function comuting message representative for ECDSA	
void DL_SignatureMessageEncodingMethod_DSA_Hash::ComputeMessageRepresentative(RandomNumberGenerator &rng,
		const byte *hashMsg, size_t recoverableMessageLength,
		HashTransformation &hash, HashIdentifier hashIdentifier, bool messageEmpty,
		byte *representative, size_t representativeBitLength) const
{
	const size_t representativeByteLength = BitsToBytes(representativeBitLength);
	const size_t digestSize = hash.DigestSize();
	const size_t paddingLength = SaturatingSubtract(representativeByteLength, digestSize);

	memset(representative, 0, paddingLength);
	memcpy(representative + paddingLength, hashMsg, digestSize);
	if (digestSize * 8 > representativeBitLength)
	{
		Integer h(representative, representativeByteLength);
		h >>= representativeByteLength * 8 - representativeBitLength;
		h.Encode(representative, representativeByteLength);
	}
}

}

