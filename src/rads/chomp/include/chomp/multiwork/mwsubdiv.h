/// @addtogroup multiwork
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file multiwork/mwsubdiv.h
///
/// This file contains the definition of the MultiWork subdivision framework.
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

// Started in November 2005. Last revision: February 19, 2009.


#ifndef _CHOMP_MULTIWORK_MWSUBDIV_H_
#define _CHOMP_MULTIWORK_MWSUBDIV_H_

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <exception>

#include "chomp/system/config.h"
#include "chomp/system/textfile.h"
#include "chomp/system/timeused.h"
#include "chomp/system/arg.h"
#include "chomp/cubes/pointset.h"
#include "chomp/cubes/cube.h"
#include "chomp/multiwork/mw.h"


// --------------------------------------------------
// --------------- the output syntax ----------------
// --------------------------------------------------

// The syntax of lines in the text data file:
// + n intervals. - data no. n sent
// * n result level cube. - cube no. n received
// # level cube. - cube subdivided

// The syntax of lines in the log file of a coordinator:
// + n. - data no. n prepared
// * n result. - cube no. n received by the coordinator
// ! Message. - data rejected by one of the workers (error)

// The syntax of lines in the log file of a worker:
// - n. - data no. n received by the worker
// = n result. - data no. n sent back from the worker
// ! Message. - data rejected by the worker (error)

namespace chomp {
namespace multiwork {

using chomp::homology::sout;
using chomp::homology::sbug;
using chomp::homology::slog;
using chomp::homology::ignorecomments;
using chomp::homology::ignoreline;
using chomp::homology::hashedset;
using chomp::homology::multitable;
using chomp::homology::tRectangle;
using chomp::homology::tCubeFix;
using chomp::homology::arguments;
using chomp::homology::arg;
using chomp::homology::argswitch;
using chomp::homology::arghelp;
using chomp::homology::argstreams;
using chomp::homology::setstreams;
using chomp::homology::commandline;
using chomp::homology::currenttime;
using chomp::homology::program_time;


// --------------------------------------------------
// ---------------- the worker class -----------------
// --------------------------------------------------

/// The type of a function which computes a value of interest
/// for the given product of intervals. The subdivision level
/// is provided in case the computations on higher levels need
/// to be done in a more thorough way.
typedef int (*fcompute) (const double *left, const double *right,
	int dim, int level);

/// This class defines a worker for the multi-work subdivision framework.
template <int dim, class coord>
class mwSubWorker: public mwWorker
{
public:
	/// The constructor.
	mwSubWorker (fcompute _compute);

private:
	/// A function for processing the data by a worker.
	int Process (mwData &data);

	/// The address of a function to use for the computations.
	fcompute compute;

}; /* class mwSubWorker */

// --------------------------------------------------

template <int dim, class coord>
inline mwSubWorker<dim,coord>::mwSubWorker (fcompute _compute):
	compute (_compute)
{
	return;
} /* mwSubWorker<dim,coord>::mwSubWorker */

// --------------------------------------------------

template <int dim, class coord>
int mwSubWorker<dim,coord>::Process (mwData &data)
{
	// decode the number of the data pack to process
	int current = 0;
	data >> current;

	// say which data pack is being processed
	sout << "- " << current << ".\n";

	// decode the subdivision level
	int level = 0;
	data >> level;

	// decode the information on whether this is a full box or a probe
	bool full = false;
	data >> full;

	// the intervals
	double left [dim];
	double right [dim];
	for (int i = 0; i < dim; ++ i)
	{
		data >> left [i];
		if (full)
			data >> right [i];
	}

	// the marker of the end of data
	int dataEndMarker = 0;
	data >> dataEndMarker;
	if (dataEndMarker != 13)
	{
		data. Reset ();
		sout << "! Incompatible data received. Rejecting it.\n";
		return mwReject;
	}

	// rune one set of computations
	int result = compute (left, full ? right : left, dim, level);

	// if the result is wrong then reject this piece of data
	if (result < 0)
	{
		sout << "! Data no. " << current <<
			" produced a negative result. Rejecting it.\n";
		return mwReject;
	}

	// say what has been computed
	sout << "= " << current << " " << result << ".\n";

	// send back the data containing the result of the processing
	data. Reset ();
	data << current;
	data << result;
	return mwOk;
} /* mwSubWorker<dim,coord>::Process */


// --------------------------------------------------
// --------------- subdivision points ---------------
// --------------------------------------------------

/// Returns a subdivision point corresponding to the given fraction
/// of the provided interval.
/// Should be called with part = 0, ..., full, to get a subdivision
/// of the entire interval, including the endpoints.
template <class coord>
inline double mwSubdivPoint (const double &left, const double &right,
	coord part, coord full)
{
	// uncomment if using CAPD to switch the rounding mode to the nearest
//	round_nearest ();

	// if this is a boundary subdivision point then return the bound
	if (part == 0)
		return left;
	else if (part == full)
		return right;

	// divide 'part' and 'full' by their largest common divisor
	// which is a power of two
	while (!(part & 1) && !(full & 1))
	{
		part >>= 1;
		full >>= 1;
	}

	// compute the corresponding subdivision point of the interval
	double fraction = static_cast<double> (part) / full;
	return (left + (right - left) * fraction);
} /* mwSubdivPoint */


// --------------------------------------------------
// ----------------- initial probes -----------------
// --------------------------------------------------

/// This is a helper class for producing an initial set of probes
/// to test the interior of the requested area of parameters.
template <int dim, class coord>
class mwIniProbes
{
public:
	/// The only constructor allowed.
	mwIniProbes (int level = 2);

	/// The destructor.
	~mwIniProbes ();

	/// Returns the next available probe or throws an error message
	/// if all the probes have already been taken.
	tCubeFix<dim,coord> get ();

	/// Returns true iff there is at least one more probe available.
	bool available () const;

private:
	/// The copy constructor is not allowed.
	mwIniProbes (const mwIniProbes<dim,coord> &);

