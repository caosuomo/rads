/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file simreduc.cpp
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

// Started on September 5, 2003. Last revision: September 5, 2003.

// Former name(s) of this program: simdemo1.cpp (until November 11, 2003).


#include "chomp/homology/homology.h"

#include <exception>
#include <cstdlib>
#include <new>
#include <iostream>
#include <fstream>

using namespace chomp::homology;


// --------------------------------------------------
// -------------------- OVERTURE --------------------
// --------------------------------------------------

const char *title = "\
SIMREDUC, ver. 0.01. Copyright (C) 1997-2010 by Pawel Pilarczyk.\n\
This is free software. No warranty. Consult 'license.txt' for details.";

const char *helpinfo = "\
Call with: X.sim [[A.sim] result.sim].\n\
This is a demonstration program which collapses a simplicial complex\n\
or a pair of simplicial complexes and writes the result to the given file.\n\
Optional arguments:\n\
-h - display this brief help information only and exit.\n\
For more information ask the author at http://www.PawelPilarczyk.com/.";


// --------------------------------------------------
// ---------------------- DEMO ----------------------
// --------------------------------------------------

void demo (char *Xname, char *Aname, char *resultname)
{
	// read the first set of simplices
	simplicialcomplex X;
	readcells (Xname, X, "X");

	// read the second set of cubes if any
	simplicialcomplex A;
	readcells (Aname, A, "A");

	// make the sets disjoint if they are not
	removeAfromX (X, A, "X", "A");

	// determine the maximal dimension of the simplices
	int dim = X. dim ();

	// decrease the dimension of A if necessary
	decreasedimension (A, dim, "A");

	// collapse the pair of simplicial complexes and remove A from X
	collapse (X, A, "X", "A");

	// save the result to a file if requested to
	if (!resultname)
		return;
	sout << "Writing the result to '" << resultname << "'... ";
	std::ofstream out (resultname);
	if (!out)
		fileerror (resultname, "create");
	out << X;
	sout << X. size () << " simplices saved.\n";

	return;
} /* demo */


// --------------------------------------------------
// ---------------------- MAIN ----------------------
// --------------------------------------------------

int main (int argc, char *argv [])
// Return: 0 = Ok, -1 = Error, 1 = Help displayed, 2 = Wrong arguments.
{
	// prepare user-configurable data
	char *Xname = NULL, *Aname = NULL, *resultname = NULL;

	// interprete the command-line arguments
	arguments a;
	arg (a, NULL, Xname);
	arg (a, NULL, Aname);
	arg (a, NULL, resultname);
	arghelp (a);

	argstreamprepare (a);
	int argresult = a. analyze (argc, argv);
	argstreamset ();

	// show the program's main title
	if (argresult >= 0)
		sout << title << '\n';

	// make a correction to the names
	if (!resultname)
	{
		resultname = Aname;
		Aname = NULL;
	}

	// if there are not enough file names, help should be displayed
	if (!Xname)
		argresult = 1;

	// if something was incorrect, show an additional message and exit
	if (argresult < 0)
	{
		sout << "Call with '--help' for help.\n";
		return 2;
	}

	// if help requested, show help information
	if (argresult > 0)
	{
		sout << helpinfo << '\n';
		return 1;
	}

	// set an appropriate program time message
	program_time = "Aborted after:";

	// try running the main function and catch an error message if thrown
	try
	{
		demo (Xname, Aname, resultname);
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

	// set an appropriate program time message
	program_time = "Total time used:";
	return 0;
} /* main */

/// @}

