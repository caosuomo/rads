/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file simchain.cpp
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

// Started in August 1997 as CubChain. Last revision: November 1, 2003.

// Note: This program replaces the program "simplchn.cpp".


#include "chomp/system/config.h"
#include "chomp/system/textfile.h"
#include "chomp/simplices/simplex.h"
#include "chomp/homology/homtools.h"
#include "chomp/system/timeused.h"
#include "chomp/system/arg.h"

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
SIMCHAIN, ver. 2.10. Copyright (C) 1997-2010 by Pawel Pilarczyk.\n\
This is free software. No warranty. Consult 'license.txt' for details.";

const char *helpinfo = "\
Call with: X.sim [A.sim] CX.chn.\n\
This program creates a chain complex in the format compatible with\n\
'homchain' from a simplicial complex. If two input file names are given,\n\
a quotient chain complex of this pair of simplicial complexes is created.\n\
Consult the accompanying documentation for the input text data format.\n\
Optional arguments:\n\
-d - use digital names (numbers) to save space and to use 'homchain -d'.\n\
-s - use symbolic names for simplices (opposite to '-d'; default),\n\
-h - display this brief help information only and exit.\n\
For more information ask the author at http://www.PawelPilarczyk.com/.";


// --------------------------------------------------
// -------------------- SIMCHAIN --------------------
// --------------------------------------------------

static int writechaincomplex (const simplicialcomplex &Xcompl,
	const char *Xname, const char *Aname,
	const char *outname, bool symbolicnames)
// Write a chain complex of the given simplicial complex to an output file.
{
	if (!outname)
		return 0;
	sout << "Writing the chain complex to '" << outname << "'... ";
	std::ofstream out (outname);
	if (!out)
		fileerror (outname, "create");
	out << "; This is a chain complex generated by SIMCHAIN\n";
	out << "; from the simplices stored in '" << Xname;
	if (Aname)
		out << "' and '" << Aname;
	out << "'.\n";

	writechaincomplex (out, Xcompl, symbolicnames);

	sout << "Done.\n";
	return 0;
} /* writechaincomplex */

static int simchain (char *Xname, char *Aname, char *outname,
	bool symbolicnames)
// Write a chain complex of a simplicial complex - the main procedure.
// Return: 0 = Ok, -1 = Error (show messages).
{
	// read the first simplicial complex
	simplicialcomplex Xcompl;
	readcells (Xname, Xcompl, "X");

	// read the second simplicial complex if any
	simplicialcomplex Acompl;
	readcells (Aname, Acompl, "A");

	// decrease the dimension of A to the dimension of X
	if (Acompl. dim () > Xcompl. dim ())
		decreasedimension (Acompl, Xcompl. dim (), "A");

	// remove from X cells which are in A
	removeAfromX (Xcompl, Acompl, "X", "A");

	// if the set X is empty, the answer is obvious
	if (Xcompl. empty ())
	{
		sout << "The set X is contained in A. The relative "
			"chain complex of (X,A) is trivial.\n";
		return 0;
	}

	// create a full simplicial complex (with all the faces) of X\A
	addboundaries (Xcompl, Acompl, 0, false, "X", "A");

	// write the chain complex to a file
	writechaincomplex (Xcompl, Xname, Aname, outname, symbolicnames);

	return 0;
} /* simchain */


// --------------------------------------------------
// ---------------------- MAIN ----------------------
// --------------------------------------------------

int main (int argc, char *argv [])
// Return: 0 = Ok, -1 = Error, 1 = Help displayed, 2 = Wrong arguments.
{
	// prepare user-configurable data
	char *Xname = NULL, *Aname = NULL, *outname = NULL;
	bool symbolicnames = true;

	// interprete the command-line arguments
	arguments a;
	arg (a, NULL, Xname);
	arg (a, NULL, Aname);
	arg (a, NULL, outname);
	argswitch (a, "s", symbolicnames, true);
	argswitch (a, "d", symbolicnames, false);
	arghelp (a);

	argstreamprepare (a);
	int argresult = a. analyze (argc, argv);
	argstreamset ();

	// show the program's main title
	if (argresult >= 0)
		sout << title << '\n';

	// shift names if necessary
	if (!outname)
	{
		outname = Aname;
		Aname = NULL;
	}

	// if something was incorrect, show an additional message and exit
	if (argresult < 0)
	{
		sout << "Call with '--help' for help.\n";
		return 2;
	}

	// if help requested or no filenames present, show help information
	if (!Xname || !outname || (argresult > 0))
	{
		sout << helpinfo << '\n';
		return 1;
	}

	// try running the main function and catch an error message if thrown
	try
	{
		simchain (Xname, Aname, outname, symbolicnames);
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