	/// The assignment operator is not allowed.
	mwIniProbes<dim,coord> &operator = (const mwIniProbes<dim,coord> &);

	/// The coordinates of the leftmost corner of the iterated area.
	coord left [dim];

	/// The coordinates of the rightmost corner of the iterated area.
	coord right [dim];

	/// The actual probe iterator.
	tRectangle<coord> rect;

	/// The next coordinates to return or 0 if none.
	const coord *next;

}; /* class mwIniProbes */

// --------------------------------------------------

template <int dim, class coord>
inline mwIniProbes<dim,coord>::mwIniProbes (int level)
{
	if (level < 2)
		throw "Too low initial level. Should be at least 2.";
	for (int i = 0; i < dim; ++ i)
	{
		this -> left [i] = 1;
		this -> right [i] = 1 << level;
	}
	this -> rect = tRectangle<coord> (this -> left, this -> right, dim);
	this -> next = this -> rect. get ();
	return;
} /* mwIniProbes::mwIniProbes */

template <int dim, class coord>
inline mwIniProbes<dim,coord>::mwIniProbes (const mwIniProbes<dim,coord> &)
{
	return;
} /* mwIniProbes::mwIniProbes */

template <int dim, class coord>
inline mwIniProbes<dim,coord> &mwIniProbes<dim,coord>::operator =
	(const mwIniProbes<dim,coord> &)
{
	return *this;
} /* mwIniProbes::operator = */

template <int dim, class coord>
inline mwIniProbes<dim,coord>::~mwIniProbes ()
{
	return;
} /* mwIniProbes::~mwIniProbes */

template <int dim, class coord>
inline tCubeFix<dim,coord> mwIniProbes<dim,coord>::get ()
{
	tCubeFix<dim,coord> q (this -> next, dim);
	this -> next = this -> rect. get ();
	return q;
} /* mwIniProbes::get */

template <int dim, class coord>
inline bool mwIniProbes<dim,coord>::available () const
{
	return !!(this -> next);
} /* mwIniProbes::available */


// --------------------------------------------------
// ------------- boxes around a vertex --------------
// --------------------------------------------------

/// This is a helper class for iterating all the boxes
/// which share a given vertex.
template <int dim, class coord>
class mwBoxes
{
public:
	/// The only constructor allowed.
	mwBoxes (const tCubeFix<dim,coord> &q);

	/// The destructor.
	~mwBoxes ();

	/// Returns the next available box or throws an error message
	/// if all the boxes have already been taken.
	tCubeFix<dim,coord> get ();

	/// Returns true iff there is at least one more box available.
	bool available () const;

private:
	/// The copy constructor is not allowed.
	mwBoxes (const mwBoxes<dim,coord> &);

	/// The assignment operator is not allowed.
	mwBoxes<dim,coord> &operator = (const mwBoxes<dim,coord> &);

	/// The coordinates of the leftmost corner of the iterated area.
	coord left [dim];

	/// The coordinates of the rightmost corner of the iterated area.
	coord right [dim];

	/// The actual box iterator.
	tRectangle<coord> rect;

	/// The next coordinates to return or 0 if none.
	const coord *next;

}; /* class mwBoxes */

// --------------------------------------------------

template <int dim, class coord>
inline mwBoxes<dim,coord>::mwBoxes (const tCubeFix<dim,coord> &q)
{
	q. coord (this -> left);
	for (int i = 0; i < dim; ++ i)
	{
		this -> right [i] = left [i] + 1;
		-- (this -> left [i]);
	}
	this -> rect = tRectangle<coord> (this -> left, this -> right, dim);
	this -> next = this -> rect. get ();
	return;
} /* mwBoxes::mwBoxes */

template <int dim, class coord>
inline mwBoxes<dim,coord>::mwBoxes (const mwBoxes<dim,coord> &)
{
	return;
} /* mwBoxes::mwBoxes */

template <int dim, class coord>
inline mwBoxes<dim,coord> &mwBoxes<dim,coord>::operator =
	(const mwBoxes<dim,coord> &)
{
	return *this;
} /* mwBoxes::operator = */

template <int dim, class coord>
inline mwBoxes<dim,coord>::~mwBoxes ()
{
	return;
} /* mwBoxes::~mwBoxes */

template <int dim, class coord>
inline tCubeFix<dim,coord> mwBoxes<dim,coord>::get ()
{
	const coord *current = this -> next;
	this -> next = this -> rect. get ();
	return tCubeFix<dim,coord> (current, dim);
} /* mwBoxes::get */

template <int dim, class coord>
inline bool mwBoxes<dim,coord>::available () const
{
	return !!(this -> next);
} /* mwBoxes::available */


// --------------------------------------------------
// ---------------- corners of a box ----------------
// --------------------------------------------------

/// This is a helper class for iterating all the corners
/// of a given box.
template <int dim, class coord>
class mwCorners
{
public:
	/// The only constructor allowed.
	mwCorners (const tCubeFix<dim,coord> &q);

	/// The destructor.
	~mwCorners ();

	/// Returns the next available corner or throws an error message
	/// if all the corners have already been taken.
	tCubeFix<dim,coord> get ();

	/// Returns true iff there is at least one more box available.
	bool available () const;

private:
	/// The copy constructor is not allowed.
	mwCorners (const mwCorners<dim,coord> &);

	/// The assignment operator is not allowed.
	mwCorners<dim,coord> &operator = (const mwCorners<dim,coord> &);

	/// The coordinates of the leftmost corner of the iterated area.
	coord left [dim];

	/// The coordinates of the rightmost corner of the iterated area.
	coord right [dim];

	/// The actual corner iterator.
	tRectangle<coord> rect;

