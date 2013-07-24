/////////////////////////////////////////////////////////////////////////////
///
/// @file wxglmain.cpp
///
/// This file contains the implementation of the function "main"
/// and all the necessary operations for displaying 3-dimensional
/// graphics using Open GL and the wxWidgets library.
/// 
/// See the header file "wxglmain.h" for more instructions on how to use it.
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

// Started on June 30, 2004. Last revision: October 22, 2008.


// #define _WXGLMAIN_CPP_

#include "chomp/system/config.h"
#include "chomp/system/textfile.h"

// compile this code only if the wxWidgets GUI is available
#ifdef ppWXWIN

#include "wxglmain.h"

// avoid the "re-definition of _puttchar" warning in wx 2.5.2 (MSW/MinGW)
#if defined (__WXMSW__) && defined (__MINGW32__)
#  include <tchar.h>
#endif

// include the declarations of sin and cos and some other functions
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>

// include the declarations for setting locale (LC_NUMERIC in particular)
#include <locale.h>

// include the wxWidgets header files
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/imagpng.h>
#include <wx/glcanvas.h>
#include <wx/colordlg.h>

const char *proginfo = "\
This program displays 3D objects with the use of OpenGL.\n\
Use left mouse button or arrows to rotate the figure,\n\
right mouse button or ADSW to move to the sides,\n\
middle mouse button or +/- to get closer/farther,\n\
hit Home or I for the initial viewing position,\n\
0 to reset rotation to zero, SPACE for fixed rotation.";

const wxChar *defaultStatusText =
	wxT("This program uses wxWidgets and OpenGL.");

const char *aboutinfo = "\
This is a wxWidgets program that uses OpenGL\n\
in a framework programmed in 2004 by Pawel Pilarczyk.\n\
This is free software. No warranty.\n\
See the GNU General Public License for details.";

// the rotation and translation matrices
float rotation [16];
float translation [3] = {0.0f, 0.0f, 0.0f};

// the background color of the scene
float bgcolor [4] = {0.0f, 0.0f, 0.0f, 0.0f};

// the magnification exponent
double magnification = 0.0;

// should the scene be plotted in the wire-frame mode?
bool wireframe = false;

// should there be a fog in the scene?
bool fog = true;

// should the perspective be used in the projection?
bool perspective = true;


// --------------------------------------------------
// ---------------- wxGL Main Canvas ----------------
// --------------------------------------------------
// The OpenGL canvas contains the OpenGL graphics
// and is contained in the main frame.

class PPCanvas: public wxGLCanvas
{
public:
	PPCanvas (wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize, long style = 0,
		const wxString &name = wxT("PP Canvas"),
		int *gl_attrib = NULL);

	~PPCanvas ();

	void OnPaint (wxPaintEvent &event);
	void OnSize (wxSizeEvent &event);
	
	void OnEraseBackground (wxEraseEvent &event);

	void OnMouseEvents (wxMouseEvent &event);
	void OnChar (wxKeyEvent &event);

	void PaintPicture ();
	void Rotate (int x, int y, float degrees = 0);
	void Rescale (int diff, int x, int y);

	wxString scriptName;
	void ProcessScript (const wxString &script);

	wxWindow *parentWindow;

	DECLARE_EVENT_TABLE()
}; /* PPCanvas */

PPCanvas *ppCanvas = (PPCanvas *) NULL;

// --------------------------------------------------

BEGIN_EVENT_TABLE (PPCanvas, wxGLCanvas)
	EVT_MOUSE_EVENTS (PPCanvas::OnMouseEvents)
	EVT_PAINT (PPCanvas::OnPaint)
	EVT_SIZE (PPCanvas::OnSize)
#ifndef __WXGTK__
	EVT_CHAR (PPCanvas::OnChar)
#endif
	EVT_ERASE_BACKGROUND (PPCanvas::OnEraseBackground)
END_EVENT_TABLE ()


// --------------------------------------------------
// ---------------- wxGL Main Frame -----------------
// --------------------------------------------------
// This class is the main frame in the program.
// It contains the OpenGL canvas, and a panel which receives
// the keyboard events (the latter is necessary only with GTK).

class PPFrame: public wxFrame
{
public:
	bool showScaling;
	bool showKeys;
	bool showMouse;
	bool showRefresh;

	// frame constructor and destructor
	PPFrame (wxWindow *parent, const wxString &title,
		const wxPoint &pos, const wxSize &size);
	~PPFrame ();

	// event handling
	void OnQuit (wxCommandEvent &event);
	void Save (const wxString &files, wxBitmapType type, bool askfile);
	void OnSaveBMP (wxCommandEvent &event);
	void OnSavePNG (wxCommandEvent &event);
	void OnShowWireFrame (wxCommandEvent &event);
	void OnShowFog (wxCommandEvent &event);
	void OnPerspective (wxCommandEvent &event);
	void OnBgColor (wxCommandEvent& event);
	void OnShowScaling (wxCommandEvent &event);
	void OnShowKeys (wxCommandEvent &event);
	void OnShowMouse (wxCommandEvent &event);
	void OnShowRefresh (wxCommandEvent &event);
	void OnAbout (wxCommandEvent &event);
	void OnHelpGeneral (wxCommandEvent &event);
	void OnHelpSpecific (wxCommandEvent &event);
	void OnSize (wxSizeEvent &event);
	void OnClose (wxCloseEvent& event);

	void OnChar (wxKeyEvent &event)
	{
		if (ppCanvas)
			ppCanvas -> OnChar (event);
		return;
	}
	
	DECLARE_EVENT_TABLE ()

}; /* class PPFrame */

