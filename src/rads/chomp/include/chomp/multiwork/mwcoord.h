/// @addtogroup multiwork
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file multiwork/mwcoord.h
///
/// This file contains the definition of the MultiWork coordinator class.
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

// Started on August 11, 2004. Last revision: December 11, 2007.


#ifndef _CHOMP_MULTIWORK_MWCOORD_H_
#define _CHOMP_MULTIWORK_MWCOORD_H_

#include <algorithm>
#include <string>
#include <cstring>

#include "chomp/multiwork/mwconfig.h"
#include "chomp/multiwork/mwlowlev.h"
#include "chomp/multiwork/mwdata.h"
#include "chomp/multiwork/mwtask.h"


namespace chomp {
namespace multiwork {

class mwWorker;


// --------------------------------------------------
// ------------------ mwWorkerData ------------------
// --------------------------------------------------

/// A helper class for storing data on a single worker.
/// This data structure is used internally by the coordinator.
class mwWorkerData
{
public:
	/// The default constructor.
	mwWorkerData ();

	/// The destructor.
	~mwWorkerData ();

	/// The data which was sent do the worker and is being processed.
	mwData data;

	/// The file descriptor number of the socket for the communication
	/// with the given worker.
	/// Negative if the worker is not connected.
	int fd;

	/// The computer name of the worker.
	std::string name;

	/// The port number at which the worker is going to listen next time.
	int port;

	/// The status of the worker: 0 = ok, -1 = failed, 1 = data acquired
	/// from a working one or data sent to a waiting one.
	int status;

	/// Swaps data between two objects of this type.
	friend void swap (mwWorkerData &data1, mwWorkerData &data2);

private:
	/// The copy constructor is not allowed.
	mwWorkerData (const mwWorkerData &) {return;};

	/// The assignment operator is not allowed.
	mwWorkerData &operator = (const mwWorkerData &) {return *this;};

}; /* class mwWorkerData */

inline mwWorkerData::mwWorkerData ():
	fd (-1), name (""), port (0), status (0)
{
	return;
} /* mwWorkerData::mwWorkerData */

inline mwWorkerData::~mwWorkerData ()
{
	return;
} /* mwWorkerData::~mwWorkerData */

inline void swap (mwWorkerData &data1, mwWorkerData &data2)
{
	swap (data1. data, data2. data);
	std::swap (data1. fd, data2. fd);
	std::swap (data1. name, data2. name);
	std::swap (data1. port, data2. port);
	std::swap (data1. status, data2. status);
	return;
} /* swap */


// --------------------------------------------------
// ----------------- mwCoordinator ------------------
// --------------------------------------------------

/// This class defines a generic coordinator task object for the multi-work
/// distributed computations framework.
/// Each coordinator class defined by the user must inherit from this class.
class mwCoordinator: public virtual mwTask
{
public:
	// --- constructor/destructor ---
	
	/// The default constructor.
	mwCoordinator ();
		
	/// The virtual destructor.
	virtual ~mwCoordinator ();

	// --- configuration ---

	/// Makes workers keep running after the coordinator's completion.
	void KeepWorkers (bool keep = true);

	/// Saves addresses of workers to the given file in the form of
	/// address:port. Repeated combinations are saved once only.
	int SaveWorkers (const char *filename);

	// --- initialization data ---

	/// Defines a portion of initialization data which will be sent
	/// to every newly connected worker. The argument variable is reset
	/// and the data is moved to the internal buffer.
	void Init (mwData &data);

	// --- run the coordinator ---

	/// Run the coordinator until all the work has been completed.
	/// If a worker object is supplied, the work will be done locally
	/// unless any remote worker is connected to the coordinator.
	/// Return mwOk or mwError.
	int Coordinate (mwWorker *w = NULL);

private:
	// --- coordinator's job ---

	/// Prepares a piece of data to be sent to a worker.
	/// Should return mwOk on success, mwNothing if there is no data
	/// to send based on the computations completed so far,
	/// mwError to cancel the computations.
	/// Please, overload this function in the class you derive
	/// from mwCoordinator.
	virtual int Prepare (mwData &data);
	
