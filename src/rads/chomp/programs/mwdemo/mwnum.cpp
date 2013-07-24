/////////////////////////////////////////////////////////////////////////////
///
/// @file mwnum.cpp
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

// Started on August 10, 2004. Last revision: November 29, 2007.


// --------------------------------------------------
// ------------------ header files ------------------
// --------------------------------------------------

// the multi-work library header
#include "chomp/multiwork/mw.h"
using namespace chomp::multiwork;

// various system headers
#if defined (__WIN32__)
#include <windows.h> // for "Sleep"
#endif

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <unistd.h> // for "sleep"
#include <cstdlib> // for "atoi" and "rand"


// --------------------------------------------------
// ------------- title and information --------------
// --------------------------------------------------

static const char *title = "\
Multi-Work demo: Numbers. Copyright (C) 1997-2010 by Pawel Pilarczyk.\n\
This is free software. No warranty. Consult 'license.txt' for details.";

static const char *helpinfo = "\
This program is a simple demonstration of the Multi-Work interface.\n\
To see the effects, run one copy of this program on your computer,\n\
and then run sevral other copies with the arguments '-w localhost'.\n\
You can also run the other copies on different machines, and then replace\n\
'localhost' with the name of the computer running the first program.\n\
The first program acts as a coordinator who sends numbers to workers,\n\
and the other programs act as workers who process the numbers and send\n\
back their answers.\n\
Command line arguments:\n\
-w - run as a worker (by default the program runs as a coordinator),\n\
-c - run as a coordinator only (don't process any data locally),\n\
-k - keep workers running and waiting when the computations are done,\n\
-fNUM - set the first number to be used in the test,\n\
-mNUM - set the maximal number to be used in the test,\n\
-nNUM - set the number to reject all numbers divisible by this number,\n\
-oN - set the connection time-out to N seconds,\n\
-pN - set the port number to N (use 0 to reject incoming connections),\n\
-lFILE - log communication details to this file,\n\
computer:port - use this connection at start-up (can be repeated),\n\
-sFILE - save the workers' list to this file,\n\
-rFILE - retrieve the workers' list from this file,\n\
-q - quit all the workers who are waiting\n\
-h - show this brief help information and exit.\n\
For more details ask the author at http://www.PawelPilarczyk.com/.";


// --------------------------------------------------
// ---------------- the worker class ----------------
// --------------------------------------------------

class Worker: public mwWorker
{
public:
	/// The default constructor.
	Worker ();

private:
	/// A function for initializing a worker.
	int Initialize (mwData &x);

	/// A function for processing the data as a worker.
	int Process (mwData &x);

	/// A lucky number in the possession of the worker.
	int luckynumber;

}; /* class Worker */

// --------------------------------------------------

Worker::Worker (): luckynumber (5)
{
	return;
} /* Worker::Worker */

// --------------------------------------------------

int Worker::Initialize (mwData &x)
{
	x >> luckynumber;
	if (luckynumber > 0)
	{
		std::cout << "Initializing the worker to reject numbers "
			"which divide by " << luckynumber << ".\n";
		return mwOk;
	}
	else
	{
		std::cout << "Incorrect initialization number received.\n";
		return mwError;
	}
} /* Worker::Initialize */

int Worker::Process (mwData &x)
{
	// retrieve the number sent and make it non-negative
	int nr = 0;
	x >> nr;
	if (nr < 0)
		nr = -nr;

	// select an arbitrary sleep time depending on the data
	int sleeptime = (rand () % (700 + nr)) % 9 + 1;

	// say that the data is being processed
	std::cout << "Processing " << nr << " (" << sleeptime << " sec)... " <<
		std::flush;
		
	// process the data (only sleep a few seconds in this example)
	#if defined (__WIN32__)
	Sleep (1000 * sleeptime);
	#else
	sleep (sleeptime);
	#endif
	
	// if the result should be rejected, return the rejection code
	if (!(nr % luckynumber))
	{
		std::cout << "Divides by " << luckynumber <<
			". Rejecting." << std::endl;
		return mwReject;
	}

	// send back the data containing the result of the processing
	nr = -nr;
	std::cout << "Sending back " << nr << "." << std::endl;
	x. Reset ();
	x << nr;
	return mwOk;
} /* Worker::Process */


// --------------------------------------------------
// ------------- the coordinator class --------------
// --------------------------------------------------

class Coordinator: public mwCoordinator
{
public:
	/// The default constructor.
	Coordinator ();

	/// The first number to send.
	int first;

	/// The number beyond the last one to send.
	int maximal;

private:
	/// A function for preparing data by the coordinator.
	int Prepare (mwData &data);