	/// The next coordinates to return or 0 if none.
	const coord *next;

}; /* class mwCorners */

// --------------------------------------------------

template <int dim, class coord>
inline mwCorners<dim,coord>::mwCorners (const tCubeFix<dim,coord> &q)
{
	q. coord (this -> left);
	for (int i = 0; i < dim; ++ i)
	{
		this -> right [i] = left [i] + 2;
	}
	this -> rect = tRectangle<coord> (this -> left, this -> right, dim);
	this -> next = this -> rect. get ();
	return;
} /* mwCorners::mwCorners */

template <int dim, class coord>
inline mwCorners<dim,coord>::mwCorners (const mwCorners<dim,coord> &)
{
	return;
} /* mwCorners::mwCorners */

template <int dim, class coord>
inline mwCorners<dim,coord> &mwCorners<dim,coord>::operator =
	(const mwCorners<dim,coord> &)
{
	return *this;
} /* mwCorners::operator = */

template <int dim, class coord>
inline mwCorners<dim,coord>::~mwCorners ()
{
	return;
} /* mwCorners::~mwCorners */

template <int dim, class coord>
inline tCubeFix<dim,coord> mwCorners<dim,coord>::get ()
{
	const coord *current = this -> next;
	this -> next = this -> rect. get ();
	return tCubeFix<dim,coord> (current, dim);
} /* mwCorners::get */

template <int dim, class coord>
inline bool mwCorners<dim,coord>::available () const
{
	return !!(this -> next);
} /* mwCorners::available */


// --------------------------------------------------
// ---------------- data pack class -----------------
// --------------------------------------------------

/// This is a helper class which defines a single data pack
/// used in the communication between coordinator and workers
/// in the multi-work subdivision framework.
template <int dim, class coord>
class mwSubDataPack
{
public:
	/// The default constructor.
	mwSubDataPack () {}
	
	/// A nice constructor.
	mwSubDataPack (int _num, const tCubeFix<dim,coord> &_q,
		int _level, bool _full):
		num (_num), q (_q), level (_level), full (_full) {}

	// the default copy constructor and operator = is good;
	// no destructor is necessary

	/// The number of the data pack.
	int num;

	/// The cube sent to a worker.
	tCubeFix<dim,coord> q;

	/// The subdivision level of the cube.
	int level;

	/// Is this a full cube (or just a probe)?
	bool full;

}; /* class mwSubDataPack */


// --------------------------------------------------
// ------------- the coordinator class --------------
// --------------------------------------------------

/// This class defines a coordinator for the multi-work subdivision
/// framework.
template <int dim, class coord>
class mwSubCoordinator: public mwCoordinator
{
	/// The maximal feasible subdivision level.
	static const int ppMaxSubdiv = 8 * sizeof (coord) - 2;

	/// The maximal feasible subdivision level.
//	static const int ppMaxSubdiv;

	/// The type of a cube of parameters.
	typedef tCubeFix<dim,coord> ppCube;

	/// The maximal dimension of cubes.
	static const int ppMaxDim = ppCube::MaxDim;

	/// The type of a set of cubes in the parameter space.
	typedef hashedset<ppCube> ppCubes;

	/// The kind of rectangle used to iterate ranges of cubes.
	typedef tRectangle<coord> ppRect;

	/// The type of a sent data pack kept in an internal array.
	typedef mwSubDataPack<dim,coord> ppPack;

public:
	/// The constructor of an uninitialized coordinator.
	mwSubCoordinator ();

	/// The complete constructor of a coordinator object.
	mwSubCoordinator (const char *filename,
		int _inilevel, int _maxlevel, bool _flushfile,
		const double *_paramLeft, const double *_paramRight);

	/// The destructor.
	~mwSubCoordinator ();

private:
	// --- the MultiWork interface functions ---

	/// A function for preparing data by a coordinator.
	int Prepare (mwData &data);

	/// A function for accepting results by a coordinator.
	int Accept (mwData &data);

	/// A function for taking rejected data by a coordinator.
	int Reject (mwData &data);

	// --- classes of probes and boxes ---

	/// The set N: Probes for which the verification of the property
	/// was negative.
	ppCubes Negative [ppMaxSubdiv];

	/// The set G: Good probes, that is, probes for which the property
	/// was verified successfully.
	ppCubes Good [ppMaxSubdiv];

	/// The set T: Probes which have to be sent for testing.
	ppCubes Test [ppMaxSubdiv];

	/// Part of the set C2: Currently processed probes.
	ppCubes CurProbes [ppMaxSubdiv];

	/// The set F: Boxes for which the verification failed.
	ppCubes Failed [ppMaxSubdiv];

	/// The set A: Boxes which are successfully verified to satisfy
	/// the given property and thus are contained in the set A.
	ppCubes Successful [ppMaxSubdiv];

	/// The set W: Boxes waiting for verification.
	ppCubes Waiting [ppMaxSubdiv];

	/// Part of the set C: Currently processed boxes.
	ppCubes CurBoxes [ppMaxSubdiv];

	// --- small subroutines ---

	/// Finds a cube at the lowest level (>= min, < max) of the given
	/// set of cubes that was not sent, yet. Modifies 'level' and 'q'.
	/// Returns true if found, false if not found.
	bool FindMinCube
		(const typename mwSubCoordinator<dim,coord>::ppCubes tab [],
		const typename mwSubCoordinator<dim,coord>::ppCubes sent [],
		int minlevel, int maxlevel, int &level,
		typename mwSubCoordinator<dim,coord>::ppCube &q);

	/// Returns a probe with respect to a higher level grid,
	/// that is, multiplies all the coordinates by 2.
	static typename mwSubCoordinator<dim,coord>::ppCube UpLevel
		(const typename mwSubCoordinator<dim,coord>::ppCube &q);

	/// Returns a box with respect to a higher level grid which is
	/// contained in the given box and contains the given corner.
	static typename mwSubCoordinator<dim,coord>::ppCube UpLevel
		(const typename mwSubCoordinator<dim,coord>::ppCube &box,
		const typename mwSubCoordinator<dim,coord>::ppCube &corner);

