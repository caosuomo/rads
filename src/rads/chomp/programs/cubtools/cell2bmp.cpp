/// @addtogroup homology
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file cell2bmp.cpp
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

// Based on pset2bmp.cpp, September 5, 1998 - February 16, 2006.
// Started on February 16, 2006. Last revision: February 16, 2006.

#include <exception>
#include <new>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;


#include "chomp/system/config.h"
#include "chomp/bitmaps/bitmaps.h"
#include "chomp/system/textfile.h"
#include "chomp/cubes/pointset.h"
#include "chomp/system/timeused.h"
#include "chomp/system/arg.h"
#include "chomp/struct/hashsets.h"

using namespace chomp::homology;


// --------------------------------------------------
// -------------------- OVERTURE --------------------
// --------------------------------------------------

const char *title = "\
Cells -> BitMaP. Copyright (C) 1997-2010 by Pawel Pilarczyk.\n\
This is free software. No warranty. Consult 'license.txt' for details.";

const char *helpinfo = "\
Call with: file1.cel [... fileN.cel] file.bmp\n\
This program reads a set of cells spanning over several full-dimensional\n\
squares or (hyper)cubes and plots them in a BMP file.\n\
At most 254 files can be submitted a time, and will result in the cells\n\
plotted in different colors.\n\
\n\
\tNOTE:\tTHIS PROGRAM IS CURRENTLY UNDER CONSTRUCTION\n\
\t\tAND MAY NOT BE FULLY FUNCTIONAL, YET.\n\
\n\
Additional arguments:\n\
-xn - the number of the coordinate assigned to the X axis (default: 1),\n\
-yn - as above, but to the vertical axis Y (default: 2; range: 1 to dim),\n\
-rn - the resolution of the bitmap picture in dots per inch (default: 0),\n\
-sn - the size of squares in pixels (default: 1),\n\
-pn - pause between the squares corresponding to the points (default: 0),\n\
-mn - margins left at each side of the picture in squares (default: 0),\n\
-gn - add a grid line every 'n' squares (default: 0 for no grid),\n\
-b[n] - add a color bar of size n beneath the picture (no n => auto size),\n\
--gray - use shadows of grays instead of colors for plotting cubes,\n\
-h  - display this brief help information only and exit.\n\
For more information ask the author at http://www.PawelPilarczyk.com/.";

#define MAXNAMES 254

#ifndef MAXDIM
#define MAXDIM 128
#endif


// --------------------------------------------------
// -------------------- SQUARES ---------------------
// --------------------------------------------------
// A class that represents squares to be plotted
// in the BMP file.

template <class coordT>
class square
{
public:
	typedef coordT coordType;

	// the default constructor of an undefined square
	square () {}

	// the constructor of a square
	square (coordT _x1, coordT _y1, coordT _x2, coordT _y2);

	// the coordinates of a square: the square begins at (x1, y1)
	// and contains all the pixels that are less than (x2, y2)
	coordT x1, y1, x2, y2;

	// the scopes of coordinates of all the squares
	static coordT xmin, ymin, xmax, ymax;

private:
}; /* class square */

// --------------------------------------------------

template <class coordT>
coordT square<coordT>::xmin = 0;

template <class coordT>
coordT square<coordT>::ymin = 0;

template <class coordT>
coordT square<coordT>::xmax = 0;

template <class coordT>
coordT square<coordT>::ymax = 0;

// --------------------------------------------------

template <class coordT>
inline square<coordT>::square (coordT _x1, coordT _y1,
	coordT _x2, coordT _y2): x1 (_x1), y1 (_y1), x2 (_x2), y2 (_y2)
{
	if ((x1 >= x2) || (y1 >= y2))
		throw "Wrong square coordinate ranges.";
	if (xmin == xmax)
	{
		xmin = x1;
		ymin = y1;
		xmax = x2;
		ymax = y2;
	}
	else
	{
		if (xmin > x1)
			xmin = x1;
		if (ymin > y1)
			ymin = y1;
		if (xmax < x2)
			xmax = x2;
		if (ymax < y2)
			ymax = y2;
	}
	return;
} /* square<coordT>::square */


// --------------------------------------------------
// -------------------- CELL2BMP --------------------
// --------------------------------------------------

int cells2bitmap (char *names [], int namecount, char *outname, int x,
	int y, int resolution, int size, int pause, int margin, int grid,
	bool gray, int colorbar)