PPFrame *ppFrame = (PPFrame *) NULL;

// --------------------------------------------------

enum
{
	ID_Quit = 1,
	ID_SaveBMP,
	ID_SavePNG,
	ID_About,
	ID_HelpGeneral,
	ID_HelpSpecific,
	ID_ShowWireFrame,
	ID_ShowFog,
	ID_Perspective,
	ID_BgColor,
	ID_ShowScaling,
	ID_ShowKeys,
	ID_ShowMouse,
	ID_ShowRefresh
};

BEGIN_EVENT_TABLE (PPFrame, wxFrame)
	EVT_MENU (ID_Quit, PPFrame::OnQuit)
	EVT_MENU (ID_SaveBMP, PPFrame::OnSaveBMP)
	EVT_MENU (ID_SavePNG, PPFrame::OnSavePNG)
	EVT_MENU (ID_HelpGeneral, PPFrame::OnHelpGeneral)
	EVT_MENU (ID_HelpSpecific, PPFrame::OnHelpSpecific)
	EVT_MENU (ID_About, PPFrame::OnAbout)
	EVT_MENU (ID_ShowWireFrame, PPFrame::OnShowWireFrame)
	EVT_MENU (ID_ShowFog, PPFrame::OnShowFog)
	EVT_MENU (ID_Perspective, PPFrame::OnPerspective)
	EVT_MENU (ID_BgColor, PPFrame::OnBgColor)
	EVT_MENU (ID_ShowScaling, PPFrame::OnShowScaling)
	EVT_MENU (ID_ShowKeys, PPFrame::OnShowKeys)
	EVT_MENU (ID_ShowMouse, PPFrame::OnShowMouse)
	EVT_MENU (ID_ShowRefresh, PPFrame::OnShowRefresh)
	EVT_SIZE (PPFrame::OnSize)
#ifndef __WXGTK__
	EVT_CHAR (PPFrame::OnChar)
#endif
	EVT_CLOSE (PPFrame::OnClose)
END_EVENT_TABLE ()


#ifdef __WXGTK__
// --------------------------------------------------
// ------------------- PP Panel -------------------
// --------------------------------------------------
// This is a panel that receives keyboard events in GTK.

class PPPanel: public wxPanel
{
public:
	PPPanel (wxWindow *parent): wxPanel (parent, -1,
		wxPoint (0, 0), wxSize (0, 0)) {return;}

	void OnChar (wxKeyEvent &event)
	{
		if (ppCanvas)
			ppCanvas -> OnChar (event);
		return;
	}
	
	DECLARE_EVENT_TABLE ()

}; /* class PPPanel */

PPPanel *ppPanel = (PPPanel *) NULL;

// --------------------------------------------------

BEGIN_EVENT_TABLE (PPPanel, wxPanel)
	EVT_CHAR (PPPanel::OnChar)
END_EVENT_TABLE ()

#endif // __WXGTK__


// --------------------------------------------------
// ---------------- wxGL Application ----------------
// --------------------------------------------------
// This is the main application.

class PPApp: public wxApp
{
virtual bool OnInit ();
virtual int OnExit ();

}; /* class PPApp */

// --------------------------------------------------

IMPLEMENT_APP (PPApp)


// --------------------------------------------------
// ---------------- wxGL Main Frame -----------------
// --------------------------------------------------

PPFrame::PPFrame (wxWindow *parent, const wxString& title,
	const wxPoint& pos, const wxSize& size):
	wxFrame ((wxWindow *) NULL, -1, title, pos, size)
{
	// switches for showing additional info
	showScaling = false;
	showKeys = false;
	showMouse = false;
	showRefresh = false;

	// StatusBar
	CreateStatusBar ();
	SetStatusText (defaultStatusText);

	// the File menu
	wxMenu *menuFile = new wxMenu;
	menuFile -> Append (ID_SaveBMP, wxT("Save &BMP"),
		wxT("Save the image to a BMP file"));
	menuFile -> Append (ID_SavePNG, wxT("Save &PNG"),
		wxT("Save the image to a PNG file"));
	menuFile -> AppendSeparator ();
	menuFile -> Append (ID_Quit, wxT("E&xit"),
		wxT("Exit the program"));

	// the Options menu
	wxMenu *menuOptions = new wxMenu;
	menuOptions -> AppendCheckItem (ID_ShowWireFrame, wxT("&Wireframe"),
		wxT("Show the scene as wireframe"));
	menuOptions -> Check (ID_ShowWireFrame, wireframe);
	menuOptions -> AppendCheckItem (ID_ShowFog, wxT("&Fog"),
		wxT("Add the fog effect to the scene"));
	menuOptions -> Check (ID_ShowFog, fog);
	menuOptions -> AppendCheckItem (ID_Perspective, wxT("&Perspective"),
		wxT("Use perspective in the scene"));
	menuOptions -> Check (ID_Perspective, perspective);
	menuOptions -> AppendSeparator ();
	menuOptions -> Append (ID_BgColor, wxT("&Background"),
		wxT("Change background color"));

	// the Info menu
	wxMenu *menuInfo = new wxMenu;
	menuInfo -> AppendCheckItem (ID_ShowScaling, wxT("&Magnification"),
		wxT("Show the scaling exponent"));
	menuInfo -> AppendCheckItem (ID_ShowKeys, wxT("&Keys"),
		wxT("Show the keys pressed"));
	menuInfo -> AppendCheckItem (ID_ShowMouse, wxT("&Mouse"),
		wxT("Show the mouse position and status"));
	menuInfo -> AppendCheckItem (ID_ShowRefresh, wxT("&Refresh"),
		wxT("Show the screen refresh counter"));

	// the Help menu
	wxMenu *menuHelp = new wxMenu;
	menuHelp -> Append (ID_HelpGeneral, wxT("&General..."),
		wxT("Show general help information"));
	if (helpinfo && *helpinfo)
		menuHelp -> Append (ID_HelpSpecific, wxT("&Specific..."),
		wxT("Show specific help information"));
	menuHelp -> AppendSeparator ();
	menuHelp -> Append (ID_About, wxT("&About..."),
		wxT("About the program"));

	// the menu bar
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar -> Append (menuFile, wxT("&File"));
	menuBar -> Append (menuOptions, wxT("&Options"));
	menuBar -> Append (menuInfo, wxT("&Info"));
	menuBar -> Append (menuHelp, wxT("&Help"));

	SetMenuBar (menuBar);

	// recalculate the new size
//	SendSizeEvent ();

	// show the frame
//	Show (TRUE);
	
	// other initializations

	// paint the initial screen
//	Refresh ();
	
	return;
} /* PPFrame::PPFrame */

