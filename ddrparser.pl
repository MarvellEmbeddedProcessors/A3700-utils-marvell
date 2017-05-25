#!/usr/bin/perl

sub parse_dram_data
{
	my ($file_name, $ddr_clk) = @_;
	my $ddr_type;
	my $file;
	my $line;
	my $path = dirname(abs_path($file_name));
	my $memory_size, $num_row_addr_bits, $addr_memory_start;
	my $cap1, $cap2;

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
		} elsif ($name = ~ m/CAP/) {
			# recorde each chip's ddr size value
			if ($cs_num eq 1) {
				   $cap1 = $value;
			} elsif($cs_num eq 2) {
				   $cap2 = $value;
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

	# The below code is to update ddr static configuration's ddr size regitsers value
	# according to the memory size per each chip select in DDR_TOPOLOGY_X.txt file.

	# configure ddr size and ddr memory row number bit according to
	# memory size value for first chip
	if ($cap1 eq 512) {
			$memory_size = 'D';
			$num_row_addr_bits = '5';
	} elsif ($cap1 eq 1024) {
			$memory_size = 'E';
			$num_row_addr_bits = '6';
	} elsif ($cap1 eq 2048) {
			$memory_size = 'F';
			$num_row_addr_bits = '6';
	}

	# The first chip-select:
	# updates length of memory block Bit[19:16]. Although Bit[20] is also one
	# of memory block length bits, Armada37x0 supports 4GB DRAM size at maximum.
	# So this code keeps bit[20] without updating.
	system("sed -ri 's/(0xC0000200\\s+0x...).(....)/\\1$memory_size\\2/g' '${path}/ddr_static.txt'");
	system("sed -ri 's/(0xC0000220\\s++0x.....).(..)/\\1$num_row_addr_bits\\2/g' '${path}/ddr_static.txt'");

	# configure ddr size and ddr memory row number bit according to
	# memory size value for second chip. What's more, it also configure
	# the second ddr chip-select start memory address.
	if ($cs_num eq 2) {
		if ($cap2 eq 512) {
				$memory_size = 'D';
				$num_row_addr_bits = '5';
				$addr_memory_start = '20'
		} elsif ($cap2 eq 1024) {
				$memory_size = 'E';
				$num_row_addr_bits = '6';
				$addr_memory_start = '40'
		} elsif ($cap2 eq 2048) {
				$memory_size = 'F';
				$num_row_addr_bits = '6';
				$addr_memory_start = '80'
		}
		# The second chip-select:
		# update memory start address bank low[31:24] for the second chip-select.
		# Although bit[23] is included for starting address, this script processes this step as 16MB
		# instead of 8MB. So setting $addr_memory_start='20' means 0x20*16MB=512MB.
		system("sed -ri 's/(0xC0000208\\s+0x)..(.).(....)/\\1$addr_memory_start\\2$memory_size\\3/g' '${path}/ddr_static.txt'");
		system("sed -ri 's/(0xC0000224\\s++0x.....).(..)/\\1$num_row_addr_bits\\2/g' '${path}/ddr_static.txt'");
	}

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
