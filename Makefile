ROOT_PATH		:= $(shell pwd)
LOCAL_VERSION_FILE	:= $(ROOT_PATH)/localversion
LOCAL_VERSION_STRING	:= $(shell cat $(LOCAL_VERSION_FILE))
TIM_DDR_PATH 		:= $(ROOT_PATH)/tim/ddr
SCRIPT_PATH		:= $(ROOT_PATH)/script

DDR_TOPOLOGY		?= 0
DDR_TOPOLOGY_FILE 	:= $(TIM_DDR_PATH)/DDR_TOPOLOGY_$(DDR_TOPOLOGY).txt
$(if $(wildcard $(DDR_TOPOLOGY_FILE)),,$(error "DDR_TOPOLOGY=$(DDR_TOPOLOGY) is invalid"))
GET_DDR_PARAMS=$(SCRIPT_PATH)/getddrparams.sh
CONFIG_DDR_TYPE=$(shell ($(GET_DDR_PARAMS) $(DDR_TOPOLOGY_FILE) ddr_type))

ifeq ($(CONFIG_DDR_TYPE), 1)
	DDR_TYPE = DDR4
else ifeq ($(CONFIG_DDR_TYPE), 0)
	DDR_TYPE = DDR3
endif

all: mv_ddr WTMI

$(MV_DDR_PATH)/a3700_tool: FORCE
ifndef MV_DDR_PATH
	$(error "'MV_DDR_PATH' is not set")
endif
	${Q}${MAKE} -C ${MV_DDR_PATH} PLATFORM=a3700 DDR_TYPE=$(DDR_TYPE)

$(TIM_DDR_PATH)/ddr_tool: $(MV_DDR_PATH)/a3700_tool
	@cp -f ${MV_DDR_PATH}/a3700_tool $(TIM_DDR_PATH)/ddr_tool

$(TIM_DDR_PATH)/ddr_tool.verstr: $(MV_DDR_PATH)/a3700_tool
	@echo mv_ddr-$(shell sed 's/^mv_ddr-//' $(MV_DDR_PATH)/localversion 2>/dev/null || echo 'unknown')$(if $(shell git -C $(MV_DDR_PATH) rev-parse --git-dir 2>/dev/null),-g$(shell git -C $(MV_DDR_PATH) rev-parse --verify --quiet --short HEAD 2>/dev/null)$(shell git -C $(MV_DDR_PATH) diff-index --quiet HEAD || echo -d)) > $@

mv_ddr: $(TIM_DDR_PATH)/ddr_tool $(TIM_DDR_PATH)/ddr_tool.verstr

WTMI:
	${MAKE} -C wtmi LOCAL_VERSION_STRING=$(LOCAL_VERSION_STRING)

clean:
	${Q}${MAKE} -C wtmi clean
	@rm -f tim/ddr/ddr_static.txt tim/ddr/clocks_ddr.txt tim/ddr/ddr_tool tim/ddr/ddr_tool.verstr tim/ddr/ddr_static.txt.info
ifdef MV_DDR_PATH
	${Q}${MAKE} PLATFORM=a3700 --no-print-directory -C ${MV_DDR_PATH} DDR_TYPE=$(DDR_TYPE) clean
endif

FORCE:;
.PHONY: all mv_ddr WTMI clean FORCE
