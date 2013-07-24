// This is TEXTFILE.CPP by Pawel Pilarczyk.
// It contains an implementation of the functions declared in "pptextfile.hh".

// Copyright (C) 1997-2000 by Pawel Pilarczyk. All rights reserved.
// Started in December 1997. Last revision: November 17, 2000 (September 5, 2004).


#define _PPTEXTFILE_CC_

#include "pptextfile.h"


// --------------------------------------------------
// -------------------- TEXTFILE --------------------
// --------------------------------------------------

PPtextfile::PPtextfile () {
	myFile = NULL;
	myInputstream = myFile;
	return;
} /* PPtextfile::PPtextfile */

PPtextfile::PPtextfile (istream &input) {
	// remember the pointer to this file
	myFile = NULL;
	myInputstream = &input;

	// reset the other internal variables
	myCurrentline = 0;
	myCurrentchar = '\n';

	return;
} /* PPtextfile::PPtextfile */

PPtextfile::PPtextfile (char *filename) {
	myFile = NULL;
	myInputstream = myFile;
	open( filename );
	return;
} /* PPtextfile::PPtextfile */

PPtextfile::~PPtextfile () {
	close();
	return;
} /* PPtextfile::~PPtextfile */

int PPtextfile::open (char *filename) {
	// open the file only if it has not been opened yet
	if ( myInputstream ) {
		return 1;
	}

	// try to open the file
	myFile = new ifstream;
	if ( myFile ) {
		myFile -> open(filename, ios::in);
	}

	// in the case of failure return -1 and prepare for another trial
	if ( !( myFile)  || !( *myFile) ) {
		if ( myFile ) {
			delete myFile;
		}
		myFile = NULL;
		myInputstream = myFile;
		return -1;
	}

	// initialize the other internal variables
	myInputstream = myFile;
	myCurrentline = 0;
	myCurrentchar = '\n';

	return 0;
} /* PPtextfile::open */

void PPtextfile::close () {
	if ( myFile ) {
		myFile -> close();
		delete myFile;
		myFile = NULL;
	}
	myInputstream = NULL;
	return;
} /* PPtextfile::close */

// --------------------------------------------------

static int space(int currentchar) {
// Return 1 iff the given character is a blank one.
	switch ( currentchar ) {
		case '\n':
		case '\r':
		case '\t':
		case ' ':
			return 1;
		default:
			return 0;
	}
} /* space */

long PPtextfile::linenumber () {
	if ( !myInputstream ) {
		return 0;
	}
	else {
		return myCurrentline;
	}
} /* line number */

void PPtextfile::ignorespaces () {
	if ( !myInputstream ) {
		return;
	}

	while ( 1 ) {
		switch ( myCurrentchar ) {
			case EOF:
				return;
			case ';':
				while ( ( myCurrentchar != '\n' ) && ( myCurrentchar != EOF ) ) {
					myCurrentchar = myInputstream -> get();
				}
				break;
			case '\n':
				myCurrentline++;
			default:
				if ( space( myCurrentchar ) ) {
					myCurrentchar = myInputstream -> get();
				}
				else {
					return;
				}
				break;
		}
	}
} /* PPtextfile::ignorespaces */

int PPtextfile::checkchar (void) {
	if ( !myInputstream ) {
		return EOF;
	}

	ignorespaces();

	return myCurrentchar;
} /* PPtextfile::checkchar */

void PPtextfile::ignoreline () {
	if ( !myInputstream ) {
		return;
	}

	if ( myCurrentchar == EOF ) {
		return;
	}

	if ( myCurrentchar == '\n' ) {
		myCurrentline++;
	}

	myCurrentchar = ';';
	ignorespaces();

	return;
} /* PPtextfile::ignoreline */

int PPtextfile::readchar () {
	if ( !myInputstream ) {
		return EOF;
	}

	int result = checkchar();
	myCurrentchar = myInputstream -> get();

	return result;
} /* PPtextfile::readchar */

long PPtextfile::readnumber (long defaultnumber, int ignorecolons) {
	if ( !myInputstream ) {
		return 0;
	}

	if ( ignorecolons ) {
		while ( ( checkchar() == ':' ) || ( checkchar() == '=' ) || ( checkchar() == ',' ) ) {
			readchar();
		}
	}

	if ( checkchar() == '+' ) {
		readchar();
	}

	int minus = ( checkchar() == '-' );
	if ( minus ) {
		readchar();
	}

	if ( ( checkchar() < '0' ) || ( checkchar() > '9' ) ) {
		return defaultnumber;
	}

	int number = 0;
	while ( ( myCurrentchar >= '0' ) && ( myCurrentchar <= '9' ) ) {
		number = 10 * number + myCurrentchar - '0';
		myCurrentchar = myInputstream -> get();
	}

	return ( minus ? -number : number );
} /* PPtextfile::readnumber */

int PPtextfile::readphrase (char *phrase) {
	if ( !myInputstream ) {
		return 0;
	}

	// find the first non-white character in the phrase
	int pos = 0;
	while ( phrase[pos] && space( phrase[pos] ) ) {
		pos++;
	}

	while ( phrase[pos] ) {
		// read a character from the input file and make it lowercase
		int c = readchar();
		if ( ( c >= 'A' ) && ( c <= 'Z' ) ) {
			c += 'a' - 'A';
		}

		// take a character from the phrase and make it lowercase
		int d = phrase[pos];
		if ( ( d >= 'A' ) && ( d <= 'Z' ) ) {
			d += 'a' - 'A';
		}

		// compare the two characters
		if ( c != d ) {
			cout << "Error (line " << myCurrentline << "): ";
			cout << "\"" << phrase << "\" expected." << endl;
			return -1;
		}

		// take the next non-white character
		pos++;
		while ( phrase[pos] && space( phrase[pos] ) ) {
			pos ++;
		}
	}

	return 0;
} /* PPtextfile::readphrase */

int PPtextfile::readword (char *buf, int maxlength) {
	if ( !myInputstream ) {
		return 0;
	}

	ignorespaces();

	int pos = 0;
	while ( !space( myCurrentchar ) && ( myCurrentchar != EOF ) && ( pos < maxlength - 1 ) ) {
		if ( buf ) {
			buf[pos] = (char) myCurrentchar;
		}
		pos++;
		myCurrentchar = myInputstream -> get();
	}
	if ( buf ) {
		buf[pos] = 0;
	}
	pos++;

	return pos;
} /* PPtextfile::readword */

PPtextfile::operator int (void)
{
	if ( myFile ) {
		return 1;
	}
	else if ( myInputstream ) {
		return 13;
	}
	else {
		return 0;
	}
} /* PPtextfile::operator int */
