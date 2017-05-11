########################################################################################
#
#  (C)Copyright 2005 - 2009 Marvell. All Rights Reserved.
#  
#  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MARVELL.
#  The copyright notice above does not evidence any actual or intended 
#  publication of such source code.
#  This Module contains Proprietary Information of Marvell and should be
#  treated as Confidential.
#  The information in this file is provided for the exclusive use of the 
#  licensees of Marvell.
#  Such users have the right to use, modify, and incorporate this code into 
#  products for purposes authorized by the license agreement provided they 
#  include this notice and the associated copyright notice with any such
#  product. 
#  The information in this file is provided "AS IS" without warranty.
#
########################################################################################
# /********************************************************************************
# Marvell BSD License Option
#
# If you received this File from Marvell, you may opt to use, redistribute and/or
# modify this File under the following licensing terms.
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
# 
# 	* Redistributions of source code must retain the above copyright notice,
# 	  this list of conditions and the following disclaimer.
# 
# 	* Redistributions in binary form must reproduce the above copyright
# 	  notice, this list of conditions and the following disclaimer in the
# 	  documentation and/or other materials provided with the distribution.
# 
# 	* Neither the name of Marvell nor the names of its contributors may be
# 	  used to endorse or promote products derived from this software without
# 	  specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# *******************************************************************************/



####	Makefile for the TBB Tool - Linux version	###

CC 				= 	g++
BASE_DIR		:=	.
BASE_DIR_ABS	:=	$(shell pwd)

ifeq "$(LIBDIR)" ""
LIBDIR			+=		"$(BASE_DIR)/lib/cryptopp565"
endif

SRCDIR			+=		$(BASE_DIR)/src
OUTDIR			=		$(BASE_DIR)/release

INC_DIR_FLAGS	+=		-I$(BASE_DIR)/inc -I$(BASE_DIR)
LIB_DIR_FLAGS	+=		-L$(LIBDIR)
LIB_FLAGS 		+=		-lcryptopp
DEF_FLAGS		+=		-DLIBDIR=$(LIBDIR)
CC_FLAGS 		+=		-std=c++11 --debug

OBJS 			+=	$(OUTDIR)/Aspen_Strings.o \
					$(OUTDIR)/AutoBindERD.o \
					$(OUTDIR)/BbmtERD.o \
					$(OUTDIR)/BlfHeader.o \
					$(OUTDIR)/CommandLineParser.o \
					$(OUTDIR)/ConsumerID.o \
					$(OUTDIR)/CoreResetERD.o \
					$(OUTDIR)/CoreReleaseERD.o \
					$(OUTDIR)/CryptoPP_L_interface.o \
					$(OUTDIR)/CTIM.o \
					$(OUTDIR)/DDRInitialization.o \
					$(OUTDIR)/DDROperations.o \
					$(OUTDIR)/DDRTypeERD.o \
					$(OUTDIR)/DigitalSignature.o \
					$(OUTDIR)/ECCKey.o \
					$(OUTDIR)/ErdBase.o \
					$(OUTDIR)/EscapeSeqERD.o \
					$(OUTDIR)/EscapeSeqV2ERD.o \
					$(OUTDIR)/ExtendedReservedData.o \
					$(OUTDIR)/FlashGeometryReqERD.o \
					$(OUTDIR)/GpioSetERD.o \
					$(OUTDIR)/GenPurposePatch.o \
					$(OUTDIR)/GPPOperations.o \
					$(OUTDIR)/ImageBuilder.o \
					$(OUTDIR)/ImageDescription.o \
					$(OUTDIR)/ImageMapERD.o \
					$(OUTDIR)/Instructions.o \
					$(OUTDIR)/Key.o \
					$(OUTDIR)/KeyGenSpec.o \
					$(OUTDIR)/Mmp2_Strings.o \
					$(OUTDIR)/Mmp3_Strings.o \
					$(OUTDIR)/OnDemandBootERD.o \
					$(OUTDIR)/PartitionData.o \
					$(OUTDIR)/PartitionTable.o \
					$(OUTDIR)/PinERD.o \
					$(OUTDIR)/ReservedPackageData.o \
					$(OUTDIR)/ResumeDdrERD.o \
					$(OUTDIR)/ROMResumeERD.o \
					$(OUTDIR)/RSAKey.o \
					$(OUTDIR)/Sdram_Strings.o \
					$(OUTDIR)/TBBV4.o \
					$(OUTDIR)/TbrXferERD.o \
					$(OUTDIR)/TimDescriptor.o \
					$(OUTDIR)/TimDescriptorLine.o \
					$(OUTDIR)/TimDescriptorParser.o \
					$(OUTDIR)/TimLib.o \
					$(OUTDIR)/TrustedImageBuilder.o \
					$(OUTDIR)/TrustedTimDescriptorParser.o \
					$(OUTDIR)/Ttc1_Strings.o \
					$(OUTDIR)/TzInitialization.o \
					$(OUTDIR)/TzOperations.o \
					$(OUTDIR)/UartERD.o \
					$(OUTDIR)/UsbERD.o \
					$(OUTDIR)/UsbVendorReqERD.o \




TARGET 			=	$(OUTDIR)/TBB_linux

$(OUTDIR)/%.o: $(SRCDIR)/%.cpp			; $(CC) $(DEF_FLAGS) $(INC_DIR_FLAGS) $(LIB_DIR_FLAGS) $(CC_FLAGS) -c $< -o $@ 


$(TARGET): $(OUTDIR) $(OBJS)
	$(CC) $(DEF_FLAGS) $(INC_DIR_FLAGS) $(LIB_DIR_FLAGS) $(CC_FLAGS) $(OBJS) -o $(TARGET) $(LIB_FLAGS)

$(OUTDIR):
	mkdir -p $(OUTDIR)

clean:
	rm -rf $(OUTDIR)
	rm -rf *~




