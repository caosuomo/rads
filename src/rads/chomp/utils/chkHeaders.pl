#!/usr/bin/perl

# This Perl script traces missing and unused header files,
# based on the files "makedeps" and "auto_dep/*".

# Written on June 17, 2008 by Pawel Pilarczyk. No warranty.
# Provided under the terms of the GNU General Public License.

chdir "..";

my %used;
foreach my $filename (glob ("make/auto_dep/*"), glob ("*/*/makedeps"))
{
#	print "--- $filename ---\n";
	my $file;
	open ($file, $filename) or die "Can't open $filename";
	my @lines = <$file>;
	close ($file);
	my $lines = join ('', @lines);
	while ($lines =~ m{(\b[\w\-/]+/[\w\-/]+\.h(?:pp)?)\s}sg)
	{
		my $header = $1;
		$used{$header} = 1;
#		print "$header\n";
	}
}

my %unused;
my %existing;
foreach my $filename (glob ("include/*/*/*"))
{
	$filename =~ s[include/][];
	$existing{$filename} = 1;
	$unused{$filename} = 1 unless defined $used{$filename};
}

print "Missing header files:\n";
my %missing;
for my $filename (sort keys %used)
{
	print "\t$filename\n" unless defined $existing{$filename};
}

print "Unused header files:\n";
for my $filename (sort keys %unused)
{
	print "\t$filename\n";
}


