/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file argtest.cpp
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

// Started on March 3, 2002. Last revision: October 6, 2004.


#include "chomp/system/textfile.h"
#include "chomp/system/arg.h"

using namespace chomp::homology;

using std::cout;
using std::endl;


// --------------------------------------------------
// -------------------- overture --------------------
// --------------------------------------------------

const char *title = "\
This is Argument Test program by Pawel Pilarczyk. Call with '-h' for help.";

const char *helpinfo = "\
Call this program with all or some of the following arguments:\n\
-h to display this help information,\n\
-x followed by a double precision real number (repeat up to 3 times),\n\
-m followed by a pair of integers in parentheses separated by a comma,\n\
-n followed by an integer number (default value is -1),\n\
-s - this is a switch that does not take any value,\n\
and at most three words not beginning with '-'.\n\
Note: Two words and one occurrence of '-n' is obligatory.\n\
For example: argtst -x12 -x 0.3 -m (0,0) filename1 filename2\n\
For more info see documentation (if any) \
or ask at http://www.PawelPilarczyk.com/.";


// --------------------------------------------------
// -------------------- my type ---------------------
// --------------------------------------------------

struct my_type
{
	int x, y;
}; /* my_type */

std::istream &operator >> (std::istream &in, my_type &m)
{
	in. get ();
	in >> m. x;
	in. get ();
	in >> m. y;
	in. get ();
	return in;
} /* operator >> */

std::ostream &operator << (std::ostream &out, const my_type &m)
{
	out << '(' << m. x << ',' << m. y << ')';
	return out;
} /* operator << */


// --------------------------------------------------
// ---------------------- main ----------------------
// --------------------------------------------------

int main (int argc, char *argv [])
{
	// show program's title
	cout << title << endl;

	// variables to be read form the command line
	double x [3];
	int counter = 0;
	my_type m;
	int n = 13;
	char *filename1, *filename2, *filename3;
	int show = 0;

	// declare all the command-line arguments
	arguments a;
	argbreak (a, "h");
	arg (a, "x", x, counter, 3);
	arg (a, "m", m);
	argoblig (a, "n" , n, -1);
	arg (a, NULL, filename1);
	argoblig (a, NULL, filename2);
	arg (a, NULL, filename3);
	argswitch (a, "s", show, 1);

	// analyze the command line
	argstreamprepare (a);
	int argresult = a. analyze (argc, argv);
	argstreamset ();

	// display an appropriate message if the result is nonzero
	if (argresult < 0)
	{
		cout << "-------------------------------------" << endl;
		cout << "There were " << (-argresult) << " errors." << endl;
		cout << "-------------------------------------" << endl;
	}
	else if (argresult > 0)
	{
		cout << helpinfo << endl;
		return 0;
	}

	// show the command-line arguments
	cout << "Arguments read from the command line:" << endl;
	cout << a;

	return 0;
} /* main */

/// @}

