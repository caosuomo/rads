/////////////////////////////////////////////////////////////////////////////
///
/// @file showcubes.cpp
///
/// This file contains an implementation of a program which displays
/// 3-dimensional cubical sets in an interactive way using OpenGL.
///
/// This program uses a simple framework designed by Pawel Pilarczyk
/// for writing interactive programs that display OpenGL graphics;
/// see the header file "wxglmain.h" for instructions on how to use
/// this framework.
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

// Started on June 30, 2004. Last revision: October 22, 2008.


// #define _SHOWCUBES_CPP_

#include "chomp/system/config.h"
#include "chomp/system/textfile.h"
#include "chomp/cubes/pointset.h"
#include "wxglmain.h"

// compile only if the wxWindows GUI is available
#ifdef ppWXWIN

#include <math.h>
#include <vector>

using namespace std;
using namespace chomp::homology;

const char *helpinfo = "\
Call this program with one or more names of text files\n\
to display cubical sets defined in those files.\n\
Each cube is of size 1 and is defined by its vertex\n\
with the minimal coordinates, e.g. (0,1,-1) defines\n\
the cube [0,1]x[1,2]x[-1,0].\n\
Each set of cubes is displayed in different color\n\
(there are 6 colors available).";


// --------------------------------------------------
// ------------------ CUBICAL SETS ------------------
// --------------------------------------------------

/// A class for holding and manipulating cubical sets to be displayed.
class CubicalSets
{
public:
	/// The default constructor.
	CubicalSets ();

	/// The destructor.
	~CubicalSets ();

	/// Returns the number of sets of cubes to be displayed.
	int howManySets () const;

	/// Returns the number of cubes in the given set.
	int howManyCubes (int dim, int n) const;

	/// Returns a pointer to the coordinates of the given cube.
	coordinate *cubeCoords (int dim, int nSet, int nCube) const;

	/// Returns the flags which determine the shape of the given cube.
	/// Each of the first 3 bits indicates in which direction the cube
	/// has a non-zero length, e.g. 0x02 indicates an edge in the
	/// Y direction, 0x06 - a face that spans in the Y and Z directions.
	int shapeFlags (int dim, int nSet, int nCube) const;

	/// Returns the flags that indicate which 2-dimensional faces should
	/// be displayed. If there is an adjacent full cube then their common
	/// 2-dimensional face does not have to be displayed, unless working
	/// in the wireframe mode.
	int faceFlags (int nSet, int nCube) const;

	/// Returns the coordinates of the center point of the cubical set.
	/// These coordinates are computed by the function "postProcess".
	/// If they were not computed so far, then returns the 0 pointer.
	const double *centerPoint () const;

	/// Returns the grid size at which the entire cubical set is
	/// contained in the box [-1,1]^3.
	double gridSize () const;

	/// Returns the max-metric radius of each full cube. This is
	/// a half of the grid size.
	double cubeRadius () const;

	/// Reads a set of full cubes or elementary cubes from a text file.
	/// Returns 0 on success, -1 on error.
	int readCubes (const char *filename);

	/// Post-processes all the cubical sets after they have been read
	/// from files. Computes the center of the set of cubes, etc.
	void postProcess ();

private:
	/// The copy constructor is not allowed.
	CubicalSets (const CubicalSets &c);

	/// The assignment operator is not allowed.
	CubicalSets &operator = (const CubicalSets &c);

	/// A set of points in the minimal vertex of all the cubical sets.
	/// Elementary cubes of each dimension (0-3) are stored
	/// in a separate set.
	/// Cubes of dimensions 1 and 2 have the additional 4th coordinate
	/// which contains direction flags that indiciate in which direction
	/// the cube has nonzero length: 0x01 - X, 0x02 - Y, 0x04 - Z.
	pointset coords [4];

	/// An array of face flags for full cubes which indicate faces
	/// that should be displayed.
	char *faces;

	/// The indices one-beyond-the-last point in each set.
	std::vector<int> maxPoints [4];

	/// The coordinates of the center of the cubical set.
	/// Computed after loading the cubical sets to be displayed.
	double *center;

	/// The grid size.
	double grid;

	/// The radius of the cubes in the max metric.
	double radius;

	/// The full cubical set that is used as a logo to be displayed
	/// if there are no cubical sets read from files.
	static coordinate logo [] [3];

}; /* class CubicalSets */

// --------------------------------------------------

inline CubicalSets::CubicalSets ():
	faces (0), center (0), grid (0), radius (0)
{
	coords [0]. dimension (3);
	coords [1]. dimension (4);
	coords [2]. dimension (4);
	coords [3]. dimension (3);
	return;
} /* CubicalSets::CubicalSets */

inline CubicalSets::~CubicalSets ()
{
	if (faces)
		delete [] faces;
	if (center)
		delete [] center;
	return;
} /* CubicalSets::~CubicalSets */

inline CubicalSets::CubicalSets (const CubicalSets &)
{
	throw "Trying to use a private copy constructor.";
} /* CubicalSets::CubicalSets */

