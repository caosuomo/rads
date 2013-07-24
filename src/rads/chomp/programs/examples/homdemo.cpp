/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file homdemo.cpp
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

// Started on October 24, 2005. Last revision: October 24, 2005.

// --------------------------------------------------

// This program is a demonstration showing how one can use the simplest
// available interface to the CHomP library: the "chomp" function.

#include <iostream>
#include <iomanip>
using namespace std;

// include the main CHomP library header
#include "chomp/chomp.h"
using namespace chomp::homology;

// --------------------------------------------------

// define the coordinates of a few cubes which form a cubical set
int x [10 * 3] =
{
	0, 0, 0,
	1, 0, 1,
	-1, 0, 1,
	0, 0, 2,
	0, -1, 3,
	0, 1, 3,
	-1, 0, 3,
	1, 0, 3,
	0, 0, 4,
	0, 0, 5,
};

// --------------------------------------------------

int main ()
{
	cout << "Computing Betti numbers... " << flush;

	// prepare a table to store the computed Betti numbers
	int BettiNumbers [4];

	// compute the Betti numbers of the given set of cubes
	ComputeBettiNumbers (x, 10, 3, BettiNumbers);

	// display the sequence of the Betti numbers
	for (int i = 0; i <= 3; ++ i)
		cout << BettiNumbers [i] << ' ';
	cout << endl;

	// ------------------------------------------

	cout << "With periodic boundary conditions... " << flush;

	// set space wrapping every 3 units in the X direction
	// and every 6 units in the Z direction
	int wrap [3] = {3, 0, 6};
	SetSpaceWrapping (3, wrap);

	// compute the Betti numbers of the periodic set of cubes
	ComputeBettiNumbers (x, 10, 3, BettiNumbers);

	// display the sequence of the Betti numbers
	for (int i = 0; i <= 3; ++ i)
		cout << BettiNumbers [i] << ' ';
	cout << endl;

	return 0;
} /* main */

