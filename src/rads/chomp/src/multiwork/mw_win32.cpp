/// @addtogroup multiwork
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file mw_win32.cpp
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


#include "chomp/multiwork/mw_win32.h"


#if mwNETWORK && !mwWXWIN && \
	(defined (__WIN32__) || defined (_WIN32) || defined (WIN32))


#define FD_SETSIZE (mwMAXWORK + 1)
#include <winsock.h>


namespace chomp {
namespace multiwork {

static bool initialized = false;
static bool inavailable = false;

inline int mwInitSockets (void)
{
	if (initialized)
		return mwOk;
	if (inavailable)
		return mwError;
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD (1, 1);
	if (WSAStartup (wVersionRequested, &wsaData) != 0)
	{
		inavailable = true;
		return mwError;
	}
	if (LOBYTE (wsaData. wVersion) != 1 ||
		HIBYTE (wsaData. wVersion) != 1)
	{
		WSACleanup ();
		inavailable = true;
		return mwError;
	}
	initialized = true;
	return mwOk;
} /* mwInitSockets */

// --------------------------------------------------

static SOCKET *sockets = NULL;
static int nsockets = 0;

static int mwAvailableSocket (void)
// Get the number of the first unused socket number in the 'sockets' table.
// Return the socket number (which is non-negative) or mwError.
{
	// initialize the WinSock.DLL interface
	mwInitSockets ();

	// find the first available entry in the 'sockets' table
	int first = 0;
	while ((first < nsockets) && (sockets [first] != INVALID_SOCKET))
		first ++;
	if (first < nsockets)
		return first;

	// increase the table if no available entry found
	int n = (nsockets << 1) + 3;
	SOCKET *s = new SOCKET [n];
	if (!s)
		return mwError;
	for (int i = 0; i < n; i ++)
		s [i] = (i < nsockets) ? sockets [i] : INVALID_SOCKET;
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
				if (sockets [i] != INVALID_SOCKET)
					closesocket (sockets [i]);
			delete [] sockets;
		}

		// turn off the WinSock.DLL usage if necessary
		if (initialized)
			WSACleanup ();
		return;
	}
} mwDeinitializator;

// --------------------------------------------------

int mwSendBytes (int fd, const char *buf, int len)
{
	// if there is nothing to send, exit now
	if (!len)
		return mwOk;

	// check whether the socket corresponding to the given number is Ok
	if ((fd < 0) || (fd >= nsockets) || (sockets [fd] == INVALID_SOCKET))
		return mwError;

	// send the entire data buffer
	int sent = 0;
	while (sent < len)
	{
		int result = send (sockets [fd], buf + sent, len - sent, 0);
		if (result == SOCKET_ERROR)
			return mwError;
		sent += result;
	}

	return mwOk;
} /* mwSendBytes */

int mwRecvBytes (int fd, char *buf, int len)
{
	// if there is nothing to receive, exit now
	if (!len)
		return mwOk;

	// check whether the socket corresponding to the given number is Ok
	if ((fd < 0) || (fd >= nsockets) || (sockets [fd] == INVALID_SOCKET))
		return mwError;

	// read the entire buffer
	int received = 0;
	while (received < len)
	{
		int result = recv (sockets [fd], (char *) buf + received,
			len - received, 0);
		if (result == 0)
			return mwLost;
		else if (result == SOCKET_ERROR)
			return mwError;
		else
			received += result;
	}

	return mwOk;
} /* mwRecvBytes */

int mwConnect (const char *name, int port)
{
	// get the first available socket number
	int fd = mwAvailableSocket ();
	if (fd < 0)
		return mwError;

	// find the IP number of the computer we want to connect to
	struct hostent *host = gethostbyname (name);
	if (!host)
		return mwError;

	// create a socket for the connection;
	// note: if "AF_INET" is undefined, use "PF_INET" instead
	sockets [fd] = socket (AF_INET, SOCK_STREAM, 0);
	if (sockets [fd] == INVALID_SOCKET)
		return mwError;

	// prepare the address structure of the other computer
	struct sockaddr_in address;
	address. sin_family = AF_INET;
	address. sin_port = htons ((short int) port);
	address. sin_addr = *((struct in_addr *) (host -> h_addr));
	for (int i = 0; i < 8; ++ i)
		((char *) (&address. sin_zero)) [i] = 0;

	// connect through this socket to the other computer
	int result = connect (sockets [fd], (struct sockaddr *) &address,
		sizeof (struct sockaddr));
	if (result == SOCKET_ERROR)
		return mwError;

	// return the socket number if successfully connected to the server
	return fd;
} /* mwConnect */

