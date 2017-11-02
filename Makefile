LOCAL_VERSION_PATH	:= $(shell pwd)
LOCAL_VERSION_FILE	:= $(LOCAL_VERSION_PATH)/localversion
LOCAL_VERSION_STRING	:= $(shell cat $(LOCAL_VERSION_FILE))

all:
	${MAKE} -C wtmi LOCAL_VERSION_STRING=$(LOCAL_VERSION_STRING)

clean:
	${Q}${MAKE} -C wtmi clean
	@rm -f tim/ddr/ddr_static.txt tim/ddr/clocks_ddr.txt

