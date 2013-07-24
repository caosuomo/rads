/**************************************************************************

 ptkelementarycell.cc

 Created by SYLVAIN BÉRUBÉ
 Date : December 2001

**************************************************************************/

#include "ptkelementarycell.h"

PTKelementarycell::PTKelementarycell() 
:myEmbedding(0), myDimension(0), myCoordinate(0), mySinglepoint(0) {
}

PTKelementarycell::PTKelementarycell(const PTKelementarycell& elemcell) {
	if (elemcell.myEmbedding == 0) {
		myEmbedding = 0;
		myDimension = 0;
		myCoordinate = 0;
		mySinglepoint = 0;
		return;
	}
	myEmbedding = elemcell.myEmbedding;
	myDimension = elemcell.myDimension;
	myCoordinate = new int[myEmbedding];
	mySinglepoint = new bool[myEmbedding];
	for (int i = 0; i < myEmbedding; i++) {
		myCoordinate[i] = elemcell.myCoordinate[i];
		mySinglepoint[i] = elemcell.mySinglepoint[i];
	}
}

PTKelementarycell::PTKelementarycell(const int embedding) {
	if (embedding == 0) {
		myEmbedding = 0;
		myDimension = 0;
		myCoordinate = 0;
		mySinglepoint = 0;
		return;
	}
	myEmbedding = embedding;
	myDimension = 0;
	myCoordinate = new int[myEmbedding];
	mySinglepoint = new bool[myEmbedding];
	for (int i = 0; i < myEmbedding; i++) {
		myCoordinate[i] = 0;
		mySinglepoint[i] = 1;
	}
}

PTKelementarycell::PTKelementarycell(const int embedding, const int* coordinate, 
			const bool* singlepoint) {
	if (embedding == 0) {
		myEmbedding = 0;
		myDimension = 0;
		myCoordinate = 0;
		mySinglepoint = 0;
		return;
	}
	myEmbedding = embedding;
	myDimension = embedding;
	myCoordinate = new int[myEmbedding];
	mySinglepoint = new bool[myEmbedding];
	for (int i = 0; i < myEmbedding; i++) {
		myCoordinate[i] = coordinate[i];
		mySinglepoint[i] = singlepoint[i];
		myDimension -= mySinglepoint[i];
	}
}

PTKelementarycell::~PTKelementarycell() {
	delete[] myCoordinate;
	delete[] mySinglepoint;
}

const PTKelementarycell& PTKelementarycell::operator=(const PTKelementarycell& elemcell) {
	if (elemcell.myEmbedding == 0) {
		myEmbedding = 0;
		myDimension = 0;
		myCoordinate = 0;
		mySinglepoint = 0;
		return *this;
	}
	myEmbedding = elemcell.myEmbedding;
	myDimension = elemcell.myDimension;
	myCoordinate = new int[myEmbedding];
	mySinglepoint = new bool[myEmbedding];
	for (int i = 0; i < myEmbedding; i++) {
		myCoordinate[i] = elemcell.myCoordinate[i];
		mySinglepoint[i] = elemcell.mySinglepoint[i];
	}
	return *this;
}

const bool PTKelementarycell::operator==(const PTKelementarycell& elemcell) const {
	if ((myEmbedding != elemcell.myEmbedding) 
				|| (myDimension != elemcell.myDimension)) {
		return 0;
	}
	for (int i = 0; i < myEmbedding; i++) {
		if ((myCoordinate[i] != elemcell.myCoordinate[i]) 
					|| (mySinglepoint[i] != elemcell.mySinglepoint[i])) {
			return 0;
		}
	}
	return 1;
}

const bool PTKelementarycell::operator!=(const PTKelementarycell& elemcell) const {
	return ( !(elemcell == *this) );
}

const bool PTKelementarycell::operator<(const PTKelementarycell& elemcell) const {
	if ( myEmbedding < elemcell.myEmbedding ) {
		return 1;
	}
	else if ( myEmbedding > elemcell.myEmbedding ) {
		return 0;
	}

	if ( myDimension < elemcell.myDimension ) {
		return 1;
	}
	else if ( myDimension > elemcell.myDimension ) {
		return 0;
	}

	for (int i = 0; i < myEmbedding; i++) {
		if ( mySinglepoint[i] > elemcell.mySinglepoint[i] ) {
			return 1;
		}
		else if ( mySinglepoint[i] < elemcell.mySinglepoint[i] ) {
			return 0;
		}

		if ( myCoordinate[i] < elemcell.myCoordinate[i] ) {
			return 1;
		}
		else if ( myCoordinate[i] > elemcell.myCoordinate[i] ) {
			return 0;
		}
	}
	return 0;
}

const bool PTKelementarycell::operator>(const PTKelementarycell& elemcell) const {
	if (*this == elemcell) {
		return 0;
	}
	return ( !(*this < elemcell) );
}

int PTKelementarycell::get_embedding() const {
	return myEmbedding;
}

int* PTKelementarycell::get_coordinate() const {
	if (myEmbedding == 0) {
		return 0;
	}
	int* elemcell = new int[myEmbedding];
	for (int i = 0; i < myEmbedding; i++) {
		elemcell[i] = myCoordinate[i];
	}
	return elemcell;
}

bool* PTKelementarycell::get_singlepoint() const {
	if (myEmbedding == 0) {
		return 0;
	}
	bool* singlepoint = new bool[myEmbedding];
	for (int i = 0; i < myEmbedding; i++) {
		singlepoint[i] = mySinglepoint[i];
	}
	return singlepoint;
}

void PTKelementarycell::modify_coordinate(const int position, const int coordinate, 
			const bool singlepoint) {
	if ( mySinglepoint[position] < singlepoint ) {
		myDimension--;
	}
	if ( mySinglepoint[position] > singlepoint ) {
		myDimension++;
	}
	myCoordinate[position] = coordinate;
	mySinglepoint[position] = singlepoint;
}

void PTKelementarycell::modify_coordinate(const int embedding, const int* coordinate, 
			const bool* singlepoint) {
	PTKelementarycell elemcell(embedding, coordinate, singlepoint);
	*this = elemcell;
	return;
}
	
void PTKelementarycell::write(ostream& output) const {
	for (int i = 0; i < myEmbedding; i++) {
		if ( mySinglepoint[i] == 1 ) {
			output << "(";
			output << myCoordinate[i];
			output << ")";
		}
		else {
			output << "(";
			output << myCoordinate[i];
			output << ",";
			output << myCoordinate[i] + 1;
			output << ")";
		}
		if ( i != ( myEmbedding - 1 ) ) {
			output << " x ";
		}
	}
	return;
}

ostream& operator<<(ostream& output, const PTKelementarycell& elemcell) {
	elemcell.write(output);
	return output;
}