	/// Accepts a result received from a worker.
	/// Should return mwOk on success, mwError to stop the computations.
	/// Please, overload this function in the class you derive
	/// from mwCoordinator.
	virtual int Accept (mwData &data);
	
	/// Acknowledges data that was rejected by a worker.
	/// Should return mwOk if this is fine, or mwError to cancel
	/// the computations.
	/// Please, overload this function in the class you derive
	/// from mwCoordinator unless the default behavior is fine
	/// (returning mwOk and ignoring the data).
	virtual int Reject (mwData &data);

	// --- the coordinator's functions ---

	/// Runs the main communication loop: Sends the data to workers
	/// and receives the results of their computations.
	/// If there is no_more_data then waits for the running tasks
	/// until they finalize and some data becomes available.
	/// Otherwise, returns also when a new worker is ready to acquire
	/// a portion of data for processing.
	/// Returns mwOk or mwError.
	int RunLoop (bool no_more_data);

	/// Runs the main communication loop using the local worker.
	/// Returns mwOk or mwError.
	int RunLoopLocally ();

	/// Connects to all workers in the list.
	void ConnectWorkers ();

	/// Begins listening at the given port.
	void BeginListening ();

	/// Disconnects all the workers (normally called in the destructor).
	void DisconnectAll ();

	// --- configuration ---

	/// Should data be processed locally only?
	bool singleWork;
	
	/// The address of a local worker or 0 if none is available.
	mwWorker *localWorker;

	/// Should the workers be kept running after coordinator is done?
	bool keepWorkers;

	// --- initialization data ---

	/// The initialization data that has to be sent to workers.
	mwData initData;

	// --- data processing ---

	/// The number of workers waiting for their data.
	int nWaiting;

	/// The workers waiting for their tasks.
	mwWorkerData xWaiting [mwMAXWORK];

	/// The number of workers processing their data.
	int nWorking;

	/// The workers processing their data.
	mwWorkerData xWorking [mwMAXWORK];

	/// The number of data pieces to be sent to working tasks.
	int nToDo;

	/// The data pieces waiting to be sent to working tasks.
	mwData xToDo [mwMAXWORK];

	/// The number of recently rejected pieces of data.
	int nRejected;

	/// The recently rejected pieces of data.
	mwData xRejected [mwMAXWORK];

	/// The number of recently finished pieces of data.
	int nDone;

	/// The recently finished pieces of data.
	mwData xDone [mwMAXWORK];

	// --- data used for the communication with workers ---

	/// The socket number at which new workers are listened to.
	int listensocket;

	// --- operations on arrays (tables) ---

	/// A helper function for deleting a table entry
	/// and shifting the remainder of the table backwards.
	static void mwTableDel (int *tab, int len, int pos);

	/// A helper function for deleting a table entry
	/// and shifting the remainder of the table backwards
	/// using the method "Take" of the mwData class.
	static void mwTableDel (mwData *tab, int len, int pos);

	// --- network communiation ---

	/// Sends a message with data to the given socket as a coordinator.
	/// Returns mwOk on success and mwError in the case of failure.
	int SendMessageC (int fd, unsigned int code, const mwData &x) const;

