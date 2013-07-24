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

# Started on February 24, 2005. Last revision: February 25, 2005.

my $title = <<"ENDTITLE";
VERTICES, ver. 0.01, 02/25/05. Copyright (C) 2005 by Pawel Pilarczyk.
This is free software. No warranty. Consult 'license.txt' for details.
ENDTITLE

my $helpinfo = <<"ENDHELP";
This program removes redundant simplices and vertices from a simplicial
complex. Vertices numbers must be non-negative integers, and the
simplices must be written as n-tuples of numbers, e.g. (0,1,2),
one simplex per line.
Call this program with the INPUT and OUTPUT file names.
ENDHELP

# show the title and help info
print $title;
if (scalar (@ARGV) < 2)
{
	print $helpinfo;
	exit;
}


# ---------------------------------------------------
# read all the simplices from the input file
# ---------------------------------------------------

print "Reading the simplices...\n";
my @simplices;
open (INPUT, $ARGV[0]);
while (<INPUT>)
{
	next unless /\(.*\)/;
	s/\s+//;
	push @simplices, $_;
}
close (INPUT);


# ---------------------------------------------------
# sort the simplices by dimension;
# high-dimensional ones should comes first
# ---------------------------------------------------

@simplices = sort
{
	my @a = split (',', $a);
	my @b = split (',', $b);
	return $#b <=> $#a;
}
@simplices;


# ---------------------------------------------------
# a subroutine for checking whether a list
# is contained in another one
# ---------------------------------------------------

sub contained
{
	my $small = shift;
	my $large = shift;

	my $i = 0;
	my $j = 0;
	while (($i < scalar (@$small)) && ($j < scalar (@$large)))
	{
		while ((($small -> [$i]) > ($large -> [$j])) &&
			($j < scalar (@$large)))
		{
			$j ++;
		}
		return 0 if ($j >= scalar (@$large));
		return 0 if (($small -> [$i]) != ($large -> [$j]));
		$i ++;
		$j ++;
	}
	return ($i == scalar (@$small));
}


# ---------------------------------------------------
# remove simplices that are faces of simplices
# which are already in the set
# ---------------------------------------------------

print "Eliminating redundant simplices...\n";

my @vertices;
vertloop:
for (my $i = 0; $i <= $#simplices; $i ++)
{
	# create a table of vertices of this simplex
	my $txt = $simplices [$i];
	$txt =~ s/\(|\)//g;
	my @array = split (',', $txt);
	sort @array;
	
	# check whether this simplex is contained in one of the
	# previous ones
	for (my $j = 0; $j < $i; $j ++)
	{
		next unless contained (\@array, $vertices [$j]);

		# if this is a face, remove it and redo the loop iteration
		print "* Simplex (", join (',', @array), ") is a face of (",
			join (',', @{$vertices [$j]}), ").\n";

		splice @simplices, $i, 1;
		$i --;
		next vertloop;
	}
	$vertices [$i] = \@array;
}


# ---------------------------------------------------
# count how many times each vertex is used
# and remember in which simplices
# ---------------------------------------------------

print "Analyzing the simplices...\n";

# a list of lines (simplex numbers) in which each vertex appears;
# the data is in the text format "0 12 17"
my @appear;
# the number of appearences of each vertex
my @appearcount;
# the number of vertex in each simplex
my @vertcount;

for (my $i = 0; $i <= $#simplices; $i ++)
{
	while ($simplices [$i] =~ /\d+/g)
	{
		$appear [$&] .= ' ' unless !defined $appear [$&];
		$appear [$&] .= "$i";
		$appearcount [$&] ++;
		$vertcount [$i] ++;
	}
}


# ---------------------------------------------------
# if there exist two vertices such that every simplex
# containing the first one, also contains the other
# one, then the first vertex can be removed
# ---------------------------------------------------

print "Removing redundant vertices...\n";

# analyze the sets of simplices each vertex appears in
# and remove these vertices which appear always with some other vertex
for (my $i = 0; $i <= $#appear; $i ++)
{
	# skip vertices that appear in one simplex or less
	next if $appearcount [$i] <= 1;

	# check whether one of the previous vertices appears in the same
	# set of simplices
	my @numbers = split (' ', $appear [$i]);
	
	for (my $j = 0; $j < $#appear; $j ++)
	{
		next if ($i == $j);
		my @othernumbers = split (' ', $appear [$j]);
		next unless contained (\@numbers, \@othernumbers);
		
		foreach (@numbers)
		{
			# remove the vertex from the simplex
			$simplices[$_] =~ s/\b$i,//;
			$simplices[$_] =~ s/,$i\b//;
		}
		my $howmany = scalar (@numbers);
		print "* Redundant vertex $i ($j) " .
			"removed from $howmany simplices.\n";
		$appear [$i] = '';
		$appearcount [$i] = 0;
		last;
	}
}


# ---------------------------------------------------
# if a vertex appears in exactly one simplex,
# and the dimension of that simplex is non-zero,
# then such a vertex can be removed
# ---------------------------------------------------

print "Removing those vertices which appear only once...\n";

for (my $i = 0; $i <= $#appear; $i ++)
{
	# skip vertices that appear in more than one simplex
	next unless $appearcount [$i] == 1;

	# determine the number of the simplex whose vertex is removed
	my $simplex = $appear [$i];
	$simplex =~ s/\s+//g;
	
	# skip vertices that form a zero-dimensional simplex
	next if $vertcount [$appear [$i]] <= 1;

	# decrease the dimension of the reduced simplex
	$vertcount [$simplex] --;

	# say what is going to be done
	my $line = $simplex + 1;
	print "* Vertex $i removed from $simplices[$simplex].\n";

	# remove the vertex from the simplex
	$simplices[$simplex] =~ s/\b$i,//;
	$simplices[$simplex] =~ s/,$i\b//;
}


# ---------------------------------------------------
# save the reduced simplices
# ---------------------------------------------------

print "Writing the output file...\n";

open (OUTPUT, ">$ARGV[1]");
foreach (@simplices)
{
	print OUTPUT "$_\n";
}
close (OUTPUT);


print "Done. Thank you!\n";

