#/*
#* ***************************************************************************
#* Copyright (C) 2017 Marvell International Ltd.
#* ***************************************************************************
#*
#* Redistribution and use in source and binary forms, with or without
#* modification, are permitted provided that the following conditions are met:
#*
#* Redistributions of source code must retain the above copyright notice, this
#* list of conditions and the following disclaimer.
#*
#* Redistributions in binary form must reproduce the above copyright notice,
#* this list of conditions and the following disclaimer in the documentation
#* and/or other materials provided with the distribution.
#*
#* Neither the name of Marvell nor the names of its contributors may be used
#* to endorse or promote products derived from this software without specific
#* prior written permission.
#*
#* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
#* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#* POSSIBILITY OF SUCH DAMAGE.
#*
#***************************************************************************
#*/

CROSS_CM3 ?= arm-linux-gnueabi-

CC       = $(CROSS_CM3)gcc
AR       = $(CROSS_CM3)ar

ECHO     = @echo

CPUOPTS  = -mthumb -mcpu=cortex-m3 -mlittle-endian

INCLUDE  = -I.

CFLAGS   = -g -gdwarf-2 -Wall -fno-stack-protector $(INCLUDE) -Os $(CPUOPTS)
CPPFLAGS =

CSRC = $(wildcard *.c)
CSRC += $(wildcard ddrcore/*.c)
COBJ   = $(CSRC:.c=.o)

CFG_HEADER = autoconf.h

define filechk
	@set -e;				\
	echo '  CHK     $@';		\
	mkdir -p $(dir $@);			\
	$(filechk_$(1)) > $@.tmp;		\
	if [ -r $@ ] && cmp -s $@ $@.tmp; then	\
		rm -f $@.tmp;			\
	else					\
		echo '  UPD     $@';	\
		mv -f $@.tmp $@;		\
	fi
endef

# FIX-ME:
# replace the relative path to the DDR topology
# files and scripts
DDR_TOPOLOGY_PATH=../../ddr/tim_ddr
DDR_TOPOLOGY_FILE=$(DDR_TOPOLOGY_PATH)/DDR_TOPOLOGY_$(DDR_TOPOLOGY).txt

SCRIPT_PATH=../../script
GET_DDR_PARAMS=$(SCRIPT_PATH)/getddrparams.sh

CONFIG_DDR_TYPE  := $(shell $(GET_DDR_PARAMS) $(DDR_TOPOLOGY_FILE) ddr_type)
CONFIG_BUS_WIDTH := $(shell $(GET_DDR_PARAMS) $(DDR_TOPOLOGY_FILE) ddr_bus_width_index)
CONFIG_SPEED_BIN := $(shell $(GET_DDR_PARAMS) $(DDR_TOPOLOGY_FILE) ddr_speedbin_index)
CONFIG_CS_NUM    := $(shell $(GET_DDR_PARAMS) $(DDR_TOPOLOGY_FILE) ddr_cs_mask)
CONFIG_DEV_CAP   := $(shell $(GET_DDR_PARAMS) $(DDR_TOPOLOGY_FILE) ddr_mem_size_index)

define filechk_autoconf.h
	(echo \#define DEBUG $(DEBUG);\
	echo \#define WTMI_CLOCK $(WTMI_CLOCK);\
	echo \#define CONFIG_DDR_TYPE $(CONFIG_DDR_TYPE);\
	echo \#define CONFIG_BUS_WIDTH $(CONFIG_BUS_WIDTH);\
	echo \#define CONFIG_SPEED_BIN $(CONFIG_SPEED_BIN);\
	echo \#define CONFIG_CS_NUM $(CONFIG_CS_NUM);\
	echo \#define CONFIG_DEV_CAP $(CONFIG_DEV_CAP);)
endef

.SILENT:
.PHONY: $(CFG_HEADER)

all: $(CFG_HEADER) $(COBJ)
	$(AR) -r wtmi_ddr.a $(COBJ)

$(CFG_HEADER):
	$(call filechk,autoconf.h)

%.o: %.c
	$(ECHO) "  CC    $<"
	$(CC) -MMD -MP -c $(CFLAGS) $(CPPFLAGS) -o $@ $(subst .o,.c,$@)

-include $(COBJ:.o=.d)

clean:
	$(ECHO) "  DDRGEN CLEAN"
	@$(RM) -f autoconf.h *.o *.a ddrcore/*.o