	/// Returns a box with respect to a higher level grid
	/// which contains the given box, that is,
	/// divides all the coordinates by 2.
	static typename mwSubCoordinator<dim,coord>::ppCube DownLevel
		(const typename mwSubCoordinator<dim,coord>::ppCube &q);

	/// Verifies if the given box is contained in a box at any level
	/// greater than or equal to the given minimum
	/// in the given family of boxes.
	/// Returns the level of a box which is found or 0 if none.
	int Contained (const typename mwSubCoordinator<dim,coord>::ppCube &q,
		int level,
		const typename mwSubCoordinator<dim,coord>::ppCubes tab [],
		int minlevel) const;

	// --- processing specific boxes and probes ---

	/// A function for acquiring a negative probe.
	void NegativeProbe (const ppCube &q, int level);

	/// A function for acquiring a good probe.
	void GoodProbe (const ppCube &q, int level);

	/// A function for acquiring a box for which the verification failed.
	void FailedBox (const ppCube &q, int level);

	/// A function for acquiring a successful box.
	void SuccessfulBox (const ppCube &q, int level);

	// --- parameters of the computations ---

	/// The left (minimal) vertices of the parameter cube.
	double paramLeft [dim];

	/// The right (maximal) vertices of the parameter cube.
	double paramRight [dim];

	/// The level of subdivisions in the parameter space at which
	/// the actual computations begin.
	int inilevel;

	/// The maximal allowed level of subdivision in the parameter space.
	int maxlevel;

	/// An iterator of initial probes.
	mwIniProbes<dim,coord> iniProbes;

	/// A set of previously processed initial probes (used only if
	/// continuing the computations).
	ppCubes iniProcessed;

	// --- results file ---

	/// A file to append the results to.
	std::ofstream f;

	/// Should the results file be flushed after every write?
	bool flushfile;

	/// A procedure for reading the previously computed results.
	int ReadResults (std::istream &in);

	// --- data packs ---

	/// The counter of data packs prepared by a coordinator.
	int current;

	/// The data packs sent for processing.
	multitable<ppPack> sent;

	/// The number of data packs sent for processing.
	int sentlen;

	// --- statistics ---

	/// The number of processed probes with negative result.
	int countNegative;

	/// The number of processed probes which turn out to be good.
	int countGood;

	/// The number of processed boxes for which the verification failed.
	int countFailed;

