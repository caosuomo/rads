/**************************************************************************

 ptkbitmap.cc

 Created by SYLVAIN BÉRUBÉ
 Date : December 2001

 Modified by SYLVAIN BÉRUBÉ
 Date : January 2002

**************************************************************************/

#include "ptkbitmap.h"

PTKbitmap::PTKbitmap() {
	myNumbercolor = 2;
	myColorred = new int[myNumbercolor];
	myColorgreen = new int[myNumbercolor];
	myColorblue = new int[myNumbercolor];
	myColorred[0] = 0;
	myColorgreen[0] = 0;
	myColorblue[0] = 0;
	myColorred[1] = 255;
	myColorgreen[1] = 255;
	myColorblue[1] = 255;
	myWidth = 1;
	myHeight = 1;
	myPixel = new int[ myWidth * myHeight ];
	myPixel[0] = 0;
}

PTKbitmap::PTKbitmap(const int numbercolor, int* colorred, int* colorgreen, 
			int*colorblue, int width, int height) {
	myNumbercolor = numbercolor;
	myColorred = new int[myNumbercolor];
	myColorgreen = new int[myNumbercolor];
	myColorblue = new int[myNumbercolor];
	for (int i = 0; i < myNumbercolor; i++) {
		myColorred[i] = colorred[i];
		myColorgreen[i] = colorgreen[i];
		myColorblue[i] = colorblue[i];
	}
	myWidth = width;
	myHeight = height;
	myPixel = new int[ myWidth * myHeight ];
	for (int i = 0; i < ( myWidth * myHeight ); i++) {
		myPixel[i] = 0;
	}
}

PTKbitmap::PTKbitmap(const PTKbitmap& bitmap) {
	myNumbercolor = bitmap.myNumbercolor;
	myColorred = new int[bitmap.myNumbercolor];
	myColorgreen = new int[bitmap.myNumbercolor];
	myColorblue = new int[bitmap.myNumbercolor];
	for (int i = 0; i < myNumbercolor; i++) {
		myColorred[i] = bitmap.myColorred[i];
		myColorgreen[i] = bitmap.myColorgreen[i];
		myColorblue[i] = bitmap.myColorblue[i];
	}
	myWidth = bitmap.myWidth;
	myHeight = bitmap.myHeight;
	myPixel = new int[ myWidth * myHeight ];
	for (int i = 0; i < (myWidth * myHeight); i++) {
		myPixel[i] = bitmap.myPixel[i];
	}
}

PTKbitmap::~PTKbitmap() {
	delete[] myColorred;
	delete[] myColorgreen;
	delete[] myColorblue;
	delete[] myPixel;
}

void PTKbitmap::modify_pixel(const int color, const int coordinatex, const int coordinatey) {
	if (color >= myNumbercolor) {
		cout << "Impossible to modify the pixel." << endl;
		cout << " - This bitmat have " << myNumbercolor << " colors." << endl;
		return;
	}
	if ( coordinatex >= myWidth ) {
		cout << "Impossible to modify the pixel." << endl;
		cout << " - The width of this bitmat is " << myWidth << "." << endl;
		return;
	}
	if ( coordinatey >= myHeight ) {
		cout << "Impossible to modify the pixel." << endl;
		cout << " - The height of this bitmat is " << myHeight << "." << endl;
		return;
	}
	myPixel[ ( myWidth * coordinatey ) + coordinatex ] = color;
	return;
}

