// This is PPtextfile.H by Pawel Pilarczyk.
// It contains a definition of a class 'PPtextfile', which allows easy
// and effective reading of data from a text file.

// Copyright (C) 1997-2000 by Pawel Pilarczyk. All rights reserved.
// Started in July 1997. Last revision: April 22, 2000 (September 5, 2004).


#ifndef _PPTEXTFILE_HH_
#define _PPTEXTFILE_HH_

#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

// classes defined within this header file:
class PPtextfile;


// --------------------------------------------------
// -------------------- PPtextfile --------------------
// --------------------------------------------------
// A file for reading text data.
// All blank characters and comments are ignored.

// the maximal length of a phrase read at a time
#define MAXTEXTLENGTH 200

class PPtextfile {
	public:
		// default constructor
		PPtextfile();

		// a constructor for an opened file
		PPtextfile(istream &input);

		// a constructor which opens a file
		PPtextfile(char *filename);

		// default destructor
		~PPtextfile();

		// open the file; return: 1 = already opened, 0 = ok,
		// -1 = an error occurred (no message is displayed)
		int open(char *filename);

		// close the file
		void close();

		// read an integer number;
		// if none can be read, return the suggested default value;
		// if requested, a preceding ':', '=' or ',' may be ignored
		long readnumber(long defaultnumber = 0, int ignorecolons = 0);

		// check the first non-white character waiting to be read;
		// return EOF if end-of-file has been reached
		int checkchar();

		// read one non-white character from the file;
		// return EOF if end-of-file has been reached
		int readchar();

		// read one word from the file and return its actual length
		int readword(char *buf, int maxlength);

		// read a strongly expected phrase from the file (ignore
		// white characters before the phrase and inside it);
		// return: 0 = ok, -1 = not found (show an error message)
		int readphrase(char *phrase);

		// ignore the current line
		void ignoreline();

		// return the current line number
		long linenumber();

		// return the text file's status: 0 = not opened,
		// 1 = opened, 13 = initialized with an input stream
		operator int();


	protected:

		// the character already read and waiting to be interpreted
		int myCurrentchar;

		// current line number
		long myCurrentline;

		// the true file if it was opened
		ifstream *myFile;

		// a pointer to an input stream (usually equal to 'fs')
		istream *myInputstream;

		// ignore white characters and fill in the variable 'myCurrentchar'
		// with the first non-white character
		void ignorespaces();

}; /* class PPtextfile */

#endif // _PPTEXTFILE_HH_
