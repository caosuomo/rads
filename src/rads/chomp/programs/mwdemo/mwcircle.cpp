/////////////////////////////////////////////////////////////////////////////
///
/// @file mwcircle.cpp
///
/// @author Pawel Pilarczyk
///
/////////////////////////////////////////////////////////////////////////////

// Copyright (C) 1997-2010 by Pawel Pilarczyk.
//
// This file is part of my research program package.  This is free software;
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

// Started on March 11, 2006. Last revision: December 12, 2007.


// --------------------------------------------------
// ------------ programming instructions ------------
// --------------------------------------------------
/*
	In order to prepare your own program based on this one,
	please, follow the following steps:

	1. Copy this file to a file with a different name and update the
	line "@file" at the top of this file. Keep the original file
	for reference. Note that you may need to add your file to the
	list of "PROGS" in the "makefile".

	2. Replace my name in the "author" and "copyright" lines at the top
	of this file, as well as in the "title" string below.

	3. Adjust the contents of "title" and "helpinfo" below so that
	the program displays information relevant to your computations
	(including the program title, version number, and explanations).

	4. Replace the code in the function "compute" below with yours.
	This is the function which processes a single hypercube.

	5. Modify the constants "dim", "left", and "right" with the
	dimension of the hypercube and the coordinates of its minimal
	(left) vertex and maximal (right) vertex.

	6. Update or remove the line "Started on... Last revision: ..."
	above.

	7. Note that if you publish your program, you must do it under the
	terms of the GNU General Public Licence, with its source code.

	Enjoy!
*/

// --------------------------------------------------
// --------------- usage instructions ---------------
// --------------------------------------------------
/*
	The following lines explain how to run the program.

	1. To run the computations on a single computer,
	type the following (initial level 3, maximal level 6):

		mwcircle -i 3 -m 6 -f results.txt

	2. If you want other computers to join the computations,
	prepare the following script in the file "runscript"
	(replace "host" with the computer name on which the
	previous command is running):

		#!/bin/tcsh
		mwcircle -w host >& /dev/null < /dev/null &

	and then run the following command:

		ssh computer_name runscript

	Note that you may want to suppress running the computations
	by the main (coordinating) process to avoid delays; if this
	is the case, add the switch "-c" to the command line, i.e.:

		mwcircle -c -i 3 -m 6 -f results.txt

	3. To interrupt the computations before the maximal subdivision
	level is reached, kill the process started in (1). Note that you
	must edit the file "results.txt" and delete any unfinished lines
	(to make sure every line is complete, you can just delete the last
	line), unless you added the "--flush" flag in (1), but note
	that this flag may slow down the computations if each data chunk
	computes within short time.

	4. To keep the workers waiting after the main program has finished,
	add the flags "-k -r -s" in (1). The list of workers is saved
	to the file "mwsubdiv.txt", and retrieved at the next run.
	Note that you can quit all the waiting workers listed in the
	"results.txt" file by invoking the command
	
		mwcircle -r -q

	5. If you need assistance with using this software, you are welcome
	to contact the program's author at http://www.PawelPilarczyk.com/.
*/

// --------------------------------------------------
// ---------- the syntax of the text data -----------
// --------------------------------------------------
/*
	The way in which the data is encoded in the results file
	is described below.

	1. The syntax of lines in the text data file:

	Each line begins with a special character that indicates
	the type of the information contained in each line:

	a) Data sent for processing: '+' followed by the number
	of the data chunk sent in the program's run, and then
	by the Cartesian product of intervals sent for processing.
	Note that the intervals displayed are rounded to a few
	significant digits only.

	b) Result received from processing: '*' or '@' followed by the
	number of the data chunk, then the obtained result,
	then the level of subdivisions, and the integral coordinates
	of the minimal vertex of the cube to which this result corresponds.
	'*' indicates a full box, '@' indicates a probe.

	An example of data that can appear in this file:

		+ 25 [16,32]x[-16,0].
		* 25 0 3 (5,3).

	The meaning of the lines:
		- the box [16,32] x [-16,0] was sent for processing; the
		  number of the data chunk corresponding to this box is 25
		- a result of processing the above-mentioned data chunk
		  (no. 25) has been received from processing; the result
		  of the processing is 0, and the box corresponds to the
		  cube [5,6] x [3,5] at the third subdivision level


	2. The syntax of lines displayed on the terminal by the coordinator:

	a) '+' followed by an integral number and the dot: The data chunk
	with this number has been prepared to be sent for processing.

	b) '*' followed by two integral numbers and the dot: The data chunk
	with the first number has been received from processing, and the
	second number shows the obtained result.

	c) '!' followed by a message: An error occurred. The message
	explains what happened.

	
	3. The syntax of lines displayed on the terminal by a worker,
	or by the coordinator if the data is processed locally:

	a) '-' followed by an integral number and the dot: The data chunk
	with this number has been received for processing.

	b) '=' followed by two integral numbers and the dot: The data chunk
	with the first number has been processed, and the second number
	indicates the result.

	c) '!' followed by a message: An error occurred and the data has
	been rejected. The message explains what happened.
*/


