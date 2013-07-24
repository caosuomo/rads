/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file bin2pset.cpp
///
/// @author Pawel Pilarczyk
///
/////////////////////////////////////////////////////////////////////////////

// Copyright (C) 1997-2010 by Pawel Pilarczyk.
//
// This file is part of the Homology Library.  This library is free software;
// you can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation;
// either version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this software; see the file "license.txt".  If not, write to the
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.

// Started on November 18, 2005. Last revision: November 18, 2005.


#include "chomp/system/config.h"
#include "chomp/system/textfile.h"
#include "chomp/system/timeused.h"
#include "chomp/system/arg.h"

#include <exception>
#include <new>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
using namespace std;

using namespace chomp::homology;


// --------------------------------------------------
// -------------------- OVERTURE --------------------
// --------------------------------------------------

const char *title = "\
BinCube->PointSet, ver. 0.01. Copyright (C) 1997-2010 by Pawel Pilarczyk.\n\
This is free software. No warranty. Consult 'license.txt' for details.";

const char *helpinfo = "\
Call with: file.bin file.cub -d dim -s size\n\
This program reads a set of points encoded in a binary format, and writes\n\
the coordinates of the points corresponding to nonzero bits.\n\
Additional arguments:\n\
-d N - set the space dimension (must be specified),\n\
-s N - set the size of the binary cube (repeat for other directions),\n\
-x N, -y N, -z N - set the size in the specific direction,\n\
-i N - skip the initial N bytes in the input file.\n\
-h  - display this brief help information only and exit.\n\
For more information ask the author at http://www.PawelPilarczyk.com/.";

const int maxdimension = 32;

// --------------------------------------------------
// -------------------- BIN2PSET --------------------
// --------------------------------------------------

void inc_counter (int *counter, const int *sizes, int length)
{
	while (length --)
	{
		++ *counter;
		if (*counter < *sizes)
			return;
		*counter = 0;
		++ counter;
		++ sizes;
	}
	return;
} /* inc_counter */

int writecubes (const char *buf, int size, int *coord, int dim,
	ostream &outfile)
{
	int ncubes = 0;
	int buflength = (size + 7) >> 3;
	for (int pos = 0; pos < buflength; ++ pos)
	{
		if (!buf [pos])
			continue;
		int maxbit = 8;
		if ((pos << 3) + maxbit > size)
			maxbit = size - (pos << 3);
		int byte = buf [pos];
		for (int bit = 0; bit < maxbit; ++ bit)
		{
			if (!(byte & (1 << bit)))
				continue;
			outfile << "(" << ((pos << 3) + bit);
			for (int i = 1; i < dim; ++ i)
				outfile << "," << coord [i];
			outfile << ")\n";
			++ ncubes;
		}
	}
	return ncubes;
} /* writecubes */

int bincube2pointset (char *inname, char *outname, int dim,
	const int* sizes, int initialskip)
// Returns: 0 = Ok, -1 = error (shows msg).
{
	// open the input file
	ifstream infile;
	infile. open (inname, ios::binary | ios::in);
	if (initialskip)
		infile. seekg (initialskip);
	if (!infile)
		fileerror (inname);

	// prepare data to scan for cubes in the file
	int coord [maxdimension];
	for (int i = 0; i < maxdimension; ++ i)
		coord [i] = 0;
	int linelength = (sizes [0] + 7) >> 3;
	char *buf = new char [linelength];

	// open the output file
	ofstream outfile (outname);
	if (!outfile)
		fileerror (outname, "create");

	// process all the rows from the file
	sout << "Processing lines of bits... ";
	int counter = 0;
	int ncubes = 0;
	while (1)
	{
		// read a line from the input file and break if none
		infile. read (buf, linelength);
		if (infile. eof ())
			break;

		// write the cubes listed within this line to a file
		ncubes += writecubes (buf, sizes [0], coord, dim, outfile);

		// increase the other coordinates for the next line
		inc_counter (coord + 1, sizes + 1, dim - 1);
		
		// update the counter and show it if necessary
		++ counter;
		if (!(counter % 1847))
			scon << std::setw (10) << counter <<
				"\b\b\b\b\b\b\b\b\b\b";
	}
	sout << ncubes << " cubes extracted.\n";

	// finalize
	delete [] buf;
	return 0;
} /* binary cube to pointset */


// --------------------------------------------------
// ---------------------- MAIN ----------------------
// --------------------------------------------------

int main (int argc, char *argv [])
// Return: 0 = Ok, -1 = Error, 1 = Help displayed, 2 = Wrong arguments.
{
	// prepare user-configurable data
	char *inname = 0, *outname = 0;
	int dim = 0;
	int sizes [maxdimension];
	for (int i = 0; i < maxdimension; ++ i)
		sizes [i] = 0;
	int nsizes = 0;
	int initialskip = 0;

	// analyze the command line
	arguments a;
	arg (a, 0, inname);
	arg (a, 0, outname);
	arg (a, "d", dim);
	arg (a, "s", sizes, nsizes, maxdimension);
	arg (a, "i", initialskip);
	arg (a, "x", sizes [0]);
	arg (a, "y", sizes [1]);
	arg (a, "z", sizes [2]);
	arghelp (a);

	argstreamprepare (a);
	int argresult = a. analyze (argc, argv);
	argstreamset ();

	// show the program's main title
	if (argresult >= 0)
		sout << title << '\n';

	// check whether the entered arguments are correct and adjust sizes
	if (!outname)
		argresult = 1;
	for (int i = 0; i < 3; ++ i)
		if (sizes [i] && (nsizes <= i))
			nsizes = i + 1;
	if (!dim && (nsizes > 1))
		dim = nsizes;
	if (nsizes)
	{
		for (int i = nsizes; i < maxdimension; ++ i)
			sizes [i] = sizes [nsizes - 1];
	}
	for (int i = 0; i < nsizes; ++ i)
	{
		if (sizes [i] > 0)
			continue;
		sout << "ERROR: The sizes must be positive.\n";
		argresult = 1;
		break;
	}
	if (!argresult && !nsizes)
	{
		sout << "ERROR: The size of the binary cube "
			"must be defined.\n";
		argresult = 1;
	}
	if (!argresult && !dim)
	{
		sout << "ERROR: Please, define the dimension of the "
			"binary cube.\n";
		argresult = 1;
	}

	// if something was incorrect, show an additional message and exit
	if (argresult < 0)
	{
		sout << "Call with '--help' for help.\n";
		return 2;
	}

	// if help requested or no output name defined, show help information
	if (argresult > 0)
	{
		sout << helpinfo << '\n';
		return 1;
	}

	// try running the main function and catch an error message if thrown
	try
	{
		bincube2pointset (inname, outname, dim, sizes, initialskip);
		program_time = 1;
		return 0;
	}
	catch (const char *msg)
	{
		sout << "ERROR: " << msg << '\n';
		return -1;
	}
	catch (const std::exception &e)
	{
		sout << "ERROR: " << e. what () << '\n';
		return -1;
	}
	catch (...)
	{
		sout << "ABORT: An unknown error occurred.\n";
		return -1;
	}
} /* main */

/// @}