PPFrame::~PPFrame ()
{
	return;
} /* PPFrame::~PPFrame */

void PPFrame::OnQuit (wxCommandEvent& WXUNUSED (event))
{
	Close (TRUE);
	return;
} /* PPFrame::OnQuit */

void PPFrame::Save (const wxString &files, wxBitmapType type, bool askfile)
{
	// add PNG image handler if necessary
	static bool added = false;
	if (!added)
	{
		#if wxUSE_LIBPNG
		wxImage::AddHandler (new wxPNGHandler);
		#endif
		added = true;
	}

	// show a dialog box and ask for a file name to save the image to
	wxFileDialog *dlg = new wxFileDialog (this, wxT("Save bitmap"),
		wxT(""), wxT(""), files, wxSAVE | wxOVERWRITE_PROMPT);
	if (askfile && (dlg -> ShowModal () != wxID_OK))
	{
		dlg -> Destroy ();
		return;
	}

	// capture the OpenGL graphics to a bitmap buffer
	GLint viewport [4];	// viewport: x, y, width, height
	glGetIntegerv (GL_VIEWPORT, viewport);
	int width = viewport [2];
	int height = viewport [3];
	int RGBwidth = width * 3;
	int BMPwidth = (RGBwidth + 3) & ~3;
	int BMPsize = BMPwidth * height;
	// BMPbuf must be allocated with "malloc": it is free'd in wxImage
	char *BMPbuf = (char *) malloc (BMPsize);
	if (!BMPbuf)
	{
		wxMessageBox (wxT("Can't allocate memory for the bitmap."),
			wxT("Not saved"), wxOK, this);
		dlg -> Destroy ();
		return;
	}
	glPixelStorei (GL_PACK_ALIGNMENT, 4);
	glPixelStorei (GL_PACK_ROW_LENGTH, 0);
	glPixelStorei (GL_PACK_SKIP_ROWS, 0);
	glPixelStorei (GL_PACK_SKIP_PIXELS, 0);
	glReadPixels (0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, BMPbuf);

	// remove gaps from the buffer if there are any
	if (BMPwidth != RGBwidth)
	{
		int height = viewport [3];
		char *src = BMPbuf + BMPwidth, *dst = BMPbuf + RGBwidth;
		if (!height)
			++ height;
		while (-- height)
		{
			for (int i = 0; i < RGBwidth; i ++)
				*(dst ++) = *(src ++);
			src += BMPwidth - RGBwidth;
		}
	}

	// turn the picture in the buffer upside down
	char *row1 = BMPbuf;
	char *row2 = BMPbuf + (height - 1) * RGBwidth;
	while (row1 < row2)
	{
		for (int i = 0; i < RGBwidth; i ++)
		{
			char tmp = *row1;
			*(row1 ++) = *row2;
			*(row2 ++) = tmp;
		}
		row2 -= RGBwidth;
		row2 -= RGBwidth;
	}

	// transform the buffer into a system-independent RGB image
	// note: BMPbuf will be free'd in the wxImage destructor
	wxImage BMPimg (width, height, (unsigned char *) BMPbuf, false);
	if (!BMPimg. Ok ())
	{
		wxMessageBox (wxT("Cannot create a wxImage from raw data."),
			wxT("Not saved"), wxOK, this);
		dlg -> Destroy ();
		return;
	}

	// save the bitmap image to a file
	bool result = BMPimg. SaveFile (askfile ? dlg -> GetPath (): files,
		type);
	if (!result)
	{
		wxMessageBox (wxT("Unable to save - an error occurred."),
			wxT("Not saved"), wxOK, this);
	}

	dlg -> Destroy ();
	return;
} /* PPFrame::Save */

void PPFrame::OnSaveBMP (wxCommandEvent& WXUNUSED (event))
{
	Save (wxT("Bitmap files (*.bmp)|*.bmp|All files (*.*)|*.*"),
		wxBITMAP_TYPE_BMP, true);
	return;
} /* PPFrame::OnSaveBMP */

void PPFrame::OnSavePNG (wxCommandEvent& WXUNUSED (event))
{
	Save (wxT("PNG bitmap files (*.png)|*.png|All files (*.*)|*.*"),
		wxBITMAP_TYPE_PNG, true);
	return;
} /* PPFrame::OnSavePNG */

void PPFrame::OnShowWireFrame (wxCommandEvent& event)
{
	if (event. IsChecked ())
		wireframe = true;
	else
		wireframe = false;

	ppCanvas -> Refresh ();
	return;
} /* PPFrame::OnShowWireFrame */

