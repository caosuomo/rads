/// @addtogroup multiwork
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file mw_wxwin.cpp
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

// Started on August 11, 2004. Last revision: January 13, 2006.


#include "chomp/multiwork/mw_wxwin.h"


#if (mwNETWORK && mwWXWIN)

#include <wx/socket.h>
#include <wx/app.h>
#include <wx/thread.h>


namespace chomp {
namespace multiwork {

inline void mwYield ()
{
//	wxYield (); // WRONG: only the main thread is allowed to call this!
	return;
} /* mwYield */

static wxSocketBase **sockets = NULL;
static int nsockets = 0;

static int mwAvailableSocket (void)
// Get the number of the first unused socket number in the 'sockets' table.
// Return the socket number (which is non-negative) or mwError.
{
	// find the first available entry in the 'sockets' table
	int first = 0;
	while ((first < nsockets) && (sockets [first] != NULL))
		first ++;
	if (first < nsockets)
		return first;

	// increase the table if no available entry found
	int n = (nsockets << 1) + 3;
	wxSocketBase **s = new wxSocketBase * [n];
	if (!s)
		return mwError;
	for (int i = 0; i < n; i ++)
		s [i] = (i < nsockets) ? sockets [i] : NULL;
	delete [] sockets;
	sockets = s;
	nsockets = n;
	return first;
} /* mwAvailableSocket */

// --------------------------------------------------

class mwDeinitializatorClass
{
public:
	mwDeinitializatorClass () {};
	~mwDeinitializatorClass ()
	{
		// close all the sockets that are still open
		if (sockets)
		{
			for (int i = 0; i < nsockets; i ++)
				if (sockets [i] != NULL)
					sockets [i] -> Destroy ();
			delete [] sockets;
			mwYield ();
		}
	}
} mwDeinitializator;

// --------------------------------------------------

int mwSendBytes (int fd, const char *buf, int len)
{
	// yield the program's thread for event processing
	mwYield ();

	// if there is nothing to send, exit now
	if (!len)
		return mwOk;

	// take the socket corresponding to the given number
	if ((fd < 0) || (fd >= nsockets) || (!sockets [fd]))
	{
		cerr << "mwSendBytes: wrong socket" << endl;
		return mwError;
	}
	wxSocketBase *s = sockets [fd];

	// write the entire buffer
	s -> Write (buf, len);

	// if the socket is not connected, writing should fail
	if (!s -> IsConnected ())
	{
		cerr << "mwSendBytes: socket not connected" << endl;
		return mwError;
	}

	// check whether an error occurred
	if (s -> Error ())
	{
		cerr << "mwSendBytes (" << len << "): error" << endl;
		return mwError;
	}

	// verify whether the right amount of data was sent
	if (s -> LastCount () != (unsigned int) len)
	{
		cerr << "mwSendBytes: wrong count: " <<
			s -> LastCount () << " instead of " << len << endl;
		return mwError;
	}

	// yield the program's thread for event processing
	mwYield ();

	return mwOk;
} /* mwSendBytes */

int mwRecvBytes (int fd, char *buf, int len)
{
	// yield the program's thread for event processing
	mwYield ();

	// if there is nothing to receive, exit now
	if (!len)
		return mwOk;

	// take the socket corresponding to the given number
	if ((fd < 0) || (fd >= nsockets) || (!sockets [fd]))
	{
		cerr << "mwRecvBytes: wrong socket" << endl;
		return mwError;
	}
	wxSocketBase *s = sockets [fd];

	// read the entire buffer
	s -> Read (buf, len);

	// if the socket is not connected, reading should fail
	if (!s -> IsConnected ())
	{
		cerr << "mwRecvBytes: socket not connected" << endl;
		return mwError;
	}

	// check whether an error occurred
	if (s -> Error ())
	{
		cerr << "mwRecvBytes (" << len << "): error " <<
			s -> LastError () << endl;
		return mwError;
	}

	// verify whether the right amount of data was received
	if (s -> LastCount () != (unsigned int) len)
	{
		cerr << "mwRecvBytes: wrong count: " <<
			s -> LastCount () << " instead of " << len << endl;
		return mwError;
	}

	// yield the program's thread for event processing
	mwYield ();

	return mwOk;
} /* mwRecvBytes */

int mwConnect (const char *name, int port)
{
	// yield the program's thread for event processing
	mwYield ();

	// get the first available socket number
	int fd = mwAvailableSocket ();
	if (fd < 0)
		return mwError;

	// create a new client socket
	wxSocketClient *sc = new wxSocketClient (wxSOCKET_WAITALL |
		wxSOCKET_BLOCK);
	sockets [fd] = sc;

	// fill in the address of the server to connect to
	wxIPV4address address;
	bool ok = true;
	if (ok)
		ok = address. Hostname (name);
	if (ok)
		ok = address. Service ((unsigned short) port);

	// connect to the server
	if (ok)
		ok = sc -> Connect (address, true);

	// destroy the socket and return the error code in case of errors
	if (!ok || !sc -> Ok ())
	{
		sc -> Destroy ();
		sockets [fd] = NULL;
		return mwError;
	}

	// yield the program's thread for event processing
	mwYield ();

	// return the socket number if successfully connected to the server
	return fd;
} /* mwConnect */

int mwListen (int port, int)
{
	// yield the program's thread for event processing
	mwYield ();

	// get the first available socket number
	int fd = mwAvailableSocket ();
	if (fd < 0)
		return mwError;

	// fill in the address of the server
	wxIPV4address address;
	bool ok = true;
	if (ok)
		ok = address. AnyAddress ();
	if (ok)
		ok = address. Service ((unsigned short) port);
	if (!ok)
		return mwError;

	// create a new server socket and begin listening at the given port
	wxSocketServer *ss = new wxSocketServer (address,
		wxSOCKET_WAITALL | wxSOCKET_BLOCK);
	sockets [fd] = ss;

	// yield the program's thread for event processing
	mwYield ();

	// return the socket number
	return fd;
} /* mwListen */

int mwAccept (int fd, std::string &computer, int timeout)
{
	// yield the program's thread for event processing
	mwYield ();

	// take the server socket corresponding to the given number
	if ((fd < 0) || (fd >= nsockets) || (!sockets [fd]))
		return mwError;
	wxSocketServer *ss = (wxSocketServer *) (sockets [fd]);

	// try accepting the connection if it is already waiting
	wxSocketBase *s = ss -> Accept (false);

	// wait until a connection arrives or the given time is up
	if (!s && (timeout >= 0))
	{
		// yield the program's thread for event processing
		mwYield ();

		bool result = ss -> WaitForAccept (timeout);
		if (!result)
			return mwTimeOut;
	}

	// yield the program's thread for event processing
	mwYield ();

	// accept an incoming connection if not accepted yet
	if (!s)
		s = ss -> Accept (true);
	if (!s)
		return mwError;

	// set the appropriate flags for the socket
	s -> SetFlags (wxSOCKET_WAITALL | wxSOCKET_BLOCK);

	// determine the name of the computer
	wxIPV4address address;
	if (s -> GetPeer (address))
		computer = std::string (address. Hostname ());
	else
		computer = std::string ("");

	// add the socket to the table of sockets
	int new_fd = mwAvailableSocket ();
	if (new_fd == mwError)
		s -> Destroy ();
	else
		sockets [new_fd] = s;

	// yield the program's thread for event processing
	mwYield ();

	return new_fd;
} /* mwAccept */

int mwSelect (const int *workers, int nworkers, int listensocket,
	int *ioflags, int timeout)
{
	// copy the ioflags of interest to another table
	int *chkflags = new int [nworkers + 1];
	if (!chkflags)
		return mwError;
	for (int i = 0; i <= nworkers; i ++)
		chkflags [i] = ioflags [i];

	// check the I/O flags in a loop
	bool firsttime = true;
	long waitedmsec = 0;
	ioflags [nworkers] = 0;
	while (1)
	{
		// yield the program's thread for event processing
		mwYield ();

		// set an appropriate time-out in milliseconds
		int msec = firsttime ? 0 : 200;

		// compute the new ioflags
		bool arrived = false;
		for (int i = 0; i < nworkers; i ++)
		{
			ioflags [i] = 0;
			if (chkflags [i] & mwCanRead)
			{
				if (sockets [workers [i]] ->
					WaitForRead (0, msec))
				{
					ioflags [i] |= mwCanRead;
					arrived = true;
				}
				waitedmsec += msec;
				msec = 0;
			}
			if (chkflags [i] & mwCanWrite)
			{
				if (sockets [workers [i]] ->
					WaitForWrite (0, msec))
				{
					ioflags [i] |= mwCanWrite;
					arrived = true;
				}
				waitedmsec += msec;
				msec = 0;
			}
		}
		if ((chkflags [nworkers] & mwCanRead) && (listensocket >= 0))
		{
			if (((wxSocketServer *) (sockets [listensocket])) ->
				WaitForAccept (0, msec))
			{
				ioflags [nworkers] |= mwCanRead;
				arrived = true;
			}
			waitedmsec += msec;
			msec = 0;
		}

		// if some data arrived, quit the loop
		if (arrived)
			return mwOk;

		// if nothing is waited for, then this is an error
		if (msec > 0)
			return mwError;

		// if the given time elapsed, quit the loop
		if ((timeout >= 0) && (waitedmsec / 1000 >= timeout))
			return mwTimeOut;

		// make a note that this is no longer the first loop run
		firsttime = false;
	}
} /* mwSelect */

void mwDisconnect (int fd)
{
	if (!sockets || (fd < 0) || (fd >= nsockets) || (!sockets [fd]))
		return;
	sockets [fd] -> Destroy ();
	sockets [fd] = NULL;

	// yield the program's thread for event processing
	mwYield ();

	return;
} /* mwDisconnect */


} // namespace multiwork
} // namespace chomp

#endif // mwNETWORK

/// @}

