/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file pset2bin.cpp
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

// Started on November 17, 2005. Last revision: December 1, 2005.


#include "chomp/system/config.h"
#include "chomp/system/textfile.h"
#include "chomp/cubes/bincube.h"
#include "chomp/cubes/pointset.h"
#include "chomp/system/timeused.h"
#include "chomp/system/arg.h"
#include "chomp/cubes/cube.h"

#include <exception>
#include <new>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
using namespace std;

using namespace chomp::homology;


// --------------------------------------------------
// -------------------- OVERTURE --------------------
// --------------------------------------------------

const char *title = "\
Pointset->BinCube, ver. 0.02. Copyright (C) 1997-2010 by Pawel Pilarczyk.\n\
This is free software. No warranty. Consult 'license.txt' for details.";

const char *helpinfo = "\
Call with: file.cub file.bin\n\
This program reads a set of points and plots these points in a binary\n\
cube, then writes this binary cube to the output binary file.\n\
The size of the cube must be a power of 2 and must equal at least 8;\n\
the requested value is rounded upwards if necessary.\n\
The dimension is determined automatically based on the input data.\n\
The file is scanned to determine the size and the lower left corner\n\
(the origin) of the binary cube, unless both these values are specified.\n\
Additional arguments:\n\
-s N - the size of the binary cube if known in advance,\n\
-o (x,y,z) OR -o 0 - set the origin,\n\
-h  - display this brief help information only and exit.\n\
For more information ask the author at http://www.PawelPilarczyk.com/.";

const int maxdimension = 32;

// --------------------------------------------------
// -------------------- PSET2BIN --------------------
// --------------------------------------------------

template <int dim, int twoPower>
int pset2bin (ifstream &infile, int *coord, const int *origin,
	const char *outname)
{
	bincube<dim,twoPower> b;
	int counter = 0;
	int outofrange = 0;
	while (1)
	{
		// update the counter and show it if necessary
		++ counter;
		if (!(counter % 53947))
			scon << std::setw (10) << counter <<
				"\b\b\b\b\b\b\b\b\b\b";

		// make the coordinates relative to the origin
		if (origin)
		{
			for (int i = 0; i < dim; ++ i)
				coord [i] -= origin [i];
		}

		// skip the cube if it is not within the bincube's range
		bool skip = false;
		for (int i = 0; i < dim; ++ i)
		{
			int c = coord [i];
			if ((c >= (1 << twoPower)) || (c < 0))
			{
				skip = true;
				break;
			}
		}

		// skip the cube or add it to the binary cube
		if (skip)
			++ outofrange;
		else
			b. add (coord);

		// read the next cube and quit in the case of failure
		ignorecomments (infile);
		if (infile. eof ())
			break;
		int d = readcoordinates (infile, coord, maxdimension);
		if (d != dim)
			break;
	}
	sout << counter << " cubes processed.\n";
	if (outofrange)
		sout << "WARNING: " << outofrange << " cube(s) "
			"skipped (coordinates out of range).\n";

	sout << "Writing " << (counter - outofrange) <<
		" cubes to '" << outname << "'... ";
	ofstream outfile;
	outfile. open (outname, ios::binary | ios::out);
	if (!outfile)
		fileerror (outname, "create");
	sout << "(" << b. getbufsize () << " bytes) ";
	outfile << b;
	sout << "Done.\n";

	return 0;
} /* pset2bin */

typedef int (*function) (ifstream &infile, int *coord,
	const int *origin, const char *outname);
typedef struct
{
	int dim;
	int twoPower;
	function f;
} ftabelement;

