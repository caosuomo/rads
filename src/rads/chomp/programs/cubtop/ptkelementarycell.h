/**************************************************************************

 ptkelementarycell.hh

 Created by SYLVAIN BÉRUBÉ
 Date : December 2001

**************************************************************************/

#ifndef _PTKELEMENTARYCELL_
#define _PTKELEMENTARYCELL_

#include <iostream>
using namespace std;

class PTKelementarycell {
public:
friend class PTKrepresentableset;
	PTKelementarycell();
	PTKelementarycell(const PTKelementarycell& elemcell);
	PTKelementarycell(const int embedding);
	PTKelementarycell(const int embedding, const int* coordinate, 
				const bool* singlepoint);
	~PTKelementarycell();
	const PTKelementarycell& operator=(const PTKelementarycell& elemcell);
	const bool operator==(const PTKelementarycell& elemcell) const;
	const bool operator!=(const PTKelementarycell& elemcell) const;
	const bool operator<(const PTKelementarycell& elemcell) const;
	const bool operator>(const PTKelementarycell& elemcell) const;
	int get_embedding() const;
	int* get_coordinate() const;
	bool* get_singlepoint() const;
	void modify_coordinate(const int position, const int coordinate, 
				const bool singlepoint);
	void modify_coordinate(const int embedding, const int* coordinate, 
				const bool* singlepoint);
	void write(ostream& output) const;
protected:
	int myEmbedding;
	int myDimension;
	int* myCoordinate;
	bool* mySinglepoint;
};

ostream& operator<<(ostream& output, const PTKelementarycell& elemcell);

#endif

/**************************************************************************

EXPLANATIONS :

int myEmbedding : Embedding number.
int myDimension : Dimension.
int* myCoordinate : Elementary interval open [myEmbedding].
bool* mySinglepoint : Single point [myEmbedding] (nondegenerate = 0, degenerate = 1).

EXAMPLE : 
  if
    A = ]0,1[ x [2] x ]4,5[
  then
    A.myEmbedding = 3,
    A.myDimension = 2,
    A.myCoordinate[0] = 0,
    A.myCoordinate[1] = 2,
    A.myCoordinate[2] = 4,
    A.mySinglepoint[0] = 0,
    A.mySinglepoint[1] = 1,
    A.mySinglepoint[2] = 0.

**************************************************************************/