inline CubicalSets &CubicalSets::operator = (const CubicalSets &)
{
	throw "Trying to use a private assignment operator.";
} /* CubicalSets::operator = */

inline int CubicalSets::howManySets () const
{
	return maxPoints [0]. size ();
} /* CubicalSets::howManySets */

inline int CubicalSets::howManyCubes (int dim, int n) const
{
	if (n == 0)
		return maxPoints [dim] [0];
	else
		return (maxPoints [dim] [n] - maxPoints [dim] [n - 1]);
} /* CubicalSets::howManyCubes */

inline coordinate *CubicalSets::cubeCoords (int dim, int nSet, int nCube)
	const
{
	if (nSet == 0)
		return coords [dim] [nCube];
	else
		return coords [dim] [maxPoints [dim] [nSet - 1] + nCube];
} /* CubicalSets::cubeCoords */

inline int CubicalSets::shapeFlags (int dim, int nSet, int nCube) const
{
	if (nSet == 0)
		return coords [dim] [nCube] [3];
	else
		return coords [dim] [maxPoints [dim] [nSet - 1] + nCube] [3];
} /* CubicalSets::shapeFlags */

inline int CubicalSets::faceFlags (int nSet, int nCube) const
{
	if (nSet == 0)
		return faces [nCube];
	else
		return faces [maxPoints [3] [nSet - 1] + nCube];
} /* CubicalSets::faceFlags */

inline const double *CubicalSets::centerPoint () const
{
	return center;
} /* CubicalSets::centerPoint */

inline double CubicalSets::gridSize () const
{
	return grid;
} /* CubicalSets::gridSize */

inline double CubicalSets::cubeRadius () const
{
	return radius;
} /* CubicalSets::cubeRadius */

int CubicalSets::readCubes (const char *filename)
{
	// open the input file or ignore it in case of failure
	std::ifstream in (filename);
	if (!in)
		return -1;

	// prepare arrays for coordinates of cubes or cells that are read
	const int MaxDim = 99;
	coordinate left [MaxDim], right [MaxDim];
	left [0] = left [1] = left [2] = 0;
	right [0] = right [1] = right [2] = 0;

	// read the set of cubes from a file
	int counter = 0;
	while (in)
	{
		// skip white characters and comments
		ignorecomments (in);

		// if a new generator begins, save the previous one
		if ((in. peek () == 'g') && counter)
		{
			for (int dim = 0; dim < 4; ++ dim)
			{
				maxPoints [dim]. push_back
					(coords [dim]. size ());
			}
			counter = 0;
			ignoreline (in);
			continue;
		}

		// if a coefficient found, read it together with the asterisk
		if ((in. peek () == '-') || (std::isdigit (in. peek ())))
		{
			int number = 0;
			in >> number;
			ignorecomments (in);
			if (in. peek () == '*')
				in. get ();
			continue;
		}

		// find no opening parenthesis found then ignore the line
		if (closingparenthesis (in. peek ()) == EOF)
		{
			ignoreline (in);
			continue;
		}

		// read the coordinates of a cube or cell
		int spacedim = 0;
		int type = 0; // 0 for cell, 1 for cube
		try
		{
			spacedim = readcubeorcell (in, left, right, MaxDim,
				&type);
		}
		catch (...)
		{
			// break the loop in case of error
			break;
		}

		// if unsuccessful read then skip the line
		if ((spacedim == MaxDim) || (spacedim <= 0))
		{
			ignoreline (in);
			continue;
		}

		// prepare the 4th coordinate with direction flags
		int dim = 0;
		if (type)
		{
			dim = 3;
			left [3] = 0x07;
		}
		else
		{
			left [3] = 0;
			for (int i = 0; i < 3; ++ i)
			{
				if (right [i] != left [i])
				{
					left [3] |= (1 << i);
					++ dim;
				}
			}
		}
		
		// add the cube or cell to the set and count it
		coords [dim]. add (left);
		++ counter;
	}

	// add the index of the end of the new points to the list indices
	if (counter)
	{
		for (int dim = 0; dim < 4; ++ dim)
			maxPoints [dim]. push_back (coords [dim]. size ());
	}

	return 0;
} /* CubicalSets::readCubes */