ftabelement ftab [] =
{
	{1, 3, pset2bin<1,3>}, {1, 4, pset2bin<1,4>},
	{1, 5, pset2bin<1,5>}, {1, 6, pset2bin<1,6>},
	{1, 7, pset2bin<1,7>}, {1, 8, pset2bin<1,8>},
	{1, 9, pset2bin<1,9>}, {1, 10, pset2bin<1,10>},
	{1, 11, pset2bin<1,11>}, {1, 12, pset2bin<1,12>},
	{1, 13, pset2bin<1,13>}, {1, 14, pset2bin<1,14>},
	{1, 15, pset2bin<1,15>}, {1, 16, pset2bin<1,16>},
	{1, 17, pset2bin<1,17>}, {1, 18, pset2bin<1,18>},
	{1, 19, pset2bin<1,19>}, {1, 20, pset2bin<1,20>},
	{1, 21, pset2bin<1,21>}, {1, 22, pset2bin<1,22>},
	{1, 23, pset2bin<1,23>}, {1, 24, pset2bin<1,24>},

	{2, 3, pset2bin<2,3>}, {2, 4, pset2bin<2,4>},
	{2, 5, pset2bin<2,5>}, {2, 6, pset2bin<2,6>},
	{2, 7, pset2bin<2,7>}, {2, 8, pset2bin<2,8>},
	{2, 9, pset2bin<2,9>}, {2, 10, pset2bin<2,10>},
	{2, 11, pset2bin<2,11>}, {2, 12, pset2bin<2,12>},
	{2, 13, pset2bin<2,13>}, {2, 14, pset2bin<2,14>},
	{2, 15, pset2bin<2,15>},

	{3, 3, pset2bin<3,3>}, {3, 4, pset2bin<3,4>},
	{3, 5, pset2bin<3,5>}, {3, 6, pset2bin<3,6>},
	{3, 7, pset2bin<3,7>}, {3, 8, pset2bin<3,8>},
	{3, 9, pset2bin<3,9>}, {3, 10, pset2bin<3,10>},

	{4, 3, pset2bin<4,3>}, {4, 4, pset2bin<4,4>},
	{4, 5, pset2bin<4,5>}, {4, 6, pset2bin<4,6>},
	{4, 7, pset2bin<4,7>},

	{5, 3, pset2bin<5,3>}, {5, 4, pset2bin<5,4>},
	{5, 5, pset2bin<5,5>}, {5, 6, pset2bin<5,6>},

	{6, 3, pset2bin<5,3>}, {6, 4, pset2bin<5,4>},
	{6, 5, pset2bin<5,5>},

	{7, 3, pset2bin<7,3>}, {7, 4, pset2bin<7,4>},

	{8, 3, pset2bin<8,3>},

	{9, 3, pset2bin<9,3>},

	{10, 3, pset2bin<10,3>},

	{0, 0, 0}
};

// Determines the minimal coordinates of points in the file.
// Also computes the size of the set if necessary.
// Returns the dimension on success, -1 on failure.
int determine (const char *inname, int *origin,
	const int *fixedorigin, int *size)
{
	// open the input file
	ifstream infile (inname);
	if (!infile)
		fileerror (inname);

	// ignore the word "dimension"
	ignorecomments (infile);
	if (infile. peek () == 'd')
	{
		ignoreline (infile);
		ignorecomments (infile);
	}

	sout << "Scanning the input file... ";
	int dim = -1;
	int coord [maxdimension];
	int orgcoord [maxdimension];
	if (!origin)
		origin = orgcoord;
	int sizes [maxdimension];
	int counter = 0;
	while (1)
	{
		// update the counter and show it if necessary
		++ counter;
		if (!(counter % 53947))
			scon << std::setw (10) << counter <<
				"\b\b\b\b\b\b\b\b\b\b";

		// read one point and exit the loop if none
		int d = readcoordinates (infile, coord, maxdimension);
		if (d <= 0)
			break;

		// ignore coordinates beyond the fixed dimension
		if ((dim >= 0) && (d > dim))
			d = dim;

		// update the size if necessary
		if (size && (dim >= 0))
		{
			if (fixedorigin)
			{
				for (int i = 0; i < d; ++ i)
				{
					int dist = coord [i] -
						fixedorigin [i];
					if (dist < 0)
						continue;
					if (dist >= *size)
						*size = dist + 1;
				}
			}
			else
			{
				for (int i = 0; i < d; ++ i)
				{
					int dist = coord [i] - origin [i];
					if (dist < 0)
					{
						dist = -dist;
						sizes [i] += dist;
					}
					if (sizes [i] <= dist)
						sizes [i] = dist + 1;
				}
			}
		}

		// update the table of the minimal coordinates and set dim
		if (dim < 0)
		{
			dim = d;
			for (int i = 0; i < d; ++ i)
			{
				origin [i] = coord [i];
				sizes [i] = 1;
			}
		}
		else
		{
			for (int i = 0; i < d; ++ i)
				if (origin [i] > coord [i])
					origin [i] = coord [i];
		}

		// get ready to read the next one and exit the loop if EOF
		ignorecomments (infile);
		if (infile. eof ())
			break;
	}

	// upate the actual size
	if (size)
	{
		for (int i = 0; i < dim; ++ i)
			if (*size < sizes [i])
				*size = sizes [i];
	}
	
	// say what has been done
	sout << counter << " cubes scanned.\n";
	if (origin && !fixedorigin)
	{
		for (int i = 0; i < dim; ++ i)
			sout << (i ? "," : "The origin is (") <<
				origin [i];
		if (dim >= 0)
			sout << ").\n";
	}
	if (size)
		sout << "The size is " << *size << ".\n";

	// return the dimension
	return dim;
} /* determine */

