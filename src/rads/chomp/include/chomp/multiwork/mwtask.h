/// @addtogroup multiwork
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file multiwork/mwtask.h
///
/// This file contains the definition of the MultiWork task class.
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

// Started on August 11, 2004. Last revision: November 29, 2007.


#ifndef _CHOMP_MULTIWORK_MWTASK_H_
#define _CHOMP_MULTIWORK_MWTASK_H_

#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> // for "atoi"
#include <ctime> // for log start/stop time

#include "chomp/multiwork/mwconfig.h"
#include "chomp/multiwork/mwlowlev.h"
#include "chomp/multiwork/mwdata.h"


namespace chomp {
namespace multiwork {

// --------------------------------------------------
// ----------------- message codes ------------------
// --------------------------------------------------

/// Various message codes used for the communication between the coordinator
/// and workers.
enum mwCodes
{
	/// Message: No special code information included.
	mwNoMsg = 0x0000,

	/// Message to the Worker: A piece of initialization data
	/// to be processed.
	mwInitMsg = 0x0001,

	/// Message to the Worker: A standard piece of data to be processed.
	mwStdMsg = 0x0002,

	/// Message to the Worker: Keep running after having disconnected.
	mwKeepMsg = 0x0004,

	/// Message to the Worker: Don't keep running after disconnecting.
	mwDontKeepMsg = 0x0008,

	/// Message to the Worker: Please, disconnect.
	mwByeMsg = 0x0010,

	/// Message to the Coordinator: The data has been rejected.
	mwRejectedMsg = 0x0001,

	/// Message to the Coordinator: I will listen at this port number.
	mwPortMsg = 0x0002

}; /* enum mwCodes */


// --------------------------------------------------
// --------------------- mwTask ---------------------
// --------------------------------------------------

/// This class defines a generic task object (coordinator or worker)
/// for the multi-work distributed computations framework.
/// It is the common part of a worker task and a coordinator
/// task, and contains some general settings that apply to both.
/// Note that this class is inherited by the worker and the coordinator
/// class in the virtual mode, which allows one to create a class that is
/// both a worker and a coordinator at the same time (although this is not
/// recommended in general).
class mwTask
{
public:
	/// The default constructor.
	mwTask ();
		
	/// The destructor.
	virtual ~mwTask ();

	// --- configuration ---

	/// Sets the port number for the communication or 0 to use none.
	void Port (int number);

	/// Returns the current port number.
	int Port () const;

	/// Sets the control number for identification.
	void ControlNumber (unsigned int number);

	/// Returns the currently set identification control number.
	unsigned int ControlNumber () const;

	/// Sets the network connection time-out interval in seconds.
	void TimeOut (int seconds);

	/// Returns the currently set network connection time-out interval.
	int TimeOut () const;

	// --- log file ---

	/// Begins logging detailed communication debug information
	/// to the given file.
	/// Returns mwOk on success, mwError if cannot open/create the file.
	int LogFile (const char *filename);

	/// Uses another task's log file to log this task's information.
	void LogFile (const mwTask &other);

	/// Closes the log file and adds a line with the time information
	/// unless this log file was borrowed from another task.
	void LogClose ();

	// --- computer addresses ---

	/// Adds an address to the list of computers to connect to
	/// at the beginning of working or coordinating.
	/// The addresses must be in the form "computer.domain:port".
	/// If port is not defined then the default port number is used.
	int Add (const char *name, int port = -1);
	
	/// Loads computer addresses from the given file.
	/// Returns the number of acquired addresses or mwError.
	int Load (const char *filename);
	
	// --- quit waiting workers ---

	/// Quits all the workers whose addresses were added with the 'Add'
	/// and 'Load' functions.
	/// Returns mwOk or mwError.
	int QuitWorkers ();

private:
	// --- configuration data ---
	
	/// The network communication port number.
	int portnum;

	/// The control number that is used to recognize a compatible worker
	/// or a compatible coordinator. Note: The worker sends its negation.
	unsigned int ctrlnum;

	/// The network communication time-out in seconds.
	int timeout;

	// --- log file ---

protected:
	/// The debug log file stream.
	std::ofstream *logFile;

private:
	/// Is this log file pointer borrowed from another task?
	bool logBorrowed;

protected:
	// --- network communication ---

	/// Sends a message with data to the given socket.
	/// The message includes a control number.
	/// Returns mwOk on success and mwError in the case of failure.
	static int SendMessage (int fd, unsigned int ctrl,
		unsigned int code, const mwData &x);

