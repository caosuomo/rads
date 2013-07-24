/// @addtogroup struct
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file autoarray.h
///
/// An auto_array template that mimics selected behaviors
/// of the std::auto_ptr template, but releases the memory
/// with delete[], and thus should be applied to arrays.
/// This template is based on what I found in the g++ header file "memory".
///
/// @author Pawel Pilarczyk
///
/////////////////////////////////////////////////////////////////////////////

// Copyright (C) 1997-2011 by Pawel Pilarczyk.
//
// This file is part of my research software package.  This is free software;
// you can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation;
// either version 2 of the License, or (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this software; see the file "license.txt".  If not, write to the
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.

// Started on April 10, 2011. Last revision: April 10, 2011.


#ifndef _CHOMP_STRUCT_AUTOARRAY_H_
#define _CHOMP_STRUCT_AUTOARRAY_H_


namespace chomp {
namespace homology {


// --------------------------------------------------
// ------------------- auto_array -------------------
// --------------------------------------------------

/// An auto_array template that mimics selected behaviors
/// of the std::auto_ptr template, but releases the memory
/// with delete[], and thus should be applied to arrays.
/// This template is based on what I found in the g++ header file "memory".
template <typename T>
class auto_array
{
private:
	T *ptr;

public:
	/// The type of the elements in the array.
	typedef T element_type;

	/// The constructor.
	explicit auto_array (element_type *p = 0) throw (): ptr (p) {}

	/// Copy constructor.
	auto_array (auto_array &a) throw (): ptr (a. release ()) {}

	/// Assignment operator.
	auto_array &operator = (auto_array &a) throw ()
	{
		reset (a. release ());
		return *this;
	}

	/// The destructor.
	~auto_array ()
	{
		if (ptr)
			delete [] ptr;
		return;
	}

	/// Returns the internally stored pointer to an array.
	element_type *get () const throw ()
	{
		return ptr;
	}

	/// Releases the pointer and returns it.
	element_type *release () throw ()
	{
		element_type *tmp = ptr;
		ptr = 0;
		return tmp;
	}

	/// Resets the object to hold another pointer.
	void reset (element_type *p = 0) throw ()
	{
		if (p != ptr)
		{
			if (ptr)
				delete [] ptr;
			ptr = p;
		}
	}

}; /* class auto_array */


} // namespace homology
} // namespace chomp

#endif // _CHOMP_STRUCT_AUTOARRAY_H_

/// @}

