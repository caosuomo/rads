/// @addtogroup struct
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file bitsets.h
///
/// This file defines a class that uses bit representation of a set
/// to store many small sets.
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

// Started on February 22, 2007. Last revision: March 29, 2011.


#ifndef _CHOMP_STRUCT_BITSETS_H_
#define _CHOMP_STRUCT_BITSETS_H_

#include "chomp/system/config.h"

#include <iostream>
#include <cstdlib>

namespace chomp {
namespace homology {


// --------------------------------------------------
// ------------------- BitSets ---------------------
// --------------------------------------------------

/// This class uses bit representation to store many small sets.
/// Each of the sets can contain integer numbers from 0 to the limit
/// specified in the constructor. The number of sets must also be given
/// in the constructor. A contiguous chunk of memory is used to store
/// the bits that represent the sets.
/// This class is not fool-proof, so use it carefully.
/// Minimal interface, sorry.
class BitSets
{
	static const int_t longBits = sizeof (long) * 8;
	static const int_t longBitMask = longBits - 1;
	static const int_t longBitShift = (sizeof (long) == 8) ? 6 : 5;

public:
	/// Constructor of a family of empty sets.
	/// The number of the sets and the maximal number of elements
	/// in each set must be provided at initialization.
	/// Both numbers must be positive.
	BitSets (int_t _nsets, int_t _nelem);

	/// Copy constructor.
	BitSets (const BitSets &s);

	/// Assignment operator.
	BitSets &operator = (const BitSets &s);

	/// Destructor.
	~BitSets ();

	/// Adds an element to the given set.
	void add (int_t n, int_t e);

	/// Removes an element from the given set.
	void remove (int_t n, int_t e);

	/// Checks if an element belongs to the given set.
	bool check (int_t n, int_t e) const;

	/// Adds the entire set 'm' to the set 'n'.
	void addset (int_t n, int_t m);

	/// Retrieves an element >= 'start' in the given set.
	/// Returns -1 if none.
	int_t get (int_t n, int_t start = 0) const;

private:
	/// The number of sets.
	int_t nsets;
	
	/// The maximal number of elements in each set.
	int_t nelem;

	/// The number of array entries used for each set.
	int_t nunits;

	/// The memory buffer for storing the bits.
	unsigned long *bits;

	/// Computes the size of the memory array for storing the bits.
	int_t arraySize ();

}; /* BitSets */

// --------------------------------------------------

inline int_t BitSets::arraySize ()
{
	long size = static_cast<long> (nsets) * static_cast<long> (nunits);
	if (static_cast<long> (static_cast<int_t> (size)) != size)
		throw "Too large BitSets requested.";
	return static_cast<int_t> (size);
} /* BitSets::arraySize */

inline BitSets::BitSets (int_t _nsets, int_t _nelem):
	nsets (_nsets), nelem (_nelem),
	nunits ((_nelem + (sizeof (unsigned long) << 3) - 1) /
	(sizeof (unsigned long) << 3)), bits (0)
{
	int_t size = arraySize ();
	bits = new unsigned long [size];
	for (int_t i = 0; i < size; ++ i)
		bits [i] = 0;
	return;
} /* BitSets::BitSets */

inline BitSets::BitSets (const BitSets &s):
	nsets (s. nsets), nelem (s. nelem), nunits (s. nunits), bits (0)
{
	int_t size = arraySize ();
	bits = new unsigned long [size];
	for (int_t i = 0; i < size; ++ i)
		bits [i] = s. bits [i];
	return;
} /* BitSets::BitSets */

inline BitSets &BitSets::operator = (const BitSets &s)
{
	if (this == &s)
		return *this;
	delete [] bits;
	nsets = s. nsets;
	nelem = s. nelem;
	nunits = s. nunits;
	int_t size = arraySize ();
	bits = new unsigned long [size];
	for (int_t i = 0; i < size; ++ i)
		bits [i] = s. bits [i];
	return *this;
} /* BitSets::operator = */

inline BitSets::~BitSets ()
{
	delete [] bits;
	return;
} /* BitSets::~BitSets */

inline void BitSets::add (int_t n, int_t e)
{
//	sbug << "Add " << e << " to " << n << ".\n";
	unsigned long *buf = bits + n * nunits;
	buf [e >> longBitShift] |= (1ul << (e & longBitMask));
	return;
} /* BitSets::add */

inline void BitSets::remove (int_t n, int_t e)
{
	unsigned long *buf = bits + n * nunits;
	buf [e >> longBitShift] &= ~(1ul << (e & longBitMask));
	return;
} /* BitSets::remove */

inline bool BitSets::check (int_t n, int_t e) const
{
	unsigned long *buf = bits + n * nunits;
	return !!(buf [e >> longBitShift] & (1ul << (e & longBitMask)));
} /* BitSets::check */

inline void BitSets::addset (int_t n, int_t m)
{
	unsigned long *nbuf = bits + n * nunits;
	unsigned long *mbuf = bits + m * nunits;
	for (int_t i = 0; i < nunits; ++ i)
		*(nbuf ++) |= *(mbuf ++);
	return;
} /* BitSets::addset */

inline int_t BitSets::get (int_t n, int_t start) const
{
	if (start >= nelem)
		return -1;
	unsigned long *buf = bits + n * nunits;
	int_t offset = start >> longBitShift;
	int bit = start & longBitMask;
	if (buf [offset] & (~0ul << bit))
	{
		for (; bit < longBits; ++ bit)
		{
			if (buf [offset] & (1ul << bit))
				return (offset << longBitShift) + bit;
		}
		throw "Wrong bit number in BitSets.";
	}
	for (++ offset; offset < nunits; ++ offset)
	{
		if (!buf [offset])
			continue;
		for (int bit = 0; bit < longBits; ++ bit)
		{
			if (buf [offset] & (1ul << bit))
				return (offset << longBitShift) + bit;
		}
		throw "False bit in BitSets.";
	}
	return -1;
} /* BitSets::get */


} // namespace homology
} // namespace chomp

#endif // _CHOMP_STRUCT_BITSETS_H_

/// @}

