/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file simbetti.cpp
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

// Started on September 4, 2003. Last revision: February 3, 2011.

// Former name(s) of this program: simdemo2.cpp (until November 11, 2003).


#include "chomp/homology/homology.h"

#include <exception>
#include <cstdlib>
#include <new>
#include <iostream>
#include <fstream>
#include <vector>

using namespace chomp::homology;


// --------------------------------------------------
// -------------------- OVERTURE --------------------
// --------------------------------------------------

const char *title = "\
SIMBETTI, ver. 0.01. Copyright (C) 1997-2010 by Pawel Pilarczyk.\n\
This is free software. No warranty. Consult 'license.txt' for details.";

const char *helpinfo = "\
Call with: file1.sim [file2.sim].\n\
This is a demonstration program which computes the Betti numbers\n\
of a given simplicial complex or a pair of simplicial complexes.\n\
Optional arguments:\n\
-p p - compute the homology over Z modulo p instead of Z,\n\
-g - compute and show homology generators,\n\
-h - display this brief help information only and exit.\n\
For more information ask the author at http://www.PawelPilarczyk.com/.";


// --------------------------------------------------
// ---------------------- DEMO ----------------------
// --------------------------------------------------

void demo (char *Xname, char *Aname, int p, bool generators)
{
	// add an empty line for clearer output
	sout << '\n';

	// set the requested ring of coefficients if different from Z
	if (p > 0)
		integer::initialize (p);

	// read the sets from the input files
	SimplicialComplex X, A;
	readcells (Xname, X, "X");
	readcells (Aname, A, "A");

	// compute the homology of the pair
	Chain *hom;
	Chain **gen;
	int maxlevel = Homology (X, "X", A, "A", hom,
		generators ? &gen : NULL);

	// decode the homology information manually
	// (note: the output is written to the "debug" stream,
	// call the program with "--debug" to see it)
	std::vector<int> betti_numbers (maxlevel + 1);
	for (int q = 0; q <= maxlevel; ++ q)
	{
		// get the chain that represents this homology group
		Chain &hom_q = hom [q];
		sbug << "Chain representation of H_" << q << ": " <<
			hom_q << ":\n";

		// analyze the chain to find the q-th Betti number
		// and the torsion coefficients
		betti_numbers [q] = 0;
		for (int i = 0; i < hom_q. size (); ++ i)
		{
			// each invertible entry corresponds to a free factor
			if (hom_q. coef (i). delta () == 1)
			{
				sbug << "- Pos. " << i << ": free factor.\n";
				++ betti_numbers [q];
			}

			// each non-invertible entry corresponds to
			// a torsion subgroup
			else
			{
				integer torsion = hom_q. coef (i);
				sbug << "- Pos. " << i << ": twisted by " <<
					torsion << ".\n";
			}
		}
	}

	// decode the homology generators manually, if they were computed
	// (note: the output is written to the "debug" stream,
	// call the program with "--debug" to see it)
	std::vector<std::vector<std::vector<Simplex> > >
		hom_generators (maxlevel + 1);
	for (int q = 0; q <= maxlevel; ++ q)
	{
		// skip this loop if the generators haven't been computed
		if (!generators)
			break;

		// prepare array for storing generators at level q
		std::vector<std::vector<Simplex> > &hom_generators_q =
			hom_generators [q];

		// get the array of chains that represent homology generators
		// at level q
		Chain *gen_q = gen [q];

		// determine the number of these generators from chain
		// that represents the homology q-th homology group
		int gen_count = hom [q]. size ();

		// prepare space in the vector to store these generators
		hom_generators_q. resize (gen_count);

		// determine all the generators at level q one by one
		for (int i = 0; i < gen_count; ++ i)
		{
			// get the chain that represents this generator
			Chain &gen_q_i = gen_q [i];

			// determine the number of simplices in it
			int gen_size = gen_q_i. size ();

			// prepare an array for simplices in this generator
			std::vector<Simplex> &hom_generators_q_i =
				hom_generators_q [i];

			// allocate sufficient amount of memory in the array
			hom_generators_q_i. resize (gen_size);

			// determine all the simplices in this generator
			for (int j = 0; j < gen_size; ++ j)
			{
				// determine the number of the simplex
				int simplex_number = gen_q_i. num (j);

				// ignore the coefficient in the generator
				integer coef = gen_q_i. coef (j);

				// find it in the simplicial complex
				const Simplex &s = X [q] [simplex_number];

				// add this simplex to the vector
				hom_generators_q_i [j] = s;
			}
		}

		// show homology generators at this level
		sbug << "Homology generators at level " << q << ":\n";
		for (size_t i = 0; i < hom_generators [q]. size (); ++ i)
		{
			sbug << "- generator " << i << ":";
			for (size_t j = 0; j < hom_generators [q] [i].
				size (); ++ j)
			{
				sbug << " " << hom_generators [q] [i] [j];
			}
			sbug << "\n";
		}
	}

	// show the computed homology to confirm the result
	ShowHomology (hom, maxlevel);

	// add an empty line for clearer output
	sout << '\n';

	// show the generators of homology and release the data
	if (generators && (maxlevel >= 0))
	{
		sout << "The computed homology generators are as follows:\n";
		ShowGenerators (gen, hom, maxlevel, X);
		for (int q = 0; q <= maxlevel; ++ q)
			delete [] (gen [q]);
		delete [] gen;
	}

	// show Betti numbers
	if (maxlevel < 0)
		sout << "All the Betti numbers are zero.\n\n";
	else
	{
		sout << "The computed Betti numbers are:";
		for (int q = 0; q <= maxlevel; ++ q)
			sout << ' ' << BettiNumber (hom [q]);
		sout << ".\nAll the remaining Betti numbers are zero.\n\n";
	}

	// release the allocated data from memory and finish
	if (hom)
		delete [] hom;
	return;
} /* demo */


// --------------------------------------------------
// ---------------------- MAIN ----------------------
// --------------------------------------------------

int main (int argc, char *argv [])
// Return: 0 = Ok, -1 = Error, 1 = Help displayed, 2 = Wrong arguments.
{
	// prepare user-configurable data
	char *Xname = NULL, *Aname = NULL;
	int p = 0;
	bool generators = false;

	// interprete the command-line arguments
	arguments a;
	arg (a, NULL, Xname);
	arg (a, NULL, Aname);
	arg (a, "p", p);
	argswitch (a, "g", generators, true);
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
		program_time = 1;

		// run the main procedure
		demo (Xname, Aname, p, generators);

		// set an appropriate program time message
		program_time = "Total time used:";

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

