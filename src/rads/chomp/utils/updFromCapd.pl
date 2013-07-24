#!/usr/bin/perl

# This Perl script updates files from the CAPD library to the current
# distribution of CHomP. Note that if some files change their names
# or there are some added files or removed ones, then these changes
# must be made manually before running this script.

# Written on May 24, 2007 by Pawel Pilarczyk. No warranty.
# Updated regularly upon changes in the CAPD library.
# Provided under the terms of the GNU General Public License, Version 2.

# The root paths of the CAPD and CHomP libraries.
my $chompdir = "../";
my $capddir = "../../capd";

my $countmodified = 0;

my %includefiles;

# prepares the hash of files to be included
sub prepareinclusions
{
	my $chompdir = shift;
	my $basedir = "${chompdir}/include/chomp";
	foreach my $dir (glob "${basedir}/*")
	{
		next unless -d $dir;
		my $subdir = $dir;
		$subdir =~ s[^${basedir}/][];
		foreach my $file (glob "${dir}/*")
		{
			my $basefile = $file;
			$basefile =~ s[^.*/][];
			$includefiles{$basefile} = $subdir;
		#	print "'$basefile' is in '$subdir'\n";
		}
	}
}

# transforms the name of the included file
sub transclude
{
	my $filename = shift;
	if ($filename =~ m[^capd/homology/])
	{
		$filename =~ s[^capd/homology/][];
		my $subdir = $includefiles{$filename};
		die "Unknown subdir for '${filename}'." unless $subdir;
		return "chomp/${subdir}/${filename}";
	}
	else
	{
		return "${filename}";
	}
}

# copies a .h or .cpp file and makes all the necessary alterations;
# if there are no changes, then the new file is not written
sub copyfile
{
	# determine the names of files to copy (source and destination)
	my $inname = shift;
	my $outname = shift;

	# read the source file
	open FILE, "<$inname" or die "Error: File '$inname' not found.";
	my @infile = <FILE>;
	close FILE;

	# the control of the @addtogroup command
	my $addtogroup = 0;

	# prepare the new file to write
	my @outfile;
	foreach my $line (@infile)
	{
		my $newline = $line;
		if ($newline =~ m{^(\s*/*\s*\#\s*include\s*\")([^\"]+)(\".*)}s)
		{
			my $beg = $1;
			my $file = $2;
			my $end = $3;
			$file = transclude ($file);
			$newline = "${beg}${file}${end}";
		}
		else
		{
			$newline =~ s/namespace capd/namespace chomp/g;
			$newline =~ s/capd::/chomp::/g;
		}

		# correct some issues that generate doxygen warnings
		$newline =~ s[^/// \@addtogroup 2][/// \@addtogroup MM2];

		# count the opening and closes of doxygen groupping
		$addtogroup ++ if $newline =~ m[^/// \@\{];
		$addtogroup -- if $newline =~ m[^/// \@\}];
		push @outfile, $newline;
	}
	my $outfile = join ('', @outfile);
	$outfile =~ s/\r\n/\n/g;
	$outfile =~ s/\r\n/\n/g;

	# modify some doxygen-incompatible comments
	$outfile =~ s[\/\*\*(\s*\@doc.*?)\*\/][\/\*\1\*\/]gs;

	# add a doxygen grouping ending if necessary
	$outfile .= "\n/// \@\}\n" if $addtogroup > 0;

	# verify if the new file really needs to be written
	my $write = 1;
	if (open FILE, "<$outname")
	{
		my @existingfile = <FILE>;
		my $existingfile = join ('', @existingfile);
		close FILE;
		if ($outfile eq $existingfile)
		{
		#	print "$outname - not changed\n";
			return 0;
		}
		print "$outname - updated\n";
	}
	else
	{
		print "$outname - created\n";
	}
	++ $countmodified;

	# write the new file
	open FILE, ">$outname" or die "Unable to write '$outname'.";
	print FILE $outfile;
	close FILE;
	return 1;
}

# prepares the list of all pairs of directories to update:
# $directories{$source} = $target
my %directories;
sub preparedirlist
{
	my $capddir = shift;
	my $chompdir = shift;
	# src subdirectories
	my $dirbase = "${chompdir}/src/capd-";
	foreach my $dir (glob "${dirbase}*")
	{
		next unless -d $dir;
		my $srcdir = $dir;
		$srcdir =~ s[^${dirbase}][];
		$srcdir = "${capddir}/src/capd/${srcdir}";
		$directories{$srcdir} = $dir;
	}
	# include subdirectories
	$dirbase = "${chompdir}/include/capd/";
	foreach my $dir (glob "${dirbase}*")
	{
		next unless -d $dir;
		my $srcdir = $dir;
		$srcdir =~ s[^${dirbase}][];
		$srcdir = "${capddir}/include/capd/${srcdir}";
		$directories{$srcdir} = $dir;
	}
	# the chomp program and demos
	$directories{"${capddir}/programs/chomprog"} =
		"${chompdir}/programs/chomprog";
	$directories{"${capddir}/examples/chompdemo"} =
		"${chompdir}/programs/chompdemo";

#	foreach my $dir (sort keys %directories)
#	{
#		my $outdir = $directories{$dir};
#		print "dir '${dir}' -> '${outdir}'\n";
#	}
}

# updates all the files (except 'makefile' etc.) in the given pair of dirs
sub updatedir
{
	my $indir = shift;
	my $outdir = shift;

	# update the output directory
	foreach my $outfile (glob ("${outdir}/*"))
	{
		next unless -f $outfile;
		my $basefile = $outfile;
		$basefile =~ s[${outdir}/][];
		next if $basefile eq "makefile";
		next if $basefile eq "makedeps";
		my $infile = "${indir}/${basefile}";
		next unless -e $infile;
	#	print "file '${infile}' -> '${outfile}'\n";
		copyfile ($infile, $outfile);
	}
	return;
}

# the main routine to update CHomP based on CAPD
sub capd2chomp
{
	# determine the source and target directories
	my $capddir = shift;
	$capddir =~ s[/$][];
	my $chompdir = shift;
	$chompdir =~ s[/$][];

	print "Updating CHomP based on CAPD...\n";
	prepareinclusions ($chompdir);
	preparedirlist ($capddir, $chompdir);
	foreach $dir (sort keys %directories)
	{
		updatedir ($dir, $directories{$dir});
	}
	print "$countmodified files modified.\n";
}

# ===================================================

if (scalar (@ARGV) >= 2)
{
	my $input = $ARGV [0];
	my $output = $ARGV [1];
	if (-f $input)
	{
		prepareinclusions ($chompdir);
		copyfile ($input, $output);
	}
	else
	{
		capd2chomp ($input, $output);
	}
}
else
{
	capd2chomp ($capddir, $chompdir);
}

