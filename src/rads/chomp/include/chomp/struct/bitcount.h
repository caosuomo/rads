/// @addtogroup struct
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file bitcount.h
///
/// This file defines a very simple function for counting the number of bits
/// in a byte or a multi-byte integer.
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

// Started on November 29, 2005. Last revision: November 29, 2005.


#ifndef _CHOMP_STRUCT_BITCOUNT_H_
#define _CHOMP_STRUCT_BITCOUNT_H_

namespace chomp {
namespace homology {


extern unsigned char bitcounttable [];

inline int bitcountbyte (char n)
{
	return bitcounttable [static_cast<unsigned char> (n)];
} /* bitcountbyte */

inline int bitcount (int number)
{
	if (!number)
		return 0;
	unsigned int n = static_cast<unsigned int> (number);
	if (n < 256)
		return bitcountbyte (static_cast<unsigned char> (n));
	int c = 0;
	while (n > 255)
	{
		if (n & 1)
			++ c;
		n >>= 1;
	}
	return c + bitcountbyte (static_cast<unsigned char> (n));
} /* bitcount */


} // namespace homology
} // namespace chomp

#endif // _CHOMP_STRUCT_BITCOUNT_H_

/// @}

