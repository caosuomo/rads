#!/usr/bin/perl

# Copyright (C) 2005 by Pawel Pilarczyk.
#
# This file is part of the Homology Library.  This library is free software;
# you can redistribute it and/or modify it under the terms of the GNU
# General Public License as published by the Free Software Foundation;
# either version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this software; see the file "license.txt".  If not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
# MA 02111-1307, USA.

my $title = <<"__END_TITLE__";
LIFTCUBES, ver. 0.01, 06/02/05. Copyright (C) 2005 by Pawel Pilarczyk.
This is free software. No warranty. Consult 'license.txt' for details.
__END_TITLE__

my $info = <<"__END_INFO__";
This is a Perl script for lifting cubes to different layers
in order to compute the Conley index map, as described in the paper
"Excision-preserving cubical approach to the algorithmic computation
of the discrete Conley index" by K. Stolot and P. Pilarczyk.

	Call with: F.map X.cub A.cub Y.cub B.cub.

This program accepts the map only in the general format; you may need
to use the "cnvmvmap" program to convert it first.
The program saves its results to the files with the same names as the
input files, preceded with "_", e.g., _F.map, _X.cub, etc.
__END_INFO__


print "$title";

(my $fname, my $xname, my $aname, my $yname, my $bname) = @ARGV;

unless ($bname)
{
	print $info;
	exit;
}

# ---------------------------------------------------

# Read cubes from a text file.
# Call this procedure with: $filename, \%cubes_to_exclude.
sub readcubes
{
	my $filename = shift;
	my $exclude = shift;
	$exclude = \%empty unless defined $exclude;
	open (FILE, $filename) or die "File $filename not found.";
	my @cubes;
	while (<FILE>)
	{
		next if /^\s*;/;
		next unless /[\(\[\{]([-0123456789, ]+)[\)\]\}]/;
		my $coords = $1;
		$coords =~ s/[, ]\s*/,/g;
		push @cubes, $coords unless defined $$exclude{$coords};
	}
	close (FILE);
	return @cubes;
}

# Read disjoint sets: A, X\A, B\A, Y\B. Note: %b indexes the entire B.
print "Reading the sets of cubes...\n";
my @a = readcubes ($aname);
my %a; foreach (@a){ $a{$_} = 1; }
my @x = readcubes ($xname, \%a);
my %x; foreach (@x){ $x{$_} = 1; }
my @b = readcubes ($bname, \%a);
my %b; foreach (@a,@b){ $b{$_} = 1; }
my @y = readcubes ($yname, \%b);

# ---------------------------------------------------

# Save cubes with appropriate level coordinate(s).
# Call with: ">$filename" or ">>$filename", $level, @cubes.
# Levels:  0 - lower, 2 - upper, 1 - joining level.
sub saveliftedcubes
{
	my $filename = shift;
	my $level = shift;
	open (FILE, $filename) or die "Unable to create file $filename.";
	foreach (@_)
	{
		print FILE "($_,$level)\n";
	}
	close (FILE);
}

print "Saving the new sets of cubes...\n";
saveliftedcubes (">_$xname", 2, @x);
saveliftedcubes (">_$aname", 1, @a);
saveliftedcubes (">_$yname", 2, @y);
saveliftedcubes (">_$bname", 1, @a);
saveliftedcubes (">>_$bname", 0, @b);

# ---------------------------------------------------

# Determine which level the cube should be at.
sub whichlevel
{
	my $cube = shift;
	return 2 if defined $x{$cube};
	return 1 if defined $a{$cube};
	return 0;
}

print "Transforming the map...\n";
open (FILE, $fname);
open (OUT, ">_$fname");
while (<FILE>)
{
	next if /^\s*;/;
	next unless /[\(\[\{]([-0123456789, ]+)[\}\]\)]\s*->\s*\{(.*)\}/;
	my $arg = $1;
	my $val = $2;
	$arg =~ s/[, ]\s*/,/g;
	my @val;
	while ($val =~ /[\(\[\{]([-0123456789, ]+)[\}\]\)]/g)
	{
		push @val, $1;
	}
	print OUT "($arg,", whichlevel ($arg), ") -> {";
	for (my $i = 0; $i <= $#val; $i ++)
	{
		print OUT ' ' if $i;
		print OUT "($val[$i],", whichlevel ($val[$i]), ")";
	}
	print OUT "}\n";
}
close (OUT);
close (FILE);

# ---------------------------------------------------

print "Done. Thank you!\n";

