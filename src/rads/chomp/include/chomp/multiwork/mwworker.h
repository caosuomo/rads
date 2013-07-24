/// @addtogroup multiwork
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file multiwork/mwworker.h
///
/// This file contains the definition of the MultiWork worker class.
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

// Started on August 11, 2004. Last revision: December 3, 2010.


#ifndef _CHOMP_MULTIWORK_MWWORKER_H_
#define _CHOMP_MULTIWORK_MWWORKER_H_

#include "chomp/multiwork/mwconfig.h"
#include "chomp/multiwork/mwlowlev.h"
#include "chomp/multiwork/mwdata.h"
#include "chomp/multiwork/mwtask.h"

namespace chomp {
namespace multiwork {

class mwCoordinator;


// --------------------------------------------------
// -------------------- mwWorker --------------------
// --------------------------------------------------

/// This class defines a generic worker task object for the multi-work
/// distributed computations framework.
/// Each worker class defined by the user must inherit from this class.
class mwWorker: public virtual mwTask
{
public:
	/// The default constructor.
	mwWorker ();
		
	/// The destructor.
	virtual ~mwWorker ();

	/// Runs the worker on this computer.
	/// Returns mwOk or mwError.
	int Work ();
		
	/// Makes the worker keep running after the coordinator
	/// has disconnected. Otherwise, the worker quits in that situation.
	void KeepWorker (bool keep = true);

private:
	// --- worker's job ---

	/// This function is called to process a data portion
	/// and replace it with the result of computation.
	/// It should return mwOk on success, mwReject to reject the data,
	/// or mwError in case of error to quit the program.
	/// Please, overload this function in the class you derive
	/// from the class 'mwWorker' and program it with the actual task.
	virtual int Process (mwData &data);

	/// This function is called to process initialization data, if any.
	/// This data is sent to a new worker immediately after
	/// the connection with the coordinator has been established.
	/// It should return mwOk on success or mwError in case of error
	/// to quit the program.
	/// Please, overload this function in the class you derive
	/// from the class 'mwWorker' and program it with the actual code.
	virtual int Initialize (mwData &data);

	/// Runs one working session after having connected to the
	/// coordinator at the socket identified by 'fd'.
	/// Returns mwOK if everything was fine or mwError if some error
	/// occurred and the worker should better exit.
	int WorkOne (int fd);

	// --- configuration ---

	/// Should the worker remain running after coordinator disconnects?
	bool keepWorker;

	// grant access to the protected functions and data to a coordinator
	friend class mwCoordinator;

	// --- network communiation ---

	/// Sends a message with data to the given socket as a worker.
	/// Returns mwOk on success and mwError in the case of failure.
	int SendMessageW (int fd, unsigned int code, const mwData &x) const;