// Note: x >= 0, y >= 0 (command-line parameters minus 1).
// Returns: 0 = Ok, -1 = error (shows msg).
{
	// check if the values of parameters are sensible
	if (margin < 0)
		margin = 0;
	if ((x == y) || (x < 0) || (y < 0))
	{
		sout << "ERROR: Improper values of parameters: -x" <<
			(x + 1) << " -y" << (y + 1) << ".\n";
		throw "Unable to continue.";
	}
	if ((size <= 0) || (pause < 0))
	{
		sout << "ERROR: Improper values of parameters: -s" << size <<
			" -p" << pause << ".\n";
		throw "Unable to continue.";
	}

	// read a set of cells from a file to the corresponding squares
	sout << "Reading selected coordinates of squares to paint...\n";
	multitable<square<int> > squares;
	int nsquares = 0;
	int firstsquare [MAXNAMES + 1];
	int mindim = (x < y) ? (y + 1) : (x + 1);
	for (int i = 0; i < namecount; i ++)
	{
		if (i)
			sout << '(' << nsquares << ") ";
		sout << names [i] << ' ';
		firstsquare [i] = nsquares;
		std::ifstream f (names [i]);
		if (!f)
			fileerror (names [i]);
		int point1 [MAXDIM], point2 [MAXDIM];
		ignorecomments (f);
		while (f. peek () != EOF)
		{
			while ((f. peek () != EOF) &&
				(closingparenthesis (f. peek ()) == EOF))
			{
				ignoreline (f);
				ignorecomments (f);
			}
			if (f. peek () == EOF)
				break;
			int closing = closingparenthesis (f. get ());
			if (closing == EOF)
				throw "Opening parenthesis missing.";
			ignorecomments (f);
			int dim1 = readcoordinates (f, point1, MAXDIM);
			ignorecomments (f);
			int dim2 = readcoordinates (f, point2, MAXDIM);
			ignorecomments (f);
			if (f. get () != closing)
				throw "Wrong closing parenthesis of a cell.";
			ignorecomments (f);
			if ((dim1 >= MAXDIM) || (dim2 >= MAXDIM) ||
				(dim1 <= 0) || (dim2 <= 0))
				throw "Cannot read a cell.";
			if ((dim1 < mindim) || (dim2 < mindim))
				throw "Dimension of a cell too low.";
			int x1 = point1 [x] * (size + pause);
			int y1 = point1 [y] * (size + pause);
			int x2 = point2 [x] * (size + pause) - pause;
			int y2 = point2 [y] * (size + pause) - pause;
			squares [nsquares ++] = square<int> (x1, y1, x2, y2);
		}
	}
	sout << nsquares << " cells total.\n";
	firstsquare [namecount] = nsquares;

	if (!nsquares)
		throw "Can't read any cell from the input file(s).\n";

	sout << "There are " << nsquares << " squares to paint.\n";

	// add margins to the scope of squares
	square<int>::xmin -= margin * (size + pause);
	square<int>::ymin -= margin * (size + pause);
	square<int>::xmax += margin * (size + pause);
	square<int>::ymax += margin * (size + pause);

	// compute bitmap's width and height and check if they are appropriate
	int width = square<int>::xmax - square<int>::xmin;
	int height = square<int>::ymax - square<int>::ymin;
	int marg_width = margin * (size + pause);
	int main_width = width - 2 * marg_width;
	if ((width <= 0) || (height <= 0))
		throw "The ranges of coordinates are too large.\n";
	int vshift = 0;

	// determine the width and height of color bars
	// and adjust the size of the bitmap accordingly
	if (namecount <= 1)
		colorbar = 0;
	int bar_width = colorbar;
	int bar_height = colorbar;
	if (colorbar < 0)
	{
		bar_width = main_width / namecount;
		if (bar_width <= 0)
			bar_width = 1;
		bar_height = bar_width;
	}
	// set the position of the color bar
	int bar_left = marg_width;
	int bar_bottom = 0;
	// adjust the width of the picture with the color bar
	if (bar_width * namecount > main_width + marg_width)
		width += bar_width * namecount - main_width - marg_width;
	// adjust the height of the picture to cover the color bar
	if (colorbar)
	{
		height += 2 * bar_height;
		vshift += 2 * bar_height;
	}

	// create a bitmap image
	bmpfile bmp (height);
	// set the resolution of the image if desired
	if (resolution)
	{
		bmp. x_resolution = (long) (resolution / 0.0254);
		bmp. y_resolution = (long) (resolution / 0.0254);
	}
	// the vertical coordinate should go as in mathematics
	bmp. invertedpicture ();
	// prepare a 16-color palette to use if 2-15 sets are to plot
	int32 palette16 [] = {0x000000, 0x0000FF, 0xFF0000, 0x00FF00,
		0xFFFF00, 0xFF00FF, 0x00FFFF, 0x7F7F7F,
		0x00007F, 0x007F00, 0x7F0000, 0x7F7F00,
		0x7F007F, 0x007F7F, 0xCEB505, 0xFFFFFF};
	if (gray)
	{
		for (int i = 0; i < namecount; ++ i)
		{
			int shade = 0xFF * i / namecount;
			palette16 [i] = shade | (shade << 8) | (shade << 16);
		}
		for (int i = namecount; i < 16; ++ i)
			palette16 [i] = (i << 4) | (i << 12) || (i << 20);
		palette16 [14] = 0xCEB505;
		palette16 [15] = 0xFFFFFF;
	}
	// prepare a multi-color palette for more than 14 sets of points
	int32 palette256 [256];
	if (namecount >= 15)
	{
		if (gray)
		{
			for (int i = 0; i < namecount; ++ i)
			{
				int shade = 0xFF * i / namecount;
				palette256 [i] = shade | (shade << 8) |
					(shade << 16);
			}
		}
		else
		{
			int nshades = (namecount - 7 + 5) / 6 + 1;
			int palnamecount = (namecount + 5) / 6 * 6;
			for (int i = 0; i < namecount; ++ i)
			{
				int color = 0;
				int shade = 0xFF;
				if (i > 6)
				{
					int n = (palnamecount - (i - 6)) / 6;
					shade = n * 0xFF / nshades;
				}
				int which = i ? ((i - 1) % 6 + 1) : 0;
				if (which & 0x01)
					color |= shade;
				if (which & 0x02)
					color |= shade << 8;
				if (which & 0x04)
					color |= shade << 16;
				palette256 [i] = color;
			}
		}
		for (int i = namecount; i < 256; ++ i)
			palette256 [i] = i | (i << 8) | (i << 16);
		palette256 [254] = 0xCEB505;
		palette256 [255] = 0xFFFFFF;
	}

	// create a disk file containing the picture
	if (namecount == 1)
		if (bmp. create (outname, width, height, 1) < 0)
			return -1;
		else;
	else if (namecount < 15)
		if (bmp. create (outname, width, height, 4, 16,
			palette16) < 0)
			return -1;
		else;
	else
		if (bmp. create (outname, width, height, 8, 256,
			palette256) < 0)
			return -1;

	// plot the points
	sout << "Plotting the squares...\n";
	int xsub = square<int>::xmin;
	int ysub = square<int>::ymin;
	for (int i = 0; i < namecount; i ++)
	{
		for (int s = firstsquare [i]; s < firstsquare [i + 1]; s ++)
		{
			const square<int> &sq = squares [s];
			for (int i_y = sq. y1; i_y < sq. y2; i_y ++)
			{
				for (int i_x = sq. x1; i_x < sq. x2; i_x ++)
					bmp. putpixel (i_x - xsub,
						vshift + i_y - ysub,
						i, namecount > 1);
			}
		}
	}

	if (grid)
	{
		sout << "\nWARNING: Grid plotting not supported!!!\n\n";
	}

	if (colorbar)
	{
		for (int col = 0; col < namecount; ++ col)
		{
			int offset = bar_left + col * bar_width;
			for (int i = offset; i < offset + bar_width; ++ i)
			{
				for (int j = bar_bottom; j < bar_bottom +
					bar_height; ++ j)
				{
					bmp. putpixel (i, j, col,
						namecount > 1);
				}
			}
		}
	}
	
	sout << "Done!\n";
	return 0;
} /* pointset to bitmap */


