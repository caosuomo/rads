/// @addtogroup system
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file arg.cpp
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

// Started on March 3, 2002. Last revision: October 6, 2004.


#include "chomp/system/arg.h"

namespace chomp {
namespace homology {


// --------------------------------------------------
// ------------------- argelement -------------------
// --------------------------------------------------

char *argelement::getvalue (char *str)
{
	// if there is no argument name and the string does not begin
	// with a dash, then this is a full-string argument
	if (!name || !*name)
	{
		if (*str != '-')
			return str;
		else
			return NULL;
	}

	// if this is an argument with a name, but the string does not begin
	// with a dash, then this is not this
	if (*str != '-')
		return NULL;

	// compare the name of the argument with the beginning of the string
	const char *current = name;
	++ str;
	while (*current && (*current == *str))
	{
		++ current;
		++ str;
	}
	if (!*current)
		return str;
	else
		return NULL;
} /* argelement::getvalue */


// --------------------------------------------------
// ------------------- arguments --------------------
// --------------------------------------------------

arguments::~arguments ()
{
	if (!n)
		return;
	for (int i = 0; i < n; i ++)
		delete tab [i];
	delete [] tab;
	return;
} /* arguments::~arguments */

int arguments::remove (char *name)
{
	// remember the previous number of arguments
	int prev_n = n;

	// if the given name is empty, convert it into NULL
	if (name && !*name)
		name = NULL;

	// go through all the arguments and remove those with the given name
	for (int i = 0; i < n; i ++)
	{
		// retrieve the name of the argument
		const char *argname = tab [i] -> getname ();

		// if the name is empty, change it into NULL
		if (argname && !*argname)
			argname = NULL;

		// if only one of the names is empty, they are different
		if ((name && !argname) || (!name && argname))
			continue;

		// if the names are nonempty, compare them
		if (name && strcmp (name, argname))
			continue;

		// delete this unit and shift the other pointers
		delete tab [i];
		n --;
		for (int j = i; j < n; j ++)
			tab [j] = tab [j + 1];
		i --;
	}

	// return the number of removed arguments
	return (prev_n - n);
} /* arguments::remove */

int arguments::analyze (int argc, char *argv [], std::ostream &out)
{
	int returncode = 0;
	for (int i = 1; i < argc; i ++)
	{
		int found = 0;

		// find an argument and fill it
		for (int j = 0; j < n; j ++)
		{
			// if this argument is already filled, omit it
			if (tab [j] -> get (argflags::filled))
				continue;

			// get the string for the argument's value
			char *value = tab [j] -> getvalue (argv [i]);

			// if this is not this argument, take the next one
			if (!value)
				continue;

			// read the argument's value
			// unless the value should be ignored
			int used = 0;
			if (tab [j] -> get (argflags::ignorevalue))
				if (tab [j] -> get (argflags::hasdefault))
					tab [j] -> setvalue (NULL, NULL);
				else;
			else
				used = tab [j] -> setvalue (value,
					(i < argc - 1) ? argv [i + 1] :
					NULL);

			// if the interpreting should be broken, do it
			if (tab [j] -> get (argflags::breakinterpreting))
				return 1;

			// if there was an error reading the value, say it
			if (tab [j] -> get (argflags::readerror))
			{
				out << "Unable to read the value for "
					"argument no. " << i << ": '" <<
					argv [i] << "'." << std::endl;
				tab [j] -> unset (argflags::readerror);
				returncode --;
			}

			// if a value for the argument is missing, say it
			if (tab [j] -> get (argflags::missingvalue))
			{
				out << "The value for argument no. " <<
					i << ": '" << argv [i] <<
					"' is missing." << std::endl;
				tab [j] -> unset (argflags::missingvalue);
				returncode --;
			}

			// if the next argument was used, shift arguments
			if (used)
				i ++;

			// remember that this argument was found in the list
			found = 1;
			break;
		}
		if (found)
			continue;

		// if the argument was not found in the list of known
		// arguments, display an error message
		out << "Unrecognized argument no. " << i << ": '" <<
			argv [i] << "'." << std::endl;
		returncode --;
	}

	// verify remaining properties of the arguments
	int namewarning = 0;
	for (int j = 0; j < n; j ++)
	{
		// chech for too many entries of a table
		if (tab [j] -> get (argflags::toomany))
		{
			if (tab [j] -> getname () && *tab [j] -> getname ())
				out << "Parameter '-" <<
				tab [j] -> getname () <<
				"' appears too many times." << std::endl;
			else
				out << "Too many names "
					"in the command line." << std::endl;
			returncode --;
			continue;
		}

		// check whether obligatory arguments are filled in
		if (!tab [j] -> get (argflags::obligatory) ||
			tab [j] -> get (argflags::filled))
			continue;

		if (tab [j] -> getname () && *tab [j] -> getname ())
			out << "Parameter '-" << tab [j] -> getname () <<
				"' is missing." << std::endl;
		else if (!namewarning)
		{
			out << "Too few names in the command line." <<
				std::endl;
			namewarning = 1;
		}
		returncode --;
	}

	return returncode;
} /* arguments::analyze */

void arguments::reset ()
{
	for (int i = 0; i < n; i ++)
		tab [i] -> restore ();
	return;
} /* arguments::reset */

void arguments::inctable ()
{
	if (n < length)
		return;
	length = 2 * length + 13;
	argelement **newtab = new argelement * [length];
	for (int i = 0; i < n; i ++)
		newtab [i] = tab [i];
	delete [] tab;
	tab = newtab;
	return;
} /* arguments::inctable */

std::ostream &operator << (std::ostream &out, arguments &p)
{
	for (int i = 0; i < p. n; i ++)
		out << std::setw (2) << (i + 1) << ". " << *(p. tab [i]);
	return out;
} /* operator <<  */


} // namespace homology
} // namespace chomp

/// @}

