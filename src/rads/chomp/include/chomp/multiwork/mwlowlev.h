/// @addtogroup multiwork
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file multiwork/mwlowlev.h
///
/// This file contains the definition of some low-level functions
/// for the TCP/IP streams communication and port operations.
/// These functions are used internally by the worker
/// and coordinator classes for the purpose of network communication.
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


#ifndef _CHOMP_MULTIWORK_MWLOWLEV_H_
#define _CHOMP_MULTIWORK_MWLOWLEV_H_

#include <string>

namespace chomp {
namespace multiwork {

// --------------------------------------------------
// ---------- network communication flags -----------
// --------------------------------------------------

/// Input/output flags and error codes used for network communication,
/// mainly for the 'select' function to check which sockets
/// are available for reading/writing.
enum mwIOconstants
{
	/// No flag selected.
	mwNone = 0,

	/// Reading possible.
	mwCanRead = 0x01,

	/// Writing possible.
	mwCanWrite = 0x02,

	/// A connection time out has occurred.
	mwTimeOut = -4,

	/// The network connection has been lost.
	mwLost = -5

}; /* enum mwIOconstants */


// --------------------------------------------------
// ------------- network communication --------------
// --------------------------------------------------
// The following functions are implemented
// in a system-dependent way in the "mw_*.cpp" files
// one of which (or all) must be linked with the program.

/// Sends the given buffer to the given socket.
/// Returns mwOk, mwError or mwLost.
int mwSendBytes (int fd, const char *buf, int len);

/// Receives the given amount of data from the given socket.
/// Returns mwOk, mwError or mwLost.
int mwRecvBytes (int fd, char *buf, int len);

/// Connects to the given computer at the given port.
/// Returns the socket number (non-negative) or mwError.
int mwConnect (const char *name, int port);

/// Begins listening at the given port. Allows for the given queue length.
/// Returns the socket number (non-negative) or mwError.
int mwListen (int port, int queuesize);

/// Waits for and accepts a connection at the given socket.
/// Saves the computer name in the string provided.
/// Returns the new socket number (non-negative), mwTimeOut or mwError.
int mwAccept (int fd, std::string &computer, int timeout = -1);

/// Determines IOflags for each of the workers and additionally
/// the listensocket (the last flag).
/// The IOflags of interest are initially set.
/// Returns mwOk, mwTimeOut or mwError.
int mwSelect (const int *workers, int nworkers, int listensocket,
	int *ioflags, int timeout);

/// Disconnects the given socket. If the number is negative,
/// then this function call is ignored.
void mwDisconnect (int fd);


} // namespace multiwork
} // namespace chomp

#endif // _CHOMP_MULTIWORK_MWLOWLEV_H_

/// @}