void PPFrame::OnShowFog (wxCommandEvent& event)
{
	if (event. IsChecked ())
		fog = true;
	else
		fog = false;

	ppCanvas -> Refresh ();
	return;
} /* PPFrame::OnShowFog */

void PPFrame::OnPerspective (wxCommandEvent& event)
{
	if (event. IsChecked ())
		perspective = true;
	else
		perspective = false;

	ppCanvas -> Rescale (0, 0, 0);
	ppCanvas -> Refresh ();
	return;
} /* PPFrame::OnPerspective */

inline unsigned char float2color (float x)
{
	return (unsigned char) (x * 255);
} /* float2color */

inline float color2float (unsigned char x)
{
	return (float) (x / 255.0);
} /* color2float */

void PPFrame::OnBgColor (wxCommandEvent& WXUNUSED (event))
{
	wxColor bgColor (float2color (bgcolor [0]),
		float2color (bgcolor [1]), float2color (bgcolor [2]));
	wxColourData cd;
	cd. SetColour (bgColor);
	wxColourDialog *dlg = new wxColourDialog (this, &cd);
	if (dlg -> ShowModal () == wxID_OK)
	{
		cd = dlg -> GetColourData ();
		bgColor = cd. GetColour ();
		bgcolor [0] = color2float (bgColor. Red ());
		bgcolor [1] = color2float (bgColor. Green ());
		bgcolor [2] = color2float (bgColor. Blue ());
		ppCanvas -> Refresh ();
	}
	dlg -> Destroy ();
	return;
} /* PPFrame::OnBgColor */

inline void showScalingStatus (void)
{
	wxString msg;
	msg. Printf (wxT("Magnification exponent = %.2f."), magnification);
	ppFrame -> SetStatusText (msg);
	return;
} /* showScalingStatus */

void PPFrame::OnShowScaling (wxCommandEvent& event)
{
	if (event. IsChecked ())
	{
		showScaling = true;
		showScalingStatus ();
	}
	else
	{
		showScaling = false;
		SetStatusText (defaultStatusText);
	}
	return;
} /* PPFrame::OnShowScaling */

void PPFrame::OnShowKeys (wxCommandEvent& event)
{
	if (event. IsChecked ())
		showKeys = true;
	else
	{
		showKeys = false;
		SetStatusText (defaultStatusText);
	}
	return;
} /* PPFrame::OnShowKeys */

void PPFrame::OnShowMouse (wxCommandEvent& event)
{
	if (event. IsChecked ())
		showMouse = true;
	else
	{
		showMouse = false;
		SetStatusText (defaultStatusText);
	}
	return;
} /* PPFrame::OnShowMouse */

void PPFrame::OnShowRefresh (wxCommandEvent& event)
{
	if (event. IsChecked ())
		showRefresh = true;
	else
	{
		showRefresh = false;
		SetStatusText (defaultStatusText);
	}
	return;
} /* PPFrame::OnShowRefresh */

void PPFrame::OnAbout (wxCommandEvent& WXUNUSED (event))
{
	wxMessageBox (wxString (aboutinfo, wxConvUTF8),
		wxT("About the Program"), wxOK | wxICON_INFORMATION, this);
	return;
} /* PPFrame::OnAbout */

void PPFrame::OnHelpGeneral (wxCommandEvent& WXUNUSED (event))
{
	wxMessageBox(wxString (proginfo, wxConvUTF8),
		wxT("General Help"), wxOK | wxICON_INFORMATION, this);
	return;
} /* PPFrame::OnHelpGeneral */

void PPFrame::OnHelpSpecific (wxCommandEvent& WXUNUSED (event))
{
	wxMessageBox(wxString (helpinfo, wxConvUTF8),
		wxT("Specific Help"), wxOK | wxICON_INFORMATION, this);
	return;
} /* PPFrame::OnHelpSpecific */

void PPFrame::OnSize (wxSizeEvent& event)
{
	// call the original handler for this event
	wxFrame::OnSize (event);

#ifdef __WXGTK__
	// in GTK the canvas has to be resized explicitly
	int w, h;
	GetClientSize (&w, &h);
	if (ppCanvas)
		ppCanvas -> SetSize (w, h);
#endif
	return;
} /* PPFrame::OnSize */

void PPFrame::OnClose (wxCloseEvent& WXUNUSED (event))
{
	Destroy ();
	return;
} /* PPFrame::OnClose */


// --------------------------------------------------
// --------------- matrix operations ----------------
// --------------------------------------------------

inline void makeIdentity (float m [16])
{
	m [ 0] = 1.0f;
	m [ 1] = 0.0f;
	m [ 2] = 0.0f;
	m [ 3] = 0.0f;

	m [ 4] = 0.0f;
	m [ 5] = 1.0f;
	m [ 6] = 0.0f;
	m [ 7] = 0.0f;

	m [ 8] = 0.0f;
	m [ 9] = 0.0f;
	m [10] = 1.0f;
	m [11] = 0.0f;

	m [12] = 0.0f;
	m [13] = 0.0f;
	m [14] = 0.0f;
	m [15] = 1.0f;

	return;
} /* makeIdentity */

inline void multMatrix (const float a [16], const float b [16],
	float result [16])
// Multiplicate the given two 4x4 matrices.
{
	int i, j, k;
	for (i = 0; i < 4; ++ i)
		for (j = 0; j < 4; ++ j)
		{
			result [(i << 2) + j] = 0.0f;
			for (k = 0; k < 4; ++ k)
				result [(i << 2) + j] +=
					a [(i << 2) + k] * b [(k << 2) + j];
		}
	return;
} /* multMatrix */

