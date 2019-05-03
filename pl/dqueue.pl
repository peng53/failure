#!/bin/perl

use strict;
use warnings;
use Fcntl;
use GDBM_File;
use POSIX;

our $MAX = 16;

sub loadIt {
	my $hash = shift;
	my $dfile = shift;
	tie %$hash, 'GDBM_File', $dfile, O_CREAT|O_RDWR, 0644 or die 'Couldn\'t open.';
}

sub initDBM {
	my $hash = shift;
	if (! keys %$hash) {
		$$hash{'next'} = 0;
		$$hash{'size'} = $MAX;
	}
}

sub add {
	my $hash = shift;
	$$hash{'read'} = 0 if ( ! exists $$hash{'read'});
	if (($$hash{'next'}+1)%$$hash{'size'} == $$hash{'read'}) {
		print get($hash), "\n";
		advanceRead($hash);
	}
	$$hash{$$hash{'next'}} = shift;
	($$hash{'next'}+=1) %= $$hash{'size'};
}

sub get {
	my $hash = shift;
	die 'Queue is empty.' unless exists $$hash{'read'};
	die 'Queue is empty.' if $$hash{'read'} == $$hash{'next'};
	return $$hash{$$hash{'read'}};
}

sub advanceRead {
	my $hash = shift;
	($$hash{'read'}+=1) %= $$hash{'size'};
}

sub main {
	my $file = shift or die 'No input file.';
	my $cmd = shift or die 'No command.';
	my %dhash;
	loadIt(\%dhash, $file);
	initDBM(\%dhash);
	if ($cmd eq 'get') {
		print get(\%dhash), "\n";
		advanceRead(\%dhash);
	} elsif ($cmd eq 'add') {
		while (my $item = shift) {
			print "Adding $item\n";
			add(\%dhash,$item);
		}
	} else {
		print "Adding $cmd\n";
		add(\%dhash,$cmd);
	}
	untie %dhash;
}

unless (caller) {
	main(@ARGV);
}
