/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file psetglue.cpp
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

// Started on January 28, 2000. Last revision: November 23, 2002.


#include "chomp/system/config.h"
#include "chomp/system/textfile.h"
#include "chomp/cubes/pointset.h"
#include "chomp/system/timeused.h"
#include "chomp/system/arg.h"

#include <exception>
#include <new>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace chomp::homology;


// --------------------------------------------------
// -------------------- OVERTURE --------------------
// --------------------------------------------------

const char *title = "\
PointSet Glue, ver. 0.01. Copyright (C) 1997-2010 by Pawel Pilarczyk.\n\
This is free software. No warranty. Consult 'license.txt' for details.";

const char *helpinfo = "\
Call with: file1.cub ... filen.cub outfile.cub [-p].\n\
This program creates a set of points which is the union of given sets.\n\
Switches and additional arguments:\n\
-p  - make a pause before writing the result,\n\
-h  - display this brief help information only and exit.\n\
For more information ask the author at http://www.PawelPilarczyk.com/.";

#define MAXFILES 4096


// --------------------------------------------------
// ---------------------- GLUE ----------------------
// --------------------------------------------------

int psetglue (char *inputnames [], int numnames, const char *resultname, 
	bool makepause)
{
	pointset p;
	for (int n = 0; n < numnames; n ++)
	{
		sout << "Reading '" << inputnames [n] << "'... ";
		std::ifstream in (inputnames [n]);
		if (!in)
			fileerror (inputnames [n]);
		int prev = p. size ();
		in >> p;
		sout << " * " << (p. size () - prev) << " points added.\n";
	}

	if (makepause)
	{
		scon << "\n* * * PAUSE * * *\n"
			"Press 'y' followed by 'Enter' to write '" <<
			resultname << "'... ";
		char ch;
		std::cin >> ch;
		scon << '\n';
	}

	sout << "Writing the set to '" << resultname << "'... ";
	std::ofstream out (resultname);
	if (!out)
		fileerror (resultname, "create");
	out << "; This is a union of the following pointsets:\n";
	for (int n = 0; n < numnames; n ++)
		out << ";   " << inputnames [n] << '\n';
	out << p;
	sout << "Done! Thank you.\n";

	return 0;
} /* psetglue */


// --------------------------------------------------
// ---------------------- MAIN ----------------------
// --------------------------------------------------

int main (int argc, char *argv [])
{
	char *inputnames [MAXFILES];
	for (int i = 0; i < MAXFILES; i ++)
		inputnames [i] = NULL;
	int numnames = 0;
	char *resultname = NULL;
	bool makepause = false;

	// analyze the command line
	arguments a;
	arg (a, NULL, inputnames, numnames, MAXFILES);
	argswitch (a, "p", makepause, true);
	arghelp (a);

	argstreamprepare (a);
	int argresult = a. analyze (argc, argv);
	argstreamset ();

	// show the program's title
	if (argresult >= 0)
		sout << title << '\n';

	// correct the arguments
	if (numnames > 1)
		resultname = inputnames [-- numnames];
	else
		argresult = 1;

	// if something was incorrect, show an additional message and exit
	if (argresult < 0)
	{
		sout << "Call with '-help' for help.\n";
		return 2;
	}

	// if help requested or no filenames present, show help information
	if (argresult > 0)
	{
		sout << helpinfo << '\n';
		return 1;
	}

	// try running the main function and catch an error message if thrown
	try
	{
		psetglue (inputnames, numnames, resultname, makepause);
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

