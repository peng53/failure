#!/bin/perl

use strict;
use warnings;
use Fcntl;
use GDBM_File;
use POSIX;

package DQueue;

our $MAX = 16;

sub loadIt {
	my $hash = shift;
	my $dfile = shift;
	tie %$hash, 'GDBM_File', $dfile, main::O_CREAT|main::O_RDWR, 0644 or die 'Couldn\'t open.';
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
	my $r;
	if (($$hash{'next'}+1)%$$hash{'size'} == $$hash{'read'}) {
		$r = get($hash);
		advanceRead($hash);
	}
	$$hash{$$hash{'next'}} = shift;
	($$hash{'next'}+=1) %= $$hash{'size'};
	return $r;
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

sub sizeDown {
	my $hash = shift;
	my $new_size = shift;
	my $size = $$hash{'size'};
	return if ($size < $new_size);
	print "Decreasing size to $new_size\n";
	my $next = $$hash{'next'};
	my $read = $$hash{'read'};
	my $items = readOut($hash);
	$next = 0;
	foreach (@$items) {
		$$hash{$next++} = $_;
		last if $next >= $size;
	}
	$$hash{'next'} = $next % $new_size;
	$$hash{'read'} = 0;
	$$hash{'size'} = $new_size;
}

sub sizeUp {
	my $hash = shift;
	my $new_size = shift;
	return if $new_size <= $$hash{'size'};
	print "Increasing size to $new_size\n";
	$$hash{'size'} = $new_size;
}

sub readOut {
	my $hash = shift;
	my $read = $$hash{'read'};
	my $next = $$hash{'next'};
	my $size = $$hash{'size'};
	my @items;
	while ($read != $next) {
		push(@items, $$hash{$read});
		($read+=1) %= $size;
	}
	return \@items;
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
	} elsif ($cmd eq '+size') {
		sizeUp(\%dhash,shift);
	} elsif ($cmd eq '-size') {
		sizeDown(\%dhash,shift);
	} elsif ($cmd eq '?debug') {
		print "$dhash{'next'}\n$dhash{'read'}\n$dhash{'size'}\n";
	} elsif ($cmd eq '?all') {
		print join("\n", @{readOut(\%dhash)}), "\n";
	} else {
		print "Adding $cmd\n";
		add(\%dhash,$cmd);
	}
	untie %dhash;
}

unless (caller) {
	main(@ARGV);
}