void CubicalSets::postProcess ()
{
	// if nothing was read, create a default set of cubes
	if (coords [0]. empty () && coords [1]. empty () &&
		coords [2]. empty () && coords [3]. empty ())
	{
		int i = 0;
		do
		{
			while (logo [i] [0] || logo [i] [1] || logo [i] [2])
			{
				coords [3]. add (logo [i]);
				++ i;
			}
			for (int dim = 0; dim < 4; ++ dim)
			{
				maxPoints [dim]. push_back
					(coords [dim]. size ());
			}
			++ i;
		} while (logo [i] [0] || logo [i] [1] || logo [i] [2]);
	}

	// determine which faces of the cubes should be displayed
	int nCubes = coords [3]. size ();
	faces = new char [nCubes];
	for (int n = 0; n < nCubes; ++ n)
	{
		// determine which faces of the cubes have to be drawn
		faces [n] = 0;
		coordinate c [3];
		for (int i = 0; i < 3; ++ i)
			c [i] = coords [3] [n] [i];
		char facebits = 0;
		char facebit = 0x01;
		for (int j = 0; j < 6; ++ j)
		{
			c [j >> 1] += (j & 1) ? -1 : 1;
			if (!coords [3]. check (c))
				facebits |= facebit;
			facebit <<= 1;
			c [j >> 1] -= (j & 1) ? -1 : 1;
		}
		faces [n] = facebits;
	}

	// compute the minimal and maximal coordinates
	coordinate minimal [3], maximal [3];
	bool first = true;
	for (int dim = 0; dim < 4; ++ dim)
	{
		if (coords [dim]. empty ())
			continue;
		if (first)
		{
			for (int i = 0; i < 3; ++ i)
			{
				minimal [i] = coords [dim]. minimal [i];
				maximal [i] = coords [dim]. maximal [i];
			}
			first = false;
			continue;
		}
		for (int i = 0; i < 3; ++ i)
		{
			if (minimal [i] > coords [dim]. minimal [i])
				minimal [i] = coords [dim]. minimal [i];
			if (maximal [i] < coords [dim]. maximal [i])
				maximal [i] = coords [dim]. maximal [i];
		}
	}

	// compute the coordinates of the middle of the set of cubes
	center = new double [3];
	for (int i = 0; i < 3; ++ i)
	{
		center [i] = (maximal [i] + 1 + minimal [i]) / 2.0;
	}

	// find the diameter of the set in the "maximum" metric
	int diam = 0;
	for (int i = 0; i < 3; ++ i)
	{
		if (diam < maximal [i] + 1 - minimal [i])
			diam = maximal [i] + 1 - minimal [i];
	}

	// compute the "max"-metric radius of each cube and the grid size
	radius = 1.0 / diam;
	grid = radius * 2;

	return;
} /* CubicalSets::postProcess */

// --------------------------------------------------

CubicalSets cubicalSets;


// --------------------------------------------------
// ---------------- Cubes and OpenGL ----------------
// --------------------------------------------------

/// Draws a cube defined by the given coordinates.
/// Only the requested faces are actually plotted.
inline void drawFullCube (const coordinate *c, int faces)
{
	// determine the grid size and the lower left corner
	const double grid = cubicalSets. gridSize ();
	const double *center = cubicalSets. centerPoint ();
	const double x = (c [0] - center [0]) * grid;
	const double y = (c [1] - center [1]) * grid;
	const double z = (c [2] - center [2]) * grid;

	// right
	if (faces & 0x01)
	{
		glNormal3f (1.0, 0.0, 0.0);
		glVertex3f(x + grid, y, z);
		glVertex3f(x + grid, y + grid, z);
		glVertex3f(x + grid, y + grid, z + grid);
		glVertex3f(x + grid, y, z + grid);
	}

	// left
	if (faces & 0x02)
	{
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(x, y, z);
		glVertex3f(x, y, z + grid);
		glVertex3f(x, y + grid, z + grid);
		glVertex3f(x, y + grid, z);
	}

	// back
	if (faces & 0x04)
	{
		glNormal3f (0.0, 1.0, 0.0);
		glVertex3f(x, y + grid, z);
		glVertex3f(x, y + grid, z + grid);
		glVertex3f(x + grid, y + grid, z + grid);
		glVertex3f(x + grid, y + grid, z);
	}

	// front
	if (faces & 0x08)
	{
		glNormal3f (0.0, -1.0, 0.0);
		glVertex3f(x, y, z);
		glVertex3f(x + grid, y, z);
		glVertex3f(x + grid, y, z + grid);
		glVertex3f(x, y, z + grid);
	}

	// top
	if (faces & 0x10)
	{
		glNormal3f (0.0, 0.0, 1.0);
		glVertex3f(x, y, z + grid);
		glVertex3f(x + grid, y, z + grid);
		glVertex3f(x + grid, y + grid, z + grid);
		glVertex3f(x, y + grid, z + grid);
	}

	// bottom
	if (faces & 0x20)
	{
	 	glNormal3f (0.0, 0.0, -1.0);
		glVertex3f(x, y, z);
		glVertex3f(x, y + grid, z);
		glVertex3f(x + grid, y + grid, z);
		glVertex3f(x + grid, y, z);
	}

	return;
} /* drawFullCube */