	/// Receives a message with data from the given socket.
	/// Verifies the received control code if it is correct.
	/// Returns mwOk on success or mwError in the case of failure.
	static int RecvMessage (int fd, unsigned int &ctrl,
		unsigned int &code, mwData &x);

	// --- computer addresses ---

	/// A list of workers or coordinators to connect to at start-up.
	std::vector<std::string> computers;

	/// A list of port numbers of workers to connect to at start-up.
	std::vector<int> ports;

private:
	// --- other stuff ---

	/// The copy constructor is forbidden.
	mwTask (const mwTask &) {}
		
	/// The assignment operator is forbidden.
	mwTask &operator = (const mwTask &) {return *this;}
		
}; /* class mwTask */

// --------------------------------------------------

inline mwTask::mwTask ():
	portnum (mwPORT),
	ctrlnum (mwCTRLNUM),
	timeout (mwTIMEOUT),
	logFile (0),
	logBorrowed (false)
{
	return;
} /* mwTask::mwTask */

// --------------------------------------------------

inline void mwTask::LogClose ()
{
	if (!logFile)
		return;
	if (logBorrowed)
	{
		logFile = 0;
		logBorrowed = false;
		return;
	}
	std::time_t stop_time;
	std::time (&stop_time);
	*logFile << "\nMultiWork log file closed on " <<
		std::asctime (std::localtime (&stop_time)) << "\n"
		"-----------------------------------------------------\n" <<
		std::endl;
	delete logFile;
	logFile = 0;
	return;
} /* mwTask::LogClose */

inline int mwTask::LogFile (const char *filename)
{
	// close the current log if in use
	if (logFile)
		LogClose ();

	// if no file name supplied, return now
	if (!filename || !*filename)
		return mwOk;

	// create a file stream variable
	logFile = new std::ofstream;
	if (!logFile)
		return mwError;

	// open the log file for appending
	logFile -> open (filename, std::ios::app);

	// write the current time to the log
	std::time_t start_time;
	std::time (&start_time);
	*logFile << "MultiWork log file opened on " <<
		std::asctime (std::localtime (&start_time)) << std::endl;

	// if unable to open the file or an error occurred, return mwError
	if (!*logFile)
	{
		delete logFile;
		return mwError;
	}

	return mwOk;
} /* mwTask::LogFile */

inline void mwTask::LogFile (const mwTask &other)
{
	// close the current log if in use
	LogClose ();

	// if there is no other log file, do nothing
	if (!other. logFile)
		return;

	// borrow the log file pointer
	logBorrowed = true;
	logFile = other. logFile;

	return;
} /* mwTask::LogFile */

// --------------------------------------------------

inline mwTask::~mwTask ()
{
	// close the log file if it was in use
	LogClose ();

	return;
} /* mwTask::~mwTask */

// --------------------------------------------------

inline void mwTask::Port (int number)
{
	if (number >= 0)
		portnum = static_cast<short int> (number);
	return;
} /* mwTask::Port */

inline int mwTask::Port () const
{
	return portnum;
} /* mwTask::Port */

inline void mwTask::ControlNumber (unsigned int number)
{
	ctrlnum = number;
	return;
} /* mwTask::ControlNumber */

inline unsigned int mwTask::ControlNumber () const
{
	return ctrlnum;
} /* mwTask::ControlNumber */

inline void mwTask::TimeOut (int seconds)
{
	timeout = seconds;
	return;
} /* mwTask::TimeOut */

inline int mwTask::TimeOut () const
{
	return timeout;
} /* mwTask::TimeOut */

// --------------------------------------------------

inline int mwTask::Add (const char *name, int port)
{
	// if the name is empty, ignore it
	if (!name || !*name)
		return mwError;

	// determine whether the name contains a colon and port number
	int pos = 1;
	while (name [pos])
		++ pos;
	-- pos;
	while (pos && (name [pos] != ':') &&
		(name [pos] >= '0') && (name [pos] <= '9'))
	{
		-- pos;
	}

	// if the name contains colon and some digits after the colon...
	if (pos && (name [pos] == ':') && name [pos + 1])
	{
		// append the computer name and the chosen port number
		char *compname = new char [pos + 1];
		for (int i = 0; i < pos; ++ i)
			compname [i] = name [i];
		compname [pos] = '\0';
		port = std::atoi (name + pos + 1);
		if (port <= 0)
			return mwError;
		computers. push_back (std::string (compname));
		ports. push_back (port);
		delete [] compname;
	}
	else
	{
		// if the port number is not reasonable, use the default one
		if (port <= 0)
			port = portnum;
		if (port <= 0)
			return mwError;
		computers. push_back (std::string (name));
		ports. push_back (port);
	}
	return mwOk;
} /* mwTask::Add */

inline int mwTask::Load (const char *filename)
{
	std::ifstream f (filename);
	if (!f)
		return mwError;

	char buf [512];
	int counter = 0;
	while (1)
	{
		*buf = '\0';
		f. getline (buf, 512, '\n');
		if ((*buf == ';') || (*buf == '#') || (*buf == '/'))
			continue;
		if (*buf)
		{
			int result = this -> Add (buf, portnum);
			if (result == mwOk)
				++ counter;
		}
		if (!f)
			return counter;
	}
} /* mwTask::Load */


// --------------------------------------------------
// -------------------- messages --------------------
// --------------------------------------------------

/// Sends a message to the given socket.
/// Returns mwOk, mwError or mwLost.
inline int mwTask::SendMessage (int fd, unsigned int ctrl,
	unsigned int code, const mwData &x)
{
	mwData sending;
	sending << ctrl << code << x. Length () << x;
	return mwSendBytes (fd, sending. Buffer (), sending. Length ());
} /* SendMessage */

/// Receives a message from the given socket.
/// Returns mwOk, mwError or mwLost.
inline int mwTask::RecvMessage (int fd, unsigned int &ctrl,
	unsigned int &code, mwData &x)
{
	// read the code and length of the message
	char buf00 [12];
	int result = mwRecvBytes (fd, buf00, 12);
	if (result < 0)
		return result;
	unsigned char *buf0 = reinterpret_cast<unsigned char *> (buf00);

	// extract the length of the message
	int len = (int) (buf0 [8]) << 24;
	len |= (int) (buf0 [9]) << 16;
	len |= (int) (buf0 [10]) << 8;
	len |= (int) (buf0 [11]);
	if (len < 0)
		return mwError;

	// extract the control code of the message
	ctrl = (int) (buf0 [0]) << 24;
	ctrl |= (int) (buf0 [1]) << 16;
	ctrl |= (int) (buf0 [2]) << 8;
	ctrl |= (int) (buf0 [3]);

	// extract the code of the message
	code = (int) (buf0 [4]) << 24;
	code |= (int) (buf0 [5]) << 16;
	code |= (int) (buf0 [6]) << 8;
	code |= (int) (buf0 [7]);

	// if the message length is zero, no more reading is necessary
	if (!len)
	{
		x. Reset ();
		return mwOk;
	}

	// prepare a memory buffer for the message
	char *buf = new char [len];
	if (!buf)
		return mwError;

	// read the message
	result = mwRecvBytes (fd, buf, len);
	if (result < 0)
	{
		delete [] buf;
		return result;
	}

	// transform the message to mw data
	x. Take (buf, len);
	return mwOk;
} /* RecvMessage */

inline int mwTask::QuitWorkers ()
{
	// write to the log file what you are doing
	if (logFile)
		*logFile << "Turning workers off..." << std::endl;
	
	// try connecting to each worker and ask them to exit
	int counter = 0;
	for (unsigned int n = 0; n < computers. size (); ++ n)
	{
		// retrieve the computer name and port from the list
		const char *name = computers [n]. c_str ();
		int port = ports [n];

		// if no valid name or port number read, skip this item
		if (!*name || !port)
			continue;

		// try connecting to the computer
		int fd = mwConnect (name, port);

		// if unsuccessful, make a note and take another one
		if (fd < 0)
		{
			if (logFile)
			{
				*logFile << "Worker " << name << ":" <<
					port << " could not be contacted." <<
					std::endl;
			}
			continue;
		}
		
		// prepare the control code to send to the worker
		unsigned int code = mwByeMsg | mwDontKeepMsg;

		// send the 'Bye!' message to the worker and disconnect it
		mwData empty;
		int result = this -> SendMessage (fd, ctrlnum, code, empty);
		mwDisconnect (fd);

		// add an appropriate message to the log file
		if (result == mwOk)
		{
			++ counter;
			if (logFile)
			{
				*logFile << "Worker " << name << ":" <<
					port << " exited successfully." <<
					std::endl;
			}
		}
		else if (logFile)
		{
			*logFile << "Error while sending the disconnect "
				"message to " << name << ":" << port <<
				"." << std::endl;
		}
	}

	// write to the log file how many workers were turned off
	if (logFile)
		*logFile << counter << " worker(s) have been shut down." <<
			std::endl;

	return mwOk;
} /* mwTask::QuitWorkers */


} // namespace multiwork
} // namespace chomp

#endif // _CHOMP_MULTIWORK_MWTASK_H_

/// @}

