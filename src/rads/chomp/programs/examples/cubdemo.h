/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file cubdemo.h
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

// Started on February 15, 2003. Last revision: September 6, 2003.


#ifndef _CUBDEMO_H_
#define _CUBDEMO_H_

#include "chomp/homology/homology.h"

using namespace chomp::homology;


// --------------------------------------------------
// -------------------- READ MAP --------------------
// --------------------------------------------------

// Read a map from the given file.
void readmap (const char *filename, cubicalmap &F, const char *name);


// --------------------------------------------------
// ------------------- INCLUSIONS -------------------
// --------------------------------------------------

// Reduce the full-dimensional sets of cubes in such a way that
// no information is lost if one is interested in computing the homomorphisms
// induced in homology by the inclusion maps between the subsequent sets.
// Note: The sets are modified such that they form an ascending sequence.
void ReduceFullDimCubes (cubes Xcubes [], cubes Acubes [], int n);

// Compute the homomorphisms induced in homology by the inclusion maps
// of the pairs of sets (X [i], A [i]) into (X [i + 1], A [i + 1]).
// Allocate tables of chain complexes representing the homology of each
// set or pair of sets and allocate maps induced in homology between them.
// Store the generators of each pair into the corresponding table
// of cubical complexes.
void InclusionMapsHomology (cubes Xcubes [], cubes Acubes [],
	int n, chaincomplex<integer> **&homology,
	chainmap<integer> **&inclusions,
	multitable<cubicalcomplex> generators []);

#endif // _CUBDEMO_H_

/// @}

