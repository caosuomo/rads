/// @addtogroup multiwork
/// @{

/////////////////////////////////////////////////////////////////////////////
///
/// @file multiwork/mwconfig.h
///
/// This file contains the basic configuration of the MultiWork module,
/// mainly as a definition of a series of preprocessor symbols.
/// A few constants used in the interface are also defined here.
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


#ifndef _CHOMP_MULTIWORK_MWCONFIG_H_
#define _CHOMP_MULTIWORK_MWCONFIG_H_


// --------------------------------------------------
// --------------------- mwData ---------------------
// --------------------------------------------------

// use the 'bool' data type
#ifndef mwBOOL
	#define mwBOOL 1
#endif // mwBOOL

// use 'long long' (does not work under MS Visual C++ 6.0)
#ifndef mwLONGLONG
	#if defined (_MSC_VER) || defined (__STRICT_ANSI__)
		#define mwLONGLONG 0
	#else
		#define mwLONGLONG 1
	#endif
#endif // mwLONGLONG

// use the 'string' class from the standard library
#ifndef mwSTRING
	#define mwSTRING 1
#endif // mwBOOL

// create stream I/O operators << and >> (for binary streams only!)
#ifndef mwSTREAMS
	#define mwSTREAMS 1
#endif


// --------------------------------------------------
// ---------------------- bool ----------------------
// --------------------------------------------------

// if the 'bool' data type seems to be unknown to the compiler,
// define your own 'bool', 'true' and 'false' to use in the program
#if !mwBOOL
	#define bool unsigned char
	#define true 1
	#define false 0
#endif // no mwBOOL


// --------------------------------------------------
// ---------------- network settings ----------------
// --------------------------------------------------

// are we in a network environment (otherwise this is a single process)
#ifndef mwNETWORK
	/// This constant is defined as 1 if the network environment
	/// is present and the network communication routines
	/// should be compiled. If it is set to 0 then the computations
	/// only run locally as one coordinator coordinating one worker
	/// in the same program.
	#define mwNETWORK 1
#endif // mwNETWORK

/// Unexpected errors occur while using wxWin sockets from within the Krak
/// library from the CAPD package, so I turn this feature off. This may be
/// due to the fact that the main program runs there as a secondary thread.
/// This should be fixed in the future to make the program run in WXGTK.
#define mwWXWIN 0

// should the wxWindows sockets be used?
#ifndef mwWXWIN
	#if (defined (__WXMSW__) || defined (__WXGTK__) || \
		defined (__WXMOTIF__) || defined (__WXX11__) || \
		defined (__WXMAC__) || defined (__WXUNIVERSAL__))
		#define mwWXWIN 1
	#else
		#define mwWXWIN 0
	#endif
#endif // mwWXWIN


// --------------------------------------------------
// -------------------- defaults --------------------
// --------------------------------------------------

#ifndef mwPORT
	/// The default port number used for the communication
	/// between the coordinator and workers.
	#define mwPORT 5885
#endif // mwPORT
	
#ifndef mwCTRLNUM
	/// The default control number used for data identification.
	#define mwCTRLNUM 16780
#endif // mwCTRLNUM

#ifndef mwTIMEOUT
	/// The default time-out in seconds (8 hours).
	#define mwTIMEOUT 28800
#endif // mwTIMEOUT


// --------------------------------------------------
// ------------------ limitations -------------------
// --------------------------------------------------

#ifndef mwMAXWORK
	/// The maximal number of simultaneously connected workers.
	/// Should not be set to more than 1024, because this is the limit
	/// of the macros related to the "select" function in Linux.
	/// In Windows, this number might be limited to 64 in some cases.
	#define mwMAXWORK 1024
#endif // mwMAXWORK


// --------------------------------------------------
// ------------------- constants --------------------
// --------------------------------------------------

namespace chomp {
namespace multiwork {

/// Various error codes used during the communication.
/// The OK code must equal zero, and error codes must be negative.
enum mwConstants
{
	/// Everything is fine.
	mwOk = 0,

	/// A serious error occurred.
	mwError = -1,

	/// There is no data to be sent to workers, for example,
	/// because everything has been already sent.
	mwNoData = -2,

	/// The data has been rejected.
	mwReject = -3

}; /* enum mwConstants */

} // namespace multiwork
} // namespace chomp


#endif // _CHOMP_MULTIWORK_MWCONFIG_H_

/// @}

