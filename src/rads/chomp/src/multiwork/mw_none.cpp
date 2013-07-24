/// @addtogroup multiwork
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file mw_none.cpp
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

// Started on August 11, 2004. Last revision: January 13, 2006.


#include "chomp/multiwork/mw_none.h"


#if !mwNETWORK


namespace chomp {
namespace multiwork {


int mwSendBytes (int, const char *, int)
{
	return mwError;
} /* mwSendBytes */

int mwRecvBytes (int, char *, int)
{
	return mwError;
} /* mwRecvBytes */

int mwConnect (const char *, int)
{
	return mwError;
} /* mwConnect */

int mwListen (int, int)
{
	return mwError;
} /* mwListen */

int mwAccept (int, mwData &, int)
{
	return mwError;
} /* mwAccept */

int mwSelect (const int *, int, int, int *, int)
{
	return mwError;
} /* mwSelect */

void mwDisconnect (int)
{
	return;
} /* mwDisconnect */


} // namespace multiwork
} // namespace chomp

#endif // mwNETWORK

/// @}

