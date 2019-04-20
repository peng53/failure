#/usr/bin/perl
# Script that reads collectives stored in following manner:
#	name;NAME
#	tray;12345
#	123
#	...
#	tray;12456
#	...
# To a string and a hash of arrays of numbers.
#
use warnings;
use strict;

sub lineType {
	# Returns which type the line matches.
	my $line = shift;
	return 'comment'  if ($line =~ /^#/);
	return 'name'     if ($line =~ /^name;/);
	return 'tray'     if ($line =~ /^tray;/);
	return 'numbers'  if ($line =~ /^\d+;\d/);
	return 'number'   if ($line =~ /^\d+/);
	return 'unknown';
}

sub getCollectiveName {
	# Returns the name of the collective or empty str.
	my $line = shift;
	($line =~ /^name;(.+)/) ? $1 : "";
}

sub getTrayNumber {
	# Returns tray number for a given line.
	my $line = shift;
	($line =~ /^tray;(\d+)/) ? $1 : "";
}

sub getNumbers {
	my $numbers = shift;
	my @numbers = split ';', $numbers;
	return \@numbers;
}

sub printCollectiveHash {
	# Prints CollectiveHash given its name and values.
	my $name = shift;
	my $trays = shift;
	my %trays = %$trays;
	print '_'x10, "\nCollective: $name\n", '='x10, "\n";
	foreach (keys %trays) {
		print "<|Tray: $_\n";
		foreach my $number (@{$trays{$_}}) {
			print " |  $number\n";
		}
		print " |>\n";
	}
	print '='x10, "\n";
}

sub getCollectiveHash {
	# Returns CollectiveHash name and values given a
	# filename to read from.
	open(my $fh, shift) or die "Can't open file.";
	my $line = <$fh>;
	chomp($line);
	die "Couldn't get name line." unless lineType($line) eq 'name';
	my $name = getCollectiveName($line);
	my %trays;
	my $tray_number;
	my $type;
	while ($line = <$fh>) {
		chomp($line);
		$type = lineType($line);
		next if ($type eq 'comment');
		if ($type eq 'tray') {
			$tray_number = getTrayNumber($line);
		} elsif ($type eq 'numbers') {
			push(
				@{$trays{$tray_number}},
				@{getNumbers($line)}
			);
		} elsif ($type eq 'number') {
			push(@{$trays{$tray_number}},$line);
		} else {
			die "Unexcepted line type found.";
		}
	}
	return ($name, \%trays);
}

sub main {
	my ($name, $trays) = getCollectiveHash(shift);
	my %trays = %$trays;
	printCollectiveHash($name, \%trays);
}

die "No arguments were provided." unless @ARGV>=1;
main(@ARGV);
