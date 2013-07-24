/// @addtogroup multiwork
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file mw_unix.cpp
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


#include "chomp/multiwork/mw_unix.h"


#if mwNETWORK && !mwWXWIN && \
	!defined (__WIN32__) && !defined (_WIN32) && !defined (WIN32)


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

// necessary(?) for cygwin
#ifndef MSG_WAITALL
#define MSG_WAITALL 0x100
#endif


namespace chomp {
namespace multiwork {

int mwSendBytes (int fd, const char *buf, int len)
{
	int sent = 0;
	while (sent < len)
	{
		int result = send (fd, buf + sent, len - sent, 0);
		if (result < 0)
			return mwError;
		sent += result;
	}
	return mwOk;
} /* mwSendBytes */

int mwRecvBytes (int fd, char *buf, int len)
{
	if (len <= 0)
		return mwOk;

	// read the entire buffer
	int received = 0;
	while (received < len)
	{
		int result = recv (fd, buf + received, len - received,
			MSG_WAITALL);
		if (result == 0)
			return mwLost;
		else if (result < 0)
			return mwError;
		else
			received += result;
	}

	return mwOk;
} /* mwRecvBytes */

int mwConnect (const char *name, int port)
{
	// find the IP number of the computer we want to connect to
	struct hostent *host = gethostbyname (name);
	if (!host)
		return mwError;

	// create a socket for the connection
	int fd = socket (AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		return mwError;

	// prepare the address structure of the other computer
	struct sockaddr_in address;
	address. sin_family = AF_INET;
	address. sin_port = htons ((short int) port);
	address. sin_addr = *((struct in_addr *) (host -> h_addr));
	for (int i = 0; i < 8; ++ i)
		((char *) &(address. sin_zero)) [i] = '\0';

	// connect through this socket to the other computer
	int result = connect (fd, (struct sockaddr *) &address,
		sizeof (struct sockaddr));
	if (result < 0)
		return mwError;

	// return the socket file descriptor
	return fd;
} /* mwConnect */

int mwListen (int port, int queuesize)
{
	// create a socket for the connection
	int fd = socket (AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		return mwError;

	// prepare the address structure for the socket to be used
	struct sockaddr_in address;
	address. sin_family = AF_INET;
	address. sin_port = htons ((short int) port);
	address. sin_addr. s_addr = INADDR_ANY; // fill in my IP
	for (int i = 0; i < 8; ++ i)
		((char *) &(address. sin_zero)) [i] = '\0';

	// bind the socket to the given port
	int result = bind (fd, (struct sockaddr *) &address,
		sizeof (struct sockaddr));
	if (result < 0)
	{
		close (fd);
		return mwError;
	}
	
	// start listening at the socket
	result = listen (fd, queuesize);
	if (result < 0)
	{
		close (fd);
		return mwError;
	}
	
	return fd;
} /* mwListen */

int mwAccept (int fd, std::string &computer, int timeout)
{
	// wait until a connection arrives or the given time is up
	if (timeout >= 0)
	{
		fd_set s;
		FD_ZERO (&s);
		FD_SET (fd, &s);
		struct timeval t;
		t. tv_sec = timeout;
		t. tv_usec = 0;
		int result = select (fd + 1, &s, NULL, NULL, &t);
		if (result < 0)
			return mwError;
		if (!FD_ISSET (fd, &s))
			return mwTimeOut;
	}

	// accept the connection
	socklen_t size = sizeof (struct sockaddr_in);
	struct sockaddr_in address2;
	int new_fd = accept (fd, (struct sockaddr *) &address2, &size);
	if (new_fd < 0)
		return mwError;

	// determine the name of the computer
	struct hostent *host;
	host = gethostbyaddr ((const char *) &(address2. sin_addr), 4,
		AF_INET);
	if (!host)
		computer = std::string ("");
	else
		computer = std::string (host -> h_name);

	// return the socket file descriptor
	return new_fd;
} /* mwAccept */

int mwSelect (const int *workers, int nworkers, int listensocket,
	int *ioflags, int timeout)
{
	int i;

	// prepare sets of file descriptors to watch for
	fd_set readfds, writefds;
	FD_ZERO (&readfds);
	FD_ZERO (&writefds);
	if ((listensocket >= 0) && (ioflags [nworkers] & mwCanRead))
		FD_SET (listensocket, &readfds);
	for (i = 0; i < nworkers; ++ i)
	{
		if (ioflags [i] & mwCanRead)
			FD_SET (workers [i], &readfds);
		if (ioflags [i] & mwCanWrite)
			FD_SET (workers [i], &writefds);
	}
	
	// clear the flags
	for (i = 0; i <= nworkers; ++ i)
		ioflags [i] = mwNone;

	// compute the maximal file descriptor to be considered
	int max_fd = listensocket;
	for (i = 0; i < nworkers; ++ i)
		if (max_fd < workers [i])
			max_fd = workers [i];

	// if there are no file descriptors to look at, quit here
	if (max_fd < 0)
		return mwTimeOut;
	
	// prepare the timeout
	struct timeval t;
	t. tv_sec = (timeout > 0) ? timeout : 0;
	t. tv_usec = 0;

	// determine the availability of all the sockets of interest
	int result = select (max_fd + 1, &readfds, &writefds, NULL, &t);

	// in case of an error, return the error code
	if (result < 0)
		return mwError;

	// set the ioflags
	bool WasTimeout = true;
	if ((listensocket >= 0) && FD_ISSET (listensocket, &readfds))
	{
		ioflags [nworkers] |= mwCanRead;
		WasTimeout = false;
	}
	for (i = 0; i < nworkers; ++ i)
	{
		if (workers [i] < 0)
			continue;
		if (FD_ISSET (workers [i], &readfds))
		{
			ioflags [i] |= mwCanRead;
			WasTimeout = false;
		}
		if (FD_ISSET (workers [i], &writefds))
		{
			ioflags [i] |= mwCanWrite;
			WasTimeout = false;
		}
	}
	
	if (WasTimeout)
		return mwTimeOut;
	return mwOk;
} /* mwSelect */

void mwDisconnect (int fd)
{
	if (fd >= 0)
		close (fd);
	return;
} /* mwDisconnect */


} // namespace multiwork
} // namespace chomp

#endif // mwNETWORK

/// @}

