/// @addtogroup struct
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file localvar.h
///
/// This file contains the definition of a template of a class
/// whose object can define a local value of a given variable,
/// and restores the original value upon destruction.
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

// Started on June 12, 2008. Last revision: June 13, 2008.


#ifndef _CHOMP_STRUCT_LOCALVAR_H_
#define _CHOMP_STRUCT_LOCALVAR_H_


namespace chomp {
namespace homology {


// --------------------------------------------------
// -------------------- LOCALVAR --------------------
// --------------------------------------------------

/// Local variable guardian.
/// This template contains the definition of a class whose object
/// remembers the previous value of a variable provided upon initialization,
/// sets the new value, and restores the previous value upon destruction.
/// This corresponds to certain extent to a local variable in Perl.
template <class varType>
class local_var
{
public:
	/// The constructor of a local variable guardian.
	local_var (varType &_variable);

	/// The constructor of a local variable guardian
	/// which assigns a new value upon initialization.
	local_var (varType &_variable, const varType &_newValue);

	/// The destructor of a local variable guardian.
	~local_var ();

private:
	/// Reference to the global variable.
	varType &var;

	/// The original value of the variable which is going to be
	/// restored upon destruction of the guardian object.
	const varType value;

}; /* class local_var */

// --------------------------------------------------

template <class varType>
inline local_var<varType>::local_var (varType &_variable):
	var (_variable), value (_variable)
{
	return;
} /* local_var */

template <class varType>
inline local_var<varType>::local_var (varType &_variable,
	const varType &_newValue): var (_variable), value (_variable)
{
	_variable = _newValue;
	return;
} /* local_var */

template <class varType>
inline local_var<varType>::~local_var ()
{
	var = value;
	return;
} /* ~local_var */


} // namespace homology
} // namespace chomp

#endif // _CHOMP_STRUCT_LOCALVAR_H_

/// @}