	/// A function for accepting results by the coordinator.
	int Accept (mwData &data);

	/// A function for taking rejected data by the coordinator.
	int Reject (mwData &data);

	/// The number to be sent by the coordinator.
	int current;

}; /* class Coordinator */

// --------------------------------------------------

Coordinator::Coordinator ()
{
	current = -1;
	first = 1;
	maximal = 14;
	return;
} /* Coordinator::Coordinator */

// --------------------------------------------------

int Coordinator::Prepare (mwData &data)
{
	// initialize the current number if this is the first run
	if (current < 0)
		current = first;

	// if all the numbers have been sent already, say that
	if (current > maximal)
		return mwNoData;

	// send the current number and increase it
	std::cout << "Sending number " << current << "." << std::endl;
	data << current ++;

	return mwOk;
} /* Coordinator::Prepare */

int Coordinator::Accept (mwData &data)
{
	// decode the number
	int n = 0;
	data >> n;

	// say what was received
	std::cout << "Received number " << n << "." << std::endl;

	return mwOk;
} /* Coordinator::Accept */

int Coordinator::Reject (mwData &data)
{
	// decode the number
	int n = 0;
	data >> n;

	// say what was received
	std::cout << "This number was rejected: " << n << "." << std::endl;

	return mwOk;
} /* Coordinator::Reject */


// --------------------------------------------------
// ---------------------- main ----------------------
// --------------------------------------------------

int main (int argc, char *argv [])
{
	// show the program's title
	std::cout << title << std::endl;

	// create one instance of the task class
	Worker worker;
	Coordinator coordinator;
	bool localWork = true;

	// analyze the command-line arguments
	bool work = false;
	char *savename = 0;
	bool quit = false;
	int luckynumber = 7;
	for (int i = 1; i < argc; ++ i)
	{
		// if this argument does not begin with '-'...
		if (argv [i] [0] != '-')
		{
			worker. Add (argv [i]);
			coordinator. Add (argv [i]);
			continue;
		}

		// if the argument is just '-' then ignore it
		if (!argv [i] [1])
		{
			continue;
		}

		// determine the right option name and its value
		char option = argv [i] [1];
		char *value = argv [i] + 2;
		if ((option != 'w') && (option != 'c') && (option != 'k') &&
			 (option != 'q') && !*value && (i + 1 < argc) &&
			(argv [i + 1] [0] != '-'))
		{
			value = argv [++ i];
		}

		// interprete the option and its value
		switch (option)
		{
		case 'w':
			work = true;
			break;
		case 'c':
			localWork = false;
			break;
		case 'l':
			coordinator. LogFile (value);
			worker. LogFile (coordinator);
			break;
		case 'p':
			coordinator. Port (atoi (value));
			worker. Port (atoi (value));
			break;
		case 'o':
			coordinator. TimeOut (atoi (value));
			worker. TimeOut (atoi (value));
			break;
		case 'k':
			if (*value == '0')
				coordinator. KeepWorkers (false);
			else
				coordinator. KeepWorkers (true);
			break;
		case 's':
			savename = value;
			break;
		case 'r':
			coordinator. Load (value);
			break;
		case 'f':
			coordinator. first = atoi (value);
			if (coordinator. first < 0)
				coordinator. first = 0;
			break;
		case 'm':
			coordinator. maximal = atoi (value);
			break;
		case 'n':
			luckynumber = atoi (value);
			break;
		case 'q':
			quit = true;
			break;
		case '-':
		case 'h':
		case '?':
			std::cout << helpinfo << std::endl;
			return 0;
		default:
			break;
		}
	}

	// quit all the workers if this is the only thing requested
	if (quit)
	{
		coordinator. QuitWorkers ();
	}

	// run as a coordinator if requested to
	else if (!work)
	{
		// send some initialization data to all the workers
		std::cout << "Running as a coordinator..." << std::endl;
		mwData initData;
		initData << luckynumber;
		coordinator. Init (initData);
		int result = coordinator. Coordinate
			(localWork ? &worker : 0);
		if (result == mwOk)
			std::cout << "Done." << std::endl;
		else
			std::cout << "The computations could not be "
				"completed." << std::endl;

		// save the workers and their port numbers for future use
		if (savename)
			coordinator. SaveWorkers (savename);
	}

	// run as a worker otherwise
	else
	{
		std::cout << "Running as a worker..." << std::endl;
		int result = worker. Work ();
		if (result == mwOk)
			std::cout << "Done." << std::endl;
		else
			std::cout << "Could not work - probably an error "
				"occurred." << std::endl;
	}

	return 0;	
} /* main */

