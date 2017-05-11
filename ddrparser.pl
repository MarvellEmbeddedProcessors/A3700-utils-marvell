#!/usr/bin/perl

sub parse_dram_data
{
	my ($file_name, $ddr_clk) = @_;
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
		($name, $value) = split(":", $line);
		# trim spaces from both ends
		$name =~ s/^\s+|\s+$//g;
		$value =~ s/^\s+|\s+$//g;

		if ($name =~ m/CS/) {
			$cs_num = $cs_num + 1;
			next;
		} elsif ($name =~ m/DDR_TYPE/) {
			if ($value =~ m/DDR3/) {
				$ddr_type = "ddr3";
			} elsif ($value =~ m/DDR4/) {
				$ddr_type = "ddr4";
			} else {
				print("ERROR: unknown ddr type\n");
				return 1;
			}
		}
	}

	close($file);

	unlink("${path}/ddr_static.txt");

	unless (-e "${path}/${ddr_type}-${ddr_clk}-${cs_num}CS.txt") {
		print("ERROR: unsupported ddr topology\n");
		return 1;
	}

	copy("${path}/${ddr_type}-${ddr_clk}-${cs_num}CS.txt", "${path}/ddr_static.txt");

	return 0;
}

sub usage
{
	print("\nDRAM info parser according to DDR topology map.\n");
	print("Example: ddrparser.pl -i DDR_TM_DB-88F3720-DDR3-Modular-512MB.txt -c 800\n");
	print("Options:\n");
	print("\t-i\tinput file of DDR topology map\n");
	print("\t-c\tDDR clock frequency\n");
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

getopt('i:c:h');

if ($opt_h) {
	usage();
	exit 0;
}

unless ($opt_i) {
	print("ERROR: please specify DDR topology map\n");
	usage();
	exit 1;
}

unless ($opt_c) {
	print("ERROR: please specify DDR clock frequency\n");
	usage();
	exit 1;
}

if (parse_dram_data($opt_i, $opt_c)) {
	exit 1;
}

exit 0;
