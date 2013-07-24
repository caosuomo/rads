/**************************************************************************

 ptkbitmap.hh

 Created by SYLVAIN BÉRUBÉ
 Date : December 2001

 Modified by SYLVAIN BÉRUBÉ
 Date : January 2002

**************************************************************************/

#ifndef _PTKBITMAP_
#define _PTKBITMAP_

#include <iostream>
using namespace std;

class PTKbitmap {
public:
	PTKbitmap();
	PTKbitmap(const int numbercolor, int* colorred, int* colorgreen, int*colorblue, 
				int width, int height);
	PTKbitmap(const PTKbitmap& bitmap);
	~PTKbitmap();
	void modify_pixel(const int color, const int coordinatex, const int coordinatey);
	void write(ostream& output) const;
protected:
	int myNumbercolor;
	int* myColorred;
	int* myColorgreen;
	int* myColorblue;
	int myWidth;
	int myHeight;
	int* myPixel;
};

ostream& operator<<(ostream& output, const PTKbitmap& bitmap);

#endif


/*

 Number of colors (1-255) & colors.
 Width & height.

*/