inline void copyMatrix (float dst [16], float src [16])
{
	dst [ 0] = src [ 0];
	dst [ 1] = src [ 1];
	dst [ 2] = src [ 2];
	dst [ 3] = src [ 3];
	dst [ 4] = src [ 4];
	dst [ 5] = src [ 5];
	dst [ 6] = src [ 6];
	dst [ 7] = src [ 7];
	dst [ 8] = src [ 8];
	dst [ 9] = src [ 9];
	dst [10] = src [10];
	dst [11] = src [11];
	dst [12] = src [12];
	dst [13] = src [13];
	dst [14] = src [14];
	dst [15] = src [15];
	return;
} /* copyMatrix */


// --------------------------------------------------
// ---------------- wxGL Main Canvas ----------------
// --------------------------------------------------

inline int sgn (int x)
{
	if (x < 0)
		return -1;
	else if (x > 0)
		return 1;
	else
		return 0;
} /* sgn */

void resizePicture (int width, int height)
// This function is called on canvas resize and when the magnification
// exponent, the translation vector or the projection mode have changed.
{
	// initialize the projection matrices
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	// calculate the correct perspective projection
	double w, h;
	if (!width)
		width = 1;
	if (!height)
		height = 1;
	if (width < height)
	{
		w = exp (-magnification - (perspective ? 0.45 : -0.2));
		h = w * height / width;
	}
	else
	{
		h = exp (-magnification - (perspective ? 0.45 : -0.2));
		w = h * width / height;
	}

	// define the perspective projection matrix
	if (perspective)
		glFrustum (-w - translation [0], w - translation [0],
			-h - translation [1], h - translation [1], 2.0, 6.0);
	else
		glOrtho (-w - translation [0], w - translation [0],
			-h - translation [1], h - translation [1], 2.0, 6.0);
	glTranslatef (0.0f, 0.0f, -4.75f);

	// prepare and reset the model view matrix
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();

	return;
} /* resizePicture */

inline void PPCanvas::Rotate (int x, int y, float degrees)
{
	float step = exp (-magnification - 4);
	if (degrees)
	{
		x = sgn (x);
		y = sgn (y);
		step = degrees * 3.1415926f / 180;
	}

	float newMatrix [16];
	if (x)
	{
		double cos_alpha = std::cos (x * step);
		double sin_alpha = std::sin (x * step);
		float rot_alpha [16];
		makeIdentity (rot_alpha);
		rot_alpha [ 0] = cos_alpha;
		rot_alpha [ 2] = -sin_alpha;
		rot_alpha [ 8] = sin_alpha;
		rot_alpha [10] = cos_alpha;
		multMatrix (rotation, rot_alpha, newMatrix);
	}
	
	if (y)
	{
		double cos_beta = std::cos (y * step);
		double sin_beta = std::sin (y * step);
		float rot_beta [16];
		makeIdentity (rot_beta);
		rot_beta [ 5] = cos_beta;
		rot_beta [ 6] = sin_beta;
		rot_beta [ 9] = -sin_beta;
		rot_beta [10] = cos_beta;
		if (x)
			multMatrix (newMatrix, rot_beta, rotation);
		else
			multMatrix (rotation, rot_beta, newMatrix);
	}

	if ((x && !y) || (!x && y))
		copyMatrix (rotation, newMatrix);
	
	return;
} /* PPCanvas::Rotate */

inline void PPCanvas::Rescale (int diff, int x, int y)
{
	if (diff)
		magnification += 0.01 * diff;

	if (x || y)
	{
		float step = exp (-magnification - 6);
		if (x)
			translation [0] += x * step;
		if (y)
			translation [1] -= y * step;
	}
	
	int w, h;
	GetClientSize (&w, &h);
	SetCurrent ();
	glViewport (0, 0, (GLint) w, (GLint) h);

	resizePicture (w, h);

	// show the new magnification exponent if necessary
	if (ppFrame -> showScaling)
		showScalingStatus ();

	return;
} /* PPCanvas::Rescale */

