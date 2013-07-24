/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file bitcode.h
///
/// This file contains the declaration of functions for reading and writing
/// full cubical sets encoded in the text format as bit codes used in the
/// "chom" program written by Bill Kalies.
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

// Started on November 12, 2001. Last revision: August 16, 2007.


#ifndef _CHOMP_CUBES_BITCODE_H_
#define _CHOMP_CUBES_BITCODE_H_

#include "chomp/system/config.h"
#include "chomp/system/textfile.h"
#include "chomp/cubes/pointset.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

namespace chomp {
namespace homology {


// --------------------------------------------------
// --------------------- BITCODE --------------------
// --------------------------------------------------

/// Reads a set of full cubical sets represented as a set of points
/// from a file encoded in the "bitcode" format used by Bill Kalies.
/// The depth of bit codes is saved to '*bitcode_depth' unless NULL.
/// Returns 0 on success and -1 on error (and displays a message).
int readbitpoints (std::istream &in, pointset &p, int *bitcode_depth = NULL);

/// Writes a full cubical set represented by a set of points to a file
/// in the "bitcode" format.
/// If 'sorted', bit codes are sorted as the software by Bill Kalies needs.
/// Otherwise they are saved in the same order as the points in 'p'.
/// The depth of bit fields is determined automatically to the minimal
/// necessary value unless 'fixed_depth' is positive.
/// As the lower left corner, the minimal coordinates of the points are
/// selected unless 'fixed_corner' is NULL.
/// Returns 0 on success and -1 on error (and displays a message).
int writebitpoints (std::ostream &out, pointset &p, bool sorted = true,
	int fixed_depth = 0, coordinate *fixed_corner = NULL);


} // namespace homology
} // namespace chomp

#endif // _CHOMP_CUBES_BITCODE_H_

/// @}