void PTKbitmap::write(ostream& output) const {
// Variables for the creation of the Bitmap.
	int myFilesize;
	char myHeadersignature[2];
	char myHeadersize[4];
	char myHeaderreserved[4];
	char myHeaderdataoffset[4];
	char myInfoheadersize[4];
	char myInfoheaderwidth[4];
	char myInfoheaderheight[4];
	char myInfoheaderplaned[2];
	char myInfoheaderbitcount[2];
	char myInfoheadercompression[4];
	char myInfoheaderimagesize[4];
	char myInfoheaderxpixels[4];
	char myInfoheaderypixels[4];
	char myInfoheadercolorsused[4];
	char myInfoheadercolorsimportant[4];
	char* myColortablered;
	char* myColortablegreen;
	char* myColortableblue;
	char myColortablereserved = '\0';
	char* myRasterdata;

	myFilesize = 54 + ( myNumbercolor * 4 ) + ( ( ( int( ( myWidth - 1) / 4 ) + 1 ) * 4 ) * myHeight );
	
	myHeadersignature[0] = 'B';
	myHeadersignature[1] = 'M';

	myHeadersize[0] = ( myFilesize %256 );
	myHeadersize[1] = ( int( myFilesize / 256 ) %256 );
	myHeadersize[2] = ( int( myFilesize / 65536 ) %256 );
	myHeadersize[3] = ( int( myFilesize / 16777216 ) %256 );
	
	myHeaderreserved[0] = 0;
	myHeaderreserved[1] = 0;
	myHeaderreserved[2] = 0;
	myHeaderreserved[3] = 0;
	
	myHeaderdataoffset[0] = ( ( 54 + ( myNumbercolor * 4 ) ) %256);
	myHeaderdataoffset[1] = ( int( ( 54 + ( myNumbercolor * 4 ) ) / 256 ) %256 );
	myHeaderdataoffset[2] = 0;
	myHeaderdataoffset[3] = 0;
	
	myInfoheadersize[0] = 40;
	myInfoheadersize[1] = 0;
	myInfoheadersize[2] = 0;
	myInfoheadersize[3] = 0;
	
	myInfoheaderwidth[0] = ( myWidth %256 );
	myInfoheaderwidth[1] = ( int( myWidth / 256 ) %256 );
	myInfoheaderwidth[2] = ( int( myWidth / 65536 ) %256 );
	myInfoheaderwidth[3] = ( int( myWidth / 16777216 ) %256 );
	
	myInfoheaderheight[0] = ( myHeight %256 );
	myInfoheaderheight[1] = ( int( myHeight / 256 ) %256 );
	myInfoheaderheight[2] = ( int( myHeight / 65536 ) %256 );
	myInfoheaderheight[3] = ( int( myHeight / 16777216 ) %256 );
	
	myInfoheaderplaned[0] = 1;
	myInfoheaderplaned[1] = 0;
	
	myInfoheaderbitcount[0] = 8;
	myInfoheaderbitcount[1] = 0;
	
	myInfoheadercompression[0] = 0;
	myInfoheadercompression[1] = 0;
	myInfoheadercompression[2] = 0;
	myInfoheadercompression[3] = 0;
	
	myInfoheaderimagesize[0] = 0;
	myInfoheaderimagesize[1] = 0;
	myInfoheaderimagesize[2] = 0;
	myInfoheaderimagesize[3] = 0;
	
	myInfoheaderxpixels[0] = 0;
	myInfoheaderxpixels[1] = 0;
	myInfoheaderxpixels[2] = 0;
	myInfoheaderxpixels[3] = 0;
	
	myInfoheaderypixels[0] = 0;
	myInfoheaderypixels[1] = 0;
	myInfoheaderypixels[2] = 0;
	myInfoheaderypixels[3] = 0;
	
	myInfoheadercolorsused[0] = myNumbercolor;
	myInfoheadercolorsused[1] = 0;
	myInfoheadercolorsused[2] = 0;
	myInfoheadercolorsused[3] = 0;
 	
	myInfoheadercolorsimportant[0] = 0;
	myInfoheadercolorsimportant[1] = 0;
	myInfoheadercolorsimportant[2] = 0;
	myInfoheadercolorsimportant[3] = 0;
	
	myColortablered = new char[myNumbercolor];
	myColortablegreen = new char[myNumbercolor];
	myColortableblue = new char[myNumbercolor];
	
	for (int i = 0; i < myNumbercolor; i++ ) {
		myColortablered[i] = myColorred[i];
		myColortablegreen[i] = myColorgreen[i];
		myColortableblue[i] = myColorblue[i];
	}
	
	myRasterdata = new char[ ( ( int( ( myWidth - 1 ) / 4 ) + 1 ) * 4 ) * myHeight ];
	for (int i = 0; i < ( ( ( int( ( myWidth - 1 ) / 4 ) + 1 ) * 4 ) * myHeight ); i++) {
		if ( ( i %( ( int( ( myWidth -1 ) / 4 ) + 1 ) * 4 ) ) >= myWidth) {
			myRasterdata[i] = 0;
		}
		else {
			myRasterdata[i] = myPixel[ ( int( i / ( ( int( ( myWidth - 1 ) / 4 ) + 1 ) * 4 ) ) * myWidth )
					+ ( i %( ( int( ( myWidth - 1)  / 4 ) + 1 ) * 4 ) ) ];
		}
	}
	
	for (int i = 0; i < 2; i++) {
		output << myHeadersignature[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myHeadersize[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myHeaderreserved[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myHeaderdataoffset[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myInfoheadersize[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myInfoheaderwidth[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myInfoheaderheight[i];
	}
	for (int i = 0; i < 2; i++) {
		output << myInfoheaderplaned[i];
	}
	for (int i = 0; i < 2; i++) {
		output << myInfoheaderbitcount[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myInfoheadercompression[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myInfoheaderimagesize[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myInfoheaderxpixels[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myInfoheaderypixels[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myInfoheadercolorsused[i];
	}
	for (int i = 0; i < 4; i++) {
		output << myInfoheadercolorsimportant[i];
	}
	for (int i = 0; i < myNumbercolor; i++) {
		output << myColortablered[i];
		output << myColortablegreen[i];
		output << myColortableblue[i];
		output << myColortablereserved;
	}
	for (int i = 0; i < (((int((myWidth-1)/4)+1)*4)*myHeight); i++) {
		output << myRasterdata[i];
	}
}

ostream& operator<<(ostream& output, const PTKbitmap& bitmap) {
	bitmap.write(output);
	return output;
}