#include "chomp/multiwork/mwsubdiv.h"

#if defined (__WIN32__)
#include <windows.h> // for "Sleep (milliseconds)"
#else
#include <unistd.h> // for "usleep (microseconds)"
#include <time.h> // for "nanosleep"
#endif


// --------------------------------------------------
// -------------- title and help info ---------------
// --------------------------------------------------

const char *title = "\
MWcircle, ver. 0.01. Copyright (C) 1997-2010 by Pawel Pilarczyk.\n\
This is free software. No warranty. Consult 'license.txt' for details.";

const char *helpinfo = "\
This is a demonstration program for running distributed computations\n\
based on recursive subdivisions of a hypercube.\n\
Please, consult the source code for details on how to use\n\
this program for your tasks.";


// --------------------------------------------------
// --------------- Multi-Work options ---------------
// --------------------------------------------------

/// The default port number for the multiwork communication.
/// Use a TCP port available on your computer. Note that
/// numbers below 1024 are reserved for the system. Suggested
/// value is between 4000 and 5000, but anything above 1024
/// and below some 50000 should be fine.
const int defaultPortNumber = 4811;

/// The control number used to confirm that the worker and the
/// coordinator are doing the same computations.
const int controlNumber = 30303;


// --------------------------------------------------
// ------------ the range of parameters -------------
// --------------------------------------------------

/// The dimension of the parameter space. Must not exceed 30.
const int dim = 2;

/// The leftmost corner of the entire range of parameters to scan.
const double leftBounds [dim] = {-64, -64};

/// The rightmost corner of the entire range of parameters to scan.
const double rightBounds [dim] = {64, 64};


// --------------------------------------------------
// ---------------- one computation -----------------
// --------------------------------------------------

/// This is the procedure which performs one computation for the given box.
/// Returns: >0 if the property holds true, 0 if this verification could not
/// prove that the property holds true, <0 if an error occurred.
int computeFunction (const double *left, const double *right,
	int dim, int level)
{
	if (dim > 32)
		return -1;
	if (!level)
		return 0;

#if defined (__WIN32__)
	Sleep (100);
#else
	usleep (100000);

//	struct timespec waiting;
//	waiting. tv_sec = 0;
//	waiting. tv_nsec = 100000000l;
//	nanosleep (&waiting, 0);
#endif

	double center = 0;
	double innerRadius2 = 500;
	double outerRadius2 = 3400;

	int mincoord [32];
	int maxcoord [32];
	for (int i = 0; i < dim; ++ i)
	{
		mincoord [i] = 0;
		maxcoord [i] = 2;
	}
	chomp::homology::tRectangle<int> r (mincoord, maxcoord, dim);
	const int *c;
	while ((c = r. get ()) != 0)
	{
		double r2 = 0;
		for (int i = 0; i < dim; ++ i)
		{
			double x = (c [i] ? left [i] : right [i]) - center;
			r2 += x * x;
		}
		if ((r2 < innerRadius2) || (r2 > outerRadius2))
			return 0;
	}
	return 1;
} /* computeFunction */


// --------------------------------------------------
// ---------------------- MAIN ----------------------
// --------------------------------------------------

int main (int argc, char *argv [])
{
	// call the actual "main" function
	return chomp::multiwork::mwSubdivMain<dim,int> (argc, argv,
		title, helpinfo, defaultPortNumber, controlNumber,
		leftBounds, rightBounds, 2, computeFunction);
} /* main */

