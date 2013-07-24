/// @addtogroup multiwork
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file multiwork/mw.h
///
/// This is the main header file in the MultiWork group which includes
/// all the other header files.
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

// Started on August 10, 2004. Last revision: November 29, 2007.


#ifndef _CHOMP_MULTIWORK_MW_H_
#define _CHOMP_MULTIWORK_MW_H_


// --------------------------------------------------
// --------------------- config ---------------------
// --------------------------------------------------
// configuration and constants

#include "chomp/multiwork/mwconfig.h"


// --------------------------------------------------
// --------------------- mwData ---------------------
// --------------------------------------------------
// portable data buffer

#include "chomp/multiwork/mwdata.h"


// --------------------------------------------------
// --------------------- mwTask ---------------------
// --------------------------------------------------
// the two task classes

#include "chomp/multiwork/mwworker.h"
#include "chomp/multiwork/mwcoord.h"


namespace chomp {

/// This namespace contains an interface to the MultiWork module
/// which allows one to easily distribute a computational task
/// over multiple processes and run concurrent computations.
namespace multiwork {


} // namespace multiwork
} // namespace chomp

#endif // _CHOMP_MULTIWORK_MW_H_

/// @}