PPCanvas::PPCanvas (wxWindow *parent, wxWindowID id,
	const wxPoint &pos, const wxSize &size, long style,
	const wxString &name, int *gl_attrib):
	wxGLCanvas (parent, id, pos, size, style, name, gl_attrib),
	parentWindow (parent)
{
	// show the parent frame on the screen
	parent -> Show (TRUE);

	// convert the arguments from unicode to the usual characters
	// and interpret the "--script" argument at the same time
	int argc = wxTheApp -> argc;
	char **argv = new char * [argc];
	wxChar **wxArgv = wxTheApp -> argv;
	int skipped = 0;
	for (int i = 0; i < argc; ++ i)
	{
		wxString arg (wxArgv [i]);
		if (arg. StartsWith (wxT("--script"), &scriptName))
		{
			++ skipped;
			if (scriptName. empty () && (i + 1 < argc))
			{
				scriptName = wxArgv [i + 1];
				++ i;
				++ skipped;
			}
		}
		else
		{
			argv [i - skipped] = new char [arg. length () + 1];
			std::strcpy (argv [i - skipped], arg. mb_str ());
		}
	}
	argc -= skipped;

	// resize the picture - needed to get rid of scrollbars
	// in the x11 version of wxWidgets,
	// tested on OS/X 10.5.4 (Intel) and SunOS 5.8 (Sparc)
#ifdef __WXX11__
	this -> SetSize (size. x - 1, size. y);
	this -> SetSize (size. x, size. y);
#endif

	// initialize the picture
	SetCurrent ();
	glViewport (0, 0, (GLint) (size. x), (GLint) (size. y));

	// enable OpenGL lighting
	glEnable (GL_LIGHTING);

	// light number 0
	static GLfloat Ambient0 [4] = { 0.0, 0.2, 0.3, 1.0};
	static GLfloat Diffuse0 [4] = { 0.3, 0.3, 0.5, 1.0};
	static GLfloat Specular0 [4] = { 0.3, 0.3, 1.0, 1.0};
	static GLfloat Position0 [4] = { 0.0, 1.0, 1.5, 0.0};
	glLightfv (GL_LIGHT0, GL_AMBIENT, Ambient0);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, Diffuse0);
	glLightfv (GL_LIGHT0, GL_SPECULAR, Specular0);
	glLightfv (GL_LIGHT0, GL_POSITION, Position0);
	glEnable (GL_LIGHT0);

	// light number 1
	static GLfloat Ambient1 [4] = { 0.3, 0.1, 0.0, 1.0};
	static GLfloat Diffuse1 [4] = { 0.5, 0.3, 0.3, 1.0};
	static GLfloat Specular1 [4] = { 1.0, 0.3, 0.3, 1.0};
	static GLfloat Position1 [4] = { 1.5, 1.0, 0.0, 0.0};
	glLightfv (GL_LIGHT1, GL_AMBIENT, Ambient1);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, Diffuse1);
	glLightfv (GL_LIGHT1, GL_SPECULAR, Specular1);
	glLightfv (GL_LIGHT1, GL_POSITION, Position1);
	glEnable (GL_LIGHT1);

	// light number 7 (used only for the wireframe display)
	static GLfloat Ambient7 [4] = { 1.0, 1.0, 1.0, 1.0};
	glLightfv (GL_LIGHT7, GL_AMBIENT, Ambient7);
	glDisable (GL_LIGHT7);

	// enable the depth test
	glEnable (GL_DEPTH_TEST);
	
	// enable colors of plotted objects
	glEnable (GL_COLOR_MATERIAL);

	// define the fog in the scene
	glFogi (GL_FOG_MODE, GL_EXP2);
	glFogf (GL_FOG_DENSITY, 0.4);
	GLfloat fogColor [] = {1.0, 1.0, 1.0, 1.0};
	glFogfv (GL_FOG_COLOR, fogColor);

	// initialize the user-specific aspects of the picture
	initPicture (argc, argv);

	// clean up the memory allocated for the command-line arguments
	for (int i = 0; i < argc; ++ i)
		delete [] argv [i];
	delete [] argv;

	// initialize the rotation matrix
	makeIdentity (rotation);
	if (scriptName. empty ())
		Rotate (37, 22);

	// set the right size of the scene projections etc.
	resizePicture (size. x, size. y);

	// process the script if necessary
	if (!scriptName. empty ())
		ProcessScript (scriptName);

	return;
} /* PPCanvas::PPCanvas */

PPCanvas::~PPCanvas ()
{
	return;
} /* PPCanvas::~PPCanvas */

void PPCanvas::OnChar (wxKeyEvent& event)
{
	// read the code of the key pressed
	int ch = event. GetKeyCode ();

	// show the information on the key if requested to
	if (ppFrame -> showKeys)
	{
		wxString msg;
		msg. Printf (wxT("Key: %d (%c)."), ch, (char) ch);
		ppFrame -> SetStatusText (msg);
	}

	// interprete the key
	switch (ch)
	{
		case WXK_ESCAPE:
			parentWindow -> Destroy ();
		//	wxTheApp -> ExitMainLoop ();
			break;
		case WXK_LEFT:
			Rotate (-1, 0, 7.5);
			Refresh ();
			break;
		case WXK_RIGHT:
			Rotate (1, 0, 7.5);
			Refresh ();
			break;
		case WXK_UP:
			Rotate (0, -1, 7.5);
			Refresh ();
			break;
		case WXK_DOWN:
			Rotate (0, 1, 7.5);
			Refresh ();
			break;
		case ' ':
			Rotate (1, 0, 7.5);
			Refresh ();
			break;
		case '+':
			Rescale (10, 0, 0);
			Refresh ();
			break;
		case '-':
			Rescale (-10, 0, 0);
			Refresh ();
			break;
		case 's':
		case 'S':
			Rescale (0, 0, 15);
			Refresh ();
			break;
		case 'w':
		case 'W':
			Rescale (0, 0, -15);
			Refresh ();
			break;
		case 'a':
		case 'A':
			Rescale (0, -15, 0);
			Refresh ();
			break;
		case 'd':
		case 'D':
			Rescale (0, 15, 0);
			Refresh ();
			break;
		case '0':
		case 'i':
		case 'I':
		case WXK_HOME:
			translation [0] = 0.0f;
			translation [1] = 0.0f;
			translation [2] = 0.0f;
			magnification = 0.0;
			Rescale (0, 0, 0);
			makeIdentity (rotation);
			if (ch != '0')
				Rotate (37, 22);
			Refresh ();
			break;
		case 'p':
			ProcessScript (scriptName);
			break;
		default:
			event. Skip ();
			break;
	}
} /* PPCanvas::OnChar */

