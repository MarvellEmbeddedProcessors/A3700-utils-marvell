#/*
# * ***************************************************************************
# * Copyright (C) 2015 Marvell International Ltd.
# * ***************************************************************************
# * This program is free software: you can redistribute it and/or modify it
# * under the terms of the GNU General Public License as published by the Free
# * Software Foundation, either version 2 of the License, or any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program.  If not, see <http://www.gnu.org/licenses/>.
# * ***************************************************************************
#*/

CROSS_CM3 ?= arm-linux-gnueabi-

LD       = $(CROSS_CM3)ld
CC       = $(CROSS_CM3)gcc
AS       = $(CROSS_CM3)as
OBJCOPY  = $(CROSS_CM3)objcopy
OBJDUMP  = $(CROSS_CM3)objdump
HOSTCC   = gcc

RM       = @rm -rf
MKDIR    = @mkdir -p
CD       = @cd
MV       = @mv
CAT      = @cat
PWD      = @pwd
ECHO     = @echo

CPUOPTS  = -mthumb -mcpu=cortex-m3 -mlittle-endian

BINPATH  = build
LDSCRIPT = wtmi.ld
INCLUDE  = -I.

BUILD_STRING	:=	$(shell git log -n 1 --pretty=format:"%h")
LOCAL_VERSION_PATH	:=	..
LOCAL_VERSION_FILE	:=	$(LOCAL_VERSION_PATH)/localversion
LOCAL_VERSION_STRING	:=	$(shell cat $(LOCAL_VERSION_FILE))
VERSION_STRING	:=	$(LOCAL_VERSION_STRING)-$(BUILD_STRING)

CFLAGS   = -g -gdwarf-2 -Wall -fno-stack-protector $(INCLUDE) -Os $(CPUOPTS) -DVERSION=\"$(VERSION_STRING)\"
CPPFLAGS =
ASFLAGS  = -g --gdwarf-2 --warn $(INCLUDE) $(CPUOPTS)
LDFLAGS  = -nostdlib -T $(LDSCRIPT) -Xlinker "--build-id=none"


CSRC = $(wildcard *.c)
ASRC = $(wildcard *.S)

COBJ   = $(CSRC:.c=.o)
AOBJ   = $(ASRC:.S=.o)

WTMI_DDR_PATH = ../ddr/wtmi_ddr
WTMI_DDR_LIB  = $(WTMI_DDR_PATH)/wtmi_ddr.a

CLOCKSPRESET		?= CPU_800_DDR_800
DDR_TOPOLOGY		?= 0

ifeq ($(CLOCKSPRESET), CPU_600_DDR_600)
		WTMI_CLOCK = 0
else ifeq ($(CLOCKSPRESET), CPU_800_DDR_800)
		WTMI_CLOCK = 1
else ifeq ($(CLOCKSPRESET), CPU_1000_DDR_800)
		WTMI_CLOCK = 2
else ifeq ($(CLOCKSPRESET), CPU_1200_DDR_750)
		WTMI_CLOCK = 3
endif

COMPILER_FOUND  = $(shell which $(CC))
TOOLCHAIN_CHECK = \
	if [ -z "$(COMPILER_FOUND)" ]; then \
		echo "***************************************************" >&2; \
		echo "ARM GCC toolchain installation is missing!" >&2; \
		echo "Please install GNU ARM toolchain suitable for" >&2; \
		echo "compiling code for Cortex M3 targets" >&2; \
		echo "On Debian/Ubuntu run the following command for" >&2; \
		echo "the default toolchain installation:" >&2; \
		echo " sudo apt-get install gcc-arm-linux-gnueabi" >&2; \
		echo "If non-default toolchain is used, please export" >&2; \
		echo "the CROSS_CM3 environment variable pointin to it." >&2; \
		echo "For instance, with BASH shell:" >&2; \
		echo " export CROSS_CM3=/opt/arm-cross/bin/arm-linux-gnueabi" >&2; \
		echo "***************************************************" >&2; \
		exit 1; \
	fi

.SILENT:

all: tools wtmiddr $(COBJ) $(AOBJ) $(BINPATH)
	$(ECHO) "  LD    $(MAKECMDGOALS)"
	$(CC) $(LDFLAGS)  $(AOBJ)  $(COBJ) $(WTMI_DDR_LIB) -o $(BINPATH)/wtmi.elf
	$(OBJCOPY) -S -O binary $(BINPATH)/wtmi.elf $(BINPATH)/wtmi.bin
	@if [ `uname -m` = "x86_64" ]; then $(OBJDUMP) -D -S $(BINPATH)/wtmi.elf > $(BINPATH)/wtmi.dis; fi

%.o: %.c
	$(ECHO) "  CC    $<"
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $(subst .o,.c,$@)

%.o: %.S
	$(ECHO) "  AS    $<"
	$(AS) $(ASFLAGS) -o $@ $(subst .o,.S,$@)

$(BINPATH):
	@$(MKDIR) $(BINPATH)

clean:
	$(ECHO) "  CLEAN"
	@$(RM) -rf $(BINPATH)
	@$(RM) -f *.o *.elf *.bin *dis
	@${Q}${MAKE} --no-print-directory -C $(WTMI_DDR_PATH) clean

tools:
	@$(TOOLCHAIN_CHECK)

wtmiddr:
	${MAKE} -C $(WTMI_DDR_PATH) DEBUG=$(DEBUG) WTMI_CLOCK=$(WTMI_CLOCK) DDR_TOPOLOGY=$(DDR_TOPOLOGY)
