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
	my $hash = shift;
	#foreach (keys %$hash) {
	#	print "$_ : ", {${%$hash}}{$_}, "\n";
	#}
	while (my ($k, $v) = each %$hash) {
		print $k, ' : ', $v, "\n";
	}
}

my %dhash;
loadIt(\%dhash, 'dfile.dbm');
tied %dhash or die;

if (! keys %dhash) {
	print "Reading from file.\n";
	my ($name, $trays) = getCollectiveHash(shift);
	my %trays = %{$trays};
	$dhash{'location'} = $name;
	foreach (keys %trays) {
		foreach my $number (@{$trays{$_}}) {
			$dhash{$number} = $_;
		}
	}
}

basicPrintHash(\%dhash);

untie %dhash;
