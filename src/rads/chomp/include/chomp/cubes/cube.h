/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file cube.h
///
/// This file includes header files with various definitions of full cubes
/// and defines the standard types for most commonly used structures
/// related to full cubes.
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

// Started in January 2002. Last revision: October 25, 2005.


#ifndef _CHOMP_CUBES_CUBE_H_
#define _CHOMP_CUBES_CUBE_H_

#include "chomp/system/config.h"
#include "chomp/system/textfile.h"
#include "chomp/cubes/pointset.h"
#include "chomp/struct/bitfield.h"
#include "chomp/struct/integer.h"
#include "chomp/struct/hashsets.h"
#include "chomp/cubes/pointbas.h"
#include "chomp/cubes/cubemain.h"
#include "chomp/cubes/cubebase.h"
#include "chomp/cubes/cubefix.h"
#include "chomp/cubes/cubevar.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>


namespace chomp {
namespace homology {


/// The default cube type.
typedef tCubeBase<coordinate> Cube;

/// An alternative name for a cube.
typedef Cube FullCube;

/// An alternative name for a cube.
typedef Cube HyperCube;

/// The default type of a set of cubes.
typedef hashedset<Cube> SetOfCubes;

/// The default type of a combinatorial cubical multivalued map.
/// This is a map which assigns a set of cubes to each cube in its domain.
typedef mvmap<Cube,Cube> CombinatorialMultivaluedMap;

/// A lower-case name of a cube [deprecated].
typedef Cube cube;

/// An abbreviation for a set of cubes [deprecated].
typedef SetOfCubes cubes;

/// An abbreviation for a combinatorial cubical multivalued map.
typedef CombinatorialMultivaluedMap CubicalMap;

/// A lower-case version of the name of a combinatorial cubical
/// multivalued map [deprecated].
typedef CombinatorialMultivaluedMap cubicalmap;


} // namespace homology
} // namespace chomp

#endif // _CHOMP_CUBES_CUBE_H_

/// @}