int mwListen (int port, int queuesize)
{
	// get the first available socket number
	int fd = mwAvailableSocket ();
	if (fd < 0)
		return mwError;

	// create a socket for the connection;
	sockets [fd] = socket (AF_INET, SOCK_STREAM, 0);
	if (sockets [fd] == INVALID_SOCKET)
		return mwError;

	// prepare the address structure for the socket to be used
	struct sockaddr_in address;
	address. sin_family = AF_INET;
	address. sin_port = htons ((short int) port);
	address. sin_addr. s_addr = INADDR_ANY; // fill in my IP
	for (int i = 0; i < 8; ++ i)
		((char *) (&address. sin_zero)) [i] = 0;

	// bind the socket to the given port
	int result = bind (sockets [fd], (struct sockaddr *) &address,
		sizeof (struct sockaddr));
	if (result == SOCKET_ERROR)
	{
		closesocket (sockets [fd]);
		sockets [fd] = INVALID_SOCKET;
		return mwError;
	}
	
	// start listening at the socket
	result = listen (sockets [fd], queuesize);
	if (result == SOCKET_ERROR)
	{
		closesocket (sockets [fd]);
		sockets [fd] = INVALID_SOCKET;
		return mwError;
	}
	
	return fd;
} /* mwListen */

int mwAccept (int fd, std::string &computer, int timeout)
{
	// check whether the socket corresponding to the given number is Ok
	if ((fd < 0) || (fd >= nsockets) || (sockets [fd] == INVALID_SOCKET))
		return mwError;

	// wait until a connection arrives or the given time is up
	if (timeout >= 0)
	{
		fd_set s;
		FD_ZERO (&s);
		FD_SET (sockets [fd], &s);
		struct timeval t;
		t. tv_sec = timeout;
		t. tv_usec = 0;
		int result = select (0, &s, NULL, NULL, &t);
		if (result == SOCKET_ERROR)
			return mwError;
		if (!FD_ISSET (sockets [fd], &s))
			return mwTimeOut;
	}

	// accept the connection
	int size = sizeof (struct sockaddr_in);
	struct sockaddr_in address2;
	SOCKET s = accept (sockets [fd], (struct sockaddr *) &address2,
		&size);
	if (s == INVALID_SOCKET)
		return mwError;

	// determine the name of the computer
	struct hostent *host;
	host = gethostbyaddr ((const char *) &(address2. sin_addr), 4,
		AF_INET);
	if (!host)
		computer = std::string ("");
	else
		computer = std::string (host -> h_name);

	// add the socket to the table of sockets
	int new_fd = mwAvailableSocket ();
	if (new_fd == mwError)
		closesocket (s);
	else
		sockets [new_fd] = s;

	// return the socket number
	return new_fd;
} /* mwAccept */

int mwSelect (const int *workers, int nworkers, int listensocket,
	int *ioflags, int timeout)
{
	int i;

	// prepare sets of sockets to watch for
	bool nosockets = true;
	fd_set readfds, writefds;
	FD_ZERO (&readfds);
	FD_ZERO (&writefds);
	if ((listensocket >= 0) && (ioflags [nworkers] & mwCanRead))
	{
		FD_SET (sockets [listensocket], &readfds);
		nosockets = false;
	}
	for (i = 0; i < nworkers; ++ i)
	{
		if (ioflags [i] & mwCanRead)
		{
			FD_SET (sockets [workers [i]], &readfds);
			nosockets = false;
		}
		if (ioflags [i] & mwCanWrite)
		{
			FD_SET (sockets [workers [i]], &writefds);
			nosockets = false;
		}
	}

	// clear the flags
	for (i = 0; i <= nworkers; ++ i)
		ioflags [i] = mwNone;

	// if there are no sockets to look at, quit here
	if (nosockets)
		return mwTimeOut;
	
	// prepare the timeout
	struct timeval t;
	t. tv_sec = (timeout > 0) ? timeout : 0;
	t. tv_usec = 0;

	// determine the availability of all the sockets of interest
	int result = select (0, &readfds, &writefds, NULL, &t);

	// in case of an error, return an appropriate code
	if (result == SOCKET_ERROR)
		return mwError;

	// in case of timeout, exit now
	if (result == 0)
		return mwTimeOut;

	// set the ioflags
	if ((listensocket >= 0) &&
		FD_ISSET (sockets [listensocket], &readfds))
		ioflags [nworkers] |= mwCanRead;
	for (i = 0; i < nworkers; ++ i)
	{
		if (sockets [workers [i]] == INVALID_SOCKET)
			continue;
		if (FD_ISSET (sockets [workers [i]], &readfds))
			ioflags [i] |= mwCanRead;
		if (FD_ISSET (sockets [workers [i]], &writefds))
			ioflags [i] |= mwCanWrite;
	}
	
	return mwOk;
} /* mwSelect */

void mwDisconnect (int fd)
{
	if (!sockets || (fd < 0) || (fd >= nsockets) ||
		(sockets [fd] == INVALID_SOCKET))
		return;
//	shutdown (sockets [fd], 2);
	closesocket (sockets [fd]);
	sockets [fd] = INVALID_SOCKET;
	return;
} /* mwDisconnect */


} // namespace multiwork
} // namespace chomp

#endif // mwNETWORK

/// @}