void PPCanvas::PaintPicture ()
{
	wxPaintDC dc (this);

	// begin drawing the picture
	SetCurrent ();

	// clear the picture buffers
	glClearColor (bgcolor [0], bgcolor [1], bgcolor [2], bgcolor [3]);
	glClearDepth(1.0f);				// Depth Buffer Setup
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// call the user's procedure for drawing the picture
	// load the rotation matrix to the model view
	glLoadIdentity ();
	glTranslatef (0.0f, 0.0f, 0.75f);
	glMultMatrixf (rotation);

	// set the wireframe or full picture
	if (wireframe)
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
		// turn on the full-brightness ambient light
		glEnable (GL_LIGHT7);
		// turn off all the other lights
		glDisable (GL_LIGHT0);
		glDisable (GL_LIGHT1);
	}
	else
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		// turn off the wireframe light
		glDisable (GL_LIGHT7);
		// turn on all the other lights
		glEnable (GL_LIGHT0);
		glEnable (GL_LIGHT1);
	}

	// make the fog visible or invisible
	if (fog)
		glEnable (GL_FOG);
	else
		glDisable (GL_FOG);
	
	// draw interior faces for wireframe only
	if (wireframe)
		glDisable (GL_CULL_FACE);
	else
		glEnable (GL_CULL_FACE);

	drawPicture ();

	// finish drawing and show the picture
	glFinish ();
	SwapBuffers ();

	return;
} /* PPCanvas::PaintPicture */

void PPCanvas::OnPaint (wxPaintEvent& WXUNUSED (event))
{
	// paint the current picture
	PaintPicture ();

	// count the refresh events
	static int counter = 0;
	++ counter;

	// show status info if requested to
	if (ppFrame -> showRefresh)
	{
		wxString msg;
		msg. Printf (wxT("Repaint window: %d"), counter);
		ppFrame -> SetStatusText (msg);
	}
	
	return;
} /* PPCanvas::OnPaint */

void PPCanvas::OnSize (wxSizeEvent& event)
{
	// this is also necessary to update the context on some platforms
	wxGLCanvas::OnSize (event);

	// set GL viewport (not called by wxGLCanvas::OnSize on all platforms)
	// also recalculate the perspective and refresh the painting
	Rescale (0, 0, 0);
	Refresh ();
	
	return;
} /* PPCanvas::OnSize */

void PPCanvas::OnMouseEvents (wxMouseEvent& event)
{
	// previous mouse position
	static int mouse_x = 0, mouse_y = 0;
	
	// button states (left, right, middle)
	static bool rotating = false;
	static bool moving = false;
	static bool scaling = false;

	// show mouse information in the status bar if requested to
	if (ppFrame -> showMouse)
	{
		wxString msg;
		msg. Printf (wxT("Mouse: %d, %d. Buttons: left is %s, ")
			wxT("right is %s, middle is %s."),
			event. GetX (), event. GetY (),
			event. LeftIsDown () ? wxT("down") : wxT("up"),
			event. RightIsDown () ? wxT("down") : wxT("up"),
			event. MiddleIsDown () ? wxT("down") : wxT("up"));
		ppFrame -> SetStatusText (msg);
	}
	
	// should the screen be repainted after the mouse operations?
	bool refresh = false;
	
	// interprete the left mouse button event
	if (event. LeftIsDown ())
	{
		if (!rotating)
			rotating = true;
		else if ((event. GetX () != mouse_x) ||
			(event. GetY () != mouse_y))
		{
			Rotate (event. GetX () - mouse_x,
				event. GetY () - mouse_y);
			refresh = true;
		}
	}
	else
		rotating = false;

	// interprete the right mouse button event
	if (event. RightIsDown ())
	{
		if (!moving)
			moving = true;
		else if ((event. GetX () != mouse_x) ||
			(event. GetY () != mouse_y))
		{
		//	Rescale (mouse_y - event. GetY (), 0, 0);
			Rescale (0, event. GetX () - mouse_x,
				event. GetY () - mouse_y);
			refresh = true;
		}
	}
	else
		moving = false;

	// interprete the middle mouse button event
	if (event. MiddleIsDown ())
	{
		if (!scaling)
			scaling = true;
		else if (event. GetY () != mouse_y)
		{
			Rescale (mouse_y - event. GetY (), 0, 0);
			refresh = true;
		}
	}
	else
		scaling = false;

	// remember this mouse position for future use
	mouse_x = event. GetX ();
	mouse_y = event. GetY ();

	// refresh the picture if anything changed
	if (refresh)
		Refresh ();
		
#ifdef __WXGTK__
	// restore the focus for the panel, because the panel seems
	// to lose it and cannot capture the keyboard events
	ppPanel -> SetFocus ();
#endif

	return;
} /* PPCanvas::OnMouseMove */

void PPCanvas::OnEraseBackground (wxEraseEvent &WXUNUSED(event))
{
	// do nothing, to avoid flashing
	return;
} /* PPCanvas::OnEraseBackground */


// --------------------------------------------------
// --------------- script processing ----------------
// --------------------------------------------------

