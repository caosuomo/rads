/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file chomp.h
///
/// This file includes most of the other CHomP library header files.
/// For simplicity of use, one can just include this header file
/// without taking care about the remaining header files in the library.
/// Note that each program that includes this file must be linked
/// with the compiled CHomP library.
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

// Started on October 26, 2005. Last revision: October 27, 2005.


#ifndef _CHOMP_CHOMP_H_
#define _CHOMP_CHOMP_H_

/// \mainpage The CHomP C++ Library Documentation
///
/// \section intro Introduction
///
/// This documentation of the CHomP library is currently under construction.
/// Although many classes and functions already have their descriptions,
/// I am aware of the fact that these descriptions are not as detailed
/// as the user might wish to have. Therefore, this documentation
/// has been generated in such a way that it includes the entire source code
/// which sould be consulted if in doubt. The code has many additional
/// comments which shed light on what it actually does.
///
/// I am still working on making this documentation more informative
/// and complete, but it takes A LOT of time.
/// Therefore, I apologize for any inconvenience caused by the incomplete
/// documentation and I invite to browse the source code instead,
/// or ask me specific questions by email.
///
/// \section begin Where to Begin
///
/// If in doubt, I recommend to begin browsing with the contents of the
/// file "chomp/homology/homology.h" which contains some high-level interface
/// for the homology computation.
///
/// It is also a good idea to see the contents of header files in the
/// "capd/homengin/" subdirectory. This is a multi-engine interface
/// to the homology computation routines of various kinds which mainly
/// represent full cubical sets in terms of bitmaps.
///
/// \section examples Examples
///
/// Several examples of how the CHomP library can be used in one's own
/// programs are collected in the "programs/examples/" subdirectory
/// of the CHomP source code distribution.
/// These examples are mainly stand-alone simple small programs which do
/// one specific thing and demonstrate how to make use of the CHomP library
/// functions and classes.
///
/// Pawel Pilarczyk

#include "chomp/system/config.h"
#include "chomp/system/textfile.h"
#include "chomp/system/timeused.h"
#include "chomp/system/arg.h"
#include "chomp/bitmaps/bitmaps.h"
#include "chomp/struct/digraph.h"
#include "chomp/homology/homology.h"


/// This namespace contains the entire CHomP library interface.
/// Although currently some classes or functions are available in the
/// global namespace, eventually everything should be gathered in this
/// namespace, divided into several other specialized namespaces.
namespace chomp {

} // namespace chomp


#endif // _CHOMP_CHOMP_H_

/// @}

