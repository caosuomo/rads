/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file tabulate.h
///
/// This file contains the definition of a class which stores
/// tabulated configuration of full cubical neighborhoods
/// used in the cubical reduction procedures.
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

// Started in January 2002. Last revision: June 2, 2007.


#ifndef _CHOMP_HOMOLOGY_TABULATE_H_
#define _CHOMP_HOMOLOGY_TABULATE_H_

#include "chomp/system/config.h"
#include "chomp/system/textfile.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

namespace chomp {
namespace homology {

/// A class for storing tabulated configurations of neighbors
/// for various dimensions.
class Tabulated
{
public:
	/// The default constructor.
	Tabulated ();

	/// The destructor.
	~Tabulated ();

	/// Reads tabulated configurations from a file.
	int read (int dim, const char *filename);

	/// Writes tabulated configurations to a file.
	int write (int dim, const char *filename) const;

	/// Computes tabulated configurations for a specific dimension.
	int compute (int dim);

	/// Sets tabulated configuration bits to a given table.
	/// Note: This buffer will not be deallocated automatically.
	/// Call this function with the null pointer to disable the table.
	int define (int dim, char *buffer);

	/// Retrieves the buffer allocated for the specific dimension
	/// or returns the null pointer if none.
	const char *operator [] (int dim) const;

	/// Retrieve the given bit from the given table.
	static int get (const char *table, int_t bitnumber);

	/// Sets the given bit in the given table.
	static void set (char *table, int_t bitnumber);

private:
	/// The strict upper bound for the supported dimensions.
	static const int maxdim = 8;

	/// The tabulated configurations.
	char *tables [maxdim];

	/// Should the configuration tables be deallocated?
	bool deallocate [maxdim];

	/// The size of the table in bytes for each dimension.
	int size [maxdim];
	
}; /* Tabulated */

// --------------------------------------------------

inline int Tabulated::define (int dim, char *buffer)
{
	if ((dim <= 0) || (dim >= maxdim))
		return -1;
	if (tables [dim] && deallocate [dim])
	{
		delete [] (tables [dim]);
		deallocate [dim] = false;
	}
	tables [dim] = buffer;
	return 0;
} /* Tabulated::define */

inline const char *Tabulated::operator [] (int dim) const
{
	if ((dim <= 0) || (dim >= maxdim))
		return 0;
	else
		return tables [dim];
} /* Tabulated::operator [] */

inline int Tabulated::get (const char *table, int_t bitnumber)
{
	if (table [bitnumber >> 3] & (1 << (bitnumber & 0x07)))
		return 1;
	else
		return 0;
} /* Tabulated::get */

inline void Tabulated::set (char *table, int_t bitnumber)
{
	table [bitnumber >> 3] |=
		static_cast<char> (1 << (bitnumber & 0x07));
	return;
} /* Tabulated::set */

// --------------------------------------------------

/// The global instance of this class which stores tabulated configurations
/// to use in the full cube reduction procedures.
extern Tabulated tabulated;


} // namespace homology
} // namespace chomp

#endif // _CHOMP_HOMOLOGY_TABULATE_H_

/// @}

