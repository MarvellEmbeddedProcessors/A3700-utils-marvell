all:
	${MAKE} -C wtmi

clean:
	${Q}${MAKE} --no-print-directory -C wtmi clean
	@rm -f ddr/tim_ddr/ddr_static.txt ddr/tim_ddr/clocks_ddr.txt

