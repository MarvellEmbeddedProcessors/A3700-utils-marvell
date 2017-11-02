#!/usr/bin/perl

sub parse_dram_data
{
	my ($file_name) = @_;
	my $ddr_type;
	my $file;
	my $line;
	my $path = dirname(abs_path($file_name));

	unless (open ($file, "<$file_name")) {
		print("ERROR: failed to open the file - $file_name\n");
		return 1;
	}

	while (defined($line = readline($file))) {
		chomp $line;
		($name, $value) = split("=", $line);
		# trim spaces from both ends
		$name =~ s/^\s+|\s+$//g;
		$value =~ s/^\s+|\s+$//g;

		if ($name =~ m/ddr_type/) {
			$ddr_type = $value;
		}
	}
	close($file);

	unlink("${path}/ddr_static.txt");

	# to change some specific register, we could add "-r" option like below:
	# ./ddr_tool -i DDR_TOPOLOGY_0.txt -o ddr_static.txt -r 0xC0000380=0x0007A120
	system("${path}/ddr_tool -i $file_name -o ${path}/ddr_static.txt");

	unlink("${path}/clocks_ddr.txt");
	# ddr_type	DDR3=0, DDR4=1
	if ($ddr_type eq "0") {
		copy("${path}/clocks-ddr3.txt", "${path}/clocks_ddr.txt");
	} elsif ($ddr_type eq "1") {
		copy("${path}/clocks-ddr4.txt", "${path}/clocks_ddr.txt");
	}

	return 0;
}

sub usage
{
	print("\nDRAM info parser according to DDR topology map.\n");
	print("Example: ddrparser.pl -i DDR_TOPOLOGY_0.txt\n");
	print("Options:\n");
	print("\t-i\tinput file of DDR topology map\n");
	print("\n");
}

# Main
use strict;
use warnings;
use Getopt::Std;
use File::Basename;
use File::Copy;
use Cwd 'abs_path';

use vars qw($opt_i $opt_c $opt_h);

our $cs_num = 0;

getopt('i:h');

if ($opt_h) {
	usage();
	exit 0;
}

unless ($opt_i) {
	print("ERROR: please specify DDR topology map\n");
	usage();
	exit 1;
}

if (parse_dram_data($opt_i)) {
	exit 1;
}

exit 0;