/// Draws a cube defined by the given coordinates.
/// Only the requested faces are actually plotted.
inline void drawCubeFace (const coordinate *c, int which)
{
	// determine the grid size and the lower left corner
	const double grid = cubicalSets. gridSize ();
	const double *center = cubicalSets. centerPoint ();
	const double x = (c [0] - center [0]) * grid;
	const double y = (c [1] - center [1]) * grid;
	const double z = (c [2] - center [2]) * grid;

	switch (which)
	{
	// the face in the XY direction (both sides)
	case 0x03:
	 	glNormal3f (0.0, 0.0, -1.0);
		glVertex3f(x, y, z);
		glVertex3f(x, y + grid, z);
		glVertex3f(x + grid, y + grid, z);
		glVertex3f(x + grid, y, z);

	 	glNormal3f (0.0, 0.0, 1.0);
		glVertex3f(x, y, z);
		glVertex3f(x + grid, y, z);
		glVertex3f(x + grid, y + grid, z);
		glVertex3f(x, y + grid, z);
		break;

	// the face in the XZ direction (both sides)
	case 0x05:
		glNormal3f (0.0, -1.0, 0.0);
		glVertex3f(x, y, z);
		glVertex3f(x + grid, y, z);
		glVertex3f(x + grid, y, z + grid);
		glVertex3f(x, y, z + grid);

		glNormal3f (0.0, 1.0, 0.0);
		glVertex3f(x, y, z);
		glVertex3f(x, y, z + grid);
		glVertex3f(x + grid, y, z + grid);
		glVertex3f(x + grid, y, z);
		break;

	// the face in the YZ direction (both sides)
	case 0x06:
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(x, y, z);
		glVertex3f(x, y, z + grid);
		glVertex3f(x, y + grid, z + grid);
		glVertex3f(x, y + grid, z);
	
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(x, y, z);
		glVertex3f(x, y + grid, z);
		glVertex3f(x, y + grid, z + grid);
		glVertex3f(x, y, z + grid);
		break;
	}

	return;
} /* drawCubeFace */

/// Draws a segment beginning at the given point.
inline void drawSegment (const coordinate *c, int which)
{
	// determine the grid size and the lower left corner
	const double grid = cubicalSets. gridSize ();
	const double *center = cubicalSets. centerPoint ();
	const double x = (c [0] - center [0]) * grid;
	const double y = (c [1] - center [1]) * grid;
	const double z = (c [2] - center [2]) * grid;

	switch (which)
	{
	// a segment in the X direction
	case 0x01:
		glVertex3f(x, y, z);
		glVertex3f(x + grid, y, z);
		break;

	// a segment in the Y direction
	case 0x02:
		glVertex3f(x, y, z);
		glVertex3f(x, y + grid, z);
		break;

	// a segment in the Z direction
	case 0x04:
		glVertex3f(x, y, z);
		glVertex3f(x, y, z + grid);
		break;
	}

	return;
} /* drawSegment */

/// Draws a point at the given location.
inline void drawPoint (const coordinate *c)
{
	// determine the grid size and the lower left corner
	const double grid = cubicalSets. gridSize ();
	const double *center = cubicalSets. centerPoint ();
	const double x = (c [0] - center [0]) * grid;
	const double y = (c [1] - center [1]) * grid;
	const double z = (c [2] - center [2]) * grid;

	glVertex3f(x, y, z);

	return;
} /* drawPoint */


// --------------------------------------------------
// ----------------- OpenGL drawing -----------------
// --------------------------------------------------

/// This function is called on canvas initialization.
void initPicture (int argc, char *argv [])
{
	// load the set of cubes to be displayed and analyze them
	for (int i = 1; i < argc; ++ i)
		cubicalSets. readCubes (argv [i]);

	// analyze the sets of cubes
	cubicalSets. postProcess ();

	// don't draw interior faces
	glCullFace (GL_BACK);

	return;
} /* initPicture */

/// This function is called on every paint event to draw the actual picture.
void drawPicture ()
{
	// define the material properties
	const int nMaterials = 6;
	static GLfloat Material [nMaterials] [4] =
	{
		{0.5, 0.5, 0.9, 1.0},
		{0.9, 0.5, 0.5, 1.0},
		{0.8, 0.8, 0.4, 1.0},
		{0.4, 0.8, 0.8, 1.0},
		{0.8, 0.4, 0.8, 1.0},
		{0.5, 0.8, 0.5, 1.0},
	};

	// set a very thin line width if plotting the wireframe only
	if (wireframe)
		glLineWidth (0.1);

	// plot all the cubes
	for (int i = 0; i < cubicalSets. howManySets (); ++ i)
	{
		// enable the material color analysis
		glEnable (GL_COLOR_MATERIAL);

		// define the material properties
		glColorMaterial (GL_FRONT_AND_BACK, GL_SPECULAR);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
			Material [i % nMaterials]);
		glMateriali (GL_FRONT_AND_BACK, GL_SHININESS, 128);

		// draw all the 3-dimensional cubes and 2-dimensional faces
		glBegin (GL_QUADS);
		for (int j = 0; j < cubicalSets. howManyCubes (3, i); ++ j)
		{
			int faces = wireframe ? 0xFF :
				cubicalSets. faceFlags (i, j);
			if (!faces)
				continue;
			const coordinate *coord =
				cubicalSets. cubeCoords (3, i, j);
			drawFullCube (coord, faces);
		}
		for (int j = 0; j < cubicalSets. howManyCubes (2, i); ++ j)
		{
			int shape = cubicalSets. shapeFlags (2, i, j);
			const coordinate *coord =
				cubicalSets. cubeCoords (2, i, j);
			drawCubeFace (coord, shape);
		}
		glNormal3f(-1.0, 0.0, 0.0);
		glEnd ();

		// draw 1-dimensional faces, i.e., edges
		if (!wireframe)
			glLineWidth (exp (magnification) * 2);
		glBegin (GL_LINES);
		for (int j = 0; j < cubicalSets. howManyCubes (1, i); ++ j)
		{
			int shape = cubicalSets. shapeFlags (1, i, j);
			const coordinate *coord =
				cubicalSets. cubeCoords (1, i, j);
			drawSegment (coord, shape);
		}
		glEnd ();

		// draw 0-dimensional faces, i.e., vertices (points)
		glEnable (GL_POINT_SMOOTH);
		glPointSize (wireframe ? 1 : exp (magnification) * 4);
		glBegin (GL_POINTS);
		for (int j = 0; j < cubicalSets. howManyCubes (0, i); ++ j)
		{
			const coordinate *coord =
				cubicalSets. cubeCoords (0, i, j);
			drawPoint (coord);
		}
		glEnd ();
	}

	return;
} /* drawPicture */

