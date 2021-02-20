#!/bin/bash
#
# Get TIM version
#
# $1 - the input txt file which includes TIM version string
# $2 - the output TIM file name
#

VERFILE=$1
OUTFILE=$2

usage () {
	echo ""
	echo "$0 - script for generating the script to get TIM version"
	echo ""
	echo "$0 <ver_txt> <output>"
	echo " <ver_txt>     - the input txt file which includes TIM version string"
	echo " <output>      - the output TIM file name"
	echo ""
	exit 1
}

if [ "$VERFILE" = "" ]; then
	echo "Empty input TIM version file name!"
	usage
fi

if [ "$OUTFILE" = "" ]; then
	echo "Empty output TIM file name!"
	usage
fi

# Set TIM string to TIM version with newlines
VERSTR=$(cat $VERFILE)
STR=$'\r'$'\n'$'\r'$'\n'"TIM-$VERSTR"$'\r'$'\n'
LEN=${#STR}

# Print out TIM string
echo "; TIM-$VERSTR" >> $OUTFILE
for (( i=0; i<$LEN; ))
do
	# TXFIFO has space for 32 characters and it will take 3ms to transmit them on 115200 baudrate
	echo "WAIT_FOR_BIT_SET: 0xC001200C 0x2000 3 ; Wait 3ms for TXFIFO empty" >> $OUTFILE
	for (( j=0; j<32 && i<$LEN; j++, i++ ))
	do
		CHARACTER=${STR:$i:1}
		ASCIIVAL=$(printf "0x%02X" \'"$CHARACTER")
		echo "WRITE: 0xC0012004" $ASCIIVAL "               ; Print character" >> $OUTFILE
	done
done
echo "WAIT_FOR_BIT_SET: 0xC001200C 0x40 3   ; Wait 3ms for TX empty" >> $OUTFILE

exit 0