int pointset2bincube (char *inname, char *outname, int size, char *originstr)
// Returns: 0 = Ok, -1 = error (shows msg).
{
	// determine the origin if any
	int origincoord [maxdimension];
	for (int i = 0; i < maxdimension; ++ i)
		origincoord [i] = 0;
	int *origin = 0;
	if (originstr && (originstr [0] == '0') && (originstr [1] == '\0'))
		origin = origincoord;
	if (originstr && !origin)
	{
		istringstream s (originstr);
		int dim = readcoordinates (s, origincoord, maxdimension);
		if (dim <= 0)
			sout << "WARNING: The origin was given in a wrong "
				"format. Ignoring this option.\n";
		else
		{
			origin = origincoord;
			sout << "Setting the origin to ";
			for (int i = 0; i < dim; ++ i)
				sout << (i ? "," : "(") << origin [i];
			sout << ").\n";
		}
	}

	// scan the input file to determine the origin and size if necessary
	if (!origin || !size)
	{
		int dim = determine (inname, origin ? 0 : origincoord,
			origin, size ? 0 : &size);

		if ((dim <= 0) && !origin)
			sout << "WARNING: Unable to determine the minimal "
				"coordinatess. Setting the origin to 0.\n";
		else if (!origin)
			origin = origincoord;

		if ((dim <= 0) && !size)
			sout << "WARNING: Unable to determine the size of "
				"the binary cube. Setting to 8 (minimum).\n";
	}

	// determine the power of two
	int twoPower = 3;
	while (size > (1 << twoPower))
		twoPower ++;

	// open the input file
	ifstream infile (inname);
	if (!infile)
		fileerror (inname);

	// ignore the word "dimension"
	ignorecomments (infile);
	if (infile. peek () == 'd')
	{
		ignoreline (infile);
		ignorecomments (infile);
	}

	if (infile. eof ())
		return 0;

	// read the first cube and determine the space dimension
	int coord [maxdimension];
	int dim = readcoordinates (infile, coord, maxdimension);

	// run the appropriate function to continue the computations
	ftabelement *fe = ftab;
	while (fe -> f)
	{
		if ((fe -> dim == dim) && (fe -> twoPower == twoPower))
		{
			sout << "Processing: dim = " << dim <<
				", size = 2^" << twoPower << "... ";
			return fe -> f (infile, coord, origin, outname);
		}
		++ fe;
	}
	sout << "SORRY, the program does not support dimension " << dim <<
		" and cube size 2^" << twoPower << ".\n";
	sout << "The following configurations of dimension and cube size "
		"are supported:\n";
	fe = ftab;
	int count = 0;
	while (fe -> f)
	{
		if (count && !(count % 6))
			sout << "\n";
		sout << "(" << fe -> dim << "," <<
			(1 << fe -> twoPower) << ") ";
		++ fe;
		++ count;
	}
	sout << "\n";
	sout << "Note that the cube size must be a power of 2.\n";
	sout << "To support other configurations, "
		"the program must be recompiled.";
	return -1;
} /* pointset to binary cube */


// --------------------------------------------------
// ---------------------- MAIN ----------------------
// --------------------------------------------------

int main (int argc, char *argv [])
// Return: 0 = Ok, -1 = Error, 1 = Help displayed, 2 = Wrong arguments.
{
	// prepare user-configurable data
	char *inname = 0, *outname = 0;
	int size = 0;
	char *origin = 0;

	// analyze the command line
	arguments a;
	arg (a, 0, inname);
	arg (a, 0, outname);
	arg (a, "s", size);
	arg (a, "o", origin, "");
	arghelp (a);

	argstreamprepare (a);
	int argresult = a. analyze (argc, argv);
	argstreamset ();

	// show the program's main title
	if (argresult >= 0)
		sout << title << '\n';

	// if something was incorrect, show an additional message and exit
	if (argresult < 0)
	{
		sout << "Call with '--help' for help.\n";
		return 2;
	}

	// if help requested or no output name defined, show help information
	if (!outname || (argresult > 0))
	{
		sout << helpinfo << '\n';
		return 1;
	}

	// try running the main function and catch an error message if thrown
	try
	{
		pointset2bincube (inname, outname, size, origin);
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

