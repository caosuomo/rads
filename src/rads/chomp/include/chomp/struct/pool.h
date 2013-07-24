/// @addtogroup struct
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file pool.h
///
/// This file contains the definition of a pool container
/// which keeps elements and manages their reusage.
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

// Started on August 13, 2007. Last revision: August 13, 2007.


#ifndef _CHOMP_STRUCT_POOL_H_
#define _CHOMP_STRUCT_POOL_H_


#include <chomp/struct/multitab.h>


namespace chomp {
namespace homology {


// --------------------------------------------------
// ---------------------- POOL ----------------------
// --------------------------------------------------

/// This template contains the definition of a pool of elements
/// that are stored in an extensible table. Elements that are
/// returned to the pool can be reused. Indexing of the elements
/// is done by integer numbers.
template <class element>
class pool
{
public:
	/// The constructor of a pool.
	pool ();

	/// The destructor of a pool.
	~pool ();

	/// Allocates a new element in the pool or finds a used one.
	/// Returns its number.
	int get ();

	/// Retrieves the element in the pool with the given number.
	element &operator [] (int number);

	/// Marks the given element as unused and available for retrieval
	/// the next time the pool is asked for an element.
	void release (int number);

private:
	/// The copy constructor is not allowed.
	pool (const pool<element> &);

	/// The assignment operator is not allowed.
	pool<element> &operator = (const pool<element> &);

	/// The multitable of elements.
	multitable<element> elem;

	/// The number of elements in the multitable.
	int elemCount;

	/// The multitable with indices of unused elements.
	multitable<int> unused;

	/// The number of unused elements in the multitable.
	int unusedCount;

}; /* class pool */

// --------------------------------------------------

template <class element>
inline pool<element>::pool (): elemCount (0), unusedCount (0)
{
	return;
} /* pool::pool */

template <class element>
inline pool<element>::~pool ()
{
	return;
} /* pool::~pool */

template <class element>
inline pool<element>::pool (const pool<element> &)
{
	throw "Copy constructor for a pool not implemented.";
	return;
} /* pool::pool */

template <class element>
inline pool<element> &pool<element>::operator = (const pool<element> &)
{
	throw "Assignment operator for a pool not implemented.";
	return *this;
} /* pool::operator = */

template <class element>
inline int pool<element>::get ()
{
	if (unusedCount)
		return unused [-- unusedCount];
	else
		return elemCount ++;
} /* pool::get */

template <class element>
inline element &pool<element>::operator [] (int n)
{
	return elem [n];
} /* pool::operator [] */

template <class element>
inline void pool<element>::release (int n)
{
	if (n == elemCount - 1)
		-- elemCount;
	else
		unused [unusedCount ++] = n;
	return;
} /* pool::release */


} // namespace homology
} // namespace chomp

#endif // _CHOMP_STRUCT_POOL_H_

/// @}

