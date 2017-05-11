all:
	${MAKE} -C wtmi

clean:
	${Q}${MAKE} --no-print-directory -C wtmi clean
	@rm -f atf-ntim.txt ddr_static.txt
