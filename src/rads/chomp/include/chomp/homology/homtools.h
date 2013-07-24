/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file homtools.h
///
/// This file contains various small procedures that might be useful
/// in programs which compute homology. The procedures include reading
/// and writing cubical sets and geometric complexes, as well as some
/// processing and verification of the cubical data structures.
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

// Started on April 16, 2003. Last revision: June 30, 2007.


#ifndef _CHOMP_HOMOLOGY_HOMTOOLS_H_
#define _CHOMP_HOMOLOGY_HOMTOOLS_H_

#include "chomp/homology/cubisets.h"
#include "chomp/cubes/cubmaps.h"
#include "chomp/simplices/simplex.h"
#include "chomp/bitmaps/bitmaps.h"


//#define CHATTER  // only write out chatter/messages if we want them

namespace chomp {
namespace homology {


// --------------------------------------------------
// ---------------------- READ ----------------------
// --------------------------------------------------

/// Reads all the cubes from the given file and ignores them.
/// In this way the numbers of cubes are established.
template <class tCube>
inline void scancubes (const char *name)
{
	if (!name || !*name)
		return;
	std::ifstream in (name);
	if (!in)
		fileerror (name);

	// read all the cubes contained in the file,
	// but ignore lines not starting with a parenthesis
	
        #ifdef CHATTER
	sout << "Scanning the file '" << name << "' for cubes... ";
        #endif

	int_t count = 0;
	int dim = -19;
	bool warned = false;
	ignorecomments (in);
	while (in)
	{
		typename tCube::CoordType c [tCube::MaxDim];
		if (closingparenthesis (in. peek ()) == EOF)
			ignoreline (in);
		else
		{
			// read the point from the file
			int d = readcoordinates (in, c, tCube::MaxDim);

			// verify the dimension of the point
			if (dim < 0)
				dim = d;
			else if ((dim != d) && !warned)
			{
				sout << "\nWARNING: Not all the cubes have "
					"the same dimension.\n";
				warned = true;
			}

			// add the point to the point base
			// and verify its number
			int_t n = tCube::PointBase::number (c, d);
			if ((n != count) && !warned)
			{
				cube q (c, d);
				sout << "\nWARNING: Some cubes are "
					"repeated - " << q <<
					" for example.\n";
				warned = true;
			}

			// count the point
			++ count;
		}
		ignorecomments (in);
	}
	sout << count << " cubes analyzed.\n";
	return;
} /* scancubes */

/// Reads a given set from the file and shows appropriate messages.
/// Assumes the elements of the set begin with an opening parenthesis-char
/// (or a digit, if "digitOk" == true) and ignores all the preceding data.
/// Uses the given plural name of the elements for the messages.
/// Note: This procedure is suitable for gcomplex<type> or hashedset<type>.
template <class settype>
void readtheset (const char *name, settype &s, const char *pluralname,
	const char *what/*, bool digitOk = false*/)
{
	// if no file name is given, do nothing
	if (!name)
		return;

	#ifdef CHATTER
	// show what you are doing
	sout << "Reading " << pluralname;
	if (what)
		sout << " to " << what;
	sout << " from '" << name << "'... ";
	#endif

	// open the file
	std::ifstream in (name);
	if (!in)
		fileerror (name);

	// ignore all the introductory data
	ignorecomments (in);
	while (!!in && (closingparenthesis (in. peek ()) == EOF) &&
		((in. peek () < '0') || (in. peek () > '9')) &&
		(in. peek () != '-'))
	{
		ignoreline (in);
		ignorecomments (in);
	}

	// read the set and show how many elements have been read
      
	int_t prev = s. size ();
	in >> s;
        
        #ifdef CHATTER
	sout << (s. size () - prev) << " " << pluralname << " read.\n";
	#endif

	return;
} /* readtheset */

/// Uses the general procedure "readtheset" to read a geometric complex.
template <class cell, class euclidom>
inline void readcells (const char *name, gcomplex<cell,euclidom> &s,
	const char *what)
{
	readtheset (name, s, cell::pluralname (), what);
	return;
} /* readcells */

/// Uses the general procedure "readtheset" to read a set of elements.
template <class element>
inline void readelements (const char *name, hashedset<element> &s,
	const char *what)
{
	readtheset (name, s, element::pluralname (), what);
	return;
} /* readelements */

/// Reads a set of cubes from the given file.
/// This function is necessary because cubes need special treatment.
inline void readelements (const char *name, cubes &cub, const char *what)
{
	readtheset (name, cub, cube::pluralname (), what);
	return;
} /* readelements */

/// Reads the domain of a cubical multivalued map from the given file.
template <class element>
inline void readmapdomain (const char *name, hashedset<element> &cub)
{
	if (!name)
		return;

	#ifdef CHATTER
	sout << "Reading the domain of the map from '" << name << "'... ";
	#endif

	std::ifstream in (name);
	if (!in)
		fileerror (name);
	int_t prev = cub. size ();
	readdomain (in, cub);

	#ifdef CHATTER
	sout << (cub. size () - prev) << " " << element::pluralname () <<
		" read.\n";
	#endif

	return;
} /* readmapdomain */

/// Reads the domain of a cubical multivalued map from the given file.
template <class element>
inline void readmapimage (const char *name, hashedset<element> &cub)
{
	if (!name)
		return;
	sout << "Reading the image of the map from '" << name << "'... ";
	std::ifstream in (name);
	if (!in)
		fileerror (name);
	int_t prev = cub. size ();
	readimage (in, cub);
	sout << (cub. size () - prev) << " " << element::pluralname () <<
		" read.\n";
	return;
} /* readmapimage */

/// Reads the image of a set by a cubical multivalued map
/// from the given file.
template<class element>
inline void readmapimage (const char *filename,
	const hashedset<element> &dom, const char *domname,
	hashedset<element> &cub)
{
	if (!filename)
		return;
	sout << "Reading the image of " << domname << " by the map '" <<
		filename << "'... ";
	std::ifstream in (filename);
	if (!in)
		fileerror (filename);
	int_t prev = cub. size ();
	readimage (in, dom, cub);
	sout << (cub. size () - prev) << " " << element::pluralname () <<
		" read.\n";
	return;
} /* readmapimage */

/// Reads the restriction of a multivalued map to the union of two sets.
template <class element>
inline void readmaprestriction (mvmap<element,element> &Fcubmap,
	const char *mapname,
	const hashedset<element> &Xcubes, const hashedset<element> &Acubes,
	const char *Xname, const char *purpose = 0)
{
	if (!mapname || (Xcubes. empty () && Acubes. empty ()))
		return;
	
	#ifdef CHATTER
	sout << "Reading the map on " << Xname << " from '" << mapname;
	if (purpose)
		sout << "' " << purpose << "... ";
	else
		sout << "'... ";
	#endif

	std::ifstream in (mapname);
	if (!in)
		fileerror (mapname);
	readselective (in, Xcubes, Acubes, Fcubmap);
	
	#ifdef CHATTER
	if (Fcubmap. getdomain (). size () !=
		Xcubes. size () + Acubes. size ())
	{
		sout << "\nWARNING: The map is not defined "
			"on some cubes in " << Xname << ".\n";
	}
	else
		sout << "Done.\n";
	#endif

	return;
} /* readmaprestriction */

/// Reads the restriction of a multivalued map to the given set.
template <class element>
inline void readmaprestriction (mvmap<element,element> &Fcubmap,
	const char *mapname,
	const hashedset<element> &Xcubes, const char *Xname,
	const char *purpose = NULL)
{
	hashedset<element> empty;
	readmaprestriction (Fcubmap, mapname, Xcubes, empty, Xname, purpose);
	return;
} /* readmaprestriction */


// --------------------------------------------------
// ---------------------- SAVE ----------------------
// --------------------------------------------------

/// Saves a given set to a file and shows appropriate messages.
/// Note: This procedure is suitable for gcomplex<type> or hashedset<type>.
template <class settype>
void savetheset (const char *name, const settype &s, const char *pluralname,
	const char *what, const char *filecomment = 0)
{
	// if no file name is given, do nothing
	if (!name)
		return;

	// show what you are doing
	sout << "Saving " << pluralname;
	if (what)
		sout << " in " << what;
	sout << " to '" << name << "'... ";

	// open the file
	std::ofstream out (name);
	if (!out)
		fileerror (name, "create");
		
	// save the data
	if (filecomment)
		out << filecomment;
	out << s;
	if (!out)
		fileerror (name, "save");

	// show how many elements have been written
	sout << s. size () << " " << pluralname << " written.\n";

	return;
} /* writetheset */

/// Uses the general procedure "savetheset" to save a geometric complex.
template <class cell, class euclidom>
inline void savecells (const char *name, const gcomplex<cell,euclidom> &s,
	const char *what, const char *filecomment = 0)
{
	savetheset (name, s, cell::pluralname (), what, filecomment);
	return;
} /* savecells */

/// Uses the general procedure "savetheset" to save a set of elements.
template <class element>
inline void saveelements (const char *name, const hashedset<element> &s,
	const char *what, const char *filecomment = 0)
{
	savetheset (name, s, element::pluralname (), what, filecomment);
	return;
} /* saveelements */

/// Saves a set of cubes to the given file.
/// This function is necessary because cubes need special treatment.
inline void saveelements (const char *name, const cubes &cub,
	const char *what, const char *filecomment = 0)
{
	savetheset (name, cub, cube::pluralname (), what, filecomment);
	return;
} /* saveelements */


// --------------------------------------------------
// --------------------- VERIFY ---------------------
// --------------------------------------------------

/// Checks if X is a subset of the union of Y and B. Displays messages
/// and a warning if necessary. Returns the result: true = passed.
template <class cubsettype>
bool checkinclusion (const cubsettype &Xcubes, const cubsettype &Ycubes,
	const cubsettype &Bcubes, const char *Xname, const char *YBname)
{
        #ifdef CHATTER
	sout << "Verifying if " << Xname << " is contained in " <<
		YBname << "... ";
	#endif

	bool failed = false;
	for (int_t i = 0; !failed && (i < Xcubes. size ()); ++ i)
	{
		if (!Ycubes. check (Xcubes [i]) &&
			!Bcubes. check (Xcubes [i]))
		{
			failed = true;
		}
	}

	#ifdef CHATTER
	if (failed)
		sout << "Failed.\nWARNING: The set " << Xname <<
			" is NOT contained in " << YBname << ".\n";
	else
	  sout << "Passed.\n";
	#endif

	return !failed;
} /* checkinclusion */

/// Checks for the inclusion of X in Y. Displays messages
/// and a warning if necessary. Returns the result: true = passed.
template <class cubsettype>
inline bool checkinclusion (const cubsettype &Xcubes,
	const cubsettype &Ycubes, const char *Xname, const char *Yname)
{
	cubsettype empty;
	return checkinclusion (Xcubes, Ycubes, empty, Xname, Yname);
} /* checkinclusion */

/// Checks if the image of X by F is contained in the union of Y and B.
/// Displays messages and a warning if necessary. Returns true if yes.
template <class maptype, class cubsettype>
bool checkimagecontained (const maptype &Fcubmap, const cubsettype &Xcubes,
	const cubsettype &Ycubes, const cubsettype &Bcubes,
	const char *Xname, const char *Yname)
{
        #ifdef CHATTER
	sout << "Verifying if the image of " << Xname <<
		" is contained in " << Yname << "... ";
	#endif

	bool failed = false;
	for (int_t i = 0; !failed && (i < Xcubes. size ()); ++ i)
	{
		if (!Fcubmap. getdomain (). check (Xcubes [i]))
			continue;
		const cubsettype &cset = Fcubmap (Xcubes [i]);
		for (int_t j = 0; !failed && (j < cset. size ()); ++ j)
		{
			if (!Ycubes. check (cset [j]) &&
				!Bcubes. check (cset [j]))
			{
				failed = true;
			}
		}
	}

	#ifdef CHATTER
	if (failed)
		sout << "Failed.\nWARNING: The image of " << Xname <<
			" is NOT contained in " << Yname << ".\n";
	else
		sout << "Passed.\n";
	#endif

	return !failed;
} /* checkimagecontained */

/// Checks if the image of X by F is contained in the set Y alone.
/// Displays messages and a warning if necessary. Returns true if yes.
template <class maptype, class cubsettype>
inline bool checkimagecontained (const maptype &Fcubmap,
	const cubsettype &Xcubes, const cubsettype &Ycubes,
	const char *Xname, const char *Yname)
{
	cubsettype empty;
	return checkimagecontained (Fcubmap, Xcubes, Ycubes, empty,
		Xname, Yname);
} /* checkimagecontained */

/// Checks if the image of A by F is disjoint from Y (actually, from Y\\B).
/// Displays messages and a warning if necessary. Returns true if yes.
template <class maptype, class cubsettype>
bool checkimagedisjoint (const maptype &Fcubmap, const cubsettype &Acubes,
	const cubsettype &Ycubes, const char *Aname, const char *Yname)
{
        #ifdef CHATTER
	sout << "Verifying if the image of " << Aname <<
		" is disjoint from " << Yname << "... ";
	#endif

	bool failed = false;
	for (int_t i = 0; !failed && (i < Acubes. size ()); ++ i)
	{
		if (!Fcubmap. getdomain (). check (Acubes [i]))
			continue;
		const cubsettype &cset = Fcubmap (Acubes [i]);
		for (int_t j = 0; !failed && (j < cset. size ()); ++ j)
			if (Ycubes. check (cset [j]))
				failed = true;
	}
	if (failed)
		sout << "Failed.\nSERIOUS WARNING: The image of " << Aname <<
			" is NOT disjoint from " << Yname << ".\n";
	#ifdef CHATTER
	else
		sout << "Passed.\n";
	#endif
	        
	return !failed;
} /* checkimagedisjoint */

/// Checks if the image of each element of the domain of this map is acyclic.
/// Shows messages and a warning if necessary. Returns true = yes.
template <class tCell, class tCube, class tCoef>
bool checkacyclicmap (const mvcellmap<tCell,tCoef,tCube> &Fcellcubmap,
	const char *Xname)
{
    #ifdef CHATTER
	sout << "Verifying if the map on " << Xname << " is acyclic... ";  
    #endif

	// retrieve the domain cell complex and analyze each dimension
	const gcomplex<tCell,tCoef> &dom = Fcellcubmap. getdomain ();
	int_t counter = 0;
	bool failed = false;
	for (int_t d = 0; !failed && (d <= dom. dim ()); ++ d)
	{
		// retrieve the set of cells in this dimension and analyze it
		const hashedset<tCell> &qset = dom [d];
		for (int_t i = 0; !failed && (i < qset. size ()); ++ i)
		{
		        #ifdef CHATTER
			// show progress indicator
			if (counter && !(counter % 373))
				scon << std::setw (10) << counter <<
					"\b\b\b\b\b\b\b\b\b\b";
			#endif

			++ counter;

			// reduce the image of this cell
			const hashedset<tCube> &img = Fcellcubmap (qset [i]);
			if (img. size () == 1)
				continue;

			// if could not be reduced, compute the homology
			if (!acyclic (img))
				failed = true;
		}
	}

	#ifdef CHATTER
	if (failed)
		sout << "Failed.\n*** WARNING: The map on " << Xname <<
			" is NOT acyclic. ***\n"
			"*** The result of the computations "
			"may be totally wrong. ***\n";
	else
		sout << "Passed.         \n";
	#endif

	return !failed;
} /* checkacyclicmap */


// --------------------------------------------------
// --------------------- REDUCE ---------------------
// --------------------------------------------------

/// Restricts the set of cubes 'Acubes' to these cubes which are neighbors
/// of any of the cubes in 'Xcubes' and displays appropriate messages.
template <class cubsettype>
void restrictAtoneighbors (const cubsettype &Xcubes, cubsettype &Acubes,
	const char *Xname, const char *Aname,
	const cubsettype *keepcubes = 0)
{
	// if the set 'A' is empty, there is no point in doing anything
	if (Acubes. empty ())
		return;
	
	#ifdef CHATTER
	// display the message what is being done now
	sout << "Restricting " << Aname << " to the neighbors of " <<
		Xname << "\\" << Aname << "... ";
	#endif

	// remember the previous number of cubes in 'A'
	int_t prev = Acubes. size ();

	// if the set 'X' is empty, the result is obvious
	if (Xcubes. empty ())
	{
		cubsettype empty;
		Acubes = empty;
	}

	// remove from 'A' these cubes which are not neighbors of 'X'
	#ifdef CHATTER
	sseq << "D 0\n";
	#endif 

	for (int_t i = 0; i < Acubes. size (); ++ i)
	{
		if (keepcubes && keepcubes -> check (Acubes [i]))
			continue;
		if (getneighbors (Acubes [i], 0, Xcubes, 1))
			continue;
		sseq << '0' << Acubes [i] << '\n';
		Acubes. removenum (i);
		-- i;
	}
	sseq << "D 100\n";

	#ifdef CHATTER
	// display the result
	sout << (prev - Acubes. size ()) << " cubes removed, " <<
		Acubes. size () << " left.\n";
	#endif

	return;
} /* restrictAtoneighbors */

/// Removes 'Acubes' from 'Xcubes' and shows messages.
template <class cubsettype>
void removeAfromX (cubsettype &Xcubes, const cubsettype &Acubes,
	const char *Xname, const char *Aname)
{
	if (Xcubes. empty () || Acubes. empty ())
		return;

	#ifdef CHATTER
	sout << "Computing " << Xname << "\\" << Aname << "... ";
	#endif

	int_t prev = Xcubes. size ();
	Xcubes. remove (Acubes);

        #ifdef CHATTER
	sout << (prev - Xcubes. size ()) << " cubes removed from " <<
		Xname << ", " << Xcubes. size () << " left.\n";
	#endif

	return;
} /* removeAfromX */

/// Removes from 'X' all the cells that appear in 'A'.
template <class cell, class euclidom>
void removeAfromX (gcomplex<cell,euclidom> &X,
	const gcomplex<cell,euclidom> &A,
	const char *Xname, const char *Aname)
{
	if (X. empty () || A. empty ())
		return;

	#ifdef CHATTER
	sout << "Computing " << Xname << "\\" << Aname << "... ";
	#endif

	int_t prev = X. size ();
	X. remove (A);

	#ifdef CHATTER
	sout << (prev - X. size ()) << ' ' << cell::pluralname () <<
		" removed from " << Xname << ", " << X. size () <<
		" left.\n";
	#endif

	return;
} /* removeAfromX */

/// Expands the other element of the pair into the main portion of the set.
template <class cubsettype>
void expandAinX (cubsettype &Xcubes, cubsettype &Acubes,
	const char *Xname, const char *Aname)
{
	if (Xcubes. empty () || Acubes. empty ())
		return;

	#ifdef CHATTER
	sout << "Expanding " << Aname << " in " << Xname << "... ";
	#endif

	int_t count = cubexpand (Xcubes, Acubes);

        #ifdef CHATTER
	sout << count << " cubes moved to " << Aname << ", " <<
		Xcubes. size () << " left in " << Xname << "\\" << Aname <<
		".\n";
	#endif

	return;
} /* expandAinX */

/// Expands the other element of the pair into the main portion of the set.
template <class cubsettype, class maptype>
void expandAinX (cubsettype &Xcubes, cubsettype &Acubes, cubsettype &Ycubes,
	cubsettype &Bcubes, const maptype &Fcubmap,
	const char *Xname, const char *Aname, const char *Bname,
	bool indexmap, bool checkacyclic)
{
	if (Xcubes. empty () || Acubes. empty ())
		return;
	
	#ifdef CHATTER
	sout << "Expanding " << Aname << " in " << Xname << "... ";
	#endif
	
	int_t prevB = Bcubes. size ();
	int_t prevY = Ycubes. size ();
	int_t count = cubexpand (Xcubes, Acubes, Ycubes, Bcubes,
		Fcubmap, indexmap, checkacyclic);

	#ifdef CHATTER
	sout << count << " moved to " << Aname << ", " << Xcubes. size () <<
		" left in " << Xname << "\\" << Aname << ", " <<
		(Bcubes. size () - prevB) << " added to " << Bname << ".\n";
	#endif
	// JJB -- 3/29/13 keep this warning!
	if (prevY - Ycubes. size () != Bcubes. size () - prevB)
		sout << "WARNING: The image of " << Xname << "\\" << Aname <<
			" was not contained in Y. "
			"The result can be wrong!\n";
	return;
} /* expandAinX */

/// Reduces the pair of sets of cubes. Keeps the given cubes untouched.
template <class cubsettype>
void reducepair (cubsettype &Xcubes, cubsettype &Acubes,
	const cubsettype &Xkeepcubes, const char *Xname, const char *Aname)
{
	if (Xcubes. empty ())
		return;

	#ifdef CHATTER
	sout << "Reducing full-dim cubes from ";
	if (!Acubes. empty ())
		sout << '(' << Xname << ',' << Aname << ")... ";
	else
		sout << Xname << "... ";
	#endif

	int_t count = cubreduce (Xcubes, Acubes, Xkeepcubes);
	
	#ifdef CHATTER
	sout << count << " removed, " <<
		(Xcubes. size () + Acubes. size ()) << " left.\n";
        #endif

	return;
} /* reducepair */

/// Reduces the pair of sets of cubes. Keeps the given cubes untouched.
/// Makes sure that the acyclicity of the given map is not spoiled.
template <class maptype, class cubsettype>
void reducepair (cubsettype &Xcubes, cubsettype &Acubes, maptype &Fcubmap,
	const cubsettype &Xkeepcubes, const char *Xname, const char *Aname)
{
	if (Xcubes. empty ())
		return;

	#ifdef CHATTER
	sout << "Reducing cubes from ";
	if (!Acubes. empty ())
		sout << '(' << Xname << ',' << Aname << ") [acyclic]... ";
	else
		sout << Xname << " [acyclic]... ";
	#endif

	int_t count = cubreduce (Xcubes, Acubes, Fcubmap, Xkeepcubes);

	#ifdef CHATTER
	sout << count << " removed, " <<
		(Xcubes. size () + Acubes. size ()) << " left.\n";
	#endif

	return;
} /* reducepair */

/// Adds the images of both maps to the set of cubes to be kept.
/// The image of each set is taken using the corresponding map.
/// Includes the sets 'Xcubes' and 'Acubes' if this is an index map.
template <class maptype, class cubsettype>
void addmapimg (const maptype &Fcubmap, const maptype &FcubmapA,
	const cubsettype &Xcubes, const cubsettype &Acubes,
	cubsettype &Ykeepcubes, bool indexmap)
{
	if (Fcubmap. getdomain (). empty () &&
		FcubmapA. getdomain (). empty ())
	{
		return;
	}

	#ifdef CHATTER
	sout << "Computing the image of the map... ";
	#endif

	int_t prev = Ykeepcubes. size ();
	const cubsettype &Fdomain = Fcubmap. getdomain ();
	if (!Fdomain. empty ())
	{
		if (Fdomain. size () == Xcubes. size ())
			retrieveimage (Fcubmap, Ykeepcubes);
		else
		{
			int_t n = Xcubes. size ();
			for (int_t i = 0; i < n; ++ i)
				Ykeepcubes. add (Fcubmap (Xcubes [i]));
		}
	}
	const cubsettype &FdomainA = FcubmapA. getdomain ();
	if (!FdomainA. empty ())
	{
		if (FdomainA. size () == Acubes. size ())
			retrieveimage (FcubmapA, Ykeepcubes);
		else
		{
			int_t n = Acubes. size ();
			for (int_t i = 0; i < n; ++ i)
				Ykeepcubes. add (FcubmapA (Acubes [i]));
		}
	}
	if (indexmap)
	{
	        #ifdef CHATTER
		sout << "and of the inclusion... ";
		#endif

		Ykeepcubes. add (Xcubes);
		Ykeepcubes. add (Acubes);
	}

	#ifdef CHATTER
	sout << (Ykeepcubes. size () - prev) << " cubes.\n";
	#endif

	return;
} /* addmapimg */

/// Adds the image of the given map to the set of cubes to be kept.
/// Includes the sets 'Xcubes' and 'Acubes' if this is an index map.
template <class maptype, class cubsettype>
inline void addmapimg (const maptype &Fcubmap,
	const cubsettype &Xcubes, const cubsettype &Acubes,
	cubsettype &Ykeepcubes, bool indexmap)
{
	addmapimg (Fcubmap, Fcubmap, Xcubes, Acubes, Ykeepcubes, indexmap);
	return;
} /* addmapimg */

/// Transforms cubes to full-dimensional cells.
template <class tCubes, class tCell, class tCoef>
void cubes2cells (tCubes &Xcubes, gcomplex<tCell,tCoef> &Xcompl,
	const char *Xname, bool deletecubes = true)
{
	// if there are no cubes to transform, do nothing
	if (Xcubes. empty ())
		return;

	// transform cubes into the cells of the same dimension
	int_t prev = Xcompl. size ();

	#ifdef CHATTER
	sout << "Transforming " << Xname << " into cells... ";
	#endif

	for (int_t i = 0; i < Xcubes. size (); ++ i)
		Xcompl. add (tCell (Xcubes [i]));

        #ifdef CHATTER
	sout << (Xcompl. size () - prev) << " cells added.\n";
	#endif

	// forget the set of cubes if requested to
	if (deletecubes)
	{
		tCubes empty;
		Xcubes = empty;
	}

	return;
} /* cubes2cells */

/// Collapses a pair of geometric complexes.
template <class cell, class euclidom>
void collapse (gcomplex<cell,euclidom> &Xcompl,
	gcomplex<cell,euclidom> &Acompl, gcomplex<cell,euclidom> &Xkeepcompl,
	const char *Xname, const char *Aname, bool addbd = true,
	bool addcob = false, bool disjoint = true, const int *level = NULL)
{
	// say what you are about to do
        #ifdef CHATTER
	sout << "Collapsing faces in " << Xname;
	if (!Acompl. empty ())
		sout << " and " << Aname;
	sout << "... ";
	#endif

	// collapse the faces as requested to
	int_t count = Xcompl. collapse (Acompl, Xkeepcompl,
		addbd, addcob, disjoint, level);


	#ifdef CHATTER
	// say something about the result
	sout << (count << 1) << " removed, " <<
		Xcompl. size () << " left.\n";
	#endif


	#ifdef CHATTER
	// if something remains in A, say it
	if (!Acompl. empty ())
		sout << "There are " << Acompl. size () << " faces "
			"of dimension up to " << Acompl. dim () <<
			" left in " << Aname << ".\n";
	#endif

	return;
} /* collapse */

/// Collapses a pair of geometric complexes.
/// This version does not take any complex to keep.
template <class cell, class euclidom>
inline void collapse (gcomplex<cell,euclidom> &Xcompl,
	gcomplex<cell,euclidom> &Acompl,
	const char *Xname, const char *Aname, bool addbd = true,
	bool addcob = false, bool disjoint = true, const int *level = NULL)
{
	gcomplex<cell,euclidom> empty;
	collapse (Xcompl, Acompl, empty, Xname, Aname,
		addbd, addcob, disjoint, level);
	return;
} /* collapse */

/// Collapses a single geometric complex.
template <class cell, class euclidom>
inline void collapse (gcomplex<cell,euclidom> &Xcompl,
	gcomplex<cell,euclidom> &Xkeepcompl,
	const char *Xname, bool addbd = true, bool addcob = false,
	bool disjoint = true, const int *level = NULL)
{
	gcomplex<cell,euclidom> empty;
	collapse (Xcompl, empty, Xkeepcompl, Xname, "",
		addbd, addcob, disjoint, level);
	return;
} /* collapse */

/// Collapses a single geometric complex.
/// This version does not take any complex to keep.
template <class cell, class euclidom>
inline void collapse (gcomplex<cell,euclidom> &Xcompl,
	const char *Xname, bool addbd = true, bool addcob = false,
	bool disjoint = true, const int *level = NULL)
{
	gcomplex<cell,euclidom> empty;
	collapse (Xcompl, empty, empty, Xname, "",
		addbd, addcob, disjoint, level);
	return;
} /* collapse */

/// Decreases the dimension of the geometric complex by adding boundary cells
/// to all the cells on higher dimensions and then removing these cells.
template <class cell, class euclidom>
void decreasedimension (gcomplex<cell,euclidom> &Acompl,
	int dim, const char *name)
{
	if (Acompl. dim () <= dim)
		return;
	if (dim < 0)
		dim = 0;
	
	#ifdef CHATTER
	sout << "Adding to " << name << " boundaries of high-dim " <<
		cell::pluralname () << "... ";
	#endif

	int_t howmany = 0;
	for (int i = Acompl. dim (); i > dim; -- i)
	{
		sout << '.';
		howmany += Acompl. addboundaries (i);
		Acompl. removeall (i);
	}
	
	#ifdef CHATTER
	sout << ' ' << howmany << " added.\n";
	#endif

	return;
} /* decreasedimension */

/// Adds boundaries to the geometric complex X or to both X and A.
template <class cell, class euclidom>
void addboundaries (gcomplex<cell,euclidom> &Xcompl,
	gcomplex<cell,euclidom> &Acompl,
	int minlevel, bool bothsets, const char *Xname, const char *Aname)
{
	// if there are no cells in the complex, there is nothing to do
	if (Xcompl. empty ())
		return;

	#ifdef CHATTER
	// say what you are doing
	sout << "Adding boundaries of " << cell::pluralname () << " in ";
	if (!Acompl. empty ())
		sout << Xname << " and " << Aname << "... ";
	else
		sout << Xname << "... ";
	#endif
	

	// add the boundaries and count the number of added cells
	int_t howmany = 0;
	for (int i = Xcompl. dim (); (i >= minlevel) && i; -- i)
	{
		if (bothsets)
		{
			howmany += Xcompl. addboundaries (i, true);
			if (Acompl. dim () >= i)
				howmany += Acompl. addboundaries (i,
					true);
		}
		else
			howmany += Xcompl. addboundaries (i, Acompl);
	}

	#ifdef CHATTER
	// show the total number of added cells
	sout << howmany << ' ' << cell::pluralname () << " added.\n";
	#endif

	return;
} /* addboundaries */

// Add boundaries to X or to X and A.
//void addboundaries (cubicalcomplex &Xcompl, cubicalcomplex &Acompl,
//	int minlevel, bool bothsets, const char *Xname, const char *Aname);

// Add boundaries to X or to X and A.
//void addboundaries (simplicialcomplex &Xcompl, simplicialcomplex &Acompl,
//	int minlevel, bool bothsets, const char *Xname, const char *Aname);


// --------------------------------------------------
// ---------------- READ BITMAP FILE ----------------
// --------------------------------------------------

/// Reads the squares from a bitmap file to the set of cubes.
/// Each pixel whose gray value is within the given range
/// (at least 'mingray', strictly less than 'maxgray')
/// gives raise to one square with the corresponding coordinates.
/// For color bitmaps, a weighted average of RGB is computed as gray.
/// Gray level 0 corresponds to black, gray level 255 is white.
/// Return 0 on success, throw an error message if failed.
template <class tCube>
int ReadBitmapFile (const char *bmpname, hashedset<tCube> &cset,
	int mingray = 0, int maxgray = 128)
{
	// prepare a bitmap file with the vertical axis like in mathematics
	bmpfile bmp;
	bmp. invertedpicture ();

	// open the bitmap file
	if (bmp. open (bmpname) < 0)
		fileerror (bmpname);
	
	// scan the picture and produce the list of hypercubes
	coordinate c [2];
	for (c [1] = 0; c [1] < bmp. picture_height (); ++ (c [1]))
	{
		for (c [0] = 0; c [0] < bmp. picture_width (); ++ (c [0]))
		{
			long color = bmp. getpixel (c [0], c [1]);
			long gray = (77 * ((color & 0xFF0000) >> 16) +
				154 * ((color & 0xFF00) >> 8) +
				25 * (color & 0xFF)) >> 8;
			if ((gray >= mingray) && (gray <= maxgray))
				cset. add (tCube (c, 2));
		}
	}

	return 0;
} /* ReadBitmapFile */


} // namespace homology
} // namespace chomp

#endif // _CHOMP_HOMOLOGY_HOMTOOLS_H

/// @}

