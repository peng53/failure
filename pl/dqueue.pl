#!/bin/perl

use strict;
use warnings;
use Fcntl;
use GDBM_File;
use POSIX;

our $MAX = 4;

sub loadIt {
	my $hash = shift;
	my $dfile = shift;
	tie %$hash, 'GDBM_File', $dfile, O_CREAT|O_RDWR, 0644 or die 'Couldn\'t open.';
}

sub initDBM {
	my $hash = shift;
	if (! keys %$hash) {
		$$hash{'next'} = 0;
	}
}

sub add {
	my $hash = shift;
	$$hash{$$hash{'next'}} = shift;
	$$hash{'read'} = 0 if ( ! exists $$hash{'read'});
	$$hash{'next'} = ($$hash{'next'}+1) % $MAX;
	if ($$hash{'next'} == $$hash{'read'}) {
		print "Data was overwritten.\n";
		$$hash{'read'} = ($$hash{'read'}+1) % $MAX;
	}
}

sub get {
	my $hash = shift;
	die 'Queue is empty.' unless exists $$hash{'read'};
	die 'Queue is empty.' if $$hash{'read'} == $$hash{'next'};
	return $$hash{$$hash{'read'}};
}

sub advanceRead {
	my $hash = shift;
	$$hash{'read'} = ($$hash{'read'}+1) % $MAX;
}

sub main {
	my $file = shift or die 'No input file.';
	my %dhash;
	loadIt(\%dhash, $file);
	initDBM(\%dhash);
	my $cmd = shift or die 'No command.';
	if ($cmd eq 'get') {
		print get(\%dhash), "\n";
		advanceRead(\%dhash);
	} elsif ($cmd eq 'add') {
		add(\%dhash,shift);
	} else {
		add(\%dhash,$cmd);
	}
	untie %dhash;
}

unless (caller) {
	main(@ARGV);
}