	/// The number of processed boxes for which the verification
	/// was successful.
	int countSuccessful;

}; /* class mwSubCoordinator */

// --------------------------------------------------

template <int dim, class coord>
const int mwSubCoordinator<dim,coord>::ppMaxSubdiv;

// --------------------------------------------------

template <int dim, class coord>
inline mwSubCoordinator<dim,coord>::mwSubCoordinator ()
{
	return;
} /* mwSubCoordinator<dim,coord>::mwSubCoordinator */

template <int dim, class coord>
mwSubCoordinator<dim,coord>::mwSubCoordinator (const char *filename,
	int _inilevel, int _maxlevel, bool _flushfile,
	const double *_paramLeft, const double *_paramRight):
	inilevel ((_inilevel < 2) ? 2 : _inilevel),
	maxlevel ((_maxlevel <= inilevel) ? (inilevel + 1) : _maxlevel),
	iniProbes (inilevel),
	flushfile (_flushfile),
	current (0), sentlen (0),
	countNegative (0), countGood (0),
	countFailed (0), countSuccessful (0)
{
	// display a warning if no file name is provided
	if (!filename || !*filename)
		sout << "WARNING: No results file name given. "
			"Computation results will not be saved.\n";

	// initialize the subdivision levels
	if (maxlevel > ppMaxSubdiv)
	{
		sout << "WARNING: Max subdivision level decreased from " <<
			maxlevel << " to " << ppMaxSubdiv << ".\n";
		maxlevel = ppMaxSubdiv;
		if (inilevel >= maxlevel)
			inilevel = maxlevel - 1;
	}

	// read the previously computed results of the computations
	if (filename && *filename)
	{
		std::ifstream in (filename);
		if (!in)
			sout << "Note: Could not open the results file. "
			"Any previous results will be ignored.\n";
		else
		{
			this -> ReadResults (in);
			in. close ();
		}
	}

	// open the results file for appending
	if (filename && *filename)
	{
		f. open (filename, std::ios::out | std::ios::app);
		f << "; Started on " << currenttime ();
		if (flushfile)
			f << std::flush;
	}

	// copy the left and right ends of the intervals
	for (int i = 0; i < dim; ++ i)
	{
		paramLeft [i] = _paramLeft [i];
		paramRight [i] = _paramRight [i];
	}

	return;
} /* mwSubCoordinator<dim,coord>::mwSubCoordinator */

template <int dim, class coord>
inline mwSubCoordinator<dim,coord>::~mwSubCoordinator ()
{
	f << "; A total of " << (countNegative + countGood) <<
		" probes and " << (countFailed + countSuccessful) <<
		" boxes have been processed.\n";
	f << "; The verification was negative for " << countNegative <<
		" probes, and " << countGood << " probes were good.\n";
	f << "; The verification failed for " << countFailed <<
		" boxes, and was successful for " << countSuccessful <<
		" boxes.\n";
	f << "; Finished on " << currenttime ();
	f << "; ---------------------------------------------------\n";
	f. close ();
	return;
} /* mwSubCoordinator<dim,coord>::~mwSubCoordinator */

// --------------------------------------------------

template <int dim, class coord>
inline bool mwSubCoordinator<dim,coord>::FindMinCube
	(const typename mwSubCoordinator<dim,coord>::ppCubes tab [],
	const typename mwSubCoordinator<dim,coord>::ppCubes sent [],
	int minlevel, int maxlevel,
	int &level, typename mwSubCoordinator<dim,coord>::ppCube &q)
{
	for (int l = minlevel; l < maxlevel; ++ l)
	{
		const ppCubes &theSet = tab [l];
		const ppCubes &sentSet = sent [l];
		int number = theSet. size ();
		for (int i = number - 1; i >= 0; -- i)
		{
			const ppCube &c = theSet [i];
			if (!sentSet. check (c))
			{
				level = l;
				q = c;
				return true;
			}
		}
	}
	return false;
} /* mwSubCoordinator::FindMinCube */

template <int dim, class coord>
int mwSubCoordinator<dim,coord>::Prepare (mwData &data)
{
	// prepare a data pack to send
	ppPack &pack = sent [sentlen];

	// if there is any initial probe that has not yet been processed
	// then take this probe
	bool iniProbe = false;
	while (iniProbes. available ())
	{
		pack = ppPack (current, iniProbes. get (), inilevel, false);
		if (iniProcessed. check (pack. q))
		{
			iniProcessed. remove (pack. q);
			continue;
		}
		else
		{
			iniProbe = true;
			break;
		}
	}

	// otherwise send a probe or a box which is waiting to be verified
	if (!iniProbe)
	{
		// find a probe for testing at the lowest level available
		int levelT = 0;
		ppCube cubeT;
		bool foundT = this -> FindMinCube (Test, CurProbes,
			inilevel, maxlevel, levelT, cubeT);

		// find a box that is waiting at the lowest level available
		int levelW = 0;
		ppCube cubeW;
		bool foundW = this -> FindMinCube (Waiting, CurBoxes,
			inilevel, maxlevel, levelW, cubeW);

		// if no cube for processing can be found, then interrupt
		if (!foundT && !foundW)
			return mwNoData;

		// send the probe found if this is the right thing to do
		if (foundT && (!foundW || (levelT <= levelW)))
		{
			pack = ppPack (current, cubeT, levelT, false);
			CurProbes [levelT]. add (cubeT);
		}
		// otherwise send the box found
		else
		{
			pack = ppPack (current, cubeW, levelW, true);
			CurBoxes [levelW]. add (cubeW);
		}
	}

	// prepare the intervals of parameters corresponding to the cube
	coord c [ppMaxDim];
	pack. q. coord (c);
	int fullSize = 1 << pack. level;
	double left [ppMaxDim];
	double right [ppMaxDim];
	for (int i = 0; i < dim; ++ i)
	{
		// prepare the probe or the left corner of the box
		left [i] = mwSubdivPoint (paramLeft [i], paramRight [i],
			c [i], fullSize);

		// if this is a full box then compute the right corner, too
		if (pack. full)
		{
			// prepare the right corner of the box
			right [i] =  mwSubdivPoint (paramLeft [i],
				paramRight [i], c [i] + 1, fullSize);

			// if the resolution of the representable numbers
			// has been reached then set this level to be
			// the strict upper bound for the subdivision level
			if (left [i] == right [i])
			{
				// leave a note of this situation in the file
				f << "; The resolution of representable "
					"numbers exceeded.\n; Decreasing "
					"max level from " << maxlevel <<
					" to " << pack. level << ".\n";
				if (flushfile)
					f << std::flush;

				// notify the user about this situation
				sout << "WARNING: The resolution of "
					"representable numbers exceeded.\n"
					"Decreasing max level from " <<
					maxlevel << " to " << pack. level <<
					".\n";

				// decrease the max level limit
				maxlevel = pack. level;
				return mwNoData;
			}
		}
	}

	// write th information on the prepared cube to the results file
	f << "+ " << pack. num;
	for (int i = 0; i < dim; ++ i)
	{
		f << (i ? "x[" : " [") << left [i];
		if (pack. full)
			f << "," << right [i];
		f << "]";
	}
	f << ".\n";
	if (flushfile)
		f << std::flush;

	// send the data pack number
	data << pack. num;
	
	// send the subdivision level
	data << pack. level;

	// send the information on whether this is a full box or a probe
	data << pack. full;

	// send the intervals (left and right ends where applicable)
	for (int i = 0; i < dim; ++ i)
	{
		data << left [i];
		if (pack. full)
			data << right [i];
	}

	// append the data end marker for extra verification
	const int dataEndMarker = 13;
	data << dataEndMarker;

	// show a message on which data pack has been prepared
	sout << "+ " << pack. num << ".\n";

	// increase the counters
	++ current;
	++ sentlen;

	return mwOk;
} /* mwSubCoordinator<dim,coord>::Prepare */

// --------------------------------------------------

template <int dim, class coord>
typename mwSubCoordinator<dim,coord>::ppCube
	mwSubCoordinator<dim,coord>::UpLevel
	(const typename mwSubCoordinator<dim,coord>::ppCube &q)
{
	coord c [dim];
	q. coord (c);
	for (int i = 0; i < dim; ++ i)
		c [i] <<= 1;
	return ppCube (c, dim);
} /* mwSubCoordinator::UpLevel */

template <int dim, class coord>
typename mwSubCoordinator<dim,coord>::ppCube
	mwSubCoordinator<dim,coord>::UpLevel
	(const typename mwSubCoordinator<dim,coord>::ppCube &box,
	const typename mwSubCoordinator<dim,coord>::ppCube &corner)
{
	coord cBox [dim];
	box. coord (cBox);
	coord cCorner [dim];
	corner. coord (cCorner);
	for (int i = 0; i < dim; ++ i)
	{
		if (cBox [i] == cCorner [i])
			cBox [i] <<= 1;
		else
			cBox [i] = (cBox [i] << 1) + 1;
	}
	return ppCube (cBox, dim);
} /* mwSubCoordinator::UpLevel */

template <int dim, class coord>
typename mwSubCoordinator<dim,coord>::ppCube
	mwSubCoordinator<dim,coord>::DownLevel
	(const typename mwSubCoordinator<dim,coord>::ppCube &q)
{
	coord c [dim];
	q. coord (c);
	for (int i = 0; i < dim; ++ i)
		c [i] >>= 1;
	return ppCube (c, dim);
} /* mwSubCoordinator::DownLevel */

template <int dim, class coord>
int mwSubCoordinator<dim,coord>::Contained
	(const typename mwSubCoordinator<dim,coord>::ppCube &q, int level,
	const typename mwSubCoordinator<dim,coord>::ppCubes tab [],
	int minlevel) const
{
	// verify if the box itself is contained in any set of boxes
	if (tab [level]. check (q))
		return level;

	// subdivide the box and verify at a higher level
	if (minlevel < inilevel)
		minlevel = inilevel;
	ppCube box = q;
	while (-- level > minlevel)
	{
		box = DownLevel (box);
		if (tab [level]. check (box))
			return level;
	}

	return 0;
} /* mwSubCoordinator::Contained */

template <int dim, class coord>
void mwSubCoordinator<dim,coord>::NegativeProbe (const ppCube &q, int level)
{
	// add the probe to the set of negative probes
	Negative [level]. add (q);
	sbug << "@ NegProbe [" << level << "]: " << q << "\n";

	// boxes around this probe automatically fail, so remove them
	// from the queue and interpret them as failed boxes
	mwBoxes<dim,coord> boxes (q);
	while (boxes. available ())
	{
		// take a box which has this probe in its corner
		ppCube box = boxes. get ();

		// if it is waiting for processing but was not sent yet
		// then pretend it has just returned as a failed box
		if (Waiting [level]. check (box) &&
			!CurBoxes [level]. check (box))
		{
			sbug << "# FailedBox [" << level << "]: " <<
				box << "\n";
			Waiting [level]. remove (box);
			FailedBox (box, level);
		}
	}

	return;
} /* mwSubCoordinator::NegativeProbe */

template <int dim, class coord>
void mwSubCoordinator<dim,coord>::GoodProbe (const ppCube &q, int level)
{
	// if it is already known that this probe is good then skip it
	if (Good [level]. check (q))
	{
		sbug << "@ GoodKnown [" << level << "]: " << q << "\n";
		return;
	}

	// add the probe to the set of good probes at its level
	Good [level]. add (q);
	sbug << "@ GoodProbe [" << level << "]: " << q << "\n";

	// if the next level is beyond the scope then quit here
	if (level + 1 == maxlevel)
		return;

	// transfer the probe to the next level and remember that it is good
	ppCube probe = UpLevel (q);
	Good [level + 1]. add (probe);
	sbug << "+ GoodProbe [" << (level + 1) << "]: " << probe << "\n";

	// consider all the boxes which contain this probe
	mwBoxes<dim,coord> boxes (probe);
	while (boxes. available ())
	{
		// if the box is not contained in any larger successful box
		// then add it to the waiting list
		ppCube box = boxes. get ();
		if (!Contained (box, level + 1, Successful, 0))
		{
			
			Waiting [level + 1]. add (box);
			sbug << "+ Waiting [" << (level + 1) << "]: " <<
				box << "\n";
		}
	}

	return;
} /* mwSubCoordinator::GoodProbe */

template <int dim, class coord>
void mwSubCoordinator<dim,coord>::FailedBox (const ppCube &q, int level)
{
	// add the box to the list of boxes which failed
	Failed [level]. add (q);
	sbug << "@ FailedBox [" << level << "]: " << q << "\n";

	// if no further subdivisions are to be considered then quit here
	if (level + 1 == maxlevel)
		return;

	// consider all the corners of the box
	mwCorners<dim,coord> corners (q);
	while (corners. available ())
	{
		// take the next corner of the box
		ppCube c = corners. get ();

		// if this is a negative probe then skip it
		if (Negative [level]. check (c))
			continue;

		// if this probe has not yet been checked
		// then add it to the list of probes to test
		if (!Good [level]. check (c))
		{
			Test [level]. add (c);
			sbug << "+ Test [" << level << "]: " << c << "\n";
			continue;
		}

		// add a twice smaller box located at the given corner
		// to the list of boxes which wait for being verified,
		// unless the result for this box is already known
		ppCube box = UpLevel (q, c);
		if (!Successful [level + 1]. check (box) &&
			!Failed [level + 1]. check (box))
		{
			Waiting [level + 1]. add (box);
			sbug << "+ Waiting [" << (level + 1) << "]: " <<
				box << "\n";
		}

		// mark the probe as good at the higher subdivision level
		ppCube probe = UpLevel (c);
		Good [level + 1]. add (probe);
		sbug << "+ Good [" << (level + 1) << "]: " << probe << "\n";
	}

	return;
} /* mwSubCoordinator::FailedBox */

template <int dim, class coord>
void mwSubCoordinator<dim,coord>::SuccessfulBox (const ppCube &q, int level)
{
	// if the box has already been added then skip it
	if (Successful [level]. check (q))
	{
		sbug << "@ SuccessKnown [" << level << "]: " << q << "\n";
		return;
	}

	// if this box is already contained in a larger successful box
	// then ignore it
	if (Contained (q, level, Successful, 0))
	{
		sbug << "@ SuccessSubset [" << level << "]: " << q << "\n";
		return;
	}

	// add the box to the set of successful boxes which form the set A
	Successful [level]. add (q);
	sbug << "@ SuccessBox [" << level << "]: " << q << "\n";

	// consider all the vertices of this box
	mwCorners<dim,coord> corners (q);
	while (corners. available ())
	{
		// take the next corner of the box
		ppCube c = corners. get ();

		// remove the corner probe from a list of probes to test
		if (Test [level]. check (c))
		{
			Test [level]. remove (c);
			sbug << "- Test [" << level << "]: " << c << "\n";
		}

		// remove the corner box from a list of boxes to test
		ppCube cornerBox = UpLevel (q, c);
		if (Waiting [level + 1]. check (cornerBox))
		{
			Waiting [level + 1]. remove (cornerBox);
			sbug << "- Waiting [" << (level + 1) << "]: " <<
				cornerBox << "\n";
			// note: there might be some boxes contained in q
			// waiting at deeper levels, but determining them
			// might be quite inefficient,
			// so they are not cleaned in this implementation
			// of the algorithm
		}

		// if the probe is already known to be good then skip it
		if (Good [level]. check (c))
			continue;

		// add the probe to the list of good ones
		Good [level]. add (c);
		sbug << "+ Good [" << level << "]: " << c << "\n";

		// propagate the probe one level deeper and mark it as good
		ppCube probe = UpLevel (c);
		Good [level + 1]. add (probe);
		sbug << "+ Good [" << (level + 1) << "]: " << probe << "\n";

		// consider all the boxes which contain this probe
		mwBoxes<dim,coord> boxes (probe);
		while (boxes. available ())
		{
			// take the next box
			ppCube box = boxes. get ();

			// if this box is not contained in any successful
			// larger box then add it to the waiting list
			if (!Contained (box, level + 1, Successful, 0))
			{
				Waiting [level + 1]. add (box);
				sbug << "+ Waiting [" << (level + 1) <<
					"]: " << box << "\n";
			}
		}
	}

	return;
} /* mwSubCoordinator::SuccessfulBox */

template <int dim, class coord>
int mwSubCoordinator<dim,coord>::Accept (mwData &data)
{
	// decode the data pack number
	int n = 0;
	data >> n;

	// decode the result
	int result = 0;
	data >> result;

	// say what has been received
	sout << "* " << n << " " << result << ".\n";

	// find this data among the data packs that were sent for processing
	int pos = 0;
	while ((pos < sentlen) && (sent [pos]. num != n))
		++ pos;
	if (pos >= sentlen)
	{
		sout << "ERROR: Wrong data pack number received "
			"from a worker.\n";
		return mwError;
	}
	const ppPack &pack = sent [pos];

	// save the data to the output file
	f << (pack. full ? "* " : "@ ") << n << " " << result <<
		" " << pack. level << " " << pack. q << ".\n";
	if (flushfile)
		f << std::flush;
	
	// extract the data from the data pack
	const ppCube &q = pack. q;
	int level = pack. level;
	bool full = pack. full;

	// remove the cube from appropriate sets and process it
	if (full)
	{
		CurBoxes [level]. remove (q);
		Waiting [level]. remove (q);
		if (result)
		{
			++ countSuccessful;
			SuccessfulBox (q, level);
		}
		else
		{
			++ countFailed;
			FailedBox (q, level);
		}
	}
	else
	{
		CurProbes [level]. remove (q);
		Test [level]. remove (q);
		if (result)
		{
			++ countGood;
			GoodProbe (q, level);
		}
		else
		{
			++ countNegative;
			NegativeProbe (q, level);
		}
	}

	// remove the data pack from the table containing the data packs
	-- sentlen;
	if (pos != sentlen)
		sent [pos] = sent [sentlen];

	return mwOk;
} /* mwSubCoordinator<dim,coord>::Accept */

template <int dim, class coord>
int mwSubCoordinator<dim,coord>::Reject (mwData &data)
{
	// decode the number
	int n = 0;
	data >> n;

	// say that data has been rejected
	sout << "!!! Data no. " << n << " rejected.\n";

	return mwError;
} /* mwSubCoordinator<dim,coord>::Reject */

// --------------------------------------------------

template <int dim, class coord>
int mwSubCoordinator<dim,coord>::ReadResults (std::istream &in)
{
	ignorecomments (in);
	while (!in. eof ())
	{
		// read the first char and interprete the line
		int ch = in. get ();

		// ignore the line if it contains no useful information
		if ((ch != '*') && (ch != '@'))
		{
			ignoreline (in);
			ignorecomments (in);
			continue;
		}
	
		// read the data number if relevant
		if ((ch == '*') || (ch == '@'))
		{
			int n = -1;
			in >> n;
			if (n < 0)
				break;
		}

		// read the result if any
		int result = -1;
		if ((ch == '*') || (ch == '@'))
		{
			in >> result;
			if (result < 0)
				break;
		}

		// read the subdivision level of the cube
		int level = -1;
		in >> level;
		if (level < 0)
			break;

		// read the cube itself
		ppCube q;
		in >> q;

		// make sure that there is the dot which ends the line
		if (in. peek () != '.')
			break;

		// ignore the remaining part of the line
		ignoreline (in);
		ignorecomments (in);

		// process the data as necessary
		if (ch == '*')
		{
			Waiting [level]. remove (q);
			if (result)
				SuccessfulBox (q, level);
			else
				FailedBox (q, level);
		}
		else if (ch == '@')
		{
			if (level == inilevel)
				iniProcessed. add (q);
			else
				Test [level]. remove (q);
			if (result)
				GoodProbe (q, level);
			else
				NegativeProbe (q, level);
		}
	}

	return 0;
} /* mwSubCoordinator<dim,coord>::ReadResults */


// --------------------------------------------------
// ---------------------- MAIN ----------------------
// --------------------------------------------------

/// The main procedure for running the computations in the multiwork
/// subdivision framework.
/// Returns: 0 = Ok, -1 = Error, 1 = Help displayed, 2 = Wrong arguments.
template <int dim, class coord>
int mwSubdivMain (int argc, char *argv [],
	const char *title, const char *helpinfo,
	int defaultPortNumber, int controlNumber,
	const double *paramLeft, const double *paramRight,
	int minSubdivLevel, fcompute compute)
{
	const char *arginfo = "\
Command line arguments (at least '-w' or '-m N' must be specified):\n\
-w [port] - run as a worker (by default the program runs as a coordinator),\n\
-c [port] - run as a coordinator only (don't process any data locally),\n\
-p port - set the port number for the multi-work communication,\n\
-k - keep workers waiting after the computations have been completed,\n\
computer:port - use this connection at start-up (can be repeated),\n\
-s FILE - save the workers' list to this file (default: mwsubdiv.txt),\n\
-r FILE - retrieve the workers' list from this file (def: mwsubdiv.txt),\n\
-f filename - results file (new results will be appended); coord only!\n\
--flush - flush the results file very frequently (slows down the program),\n\
-i N - set the initial subdivision level (some minimum is enforced),\n\
-m N - set the maximal subdivision level (default: inilevel + 2),\n\
-q - quit all the workers who are waiting (provide addresses or use -r),\n\
--quiet - do not display any messages on the standard output,\n\
--log filename - save the console output to the given file,\n\
--help - show this brief help information and exit.\n\
For more information ask the author at http://www.PawelPilarczyk.com/.";

	// prepare user-configurable data
	char *retrieveworkers = 0;
	char *saveworkers = 0;
	const int maxaddr = 1024;
	char *addr [maxaddr];
	int naddr = 0;
	int portnum = -1;
	int workport = -1;
	int coordport = -1;
	bool keepworkers = false;
	bool quitworkers = false;
	char *filename = 0;
	int inilevel = 0;
	int maxlevel = 0;
	bool flushfile = false;

	// interprete the command-line arguments
	arguments a;
	arg (a, 0, addr, naddr, maxaddr);
	arg (a, "r", retrieveworkers, "mwsubdiv.txt");
	arg (a, "s", saveworkers, "mwsubdiv.txt");
	arg (a, "f", filename);
	arg (a, "i", inilevel);
	arg (a, "m", maxlevel);
	arg (a, "w", workport, defaultPortNumber);
	arg (a, "p", portnum);
	arg (a, "c", coordport, defaultPortNumber);
	argswitch (a, "k", keepworkers, true);
	argswitch (a, "q", quitworkers, true);
	argswitch (a, "-flush", flushfile, true);
	arghelp (a);

	argstreamprepare (a);
	int argresult = a. analyze (argc, argv);
	argstreamset ();

	// show the program's main title
	if (argresult >= 0)
		sout << title << '\n';

	// if something was incorrect, show an additional message and exit
	if (argresult < 0)
	{
		sout << "Call with '--help' for help.\n";
		return 2;
	}

	// set the right port number and determine if to run as a worker
	// or as a coordinator, with or without local work
	int port = defaultPortNumber;
	bool localwork = (coordport < 0);
	if (coordport >= 0)
		port = coordport;
	bool worker = (workport >= 0);
	if (workport >= 0)
		port = workport;
	if (portnum >= 0)
		port = portnum;

	// if no data packs are to be processed, don't run the program
	if (!maxlevel && !worker && !quitworkers)
		argresult = 1;
	
	// if help requested, show help information
	if (argresult > 0)
	{
		sout << helpinfo << '\n' << arginfo << '\n';
		return 1;
	}

	// try running the main function and catch an error message if thrown
	try
	{
		// set an appropriate program time message
		program_time = "Aborted after:";
		program_time = 1;

		// quit all the workers from the list if requested to
		if (quitworkers)
		{
			// prepare a dummy coordinator class
			mwSubCoordinator<dim,coord> c;

			// set up the parameters necessary for identification
			c. Port (port);
			c. ControlNumber (controlNumber);

			// prepare a list of workers' addresses
			if (retrieveworkers)
				c. Load (retrieveworkers);
			for (int i = 0; i < naddr; ++ i)
				c. Add (addr [i]);

			// quit all the workers which appear in the list
			sout << "Quitting workers... ";
			c. QuitWorkers ();
			sout << "Done.\n";
		}

		// run as a worker if requested to
		else if (worker)
		{
			// prepare a worker object
			mwSubWorker<dim,coord> w (compute);

			// set up various options of the worker object
			w. Port (port);
			w. ControlNumber (controlNumber);
			for (int i = 0; i < naddr; ++ i)
				w. Add (addr [i]);

			// run the computations
			sout << "Running as a worker...\n";
			int result = w. Work ();
			if (result == mwOk)
				sout << "Work completed successfully.\n";
			else
				sout << "Could not work - probably "
					"an error occurred.\n";
		}

		// run as a coordinator otherwise
		else
		{
			// make a correction to the subdivision level bounds
			if (inilevel <= minSubdivLevel)
				inilevel = minSubdivLevel;
			if (maxlevel <= inilevel)
				maxlevel = inilevel + 2;

			// prepare a local worker and a coordinator
			mwSubWorker<dim,coord> w (compute);
			mwSubCoordinator<dim,coord> c (filename, inilevel,
				maxlevel, flushfile, paramLeft, paramRight);

			// set up various options of the coordinator
			c. KeepWorkers (keepworkers);
			c. Port (port);
			c. ControlNumber (controlNumber);
			for (int i = 0; i < naddr; ++ i)
				c. Add (addr [i]);
			if (retrieveworkers)
				c. Load (retrieveworkers);

			// run the computations
			sout << "Running as a coordinator...\n";
			int result = c. Coordinate (localwork ? &w : 0);
			if (result == mwOk)
				sout << "The task completed successfully.\n";
			else
				sout << "Could not coordinate - probably "
					"an error occurred.\n";

			// save the connected workers if necessary
			if (saveworkers)
				c. SaveWorkers (saveworkers);
		}

		// set an appropriate program time message
		program_time = "Total time used:";

		// finalize
		return 0;
	}
	catch (const char *msg)
	{
		sout << "ERROR: " << msg << '\n';
		return -1;
	}
	catch (const std::exception &e)
	{
		sout << "ERROR: " << e. what () << '\n';
		return -1;
	}
	catch (...)
	{
		sout << "ABORT: An unknown error occurred.\n";
		return -1;
	}
} /* mwSubdivMain */


} // namespace multiwork
} // namespace chomp

#endif // _CHOMP_MULTIWORK_MWSUBDIV_H_

/// @}