/// This function is called on program's exit.
void deletePicture (void)
{
	return;
} /* deletePicture */

/// The data of the logo built of full cubical sets.
coordinate CubicalSets::logo [] [3] =
{
	{68,0,30}, {85,0,30}, {68,1,30}, {86,1,30}, {2,2,30},
	{3,2,30}, {8,2,30}, {9,2,30}, {10,2,30}, {11,2,30},
	{14,2,30}, {16,2,30}, {17,2,30}, {22,2,30}, {23,2,30},
	{24,2,30}, {29,2,30}, {30,2,30}, {31,2,30}, {38,2,30},
	{39,2,30}, {42,2,30}, {43,2,30}, {44,2,30}, {52,2,30},
	{53,2,30}, {54,2,30}, {55,2,30}, {58,2,30}, {62,2,30},
	{63,2,30}, {64,2,30}, {68,2,30}, {70,2,30}, {71,2,30},
	{75,2,30}, {79,2,30}, {80,2,30}, {82,2,30}, {86,2,30},
	{90,2,30}, {1,3,30}, {4,3,30}, {7,3,30}, {11,3,30},
	{14,3,30}, {15,3,30}, {18,3,30}, {21,3,30}, {25,3,30},
	{28,3,30}, {32,3,30}, {38,3,30}, {41,3,30}, {45,3,30},
	{51,3,30}, {55,3,30}, {58,3,30}, {61,3,30}, {65,3,30},
	{68,3,30}, {69,3,30}, {72,3,30}, {75,3,30}, {78,3,30},
	{81,3,30}, {82,3,30}, {86,3,30}, {1,4,30}, {7,4,30},
	{11,4,30}, {14,4,30}, {18,4,30}, {21,4,30}, {32,4,30},
	{38,4,30}, {41,4,30}, {45,4,30}, {51,4,30}, {55,4,30},
	{58,4,30}, {65,4,30}, {68,4,30}, {72,4,30}, {75,4,30},
	{78,4,30}, {82,4,30}, {85,4,30}, {87,4,30}, {1,5,30},
	{7,5,30}, {11,5,30}, {14,5,30}, {18,5,30}, {21,5,30},
	{22,5,30}, {23,5,30}, {24,5,30}, {25,5,30}, {29,5,30},
	{30,5,30}, {31,5,30}, {38,5,30}, {41,5,30}, {45,5,30},
	{51,5,30}, {55,5,30}, {58,5,30}, {62,5,30}, {63,5,30},
	{64,5,30}, {68,5,30}, {72,5,30}, {75,5,30}, {79,5,30},
	{80,5,30}, {81,5,30}, {82,5,30}, {85,5,30}, {87,5,30},
	{1,6,30}, {7,6,30}, {11,6,30}, {14,6,30}, {18,6,30},
	{21,6,30}, {25,6,30}, {28,6,30}, {38,6,30}, {41,6,30},
	{45,6,30}, {51,6,30}, {55,6,30}, {58,6,30}, {61,6,30},
	{68,6,30}, {72,6,30}, {75,6,30}, {82,6,30}, {85,6,30},
	{87,6,30}, {1,7,30}, {4,7,30}, {7,7,30}, {11,7,30},
	{14,7,30}, {15,7,30}, {18,7,30}, {21,7,30}, {25,7,30},
	{28,7,30}, {32,7,30}, {38,7,30}, {41,7,30}, {45,7,30},
	{51,7,30}, {54,7,30}, {55,7,30}, {58,7,30}, {61,7,30},
	{65,7,30}, {68,7,30}, {69,7,30}, {72,7,30}, {75,7,30},
	{78,7,30}, {82,7,30}, {84,7,30}, {88,7,30}, {2,8,30},
	{3,8,30}, {7,8,30}, {11,8,30}, {14,8,30}, {16,8,30},
	{17,8,30}, {22,8,30}, {23,8,30}, {24,8,30}, {29,8,30},
	{30,8,30}, {31,8,30}, {37,8,30}, {38,8,30}, {39,8,30},
	{42,8,30}, {43,8,30}, {44,8,30}, {52,8,30}, {53,8,30},
	{55,8,30}, {58,8,30}, {62,8,30}, {63,8,30}, {64,8,30},
	{68,8,30}, {70,8,30}, {71,8,30}, {75,8,30}, {79,8,30},
	{80,8,30}, {81,8,30}, {84,8,30}, {88,8,30}, {14,9,30},
	{38,9,30}, {55,9,30}, {75,9,30}, {14,10,30}, {38,10,30},
	{55,10,30}, {58,10,30}, {75,10,30}, {0,0,0},
	
	{51,15,20}, {52,15,20}, {53,15,20}, {54,15,20}, {55,16,20},
	{2,17,20}, {3,17,20}, {8,17,20}, {9,17,20}, {10,17,20},
	{14,17,20}, {18,17,20}, {21,17,20}, {22,17,20}, {25,17,20},
	{26,17,20}, {28,17,20}, {31,17,20}, {34,17,20}, {38,17,20},
	{41,17,20}, {44,17,20}, {48,17,20}, {52,17,20}, {53,17,20},
	{55,17,20}, {62,17,20}, {63,17,20}, {64,17,20}, {69,17,20},
	{70,17,20}, {71,17,20}, {75,17,20}, {76,17,20}, {79,17,20},
	{80,17,20}, {81,17,20}, {89,17,20}, {90,17,20}, {91,17,20},
	{95,17,20}, {1,18,20}, {4,18,20}, {7,18,20}, {11,18,20},
	{14,18,20}, {18,18,20}, {21,18,20}, {24,18,20}, {27,18,20},
	{28,18,20}, {31,18,20}, {34,18,20}, {38,18,20}, {41,18,20},
	{44,18,20}, {48,18,20}, {51,18,20}, {54,18,20}, {55,18,20},
	{61,18,20}, {65,18,20}, {68,18,20}, {72,18,20}, {75,18,20},
	{78,18,20}, {82,18,20}, {88,18,20}, {92,18,20}, {95,18,20},
	{1,19,20}, {7,19,20}, {11,19,20}, {14,19,20}, {18,19,20},
	{21,19,20}, {24,19,20}, {28,19,20}, {31,19,20}, {34,19,20},
	{38,19,20}, {41,19,20}, {44,19,20}, {48,19,20}, {51,19,20},
	{55,19,20}, {65,19,20}, {68,19,20}, {75,19,20}, {82,19,20},
	{88,19,20}, {92,19,20}, {95,19,20}, {1,20,20}, {7,20,20},
	{11,20,20}, {14,20,20}, {18,20,20}, {21,20,20}, {25,20,20},
	{26,20,20}, {27,20,20}, {28,20,20}, {31,20,20}, {34,20,20},
	{38,20,20}, {41,20,20}, {44,20,20}, {48,20,20}, {51,20,20},
	{55,20,20}, {62,20,20}, {63,20,20}, {64,20,20}, {68,20,20},
	{69,20,20}, {70,20,20}, {71,20,20}, {72,20,20}, {75,20,20},
	{79,20,20}, {80,20,20}, {81,20,20}, {88,20,20}, {92,20,20},
	{95,20,20}, {1,21,20}, {7,21,20}, {11,21,20}, {14,21,20},
	{18,21,20}, {21,21,20}, {28,21,20}, {31,21,20}, {34,21,20},
	{38,21,20}, {41,21,20}, {44,21,20}, {48,21,20}, {51,21,20},
	{55,21,20}, {61,21,20}, {68,21,20}, {72,21,20}, {75,21,20},
	{78,21,20}, {88,21,20}, {92,21,20}, {95,21,20}, {1,22,20},
	{4,22,20}, {7,22,20}, {11,22,20}, {14,22,20}, {15,22,20},
	{18,22,20}, {21,22,20}, {24,22,20}, {28,22,20}, {31,22,20},
	{34,22,20}, {35,22,20}, {38,22,20}, {41,22,20}, {44,22,20},
	{45,22,20}, {48,22,20}, {51,22,20}, {54,22,20}, {55,22,20},
	{61,22,20}, {65,22,20}, {68,22,20}, {72,22,20}, {75,22,20},
	{78,22,20}, {82,22,20}, {88,22,20}, {92,22,20}, {95,22,20},
	{2,23,20}, {3,23,20}, {8,23,20}, {9,23,20}, {10,23,20},
	{14,23,20}, {16,23,20}, {17,23,20}, {20,23,20}, {21,23,20},
	{22,23,20}, {25,23,20}, {26,23,20}, {27,23,20}, {31,23,20},
	{34,23,20}, {36,23,20}, {37,23,20}, {41,23,20}, {44,23,20},
	{46,23,20}, {47,23,20}, {52,23,20}, {53,23,20}, {55,23,20},
	{62,23,20}, {63,23,20}, {64,23,20}, {69,23,20}, {70,23,20},
	{71,23,20}, {74,23,20}, {75,23,20}, {76,23,20}, {79,23,20},
	{80,23,20}, {81,23,20}, {89,23,20}, {90,23,20}, {91,23,20},
	{94,23,20}, {95,23,20}, {96,23,20}, {21,24,20}, {75,24,20},
	{95,24,20}, {21,25,20}, {31,25,20}, {41,25,20}, {75,25,20},
	{96,25,20}, {97,25,20}, {0,0,0},
	
	{2,32,10}, {6,32,10}, {10,32,10}, {13,32,10}, {14,32,10},
	{16,32,10}, {20,32,10}, {26,32,10}, {30,32,10}, {34,32,10},
	{35,32,10}, {37,32,10}, {40,32,10}, {44,32,10}, {48,32,10},
	{52,32,10}, {53,32,10}, {54,32,10}, {59,32,10}, {60,32,10},
	{61,32,10}, {69,32,10}, {70,32,10}, {71,32,10}, {75,32,10},
	{81,32,10}, {84,32,10}, {87,32,10}, {91,32,10}, {92,32,10},
	{93,32,10}, {98,32,10}, {99,32,10}, {100,32,10}, {2,33,10},
	{6,33,10}, {10,33,10}, {13,33,10}, {16,33,10}, {20,33,10},
	{26,33,10}, {30,33,10}, {33,33,10}, {36,33,10}, {37,33,10},
	{40,33,10}, {44,33,10}, {48,33,10}, {51,33,10}, {55,33,10},
	{58,33,10}, {62,33,10}, {68,33,10}, {72,33,10}, {75,33,10},
	{81,33,10}, {84,33,10}, {87,33,10}, {90,33,10}, {94,33,10},
	{97,33,10}, {101,33,10}, {1,34,10}, {3,34,10}, {5,34,10},
	{7,34,10}, {10,34,10}, {13,34,10}, {16,34,10}, {20,34,10},
	{26,34,10}, {30,34,10}, {33,34,10}, {37,34,10}, {40,34,10},
	{44,34,10}, {48,34,10}, {51,34,10}, {62,34,10}, {68,34,10},
	{72,34,10}, {75,34,10}, {81,34,10}, {84,34,10}, {87,34,10},
	{90,34,10}, {101,34,10}, {1,35,10}, {3,35,10}, {5,35,10},
	{7,35,10}, {10,35,10}, {13,35,10}, {16,35,10}, {20,35,10},
	{26,35,10}, {30,35,10}, {34,35,10}, {35,35,10}, {36,35,10},
	{37,35,10}, {40,35,10}, {44,35,10}, {48,35,10}, {51,35,10},
	{52,35,10}, {53,35,10}, {54,35,10}, {55,35,10}, {59,35,10},
	{60,35,10}, {61,35,10}, {68,35,10}, {72,35,10}, {75,35,10},
	{81,35,10}, {84,35,10}, {87,35,10}, {90,35,10}, {91,35,10},
	{92,35,10}, {93,35,10}, {94,35,10}, {98,35,10}, {99,35,10},
	{100,35,10}, {1,36,10}, {3,36,10}, {5,36,10}, {7,36,10},
	{10,36,10}, {13,36,10}, {16,36,10}, {20,36,10}, {26,36,10},
	{30,36,10}, {37,36,10}, {40,36,10}, {44,36,10}, {48,36,10},
	{51,36,10}, {55,36,10}, {58,36,10}, {68,36,10}, {72,36,10},
	{75,36,10}, {81,36,10}, {84,36,10}, {87,36,10}, {90,36,10},
	{94,36,10}, {97,36,10}, {0,37,10}, {4,37,10}, {8,37,10},
	{10,37,10}, {13,37,10}, {16,37,10}, {17,37,10}, {20,37,10},
	{26,37,10}, {27,37,10}, {30,37,10}, {33,37,10}, {37,37,10},
	{40,37,10}, {41,37,10}, {44,37,10}, {45,37,10}, {48,37,10},
	{51,37,10}, {55,37,10}, {58,37,10}, {62,37,10}, {68,37,10},
	{72,37,10}, {75,37,10}, {81,37,10}, {84,37,10}, {87,37,10},
	{90,37,10}, {94,37,10}, {97,37,10}, {101,37,10}, {0,38,10},
	{4,38,10}, {8,38,10}, {10,38,10}, {12,38,10}, {13,38,10},
	{14,38,10}, {16,38,10}, {18,38,10}, {19,38,10}, {26,38,10},
	{28,38,10}, {29,38,10}, {34,38,10}, {35,38,10}, {36,38,10},
	{40,38,10}, {42,38,10}, {43,38,10}, {46,38,10}, {47,38,10},
	{52,38,10}, {53,38,10}, {54,38,10}, {59,38,10}, {60,38,10},
	{61,38,10}, {69,38,10}, {70,38,10}, {71,38,10}, {74,38,10},
	{75,38,10}, {76,38,10}, {80,38,10}, {81,38,10}, {82,38,10},
	{84,38,10}, {87,38,10}, {91,38,10}, {92,38,10}, {93,38,10},
	{98,38,10}, {99,38,10}, {100,38,10}, {13,39,10}, {16,39,10},
	{75,39,10}, {81,39,10}, {87,39,10}, {10,40,10}, {13,40,10},
	{16,40,10}, {76,40,10}, {77,40,10}, {82,40,10}, {83,40,10},
	{84,40,10}, {87,40,10}, {0,0,0},
	
	{49,45,0}, {67,45,0}, {68,45,0}, {69,45,0}, {70,45,0},
	{49,46,0}, {71,46,0}, {3,47,0}, {4,47,0}, {5,47,0},
	{11,47,0}, {12,47,0}, {14,47,0}, {17,47,0}, {20,47,0},
	{26,47,0}, {27,47,0}, {29,47,0}, {33,47,0}, {36,47,0},
	{40,47,0}, {41,47,0}, {42,47,0}, {49,47,0}, {51,47,0},
	{52,47,0}, {56,47,0}, {61,47,0}, {62,47,0}, {63,47,0},
	{68,47,0}, {69,47,0}, {71,47,0}, {74,47,0}, {79,47,0},
	{80,47,0}, {82,47,0}, {85,47,0}, {89,47,0}, {93,47,0},
	{2,48,0}, {6,48,0}, {10,48,0}, {13,48,0}, {14,48,0},
	{17,48,0}, {20,48,0}, {26,48,0}, {29,48,0}, {33,48,0},
	{36,48,0}, {39,48,0}, {43,48,0}, {49,48,0}, {50,48,0},
	{53,48,0}, {56,48,0}, {60,48,0}, {64,48,0}, {67,48,0},
	{70,48,0}, {71,48,0}, {74,48,0}, {78,48,0}, {81,48,0},
	{82,48,0}, {85,48,0}, {89,48,0}, {93,48,0}, {1,49,0},
	{7,49,0}, {10,49,0}, {14,49,0}, {17,49,0}, {20,49,0},
	{26,49,0}, {29,49,0}, {33,49,0}, {36,49,0}, {43,49,0},
	{49,49,0}, {53,49,0}, {56,49,0}, {60,49,0}, {64,49,0},
	{67,49,0}, {71,49,0}, {74,49,0}, {78,49,0}, {82,49,0},
	{85,49,0}, {89,49,0}, {93,49,0}, {1,50,0}, {11,50,0},
	{12,50,0}, {13,50,0}, {14,50,0}, {17,50,0}, {20,50,0},
	{26,50,0}, {29,50,0}, {33,50,0}, {36,50,0}, {40,50,0},
	{41,50,0}, {42,50,0}, {49,50,0}, {53,50,0}, {56,50,0},
	{60,50,0}, {64,50,0}, {67,50,0}, {71,50,0}, {74,50,0},
	{79,50,0}, {80,50,0}, {81,50,0}, {82,50,0}, {85,50,0},
	{89,50,0}, {93,50,0}, {1,51,0}, {14,51,0}, {17,51,0},
	{20,51,0}, {26,51,0}, {29,51,0}, {33,51,0}, {36,51,0},
	{39,51,0}, {49,51,0}, {53,51,0}, {56,51,0}, {60,51,0},
	{64,51,0}, {67,51,0}, {71,51,0}, {74,51,0}, {82,51,0},
	{85,51,0}, {89,51,0}, {93,51,0}, {1,52,0}, {10,52,0},
	{14,52,0}, {17,52,0}, {20,52,0}, {26,52,0}, {29,52,0},
	{30,52,0}, {33,52,0}, {36,52,0}, {39,52,0}, {43,52,0},
	{49,52,0}, {50,52,0}, {53,52,0}, {56,52,0}, {57,52,0},
	{60,52,0}, {64,52,0}, {67,52,0}, {70,52,0}, {71,52,0},
	{74,52,0}, {75,52,0}, {78,52,0}, {82,52,0}, {85,52,0},
	{86,52,0}, {89,52,0}, {90,52,0}, {93,52,0}, {1,53,0},
	{7,53,0}, {11,53,0}, {12,53,0}, {13,53,0}, {17,53,0},
	{20,53,0}, {25,53,0}, {26,53,0}, {27,53,0}, {29,53,0},
	{31,53,0}, {32,53,0}, {36,53,0}, {40,53,0}, {41,53,0},
	{42,53,0}, {49,53,0}, {51,53,0}, {52,53,0}, {56,53,0},
	{58,53,0}, {61,53,0}, {62,53,0}, {63,53,0}, {68,53,0},
	{69,53,0}, {71,53,0}, {74,53,0}, {76,53,0}, {79,53,0},
	{80,53,0}, {81,53,0}, {85,53,0}, {87,53,0}, {88,53,0},
	{91,53,0}, {92,53,0}, {2,54,0}, {6,54,0}, {17,54,0},
	{20,54,0}, {26,54,0}, {29,54,0}, {3,55,0}, {4,55,0},
	{5,55,0}, {17,55,0}, {20,55,0}, {26,55,0}, {29,55,0},
	{36,55,0}, {0,0,0}, {0,0,0},
};

#endif // wxWindows