	/// Receives a message with data from the socket of a worker.
	/// Returns mwOk on success or mwError in the case of failure.
	int RecvMessageC (int fd, unsigned int &code, mwData &x) const;

}; /* class mwCoordinator */

// --------------------------------------------------

inline mwCoordinator::mwCoordinator ():
#if mwNETWORK
	singleWork (false),
#else
	singleWork (true),
#endif
	localWorker (0),
	keepWorkers (false),
	nWaiting (0),
	nWorking (0),
	nToDo (0),
	nRejected (0),
	nDone (0),
	listensocket (-1)
{
	return;
} /* mwCoordinator::mwCoordinator */

inline void mwCoordinator::DisconnectAll ()
{
	// release all the workers if they are still connected
	for (int i = 0; i < nWaiting + nWorking; ++ i)
	{
		// determine the particular worker
		mwWorkerData &w = (i < nWaiting) ? xWaiting [i] :
			xWorking [i - nWaiting];

		// if this worker is already disconnected, take the next one
		if (w. fd < 0)
			continue;

		// prepare the code to send to the worker
		unsigned int code = mwByeMsg;
		code |= keepWorkers ? mwKeepMsg : mwDontKeepMsg;

		// send the 'Bye!' message to the worker
		mwData empty;
		SendMessageC (w. fd, code, empty);

		// disconnect the worker
		mwDisconnect (w. fd);
		w. fd = -1;

		// make a note of what happened in the log file
		if (logFile)
			*logFile << "Worker " << i << " (" << w. name <<
				":" << w. port << ") disconnected and " <<
				(keepWorkers ? "waiting." : "exited.") <<
				std::endl;
	}

	return;
} /* mwCoordinator::Disconnect */

inline mwCoordinator::~mwCoordinator ()
{
	// disconnect all the workers
	DisconnectAll ();
	
	return;
} /* mwCoordinator::~mwCoordinator */

inline void mwCoordinator::KeepWorkers (bool keep)
{
	keepWorkers = keep;
	return;
} /* mwCoordinator::KeepWorkers */

inline int mwCoordinator::SaveWorkers (const char *filename)
{
	// create a file for the list of workers
	// prepare to create a file to list the workers
	std::ofstream f;
	bool first = false;

	// go through all the connected workers
	int counter = 0;
	for (int i = 0; i < nWaiting + nWorking; ++ i)
	{
		// determine the particular worker
		mwWorkerData &w = (i < nWaiting) ? xWaiting [i] :
			xWorking [i - nWaiting];

		// determine the host name
		const char *host = w. name. c_str ();

		// if no host name or port number is known, skip it
		if (!host || !*host || (w. port <= 0))
			continue;

		// check if the same worker was already listed
		int j;
		for (j = 0; j < i; ++ j)
		{
			// determine the other worker
			mwWorkerData &v = (j < nWaiting) ? xWaiting [j] :
				xWorking [j - nWaiting];

			// determine the name of the other host
			const char *host_j = v. name. c_str ();

			// if no host name or port number is known, skip it
			if (!host_j || !*host_j || (v. port <= 0))
				continue;

			// if the ports are different, skip the other worker
			if (w. port != v. port)
				continue;

			// if the names are different, skip the other worker
			if (std::strcmp (host, host_j))
				continue;

			// make a note that this is the same worker
			j = i + 1;
			break;
		}
		if (j > i)
			continue;

		// create the file if this is the first time to write data
		if (first)
		{
			f. open (filename, std::ios::out | std::ios::trunc);
			if (!f)
				return mwError;
			f << "; A list of currently running workers:\n";
			first = false;
		}

		// write the address
		f << host << ":" << w. port << "\n";
		++ counter;
	}

	// exit if no workers have been listed
	if (!counter)
		return mwOk;

	// add a summary if any workers have been listed
	f << "; A total of " << counter << " workers";
	if (counter != nWaiting + nWorking)
		f << " out of " << (nWaiting + nWorking);
	f << " saved.\n";

	if (!keepWorkers)
		f << "; The workers will exit upon disconnection.\n";
	else
		f << "; The workers will remain running "
			"after disconnection.\n";
	f. close ();
	if (!f)
		return mwError;
	else
		return mwOk;
} /* mwCoordinator::SaveWorkers */

// --------------------------------------------------

inline int mwCoordinator::Prepare (mwData &)
{
	return mwNoData;
} /* mwCoordinator::Prepare */

inline int mwCoordinator::Accept (mwData &)
{
	return mwOk;
} /* mwCoordinator::Accept */

inline int mwCoordinator::Reject (mwData &)
{
	return mwOk;
} /* mwCoordinator::Reject */

// --------------------------------------------------

inline void mwCoordinator::Init (mwData &data)
{
	initData. Take (data);
	return;
} /* mwCoordinator::Init */


// --------------------------------------------------

inline int mwCoordinator::SendMessageC (int fd, unsigned int code,
	const mwData &x) const
{
	// prepare the control number to send
	unsigned int ctrl = this -> ControlNumber ();

	// send the message with the control number and the message code
	return this -> SendMessage (fd, ctrl, code, x);
} /* mwCoordinator::SendMessageC */

inline int mwCoordinator::RecvMessageC (int fd, unsigned int &code,
	mwData &x) const
{
	// receive the message
	unsigned int ctrl = 0;
	int result = this -> RecvMessage (fd, ctrl, code, x);

	// if there was an error then return its code
	if (result != mwOk)
		return result;

	// if the control number is correct then finish successfully
	if (ctrl == ~(this -> ControlNumber ()))
		return mwOk;

	// if the control number is wrong then return an error code
	if (logFile)
		*logFile << "Wrong control code received "
			"from the worker: " << ~ctrl << "." << std::endl;
	return mwError;
} /* mwCoordinator::SendMessageC */

// --------------------------------------------------

inline int mwCoordinator::RunLoopLocally ()
{
	if (!localWorker)
		return mwError;

	// indicate that some worker is continuously waiting for data
	if (!nWaiting)
		++ nWaiting;

	// process the data
	while ((nDone < mwMAXWORK) && (nRejected < mwMAXWORK) && (nToDo > 0))
	{
		// process the data and replace it with the result
		-- nToDo;
		xToDo [nToDo]. Rewind ();
		int result = localWorker -> Process (xToDo [nToDo]);
		xToDo [nToDo]. Rewind ();

		// if the data caused an error then abort the loop
		if (result == mwError)
		{
			if (logFile)
				*logFile << "Data processing failed." <<
					std::endl;
			return mwError;
		}

		// if the data was rejected
		// then move it to the right place
		else if (result == mwReject)
		{
			xRejected [nRejected]. Take (xToDo [nToDo]);
			++ nRejected;
		}

		// if the data was accepted
		// then move it to the right place
		else
		{
			xDone [nDone]. Take (xToDo [nToDo]);
			++ nDone;
		}
	}

	return mwOk;
} /* mwCoordinator::RunLoopLocally */

inline void mwCoordinator::ConnectWorkers ()
{
	int nComputers = computers. size ();
	for (int i = 0; (nWaiting < mwMAXWORK - 1) && (i < nComputers); ++ i)
	{
		// determine the computer name and port number
		const std::string &name = computers [i];
		int port = ports [i];

		// open the connection
		int fd = mwConnect (name. c_str (), port);

		// if the connection attempt was successful, add this worker
		if (fd >= 0)
		{
			if (logFile)
				*logFile << "Connected to " << name << ":" <<
					port << "." << std::endl;
			mwWorkerData &w = xWaiting [nWaiting];
			w. fd = fd;
			w. name = name;
			w. port = port;
			++ nWaiting;

			// prepare the initialization code
			int code = mwInitMsg |
				(keepWorkers ? mwKeepMsg : mwDontKeepMsg);

			// send the initialization data to the worker
			int initResult = SendMessageC (w. fd, code,
				initData);
			if (initResult != mwOk)
			{
				if (logFile)
					*logFile << "Error while sending "
						"the initialization data." <<
						std::endl;
				-- nWaiting;
			}
		}

		// if unable to connect, make only a note in the log
		else if (logFile)
			*logFile << "Connection attempt to " << name <<
				":" << port << " failed." << std::endl;
	}
	return;
} /* mwCoordinator::ConnectWorkers */

inline void mwCoordinator::BeginListening ()
{
	// if there is no valid port number then cancel this operation
	if (this -> Port () <= 0)
		return;

	// open a listening socket at the given port
	listensocket = mwListen (this -> Port (), 15);

	// add a message to the log file whether it was successful or not
	if (logFile)
	{
		if (listensocket < 0)
			*logFile << "Listening attempt at port " <<
				this -> Port () << " failed." << std::endl;
		else
			*logFile << "Waiting for workers at port " <<
				this -> Port () << "." << std::endl;
	}

	return;
} /* mwCoordinator::BeginListening */

inline int mwCoordinator::RunLoop (bool no_more_data)
{
	if (false && logFile)
		*logFile << "\nDebug0: " << nWaiting << " waiting, " <<
			nWorking << " working, " << nToDo <<
			" data pieces." << std::endl;
	
	// if not listening and there are no workers then try switching
	// to the single-work mode, unless there is no local worker
	if ((listensocket < 0) && !nWorking && !nWaiting)
	{
		// switch to the single-work mode if allowed to
		if (localWorker)
		{
			singleWork = true;
			if (logFile)
				*logFile << "All workers disconnected. "
					"Switching to the single-work "
					"mode." << std::endl;
			return mwOk;
		}

		// if data cannot be processed locally, report a failure
		else
		{
			if (logFile)
				*logFile << "Failure: All workers "
					"disconnected. The work cannot be "
					"continued." << std::endl;
			return mwError;
		}
	}

	// TO DO: If there is no more data (options & mwNoMoreData)
	// and some workers are idle, and some others are working
	// for a long time, send their data also to a few idle
	// workers... This requires some A.I., of course. ;)

	// the i/o flags ans sockets of the workers + one for 'listensocket'
	int ioflags [mwMAXWORK];
	int sockets [mwMAXWORK];
	int nWorkers = nToDo ? (nWorking + nWaiting) : nWorking;

	// prepare flags for the working and waiting workers
	for (int i = 0; i < nWorking; ++ i)
	{
		ioflags [i] = mwCanRead;
		sockets [i] = xWorking [i]. fd;
	}
	if (nWorkers > nWorking)
	{
		for (int i = 0; i < nWaiting; ++ i)
		{
			ioflags [nWorking + i] = mwCanWrite;
			sockets [nWorking + i] = xWaiting [i]. fd;
		}
	}

	// prepare the listen socket flag
	bool listening = false;
	int listenflag = nWorkers;
	if ((listensocket >= 0) && (nWorking + nWaiting < mwMAXWORK - 1))
	{
		ioflags [listenflag] = mwCanRead;
		listening = true;
	}
	else
		ioflags [listenflag] = mwNone;

	// determine whether it is necessary to wait or not
	int timelimit = this -> TimeOut ();
	if (localWorker && !nWorking && !nWaiting)
		timelimit = 0;
	if (listening && !no_more_data && !nToDo && !nWorking && !nWaiting)
		timelimit = 0;
	if (!no_more_data && !nToDo && nWaiting && (nWorking < mwMAXWORK))
		timelimit = 0;
//	if (no_more_data && !nToDo && !nWorking)
//		timelimit = 0;

	// report the select's parameters to the log file
	if (logFile)
	{
		*logFile << ">>> Select, t = " << timelimit << ", flags =";
		for (int i = 0; i <= nWorkers; ++ i)
			*logFile << " " << ioflags [i];
		*logFile << "." << std::endl;
	}

	// wait until data can be received from or sent to any socket
	int result = mwSelect (sockets, nWorkers,
		listening ? listensocket : -1, ioflags, timelimit);

	// report the returned parametrs to the log file
	if (logFile)
	{
		*logFile << ">>> Returned flags =";
		for (int i = 0; i <= nWorkers; ++ i)
			*logFile << " " << ioflags [i];
		*logFile << "." << std::endl;
	}

	// in case of select's failure, exit the loop with a failure message
	if (result == mwError)
	{
		if (logFile)
			*logFile << "Error: The 'select' function failed." <<
				std::endl;
		return mwError;
	}
				
	// report a time-out if necessary
	if ((timelimit > 0) && (result == mwTimeOut))
	{
		if (logFile)
			*logFile << "Time-out occurred at 'select'." <<
				std::endl;
	}

	// receive data from all the workers who are ready
	for (int i = 0; (i < nWorking) && (nDone < mwMAXWORK) &&
		(nRejected < mwMAXWORK) && (nToDo < mwMAXWORK); ++ i)
	{
		// if this worker is not ready, yet, then skip it
		if (!(ioflags [i] & mwCanRead))
			continue;

		// receive the entire data chunk from the worker
		unsigned int code = 0;
		int result = RecvMessageC (sockets [i], code, xDone [nDone]);

		// remember which worker this is
		mwWorkerData &w = xWorking [i];

		// reject the worker in case of error
		if (result < 0)
		{
			// log the details of what happened
			if (logFile)
			{
				*logFile << "Worker " << i;
				if (!w. name. empty ())
					*logFile << " (" << w. name << ")";
				*logFile << " disconnected: ";
				if (result == mwLost)
					*logFile << "Connection lost.";
				else
					*logFile << "An error occurred.";
				*logFile << std::endl;
			}

			// disconnect the worker
			mwDisconnect (sockets [i]);

			// move the worker's data back to the "to-do" list
			xToDo [nToDo]. Take (w. data);
			++ nToDo;

			// make a note of this in the worker's data
			w. fd = -1;
			w. status = -1;
		}

		// if transmitting the port number only, take it
		else if (code & mwPortMsg)
		{
			// retrieve the port number
			xDone [nDone] >> w. port;

			// report this fact to the log file
			if (logFile)
				*logFile << "Port number " << w. port <<
					" received from worker " << i <<
					"." << std::endl;
		}
	
		// if the data was rejected, move it to 'xRejected'
		else if (code & mwRejectedMsg)
		{
			// report what happened
			if (logFile)
				*logFile << "Data was rejected by worker " <<
					i << "." << std::endl;

			// move the data to the rejected table
			xRejected [nRejected]. Take (w. data);
			++ nRejected;

			// indicate the status change of this worker
			w. status = 1;
		}

		// if the data was Ok, the result is in 'xDone'
		else
		{
			// report this to the log file
			if (logFile)
				*logFile << "Processed data received from "
					"worker " << i << "." << std::endl;

			// accept this piece of data
			++ nDone;

			// indicate the status change of this worker
			w. status = 1;
		}
	}

	// send data to those workers who are ready
	for (int i = 0; (i < nWaiting) && nToDo; ++ i)
	{
		// remember the worker and its offset
		mwWorkerData &w = xWaiting [i];
		int offset = nWorking + i;

		// if this worker is not ready to get data then skip it
		if (!(ioflags [offset] & mwCanWrite))
			continue;

		// prepare a message code to send
		unsigned int code = mwStdMsg;

		// send a data chunk for processing
		int result = SendMessageC (sockets [offset], code,
			xToDo [nToDo - 1]);

		// if the data was sent successfully
		if (result == mwOk)
		{
			// take the data chunk to the worker
			-- nToDo;
			w. data. Take (xToDo [nToDo]);

			// indicate the status of the worker
			w. status = 1;

			// report this to the log file
			if (logFile)
				*logFile << "Data " << nToDo << " sent to "
					"worker " << i << "." << std::endl;
		}
	
		// if an error occurred, reject the worker
		else
		{
			// report this situation to the log file
			if (logFile)
				*logFile << "Worker " << i << " disconnected"
					": " << ((result == mwLost) ?
					"Connection lost." :
					"An error occurred.") << std::endl;

			// disconnect the worker
			mwDisconnect (sockets [offset]);

			// modify the status of the worker accordingly
			w. fd = -1;
			w. status = -1;
		}
	}

	if (false && logFile)
	{
		*logFile << "Debug1: " << nWaiting << " waiting:";
		for (int i = 0; i < nWaiting; ++ i)
			*logFile << " " << xWaiting [i]. status;
		*logFile << "; " << nWorking << " working:";
		for (int i = 0; i < nWorking; ++ i)
			*logFile << " " << xWorking [i]. status;
		*logFile << std::endl;
	}
	
	// purge workers who have been disconnected and move workers
	// whose data has been acquired to the 'waiting' queue
	for (int i = 0; i < nWorking; ++ i)
	{
		// skip this worker if it is fine
		if (xWorking [i]. status == 0)
			continue;

		// swap this worker with the last one if necessary
		if (i < nWorking - 1)
			swap (xWorking [i], xWorking [nWorking - 1]);
	
		// move the worker to the waiting queue if necessary
		if (xWorking [nWorking - 1]. status > 0)
		{
			xWorking [nWorking - 1]. status = 0;
			swap (xWaiting [nWaiting], xWorking [nWorking - 1]);
			++ nWaiting;
		}

		// remove this worker from the working queue
		-- nWorking;

		// consider the same entry in the table again
		-- i;
	}

	// purge workers who have been disconnected and move workers
	// who received data to the 'working' queue
	for (int i = 0; i < nWaiting; ++ i)
	{
		// skip this worker if it is fine
		if (xWaiting [i]. status == 0)
			continue;

		// swap this worker with the last one if necessary
		if (i < nWaiting - 1)
			swap (xWaiting [i], xWaiting [nWaiting - 1]);

		// move the worker to the working queue if necessary
		if (xWaiting [nWaiting - 1]. status > 0)
		{
			xWaiting [nWaiting - 1]. status = 0;
			swap (xWorking [nWorking], xWaiting [nWaiting - 1]);
			++ nWorking;
		}

		// remove this worker from the waiting queue
		-- nWaiting;

		// consider the same entry in the table again
		-- i;
	}

	if (false && logFile)
		*logFile << "Debug2: " << nWaiting << " waiting, " <<
			nWorking << " working, " << nToDo <<
			" data pieces." << std::endl;
	
	// accept connections from new workers if any
	if (listening && (ioflags [listenflag] & mwCanRead) &&
		(nWaiting + nWorking < mwMAXWORK))
	{
		// accept the connection
		mwWorkerData &w = xWaiting [nWaiting];
		w. name = std::string ("");
		w. port = 0;
		w. status = 0;
		w. fd = mwAccept (listensocket, w. name);

		// if the new worker has been accepted successfully
		if (w. fd >= 0)
		{
			// report the worker's acceptance
			if (logFile)
				*logFile << "A worker from '" << w. name <<
					"' accepted." << std::endl;

			// add the worker to the waiting queue
			++ nWaiting;

			// prepare the initialization code
			int code = mwInitMsg |
				(keepWorkers ? mwKeepMsg : mwDontKeepMsg);

			// send the initialization data to the worker
			int initResult = SendMessageC (w. fd, code,
				initData);
			if (initResult != mwOk)
			{
				if (logFile)
					*logFile << "Error while sending "
						"the initialization data." <<
						std::endl;
				-- nWaiting;
			}
		}

		// report the error to the log file if not successful
		else if (logFile)
			*logFile << "Unsuccessful connection of a worker "
				"from '" << w. name << "'." << std::endl;
	}

	if (false && logFile)
		*logFile << "Debug3: " << nWaiting << " waiting, " <<
			nWorking << " working, " << nToDo <<
			" data pieces." << std::endl;
	
	// ask for some data to process if none is waiting
	// or too much data acquired from workers is accumulated
	if (localWorker && !nWorking && !nWaiting &&
		!no_more_data && (result == mwTimeOut) && !nToDo)
	{
		if (logFile)
			*logFile << "Asking for some data to be "
				"processed locally." << std::endl;
		return mwOk;
	}

	// run the work locally as a last resort
	if (localWorker && !nWorking && !nWaiting && (result == mwTimeOut) &&
		nToDo && (nDone < mwMAXWORK) && (nRejected < mwMAXWORK))
	{
		// make a note in the log file of what is going on
		if (logFile)
			*logFile << "Processing data locally." << std::endl;

		// process one piece of data
		-- nToDo;
		xToDo [nToDo]. Rewind ();
		int result = localWorker -> Process (xToDo [nToDo]);
		xToDo [nToDo]. Rewind ();

		if (result == mwReject)
		{
			xRejected [nRejected]. Take (xToDo [nToDo]);
			++ nRejected;
		}
		else if (result == mwError)
		{
			if (logFile)
				*logFile << "Data processing failed." <<
					std::endl;
			return mwError;
		}
		else
		{
			xDone [nDone]. Take (xToDo [nToDo]);
			++ nDone;
		}
	}

	if (false && logFile)
		*logFile << "Debug4: " << nWaiting << " waiting, " <<
			nWorking << " working, " << nToDo <<
			" data pieces." << std::endl;
	
	return mwOk;
} /* mwCoordinator::RunLoop */

// --------------------------------------------------

inline int mwCoordinator::Coordinate (mwWorker *w)
{
	// remember the local worker's address
	localWorker = w;

	// initialize the local worker if any
	if (localWorker)
		localWorker -> Initialize (initData);

	if (logFile)
	{
		*logFile << "Running as a COORDINATOR." << std::endl;

		// indicate what kind of network connection is in use
		#if !mwNETWORK
		*logFile << "There is no network in use." << std::endl;
		#elif mwWXWIN
		*logFile << "Using the sockets interface "
			"provided by wxWindows." << std::endl;
		#else
		*logFile << "Using the standard sockets "
			"for network connections." << std::endl;
		#endif

		// say if running in the single-work mode only
		if (singleWork && localWorker)
			*logFile << "Running in the single-work mode." <<
				std::endl;
	}

	// connect to workers on the list and begin listening if necessary
	if (!singleWork)
	{
		this -> ConnectWorkers ();
		this -> BeginListening ();
	}

	// if not listening and there are no workers then try switching
	// to the single-work mode, unless there is no local worker
	if (!singleWork && (listensocket < 0) && !nWaiting)
	{
		// switch to the single-work mode if allowed to
		if (localWorker)
		{
			singleWork = true;
			if (logFile)
				*logFile << "No remote workers. Switching "
					"to the single-work mode." <<
					std::endl;
		}

		// if data cannot be processed locally, report a failure
		else
		{
			if (logFile)
				*logFile << "Failure: No workers." <<
					std::endl;
			return mwError;
		}
	}

	// is there no more data to be sent?
	bool no_more_data = false;

	while (1)
	{
		// run the communications loop
		int loopresult = singleWork ? this -> RunLoopLocally () :
			this -> RunLoop (no_more_data);

		// stop if the coordinator failed badly
		if (loopresult == mwError)
			return mwError;

		// if some data was rejected, process all this data
		while (nRejected > 0)
		{
			// run the user's procedure to acquire rejected data
			-- nRejected;
			xRejected [nRejected]. Rewind ();
			int result = this -> Reject (xRejected [nRejected]);

			// interrupt if the user says that an error occurred
			if (result != mwOk)
				return mwError;

			// reset the data acquired by the user
			xRejected [nRejected]. Reset ();
			no_more_data = false;
		}

		// if some new data arrived, process all this data
		while (nDone > 0)
		{
			// call the user's procedure to accept the data piece
			-- nDone;
			xDone [nDone]. Rewind ();
			int result = this -> Accept (xDone [nDone]);

			// interrupt if the user says that an error occurred
			if (result != mwOk)
				return mwError;

			// reset the data acquired by the user
			xDone [nDone]. Reset ();
			no_more_data = false;
		}

		// determine whether a new data item must be prepared:
		// if there are workers waiting and there is no data then YES
		bool hungry = (nWaiting > 0) && !nToDo;
		// if there is no worker and no data then YES
		if ((localWorker || (listensocket >= 0)) &&
			!nWorking && !nWaiting && !nToDo)
		{
			hungry = true;
		}
		// if no more data is needed then definitely NO
		if (no_more_data)
			hungry = false;

		// prepare a new data item if necessary
		if (hungry)
		{
			// run the user's procedure for preparing data
			int result = this -> Prepare (xToDo [nToDo]);

			// break if the user says that an error occurred
			if (result == mwError)
				return mwError;

			// make a note if there is no more data
			else if (result == mwNoData)
				no_more_data = true;

			// add the data piece to the work queue otherwise
			else
				++ nToDo;
		}

		// stop if the tasks are completed and there is no more data
		if (no_more_data && !nWorking && !nToDo)
			return mwOk;
	}
} /* mwCoordinator::Coordinate */

// --------------------------------------------------

inline void mwCoordinator::mwTableDel (int *tab, int len, int pos)
// Delete the entry at position 'pos' from the table of 'len' positions.
// Shift all the positions after 'pos' to the back.
{
	for (int i = pos + 1; i < len; ++ i)
		tab [i - 1] = tab [i];
	return;
} /* mwTableDel */

inline void mwCoordinator::mwTableDel (mwData *tab, int len, int pos)
// Delete the entry at position 'pos' from the table of 'len' positions.
// Shift all the positions after 'pos' to the back.
{
	for (int i = pos + 1; i < len; ++ i)
		tab [i - 1]. Take (tab [i]);
	return;
} /* mwTableDel */


} // namespace multiwork
} // namespace chomp

#endif // _CHOMP_MULTIWORK_MWCOORD_H_

/// @}