// --------------------------------------------------
// ---------------------- MAIN ----------------------
// --------------------------------------------------

int main (int argc, char *argv [])
// Return: 0 = Ok, -1 = Error, 1 = Help displayed, 2 = Wrong arguments.
{
	// prepare user-configurable data
	char *names [MAXNAMES + 1], *outname = NULL;
	int namecount = 0;
	// numbers of coordinates
	int x = 1, y = 2;
	// width and height of squares, and space between them
	int resolution = 0, size = 1, pause = 0, grid = 0, margin = 0;
	bool gray = false;
	int colorbar = 0;

	// analyze the command line
	arguments a;
	arg (a, NULL, names, namecount, MAXNAMES + 1);
	arg (a, "x", x);
	arg (a, "y", y);
	arg (a, "s", size);
	arg (a, "r", resolution);
	arg (a, "p", pause);
	arg (a, "m", margin);
	arg (a, "g", grid);
	arg (a, "b", colorbar, -1);
	argswitch (a, "-gray", gray, true);
	argswitch (a, "-grey", gray, true);
	arghelp (a);

	argstreamprepare (a);
	int argresult = a. analyze (argc, argv);
	argstreamset ();

	// show the program's main title
	if (argresult >= 0)
		sout << title << '\n';

	// extract the output file name
	if (namecount > 1)
		outname = names [-- namecount];

	// if something was incorrect, show an additional message and exit
	if (argresult < 0)
	{
		sout << "Call with '--help' for help.\n";
		return 2;
	}

	// if help requested or no filenames present, show help information
	if (!outname || (argresult > 0))
	{
		sout << helpinfo << '\n';
		return 1;
	}

	// try running the main function and catch an error message if thrown
	try
	{
		cells2bitmap (names, namecount, outname, x - 1, y - 1,
			resolution, size, pause, margin, grid, gray,
			colorbar);
		program_time = 1;
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
} /* main */

/// @}

