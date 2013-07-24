/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file homconn.cpp
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

// Started on November 14, 2005. Last revision: December 1, 2005.


#include "chomp/homology/homology.h"

#include <exception>
#include <new>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace chomp::homology;


// --------------------------------------------------
// -------------------- OVERTURE --------------------
// --------------------------------------------------

const char *title = "\
HomConnComp, ver. 0.01. Copyright (C) 1997-2010 by Pawel Pilarczyk.\n\
This is free software. No warranty. Consult 'license.txt' for details.";

const char *helpinfo = "\
Call with: X.cub.\n\
This is an example program which computes the homology of a set of\n\
full cubes in such a way that it first divides this set into connected\n\
components, then selects one cube in each component, and computes the\n\
relative homology of each of these sets. Finally, it retrieves the regular\n\
homology of the entire set and displays it.\n\
Optional arguments:\n\
-p p - compute the homology over Z modulo p instead of plain Z,\n\
-h - display this brief help information only and exit.\n\
For more information ask the author at http://www.PawelPilarczyk.com/.";


// --------------------------------------------------
// -------------------- HOMCONN ---------------------
// --------------------------------------------------

void homconn (SetOfCubes &X)
{
	int dim = X. empty () ? 0 : X [0]. dim ();

	// divide the set of cubes into connected components
	int_t comp = 0;
	multitable<SetOfCubes> Xcomp;
	while (!X. empty ())
	{
		// take any cube from X
		// and make it a base for a new connected component
		int_t number = X. size () - 1;
		Cube q = X [number];
		X. removenum (number);
		Xcomp [comp]. add (q);
		
		// add to this connected component all the neighbors from X
		// of all the cubes in this component
		for (int_t cur = 0; cur < Xcomp [comp]. size (); ++ cur)
		{
			coordinate coord [Cube::MaxDim];
			Xcomp [comp] [cur]. coord (coord);
			neighbors n (coord, dim);
			coordinate *c;
			while ((c = n. get ()) != 0)
			{
				if (!PointBase::check (c, dim))
					continue;
				Cube q (c, dim);
				if (!X. check (q))
					continue;
				X. remove (q);
				Xcomp [comp]. add (q);
			}
		}

		// consider the next connected component
		++ comp;
	}

	// show the zeroth homology group
	sout << "H_0 = Z";
	if (comp > 1)
		sout << "^" << comp;
	sout << "\n";

	// create pairs of sets and compute their homology
	for (int_t c = 0; c < comp; ++ c)
	{
		// skip trivial connected components
		if (Xcomp [c]. size () == 1)
			continue;

		// say which connected component is processed
		sout << "Connected component no. " << (c + 1) <<
			" (" << Xcomp [c]. size () << " cubes):\n";

		// prepare a pair of sets for relative homology computation
		SetOfCubes A;
		int_t number = Xcomp [c]. size () - 1;
		A. add (Xcomp [c] [number]);
		Xcomp [c]. removenum (number);
		
		// compute the relative homology
		Chain *chn;
		int maxlevel = Homology (Xcomp [c], "X", A, "A", chn);

		// display the result
		ShowHomology (chn, maxlevel);

		// release the memory assigned to the chain
		if (chn)
			delete [] chn;
	}

	return;
} /* homconn */

void homconn (const char *Xname, int p)
{
	// set the requested ring of coefficients if different from Z
	if (p > 0)
		integer::initialize (p);

	// read the set of cubes from a file
	sout << "Reading the set of cubes from '" << Xname << "'... ";
	SetOfCubes X;
	{
		std::ifstream in (Xname);
		if (!in)
			fileerror (Xname);
		in >> X;
	}
	sout << X. size () << " cubes read.\n";

	homconn (X);
} /* homconn */


// --------------------------------------------------
// ---------------------- MAIN ----------------------
// --------------------------------------------------

int main (int argc, char *argv [])
// Return: 0 = Ok, -1 = Error, 1 = Help displayed, 2 = Wrong arguments.
{
	// prepare user-configurable data
	char *Xname = NULL;
	int p = 0;

	// interprete the command-line arguments
	arguments a;
	arg (a, NULL, Xname);
	arg (a, "p", p);
	arghelp (a);

	argstreamprepare (a);
	int argresult = a. analyze (argc, argv);
	argstreamset ();

	// show the program's main title
	if (argresult >= 0)
		sout << title << '\n';

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

	// try running the main function and catch an error message if thrown
	try
	{
		// set an appropriate program time message
		program_time = "Aborted after:";

		// run the main procedure
		homconn (Xname, p);

		// set an appropriate program time message
		program_time = "Total time used:";
		program_time = 1;

		// finalize
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