inline void PPCanvas::ProcessScript (const wxString &script)
{
	using chomp::homology::ignorecomments;

	// don't do anything if no script has been selected
	if (script. empty ())
		return;

	// open the script file and ignore initial comments and white chars
	std::ifstream in (script. mb_str ());
	ignorecomments (in);

	// create an output debugging file
	std::ofstream out ("wxglmain.log");

	// interprete all the lines of the script file
	while (!in. eof ())
	{
		// ignore comments and white spaces
		ignorecomments (in);

		// read a line from the script file
		std::string line;
		getline (in, line);

		// split the line into the command and its argument(s)
		std::string::size_type spcPos = line. find (' ');
		std::string command (line, 0, spcPos);
		if (command. empty ())
			continue;
		std::vector<std::string> arguments;
		while (spcPos < std::string::npos)
		{
			int begPos = spcPos;
			spcPos = line. find (' ', begPos + 1);
			std::string argument (line, begPos + 1,
				spcPos - begPos - 1);
			arguments. push_back (argument);
		}
		int argCount = arguments. size ();

		// debug: write what was read
		out << "CMD = '" << command << "'\n";
		for (unsigned int i = 0; i < arguments. size (); ++ i)
			out << "ARG [" << i << "] = '" << arguments [i] <<
				"'\n";
		out << "\n";

		// save the picture to a file if requested to
		if ((command == "save") && (argCount > 0))
		{
			out << "* Saving PNG to '" << arguments [0] << "'\n";
			PaintPicture ();
			ppFrame -> Save (wxString (arguments [0]. c_str (),
				wxConvUTF8), wxBITMAP_TYPE_PNG, false);
		}

		// change the background color
		if ((command == "bgcolor") || (command == "background"))
		{
			if (argCount == 1)
			{
				std::istringstream str (arguments [0]);
				int rgbcolor = 0;
				if (str. peek () == '0')
					str. get ();
				if ((str. peek () == 'x') ||
					(str. peek () == '#'))
				{
					str. get ();
					str >> std::hex;
				}
				str >> rgbcolor;
				bgcolor [0] = color2float
					((rgbcolor >> 16) & 0xFF);
				bgcolor [1] = color2float
					((rgbcolor >> 8) & 0xFF);
				bgcolor [2] = color2float (rgbcolor & 0xFF);
			}
			else if (argCount == 3)
			{
				for (int i = 0; i < 3; ++ i)
				{
					std::istringstream
						str (arguments [i]);
					int color = 0;
					str >> color;
					bgcolor [i] = color2float
						(color & 0xFF);
				}
			}
			out << "* Background color: " << bgcolor [0] <<
				", " << bgcolor [1] << ", " << bgcolor [2] <<
				"\n";
		}

		// set the magnification exponent
		if ((command == "magnification") && (argCount == 1))
		{
			std::istringstream str (arguments [0]);
			magnification = 0;
			str >> magnification;
			out << "* Magnification exponent: " <<
				magnification << "\n";
			Rescale (0, 0, 0);
		}

		// reset rotation angle if requested to
		if ((command == "reset") && (argCount == 1) &&
			(arguments [0] == "rotation"))
		{
			makeIdentity (rotation);
			out << "* Resetting rotation.\n";
		}

		// set a specific rotation angle
		if (command == "rotate")
		{
			float angle [2] = {0, 0};
			for (int i = 0; (i < argCount) && (i < 2); ++ i)
			{
				std::istringstream str (arguments [i]);
				angle [i] = 0;
				str >> angle [i];
			}
			if (angle [0])
				Rotate (1, 0, angle [0]);
			if (angle [1])
				Rotate (0, 1, angle [1]);
			out << "* Rotation: " << angle [0] << ", " <<
				angle [1] << "\n";
		}

		// delay a given number of milliseconds (100 by default)
		if (command == "delay")
		{
			int msec = 100;
			if (argCount > 0)
			{
				std::istringstream str (arguments [0]);
				str >> msec;
			}
			if (msec > 0)
				::wxMilliSleep (msec);
		}

		// show the image on the screen (repaint the screen)
		if ((command == "paint") || (command == "show"))
		{
			PaintPicture ();
		}

		// close the program if requested to
		if ((command == "quit") || (command == "close") ||
			(command == "exit"))
		{
			ppFrame -> Close (TRUE);
			return;
		}
	}
	return;
} /* PPCanvas::ProcessScript */

// --------------------------------------------------
// ---------------- wxGL Application ----------------
// --------------------------------------------------

bool PPApp::OnInit ()
{
#ifdef __WXGTK__
	// set the correct locale for reading the real numbers
	setlocale (LC_NUMERIC, "C");
#endif

	int w0 = 480, h0 = 480;
	ppFrame = new PPFrame ((wxFrame *) NULL,
		wxT("OpenGL in wxWidgets"), wxPoint (-1, -1),
		wxSize (w0, h0) /* wxSize (266 * 2 + 8, 266 * 2 + 66) */);
	int w1, h1;
	ppFrame -> GetClientSize (&w1, &h1);
	ppFrame -> SetSize (-1, -1, w0 + (w0 - w1), h0 + (h0 - h1));
	SetTopWindow (ppFrame);
	ppFrame -> Show (true);

#ifdef __WXGTK__
	// create a panel that will receive keyboard events
	ppPanel = new PPPanel (ppFrame);
	ppPanel -> SetFocus ();
#endif

	// initialize the OpenGL picture within the frame
	int w, h;
	ppFrame -> GetClientSize (&w, &h);
	ppCanvas = new PPCanvas (ppFrame, wxID_ANY, wxPoint (0, 0),
		wxSize (w, h), 0, wxT("PP Canvas"));

	return TRUE; // FALSE iff exit the program now
} /* PPApp::OnInit */

int PPApp::OnExit ()
{
	deletePicture ();
	return 0;
} /* PPApp::OnExit */

#else // wxWindows

#include <iostream>
const char *sorry = "\
-----------------------------------------------------------------------\n\
THIS PROGRAM WAS NOT COMPILED IN THE APPROPRIATE WAY AND WILL NOT WORK.\n\
-----------------------------------------------------------------------\n\
This program must be compiled with the wxWidgets library.\n\
For more information consult the Computational Homology Project website\n\
or ask the program's author directly at http://www.PawelPilarczyk.com/.\n";
#ifdef __BORLANDC__
#include <windows.h>
int PASCAL WinMain (HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main ()
#endif
{
	std::cout << sorry;
	return 0;
}

#endif // wxWindows

