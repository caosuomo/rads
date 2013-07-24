/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file cell.h
///
/// This file includes header files with various definitions of cubical
/// cells and defines the standard types for most commonly used structures
/// related to cubical cells.
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

// Started in January 2002. Last revision: November 5, 2004.


#ifndef _CHOMP_CUBES_CELL_H_
#define _CHOMP_CUBES_CELL_H_

#include "chomp/system/config.h"
#include "chomp/system/textfile.h"
#include "chomp/cubes/pointset.h"
#include "chomp/homology/chains.h"
#include "chomp/struct/bitfield.h"
#include "chomp/struct/integer.h"
#include "chomp/struct/hashsets.h"
#include "chomp/homology/gcomplex.h"
#include "chomp/cubes/pointbas.h"
#include "chomp/cubes/cellmain.h"
#include "chomp/cubes/cellbase.h"
#include "chomp/cubes/cellfix.h"
#include "chomp/cubes/cellvar.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

namespace chomp {
namespace homology {


/// The default type of a cubical cell.
typedef tCellBase<coordinate> CubicalCell;

/// An alternative name for a cubical cell.
typedef CubicalCell ElementaryCell;

/// The default type of a set of cubical cells.
typedef hashedset<CubicalCell> SetOfCubicalCells;

/// The default type of a cubical complex.
typedef gcomplex<CubicalCell,integer> CubicalComplex;

/// The default type of a cubical multivalued map. The image of each
/// cubical cell is a set of cubical cells.
typedef mvcellmap<CubicalCell,integer,CubicalCell> CubicalMultivaluedMap;


/// An abbreviation for a cubical cell [deprecated].
typedef CubicalCell qcell;

/// An abbreviation for a set of cubical cell [deprecated].
typedef SetOfCubicalCells qcells;

/// An abbreviation for a cubical complex [deprecated].
typedef CubicalComplex cubicalcomplex;


} // namespace homology
} // namespace chomp

#endif // _CHOMP_CUBES_CELL_H_

/// @}