	/// Receives a message with data from the socket of a coordinator.
	/// Returns mwOk on success or mwError in the case of failure.
	int RecvMessageW (int fd, unsigned int &code, mwData &x) const;

}; /* class mwWorker */

// --------------------------------------------------

inline mwWorker::mwWorker (): keepWorker (false)
{
	return;
} /* mwWorker::mwWorker */

inline mwWorker::~mwWorker ()
{
	return;
} /* mwWorker::~mwWorker */

// --------------------------------------------------

inline void mwWorker::KeepWorker (bool keep)
{
	keepWorker = keep;
	return;
} /* mwWorker::KeepWorkers */

// --------------------------------------------------

inline int mwWorker::Process (mwData &)
{
	return mwOk;
} /* mwWorker::Process */

inline int mwWorker::Initialize (mwData &)
{
	return mwOk;
} /* mwWorker::Initialize */

// --------------------------------------------------

inline int mwWorker::SendMessageW (int fd, unsigned int code,
	const mwData &x) const
{
	// prepare the negation of the control number to send
	unsigned int ctrl = ~(this -> ControlNumber ());

	// send the message with the control number and the message code
	return this -> SendMessage (fd, ctrl, code, x);
} /* mwWorker::SendMessageW */

inline int mwWorker::RecvMessageW (int fd, unsigned int &code,
	mwData &x) const
{
	// receive the message
	unsigned int ctrl = 0;
	int result = this -> RecvMessage (fd, ctrl, code, x);

	// if there was an error then return its code
	if (result != mwOk)
		return result;

	// if the control number is correct then finish successfully
	if (ctrl == this -> ControlNumber ())
		return mwOk;

	// if the control number is wrong then return an error code
	if (logFile)
		*logFile << "Wrong control code received "
			"from the coordinator: " << ctrl << "." << std::endl;
	return mwError;
} /* mwWorker::RecvMessageW */

// --------------------------------------------------

inline int mwWorker::WorkOne (int fd)
{
	// be ready to send the port number at which the worker is listening
	bool sendPort = (this -> Port () > 0);
	int timelimit = this -> TimeOut ();

	while (1)
	{
		// disconnect if the coordinator is not responding
		if (timelimit > 0)
		{
			// call the function 'select' to wait
			// until data is available for reading
			int portArray [1];
			portArray [0] = fd;
			int ioFlags [2];
			ioFlags [0] = mwCanRead;
			ioFlags [1] = mwNone;
			int result = mwSelect (portArray, 1, fd, ioFlags,
				timelimit);

			// disconnect in case of an I/O error
			if (result == mwError)
			{
				if (logFile)
					*logFile << "Error while "
						"waiting for data from the "
						"coordinator." << std::endl;
				return mwError;
			}

			// finish if disconnected
			if (result == mwLost)
			{
				if (logFile)
					*logFile << "Disconnected while "
						"waiting for data from the "
						"coordinator." << std::endl;
				return mwOk;
			}

			// finish if not responding
			if (result == mwTimeOut)
			{
				if (logFile)
					*logFile << "Time out reached while "
						"waiting for data from the "
						"coordinator." << std::endl;
				return mwOk;
			}
		}

		// receive a message
		mwData msg;
		unsigned int code = 0;
		int result = RecvMessageW (fd, code, msg);

		// disconnect in case of an I/O error
		if (result == mwError)
		{
			if (logFile)
				*logFile << "Error while receiving data "
					"from the coordinator." << std::endl;
			return mwError;
		}
	
		// disconnect if the connection was lost
		if (result == mwLost)
		{
			if (logFile)
				*logFile << "The connection closed "
					"by the coordinator." << std::endl;
			return mwOk;
		}

		// initialize the worker if requested to
		if (code & mwInitMsg)
		{
			if (logFile)
				*logFile << "Initializing the worker." <<
					std::endl;

			// update the 'keepWorker' flag if requested to
			if (code & mwKeepMsg)
				keepWorker = true;
			if (code & mwDontKeepMsg)
				keepWorker = false;

			// initialize the data
			int initResult = Initialize (msg);

			// if the initialization fails then quit the work
			if (initResult != mwOk)
			{
				if (logFile)
					*logFile << "The initialization "
						"failed." << std::endl;
				return mwError;
			}
		}

		// disconnect if requested to
		if (code & mwByeMsg)
		{
			// update the 'keepWorker' flag if requested to
			if (code & mwKeepMsg)
				keepWorker = true;
			if (code & mwDontKeepMsg)
				keepWorker = false;

			if (logFile)
				*logFile << "Disconnecting upon "
					"coordinator's request." <<
					std::endl;
			return mwOk;
		}

		// skip the rest if there is no standard message to process
		if (!(code & mwStdMsg))
			continue;

		// prepare a basis for the returned code
		unsigned int retcode = 0;
		
		// process the message data
		result = Process (msg);

		// if an error occurred, disconnect and quit
		if (result == mwError)
		{
			if (logFile)
				*logFile << "Data processing failed." <<
					std::endl;
			return mwError;
		}

		// if rejected, reset the data and set the rejection flag
		else if (result == mwReject)
		{
			if (logFile)
				*logFile << "* Data rejected." << std::endl;
			msg. Reset ();
			retcode |= mwRejectedMsg;
		}

		// if processed successfully, make a note of it
		else
		{
			if (logFile)
				*logFile << "* Data processed." << std::endl;
		}

		// send port number if relevant
		if (sendPort)
		{
			// send the port number at which the worker listens
			mwData d;
			d << this -> Port ();
			unsigned int code = mwPortMsg;
			int res = SendMessageW (fd, code, d);

			// quit in case of failure
			if (res < 0)
			{
				if (logFile)
					*logFile << "Error while sending "
						"the port number." <<
						std::endl;
				return mwError;
			}

			// make a note of having sent the port number
			sendPort = false;
			if (logFile)
				*logFile << "* Port number sent." <<
					std::endl;
		}

		// send the result of the processed data
		result = SendMessageW (fd, retcode, msg);

		// quit if the connection was lost
		if (result == mwLost)
		{
			if (logFile)
				*logFile << "Connection lost while sending "
					"data to the coordinator." <<
					std::endl;
			return mwError;
		}
		
		// quit if an error occurred
		if (result != mwOk)
		{
			if (logFile)
				*logFile << "Error while sending data." <<
					std::endl;
			return mwError;
		}
	}
} /* mwWorker::WorkOne */

inline int mwWorker::Work ()
{
	if (logFile)
	{
		*logFile << "Running as a WORKER." << std::endl;

		// indicate what kind of network connection is in use
		#if !mwNETWORK
		*logFile << "There is no network in use, "
			"so exiting right now." << std::endl;
		#elif mwWXWIN
		*logFile << "Using the sockets interface "
			"provided by wxWindows." << std::endl;
		#else
		*logFile << "Using the standard sockets "
			"for network connections." << std::endl;
		#endif
	}

	// connect to the first comp. on the list that is running
	int fd = -1;
	for (unsigned cur = 0; cur < this -> computers. size (); ++ cur)
	{
		// retrieve the computer name and port from the list
		int port = this -> ports [cur];
		const char *name = this -> computers [cur]. c_str ();

		// if no valid name or no port read, skip this data
		if (!*name || !port)
			continue;

		// try connecting to the computer
		fd = mwConnect (name, port);

		// add an appropriate message to the log file
		if (logFile)
		{
			*logFile << "Connection to " << name << ":" <<
				port << ((fd < 0) ? " refused." :
				" established.") << std::endl;
		}
	}

	while (1)
	{
		// if not connected then try listening
		if ((fd < 0) && (this -> Port () > 0))
		{
			// listen at the given port (use a very short queue)
			if (logFile)
				*logFile << "Waiting for a coordinator at "
					"port " << this -> Port () << "." <<
					std::endl;
			int fdlisten = mwListen (this -> Port (), 1);

			// if the listening failed, return with an error
			if (fdlisten < 0)
			{
				if (logFile)
					*logFile << "Error: This port is "
						"probably in use." <<
						std::endl;
				return mwError;
			}

			// accept a connection and stop listening
			std::string computer;
			int timeout = this -> TimeOut ();
			fd = mwAccept (fdlisten, computer, timeout);
			mwDisconnect (fdlisten);
	
			// if too much time elapsed, quit the job
			if (fd == mwTimeOut)
			{
				if (logFile)
					*logFile << "Time out. Exiting." <<
						std::endl;
				return mwOk;
			}

			// if an error occurred, quit the job
			else if (fd < 0)
			{
				if (logFile)
					*logFile << "Error while connecting "
						"to a coordinator." <<
						std::endl;
				return mwError;
			}

			// report the fact of connection to the log file
			if (logFile)
				*logFile << "Connected to a coordinator "
					"at '" << computer << "'." <<
					std::endl;
		}

		// receive messages and work
		int result = 0;
		try
		{
			result = WorkOne (fd);
			mwDisconnect (fd);
			fd = -1;
		}
		catch (...)
		{
			mwDisconnect (fd);
			if (logFile)
				*logFile << "An exception was thrown while "
					"processing data." << std::endl;
			throw;
		}

		// quit if there is no point to listen to another coordinator
		// or if an error occurred
		if ((this -> Port () <= 0) || !keepWorker ||
			(result != mwOk))
		{
			return result;
		}

		// add a line to the log file between working sessions
		if (logFile)
			*logFile << "============================" <<
				std::endl;
	}
} /* mwWorker::Work */


} // namespace multiwork
} // namespace chomp

#endif // _CHOMP_MULTIWORK_MWWORKER_H_

/// @}

