#!/bin/perl

use strict;
use warnings;
use Fcntl;
use GDBM_File;
use POSIX;

require './collective_plain.pl';

sub loadIt {
	my $hash = shift;
	my $dfile = shift;
	tie %$hash, 'GDBM_File', $dfile, O_CREAT|O_RDWR, 0644 or die 'Couldn\'t open.';
}
sub basicPrintHash {
	# Just prints key-value pairs in no particular order.
	my $hash = shift;
	while (my ($k, $v) = each %$hash) {
		print "$k : $v\n";
	}
}

my %dhash;
loadIt(\%dhash, 'dfile.dbm');
tied %dhash or die;

if (! keys %dhash) {
	# Have to populate file/hash.
	print "Reading from file.\n";
	my ($name, $trays) = getCollectiveHash(shift);
	$dhash{'location'} = $name;
	while (my ($tray, $codes) = each %$trays) {
		foreach (@$codes) {
			$dhash{$_} = $tray;
		}
	}
}

basicPrintHash(\%dhash);

untie %dhash;
