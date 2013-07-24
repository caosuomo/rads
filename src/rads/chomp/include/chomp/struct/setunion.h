/// @addtogroup struct
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file setunion.h
///
/// This file contains the definition of the container "setunion".
/// The purpose of this container is to temporarily represent a union
/// of two hashed sets of elements of the same type without actually
/// merging the two hashed sets. Both sets cannot be modified through
/// their union, they can only be accessed in the 'const' mode.
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

// Started on April 22, 2008. Last revision: April 23, 2008.


#ifndef _CHOMP_STRUCT_SETUNION_H_
#define _CHOMP_STRUCT_SETUNION_H_

#include "chomp/system/config.h"

namespace chomp {
namespace homology {


// class templates defined within this header file (in this order):
template <class set1type, class set2type>
class setunion;


// --------------------------------------------------
// ------------------- set union --------------------
// --------------------------------------------------

/// A union of two hashed sets. Thanks to the template style definition,
/// it can be used to define a union of unions of sets etc., although
/// the efficiency of this solution decreases with the increasing
/// recursion level.
template <class set1type, class set2type>
class setunion
{
public:
	/// The type of the element of each of the sets.
	typedef typename set1type::value_type value_type;

	/// The only allowed constructor.
	setunion (const set1type &_set1, const set2type &_set2);

	/// The copy constructor.
	setunion (const setunion<set1type,set2type> &s);

	/// The assignment operator.
	setunion &operator = (const setunion<set1type,set2type> &s);

	/// The destructor.
	~setunion ();

	/// Returns a const reference to the first set in the union.
	const set1type &get1 () const;

	/// Returns a const reference to the second set in the union.
	const set2type &get2 () const;

	/// Finds the given element and returns its number.
	/// Returns -1 if the element is not in the union of the sets.
	int_t getnumber (const typename set1type::value_type &e) const;

	/// Checks if the given number is an index of some element in the
	/// set union. That is, checks if the number is non-negative and
	/// strictly smaller than the number of elements in the set union.
	/// Returns true if yes, false if no.
	bool checknum (int_t n) const;

	/// Checks if the given element is in the set union.
	/// Returns true if yes, false if no.
	bool check (const typename set1type::value_type &e) const;

	/// Returns the element with the given number from the set union.
	const typename setunion<set1type,set2type>::value_type &
		operator [] (int_t n) const;

	/// Returns the element with the given number from the set union.
	const typename setunion<set1type,set2type>::value_type &get (int_t n)
		const;

	/// Returns the number of elements in the set union.
	int_t size () const;

	/// Returns true if both sets are empty. Otherwise returns false.
	bool empty () const;

private:
	/// Reference to the first set.
	const set1type *set1;

	/// Reference to the second set.
	const set2type *set2;

}; /* class setunion */

// --------------------------------------------------

template <class set1type, class set2type>
inline setunion<set1type,set2type>::setunion (const set1type &_set1,
	const set2type &_set2): set1 (&_set1), set2 (&_set2)
{
	return;
} /* setunion::setunion */

template <class set1type, class set2type>
inline setunion<set1type,set2type>::~setunion ()
{
	return;
} /* setunion::~setunion */

template <class set1type, class set2type>
inline setunion<set1type,set2type>::setunion
	(const setunion<set1type,set2type> &)
{
	throw "Trying to use the copy constructor of a set union.";
	return;
} /* setunion::setunion */

template <class set1type, class set2type>
inline setunion<set1type,set2type> &setunion<set1type,set2type>::
	operator = (const setunion<set1type,set2type> &)
{
	throw "Trying to use the assignment operator of a set union.";
	return;
} /* setunion::setunion */

template <class set1type, class set2type>
inline const set1type &setunion<set1type,set2type>::get1 () const
{
	return *set1;
} /* setunion::get1 */

template <class set1type, class set2type>
inline const set2type &setunion<set1type,set2type>::get2 () const
{
	return *set2;
} /* setunion::get2 */

template <class set1type, class set2type>
inline int_t setunion<set1type,set2type>::getnumber
	(const typename set1type::value_type &e) const
{
	int_t n = set1 -> getnumber (e);
	if (n >= 0)
		return n;
	n = set2 -> getnumber (e);
	if (n >= 0)
		return set1 -> size () + n;
	else
		return n;
} /* setunion::getnumber */

template <class set1type, class set2type>
inline bool setunion<set1type,set2type>::checknum (int_t n) const
{
	return ((n >= 0) && (n < set1 -> size () + set2 -> size ()));
} /* setunion::checknum */

template <class set1type, class set2type>
inline bool setunion<set1type,set2type>::check
	(const typename set1type::value_type &e) const
{
	return (set1 -> check (e) || set2 -> check (e));
} /* setunion::check */

template <class set1type, class set2type>
inline const typename setunion<set1type,set2type>::value_type &
	setunion<set1type,set2type>::get (int_t n) const
{
	int_t size1 = set1 -> size ();
	if (n < size1)
		return set1 -> get (n);
	else
		return set2 -> get (n - size1);
} /* setunion::get */

template <class set1type, class set2type>
inline const typename setunion<set1type,set2type>::value_type &
	setunion<set1type,set2type>::operator [] (int_t n) const
{
	return get (n);
} /* setunion::operator [] */

template <class set1type, class set2type>
inline int_t setunion<set1type,set2type>::size () const
{
	return (set1 -> size () + set2 -> size ());
} /* setunion::size */

template <class set1type, class set2type>
inline bool setunion<set1type,set2type>::empty () const
{
	return (set1 -> empty () && set2 -> empty ());
} /* setunion::empty */

// --------------------------------------------------

/// Creates an object which represents the union of two sets.
template <class set1type, class set2type>
inline setunion<set1type,set2type> makesetunion (const set1type &set1,
	const set2type &set2)
{
	return setunion<set1type,set2type> (set1, set2);
} /* makesetunion */

// Assigns the union of two sets to a single set using the assignment
// operator and the "add" function.
//template <class set1type, class set2type>
//inline set1type &operator = (set1type &set1,
//	const setunion<set1type,set2type> &set2)
//{
//	set1 = set2. get1 ();
//	set1. add (set2. get2 ());
//	return set1;
//} /* operator = */


} // namespace homology
} // namespace chomp

#endif // _CHOMP_STRUCT_SETUNION_H_

/// @}

