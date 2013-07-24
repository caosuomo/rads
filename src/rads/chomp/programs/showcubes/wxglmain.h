/////////////////////////////////////////////////////////////////////////////
///
/// @file wxglmain.h
///
/// This is a header file to be included by a program that generates
/// 3-dimensional graphics using Open GL and the wxWidgets library.
/// 
/// The procedures declared here allow one to write a simple, small program
/// that displays an Open GL graphics which can be rotated and moved
/// forward/backward and to the sides interactively by the user that
/// runs the program. It is only necessary to implement a few basic
/// functions that use OpenGL commands to display the objects of interest.
///
/// Note that the idea of using this module is quite different form the
/// usual usage of modules in C++. The file "wxglmain.cpp" actually contains
/// the function "main" which defines and controls the entire program.
/// This program calls functions declared in this module to initialize
/// the data and to display the contents of the graphics window.
/// Please, consult the program "showcubes" for a good example on how to use
/// this module.
///
/// See http://www.wxwidtegs.org/ for information on the excellent and
/// portable wxWidgets graphics toolkit.
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

// Started on June 30, 2004. Last revision: August 11, 2007.


#ifndef _WXGLMAIN_H_
#define _WXGLMAIN_H_

#include "chomp/system/config.h"

// compile only if the wxWindows GUI is available.
#ifdef ppWXWIN

// Include the appropriate OpenGL headers.
#ifdef __WXMAC__
# ifdef __DARWIN__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
# else
#   include <gl.h>
#   include <glu.h>
# endif
#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif


// --------------------------------------------------
// ---------------- already defined -----------------
// --------------------------------------------------
// The following variables are defined for you in this
// module, and you may use them in order to be able
// to prepare your OpenGL graphics accordingly.

/// Is the picture being displayed as a wireframe?
/// One may need to know this in some cases in order to display elements
/// which would otherwise be invisible, hidden behind other objects.
extern bool wireframe;

/// The magnification exponent which is initially 0 and then becomes
/// positive or negative as the user changes the size of the picture.
/// One may need to know this for example to determine the size of points
/// to plot in pixels.
extern double magnification;


// --------------------------------------------------
// --------------- need to be defined ---------------
// --------------------------------------------------
// The following variables and functions MUST be
// defined in your program.
// They are used by the interactive module.

/// This text is displayed as help information in the program (unless empty).
extern const char *helpinfo;

/// This function is called at initialization.
void initPicture (int argc, char *argv []);

/// This function is called at every paint event to draw the actual picture.
/// This function should take into account the rotation matrix and various
/// parameters of the picture (see above).
void drawPicture (void);

/// This function is called upon program termination in order to allow one
/// to free the resources you were using.
void deletePicture (void);

#endif // wxWindows

#endif // _WXGLMAIN_H_

