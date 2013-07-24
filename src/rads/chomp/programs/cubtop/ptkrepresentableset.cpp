/**************************************************************************

 ptkreprensetableset.cc

 Created by SYLVAIN BÉRUBÉ
 Date : December 2001

 Modified by SYLVAIN BÉRUBÉ and ANIK TRAHAN
 Date : January 2002

**************************************************************************/


#include "ptkrepresentableset.h"


PTKrepresentableset::PTKrepresentableset()
:myEmbedding(0) {
}


PTKrepresentableset::PTKrepresentableset(const PTKrepresentableset& repset) {
	myEmbedding = repset.myEmbedding;
	myElemcell = repset.myElemcell;
}


PTKrepresentableset::PTKrepresentableset(const int embedding) {

	// The embedding must have a positive value.
	if ( embedding < 0 ) {
		cout << "ERROR" << endl;
		return;
	}

	myEmbedding = embedding;
}


PTKrepresentableset::~PTKrepresentableset() {
}


const PTKrepresentableset& PTKrepresentableset::operator=(const PTKrepresentableset& repset) {
	// do nothing if trying to assign to self
	if (this == &repset)
		return *this;
	// Copy the representable set.
	myEmbedding = repset.myEmbedding;
	myElemcell = repset.myElemcell;
	return *this;
}


const bool PTKrepresentableset::operator==(const PTKrepresentableset& repset) const {

	// Verify if the embedding of the reprensetable set are the same.
	if ( myEmbedding != repset.myEmbedding ) {
		return 0;
	}

	// Verify if the elementary cell of the representable set are the same.
	return ( myElemcell == repset.myElemcell );
}


const bool PTKrepresentableset::operator!=(const PTKrepresentableset& repset) const {

	// How-to-use-existing-code 101.
	return ( !( *this == repset ) );
}


void PTKrepresentableset::insert(const PTKrepresentableset& repset) {

	// Verify if the embedding of the representable sets are the same.
	if ( myEmbedding != repset.myEmbedding ) {
		return;
	}

	// Insert the representable set.
	myElemcell.insert( repset.myElemcell );
	return;
}


void PTKrepresentableset::insert(const PTKelementarycell& elemcell){

	// Verify if the embedding of the elementary cell is the same 
	//   of the embedding of the reprensetable set.
	if ( myEmbedding != elemcell.myEmbedding ) {
		return;
	}

	// Insert the elementary cell.
	myElemcell.insert( elemcell );
	return;
}


void PTKrepresentableset::remove(const PTKrepresentableset& repset) {

	// Verify if the embedding of the representable sets are the same.
	if ( myEmbedding != repset.myEmbedding ) {
		return;
	}

	// Remove the representable set.
	myElemcell.remove( repset.myElemcell );
}


void PTKrepresentableset::remove(const PTKelementarycell& elemcell) {

	// Verify if the embedding of the elementary cell is the same 
	//   of the embedding of the reprensetable set.
	if ( myEmbedding != elemcell.myEmbedding ) {
		return;
	}

	// Remove the elementary cell.
	myElemcell.remove( elemcell );
}


const bool PTKrepresentableset::contains(const PTKrepresentableset& repset) const {

	// Verify if the embedding of the representable sets are the same.
	if ( myEmbedding != repset.myEmbedding ) {
		return 0;
	}

	// Verify if the representable set contains the representable set.
	return myElemcell.contains( repset.myElemcell );
}


const bool PTKrepresentableset::contains(const PTKelementarycell& elemcell) const {

	// Verify if the embedding of the elementary cell is the same 
	//   of the embedding of the reprensetable set.
	if ( myEmbedding != elemcell.myEmbedding ) {
		return 0;
	}

	// Verify if the representable set contains the elementary cell.
	return myElemcell.contains( elemcell );
}


const int PTKrepresentableset::get_numberelemcell() const {

	// Return the number of elementary cell.
	return myElemcell.get_numberofdata();
}


const PTKelementarycell PTKrepresentableset::get_min() const {

	// Return the minimal elementary cell in the representable set.
	return ( myElemcell.get_min() );
}


const PTKelementarycell PTKrepresentableset::get_max() const {

	// Return the maximal elementary cell in the representable set.
	return ( myElemcell.get_max() );
}


int* PTKrepresentableset::get_coordmin() const {

	// Verify if the representable set is empty.
	if ( isempty() ) {
		return 0;
	}

	PTKrepresentableset repset( *this );
	PTKelementarycell elemcell;
	int* min;
	min = new int[myEmbedding];
	elemcell = repset.get_min();
	for (int i = 0; i < myEmbedding; i++) {
		min[i] = elemcell.myCoordinate[i];
	}
	repset.remove( elemcell );

	// Verify the minimal coordinates for each elementary cells
	//   in the representable set.
	while ( !( repset.isempty() ) ) {
		elemcell = repset.get_min();
		for (int i = 0; i < myEmbedding; i++) {

			// If this coordinate is smaller, min is adjusted.
			if (min[i] > elemcell.myCoordinate[i]) {
				min[i] = elemcell.myCoordinate[i];
			}
		}
		repset.remove( elemcell );
	}
	return min;
}


int* PTKrepresentableset::get_coordmax() const {

	// Verify if the representable set is empty.
	if ( isempty() ) {
		return 0;
	}
	PTKrepresentableset repset( *this );
	PTKelementarycell elemcell;
	int* max;
	max = new int[myEmbedding];
	elemcell = repset.get_min();
	for (int i = 0; i < myEmbedding; i++) {
		max[i] = ( elemcell.myCoordinate[i] + ( elemcell.mySinglepoint[i] + 1 ) %2);
	}
	repset.remove( elemcell );

	// Verify the maximal coordinates for each elementary cells
	//   in the representable set.
	while ( !( repset.isempty() ) ) {
		elemcell = repset.get_min();
		for (int i = 0; i < myEmbedding; i++) {

			// If this coordinate is larger, max is adjusted.
			if ( max[i] < ( elemcell.myCoordinate[i] 
						+ ( elemcell.mySinglepoint[i] + 1 ) %2 ) ) {
				max[i] = ( elemcell.myCoordinate[i] 
							+ ( elemcell.mySinglepoint[i] + 1 ) %2);
			}
		}
		repset.remove( elemcell );
	}
	return max;
}


const bool PTKrepresentableset::isempty() const {
	return ( get_numberelemcell() == 0 );
}


void PTKrepresentableset::empty() {

	// Remove all elements in the representable set.
	myElemcell.empty_tree();
	return;
}


PTKrepresentableset PTKrepresentableset::set_union(const PTKrepresentableset& repset) const {

	// Union of the reprensetable sets.
	PTKrepresentableset repset_union( *this );
	repset_union.insert( repset );
	return ( repset_union );
}


PTKrepresentableset PTKrepresentableset::set_minus(const PTKrepresentableset& repset) const {

	//  Remove the representable set 'repset' from the representable set.
	PTKrepresentableset repset_minus( *this );
	repset_minus.remove( repset );
	return ( repset_minus );
}


PTKrepresentableset PTKrepresentableset::set_intersection(const PTKrepresentableset& repset) const {
	PTKrepresentableset repset_intersection( *this );
	PTKrepresentableset repset_copy( *this );
	PTKelementarycell elemcell;

	// Verify if the elementary cells from the representable set 'repset' is contain
	//   in the representable set.
	while ( !( repset_copy.isempty() ) ) {
		elemcell = repset_copy.get_min();

		// If the elementary cell is not contain in the representable set,
		//   then it is remove.
		if ( !(repset.contains( elemcell ) ) ) {
			repset_intersection.remove( elemcell );
		}
		repset_copy.remove( elemcell );
	}
	return repset_intersection;
}


PTKrepresentableset PTKrepresentableset::set_disjointunion(const PTKrepresentableset& repset) const {

	//  Disjoint union of the representable sets.
	PTKrepresentableset repset_uniondisjoined( set_union( repset ) );
	repset_uniondisjoined.remove( set_intersection( repset ) );
	return ( repset_uniondisjoined );
}


PTKrepresentableset PTKrepresentableset::closedhull() const {

	// Verify if the embedding is null, or if the representable set is empty.
	if ( ( myEmbedding == 0 ) || isempty() ) {
		return *this;
	}
	PTKrepresentableset repset( *this );
	PTKrepresentableset repset_closedhull( *this );
	PTKelementarycell elemcell;

	// Add the closed hull of each elementary cells of the representable set.
	while ( repset.isempty() == 0 ) {
		elemcell = repset.get_min();
		repset_closedhull.closedhull( 0, elemcell );
		repset.remove( elemcell );
	}
	return repset_closedhull;
}


PTKrepresentableset PTKrepresentableset::openhull() const {

	// Verify if the embedding is null, or if the representable set is empty.
	if (( myEmbedding == 0 ) || isempty() ) {
		return *this;
	}
	PTKrepresentableset repset( *this );
	PTKrepresentableset repset_openhull( *this );
	PTKelementarycell elemcell;

	// Add the open hull of each elementary cells of the representable set.
	while ( repset.isempty() == 0 ) {
		elemcell = repset.get_min();
		repset_openhull.openhull( 0, elemcell );
		repset.remove( elemcell );
	}
	return repset_openhull;
}


PTKrepresentableset PTKrepresentableset::get_elemcelldimk(const int dimk) const {

	// Verify if the representable set is empty.
	if ( isempty() ) {
		return ( *this );
	}
	PTKrepresentableset repset_copy( *this );
	PTKrepresentableset repset_elemcelldimk( myEmbedding );
	PTKelementarycell elemcell( myEmbedding );

	// Verify if the elementary cells from the representable set 
	//   is of dimension 'k'.
	while ( !( repset_copy.isempty() ) ) {
		elemcell = repset_copy.get_min();

		// Add it if this is the case.
		if ( elemcell.myDimension == dimk ) {
			repset_elemcelldimk.insert( elemcell );
		}
		else if ( elemcell.myDimension > dimk) {
			break;
		}
		repset_copy.remove( elemcell );
	}
	return ( repset_elemcelldimk );
}


PTKrepresentableset PTKrepresentableset::get_properfaces() const {

	// Verify if the representable set is empty.
	if ( isempty() ) {
		return ( *this );
	}

	// Verify if the representable set is closed. This function can only be
	//   applied to closed representable set.
	if ( *this == closedhull() ) {
		PTKrepresentableset repset_properfaces( myEmbedding );
		PTKrepresentableset repset_copy( *this );
		PTKrepresentableset repset_elemcell( myEmbedding );
		PTKrepresentableset repset_dimk( myEmbedding );
		PTKrepresentableset repset_dimkcopy( myEmbedding );
		int k = get_max().myEmbedding;
		while ( !( repset_copy.isempty() ) ) {
			repset_dimk = repset_copy.get_elemcelldimk( k );
			repset_dimkcopy = repset_dimk;
			while ( !( repset_dimk.isempty() ) ) {
				repset_elemcell.empty();
				repset_elemcell.insert( repset_dimk.get_min() );
				repset_properfaces.insert( repset_elemcell.closedhull().set_minus( repset_elemcell ) );
				repset_dimk.remove( repset_elemcell );
			}
			k--;
			repset_copy.remove( repset_dimkcopy.closedhull() );
		}
		return ( repset_properfaces );
	}
	else {
		cout << "Error" << endl;
		cout << "The operator get_properfaces is applied only for closed figures" << endl;
		throw "ERROR"; // to avoid unpredictable program behavior
	}
}


PTKrepresentableset PTKrepresentableset::get_freefaces() const {

	// Verify if the representable set is empty.
	if ( isempty() ) {
		return ( *this );
	}

	// Verify if the representable set is closed. This function can only be
	//   applied to closed representable set.
	if ( *this == closedhull() ) {
		PTKrepresentableset repset_freefaces( myEmbedding );
		PTKrepresentableset repset_copy( *this );
		PTKrepresentableset repset_cell( myEmbedding );
		PTKrepresentableset repset_dimk( myEmbedding );
		PTKrepresentableset repset_dimkcopy( myEmbedding );
		PTKrepresentableset repset_intersec( myEmbedding );
		int k = get_max().myEmbedding;
		while ( !( repset_copy.isempty() ) ) {
			repset_dimk = repset_copy.get_elemcelldimk( k );
			repset_dimkcopy = repset_dimk;
			while ( !( repset_dimk.isempty() ) ) {
				repset_cell.empty();
				repset_cell.insert( repset_dimk.get_min() );
repset_intersec = repset_intersec.set_union(repset_freefaces.set_intersection(repset_cell.closedhull().set_minus( repset_cell ) ) );
repset_freefaces = repset_freefaces.set_union( repset_cell.closedhull().set_minus( repset_cell ) ) ;
				repset_freefaces = repset_freefaces.set_minus(repset_intersec);
				repset_dimk.remove( repset_cell );
			}
			k--;
			repset_copy.remove( repset_dimkcopy.closedhull() );
		}
		return ( repset_freefaces.closedhull() );
	}
	else {
		cout << "Error" << endl;
		cout << "The operator freefaces is applied only for closed figures";
		cout << endl;
		throw "ERROR"; // to avoid unpredictable program behavior
	}                       
}


PTKrepresentableset PTKrepresentableset::get_maximalfaces() const {

	// Verify if the representable set is empty.
	if ( isempty() ) {
		return ( *this );
	}

	// Verify if the representable set is closed. This function can only be
	//   applied to closed representable set.
	if (*this == closedhull() ) {
		PTKrepresentableset repset_maximalfaces( set_minus( get_properfaces() ) );
		return ( repset_maximalfaces );
	}
	else {
		cout << "Error" << endl;
		cout << "The operator boundarytopo is applied only for closed figures";
		cout << endl;
		throw "ERROR"; // to avoid unpredictable program behavior
	}
}


PTKrepresentableset PTKrepresentableset::get_topologicalbound() const {

	// Verify if the representable set is empty.
	if ( isempty() ) {
		return ( *this );
	}

	// Verify if the representable set is closed. This function can only be
	//   applied to closed representable set.
	if (*this == closedhull() ) {
		PTKrepresentableset repset_freefaces( get_freefaces() );
		PTKrepresentableset repset_maximalfaces( get_maximalfaces() );
		repset_maximalfaces.remove( get_elemcelldimk( myEmbedding ) );
		repset_maximalfaces = repset_maximalfaces.closedhull();
		return ( repset_maximalfaces.set_union( repset_freefaces ) );
	}
	else {
		cout << "Error" << endl;
		cout << "The operator boundarytopo is applied only for closed figures";
		cout << endl;
		throw "ERROR"; // to avoid unpredictable program behavior
	}
}

PTKrepresentableset PTKrepresentableset::inclusion(const int new_emb) const {

	// Verify if the new embedding is larger than 
	//   the one of the representable set.
	if ( new_emb <= myEmbedding ) {
		cout << "Error" << endl;
		cout << "The new embedding number should be less than " << myEmbedding << "." << endl;
		return ( *this );
	}
	PTKrepresentableset repset_copy( *this );
	PTKrepresentableset repset_inclusion( new_emb );
	if ( repset_copy.isempty() ) {
		return ( repset_inclusion );
	}
	PTKelementarycell elemcell_copy;
	PTKelementarycell elemcell_inclusion;
	int embedding_inclusion( new_emb );
	int* coordinate_inclusion = new int[embedding_inclusion];
	bool* singlepoint_inclusion = new bool[embedding_inclusion];
	for (int i = myEmbedding; i < new_emb; i++) {
		coordinate_inclusion[i] = 0;
		singlepoint_inclusion[i] = 1;
	}

	// Include each elementary cell in the new representable set.
	while ( !( repset_copy.isempty() ) ) {
		elemcell_copy = repset_copy.get_min();
		for (int i = 0; i < myEmbedding; i++) {
			coordinate_inclusion[i] = elemcell_copy.myCoordinate[i];
			singlepoint_inclusion[i] = elemcell_copy.mySinglepoint[i];
		}
		elemcell_inclusion.modify_coordinate( embedding_inclusion, coordinate_inclusion, singlepoint_inclusion );
		repset_inclusion.insert( elemcell_inclusion );
		repset_copy.remove( elemcell_copy );
	}
	return ( repset_inclusion );
}
	
const PTKbitmap PTKrepresentableset::get_bitmap() const {

	// Set the parameters of the bitmap.
	const int BORDER_X = 50;
	const int BORDER_Y = 50;
	const int CELL_X = 30;
	const int CELL_Y = 30;
	const int NUMBERCOLOR = 29;

	PTKbitmap bitmap_error;

	// Verify if the embedding of the representable set is adequate.
	//   Only 1D and 2D bitmap representation can be obtain.
	if ( !( ( myEmbedding == 1 ) || ( myEmbedding == 2 ) ) ) {
		cout << "Impossible to create an image of a representable set ";
		cout << "with an embedding number equal to ";
		cout << myEmbedding << "." << endl;
		return ( bitmap_error );
	}
	else if ( isempty() ) {
		cout << "Impossible to create an image of an empty representable set." << endl;
		return ( bitmap_error );
	}

	// Palette colors for the bitmap.
	int* COLOR_RED = new int[NUMBERCOLOR];
	int* COLOR_GREEN = new int[NUMBERCOLOR];;
	int* COLOR_BLUE = new int[NUMBERCOLOR];;
	const int BLACK = 0;
	const int WHITE = 28;
	const int GREY = 21;
//	const int RED = 24;
//	const int GREEN = 23;
//	const int BLUE = 22;
	COLOR_RED[0] = 0;
	COLOR_GREEN[0] = 0;		// Black
	COLOR_BLUE[0] = 0;
	COLOR_RED[1] = 64;
	COLOR_GREEN[1] = 0;
	COLOR_BLUE[1] = 0;
	COLOR_RED[2] = 0;
	COLOR_GREEN[2] = 64;
	COLOR_BLUE[2] = 0;
	COLOR_RED[3] = 0;
	COLOR_GREEN[3] = 0;
	COLOR_BLUE[3] = 64;
	COLOR_RED[4] = 64;
	COLOR_GREEN[4] = 64;
	COLOR_BLUE[4] = 0;
	COLOR_RED[5] = 64;
	COLOR_GREEN[5] = 0;
	COLOR_BLUE[5] = 64;
	COLOR_RED[6] = 0;
	COLOR_GREEN[6] = 64;
	COLOR_BLUE[6] = 64;
	COLOR_RED[7] = 64;
	COLOR_GREEN[7] = 64;
	COLOR_BLUE[7] = 64;
	COLOR_RED[8] = 128;
	COLOR_GREEN[8] = 0;
	COLOR_BLUE[8] = 0;
	COLOR_RED[9] = 0;
	COLOR_GREEN[9] = 128;
	COLOR_BLUE[9] = 0;
	COLOR_RED[10] = 0;
	COLOR_GREEN[10] = 0;
	COLOR_BLUE[10] = 128;
	COLOR_RED[11] = 128;
	COLOR_GREEN[11] = 128;
	COLOR_BLUE[11] = 0;
	COLOR_RED[12] = 128;
	COLOR_GREEN[12] = 0;
	COLOR_BLUE[12] = 128;
	COLOR_RED[13] = 0;
	COLOR_GREEN[13] = 128;
	COLOR_BLUE[13] = 128;
	COLOR_RED[14] = 128;
	COLOR_GREEN[14] = 128;
	COLOR_BLUE[14] = 128;
	COLOR_RED[15] = 192;
	COLOR_GREEN[15] = 0;
	COLOR_BLUE[15] = 0;
	COLOR_RED[16] = 0;
	COLOR_GREEN[16] = 192;
	COLOR_BLUE[16] = 0;
	COLOR_RED[17] = 0;
	COLOR_GREEN[17] = 0;
	COLOR_BLUE[17] = 192;
	COLOR_RED[18] = 192;
	COLOR_GREEN[18] = 192;
	COLOR_BLUE[18] = 0;
	COLOR_RED[19] = 192;
	COLOR_GREEN[19] = 0;
	COLOR_BLUE[19] = 192;
	COLOR_RED[20] = 0;
	COLOR_GREEN[20] = 192;
	COLOR_BLUE[20] = 192;
	COLOR_RED[21] = 192;
	COLOR_GREEN[21] = 192;
	COLOR_BLUE[21] = 192;
	COLOR_RED[22] = 255;
	COLOR_GREEN[22] = 0;		// Red
	COLOR_BLUE[22] = 0;
	COLOR_RED[23] = 0;
	COLOR_GREEN[23] = 255;
	COLOR_BLUE[23] = 0;
	COLOR_RED[24] = 0;
	COLOR_GREEN[24] = 0;
	COLOR_BLUE[24] = 255;
	COLOR_RED[25] = 255;
	COLOR_GREEN[25] = 255;
	COLOR_BLUE[25] = 0;
	COLOR_RED[26] = 255;
	COLOR_GREEN[26] = 0;
	COLOR_BLUE[26] = 255;
	COLOR_RED[27] = 0;
	COLOR_GREEN[27] = 255;
	COLOR_BLUE[27] = 255;
	COLOR_RED[28] = 255;
	COLOR_GREEN[28] = 255;		// White
	COLOR_BLUE[28] = 255;

	// Get the dimension of the bitmap.
	int* coordmin = get_coordmin();
	int* coordmax = get_coordmax();
	int nbintervalx = coordmax[0] - coordmin[0] + 2;
	int nbintervaly = coordmax[1] - coordmin[1] + 2;
	const int bmp_x = ( 2 * BORDER_X ) + ( nbintervalx * CELL_X ) + 1;
	const int bmp_y = ( 2 * BORDER_Y ) + ( nbintervaly * CELL_Y ) + 1;

	// For now, set white all the pixels of the bitmap.
	int* pixel = new int[ ( bmp_x * bmp_y ) ];
	for (int i = 0; i < ( bmp_x * bmp_y ); i++) {
		pixel[i] = WHITE;
	}

	// Paint the border and the axes of the bitmap.
	paint_border( pixel, WHITE, bmp_x, bmp_y, BORDER_X, BORDER_Y );
	paint_axe( pixel, BLACK, ( BORDER_X + ( BORDER_Y * bmp_x ) ), bmp_x, CELL_X, CELL_Y, 
				nbintervalx, nbintervaly);

	int position_x(0);
	int position_y(0);
	int position(0);
	bool orientation(0);
	PTKelementarycell elemcell_max( myEmbedding );
	PTKrepresentableset repset_copy( *this );

	// If the embedding is one, the representable set is projeted in 2d
	//   with the inclusion fonction.
	if ( repset_copy.myEmbedding == 1 ) {
		repset_copy = repset_copy.inclusion( 2 );
	}

	// Add color to the bitmap for each elementary cell 
	//   of the representable set.
	while ( !( repset_copy.isempty() ) ) {
		elemcell_max = repset_copy.get_max();
		position_x = ( BORDER_X + ( ( elemcell_max.myCoordinate[0] - coordmin[0] + 1 ) * CELL_X ) );
		position_y = ( BORDER_Y + ( ( elemcell_max.myCoordinate[1] - coordmin[1] + 1 ) * CELL_Y ) );
		position = ( position_y * bmp_x ) + position_x;

		// Add an elementary cell of dimension 0.
		if ( elemcell_max.myDimension == 0 ) {
			paint_point( pixel, BLACK, WHITE, position, bmp_x, 1 );
		}

		// Add an elementary cell of dimension 1.
		else if ( elemcell_max.myDimension == 1 ) {
			orientation = elemcell_max.mySinglepoint[0];
			if ( orientation == 0 ) {
				paint_line( pixel, BLACK, WHITE, position, bmp_x, CELL_X, 1, orientation);
				paint_point( pixel, BLACK, WHITE, position, bmp_x, 0 );
				paint_point( pixel, BLACK, WHITE, ( position + CELL_X ), bmp_x, 0 );
			}
			else if ( orientation == 1 ) {
				paint_line( pixel, BLACK, WHITE, position, bmp_x, CELL_Y, 1, orientation);
				paint_point( pixel, BLACK, WHITE, position, bmp_x, 0 );
				paint_point( pixel, BLACK, WHITE, ( position + (bmp_x * CELL_Y ) ), bmp_x, 0 );
			}
		}

		// Add an elementary cell of dimension 2.
		else if ( elemcell_max.myDimension == 2 ) {
			paint_square( pixel, GREY, position, bmp_x, CELL_X, CELL_Y );

			paint_line( pixel, BLACK, WHITE, position, bmp_x, CELL_Y, 0, 0 );
			paint_line( pixel, BLACK, WHITE, position + ( bmp_x * CELL_Y ), bmp_x, CELL_Y, 0, 0 );
			paint_line( pixel, BLACK, WHITE, position, bmp_x, CELL_Y, 0, 1);
			paint_line( pixel, BLACK, WHITE, ( position + CELL_X ), bmp_x, CELL_Y, 0, 1);

			paint_point( pixel, BLACK, WHITE, position, bmp_x, 0 );
			paint_point( pixel, BLACK, WHITE, ( position + CELL_X ), bmp_x, 0 );
			paint_point( pixel, BLACK, WHITE, ( position + (bmp_x * CELL_Y ) ), bmp_x, 0 );
			paint_point( pixel, BLACK, WHITE, ( position + CELL_X + (bmp_x * CELL_Y ) ), bmp_x, 0 );
		}
		repset_copy.remove( repset_copy.get_max() );
	}

	// The bitmap of colored.
	PTKbitmap bitmap_repset( NUMBERCOLOR, COLOR_RED, COLOR_GREEN, COLOR_BLUE, bmp_x, bmp_y );
	for (int i = 0; i < bmp_x; i++) {
		for (int j = 0; j < bmp_y; j++) {
			bitmap_repset.modify_pixel( pixel[ ( j * bmp_x ) + i ], i, j );
		}
	}
	return ( bitmap_repset );
}

void PTKrepresentableset::write(ostream& output, const int format) const {

	// Writing the representable set as a representable set.
	if ( format == 0 ) {
		output << "Type of set : R" << endl;
		output << "Space Dimension : " << myEmbedding << endl;
		output << "; " << "Number of elementary cell : " << get_numberelemcell() << endl;
		output << myElemcell;
	}

	// Writing the representable set as a cubical set.
	else if ( format == 1 ) {
		if ( *this == closedhull() ) {
			PTKrepresentableset repset_copy( get_maximalfaces() );
			output << "Type of set : C" << endl;
			output << "Space Dimension : " << repset_copy.myEmbedding << endl;
			output << "; " << "Number of elements : " << repset_copy.get_numberelemcell();
			PTKelementarycell elemcell_copy( repset_copy.myEmbedding );
			while ( !(repset_copy.isempty() ) ) {
				output << endl;
				elemcell_copy = repset_copy.get_min();
				for (int i = 0; i < elemcell_copy.myEmbedding; i++) {
					if ( elemcell_copy.mySinglepoint[i] == 1 ) {
						output << "[";
						output << elemcell_copy.myCoordinate[i];
						output << "]";
					}
					else {
						output << "[";
						output << elemcell_copy.myCoordinate[i];
						output << ",";
						output << elemcell_copy.myCoordinate[i] + 1;
						output << "]";
					}
					if ( i != ( elemcell_copy.myEmbedding - 1 ) ) {
						output << " x ";
					}
				}
				repset_copy.remove( elemcell_copy );
			}
		}
		else {
			cout << "ERROR" << endl;
		}
	}
	return;
}

void PTKrepresentableset::read(istream& input) {
	PPtextfile input_repset( input );

	// Get the type of the representable set : cubical or not.
	if ( input_repset.readphrase( "Type of set :" ) ) {
		cout << " -!- Error in line " << input_repset.linenumber() << "." << endl;
		cout << " -!- Before anything else, we need to know the type of the set." << endl;
		return;
	}
	bool is_cubical(0);
	switch ( input_repset.checkchar() ) {
		case 'C':
		case 'c':
			input_repset.readchar();
			is_cubical = 1;
			break;
		case 'R':
		case 'r':
			input_repset.readchar();
			is_cubical = 0;
			break;
		default:
			cout << " -!- Error in line " << input_repset.linenumber() << "." << endl;
			cout << " -!- The type of the set is not valid." << endl;
			return;
	}

	// Get the dimension of the space.
	int dimension(0);
	if ( !(input_repset.readphrase( "Space Dimension" ) ) ) {
		dimension = input_repset.readnumber( 0, 1 );
		if ( dimension <= 0 ) {
			cout << " -!- Error in line " << input_repset.linenumber() << "." << endl;
			cout << " -!- The space dimension is not valid." << endl;
			return;
		}
	}
	else {
		cout << " -!- Error in line " << input_repset.linenumber() << "." << endl;
		cout << " -!- The input file need to precise the space dimension." << endl;
		return;
	}

	// Create the representable set.
	PTKrepresentableset repset( dimension );

	// Create the elementary cell for the representable set.
	PTKelementarycell elemcell( dimension );
	int elemcell_embedding(0);
	int* elemcell_coordinate = new int[dimension];
	bool* elemcell_singlepoint = new bool[dimension];

	char openingbracket;
	char closingbracket;
	if ( is_cubical ) {
		openingbracket = '[';
		closingbracket = ']';
	}
	else {
		openingbracket = '(';
		closingbracket = ')';
	}
	
	while ( ( input_repset.checkchar() != EOF) ) {

		// Initialise the elementary cell.
		elemcell_embedding = 0;
		for ( int i = 0; i < dimension; i++ ) {
			elemcell_coordinate[i] = 0;
			elemcell_singlepoint[i] = 0;
		}

		for (int i = 0; i < dimension; i++) {

			// Read the opening bracket.
			if ( input_repset.checkchar() != openingbracket ) {
				cout << " -!- Error in line " << input_repset.linenumber() << "." << endl;
				cout << " -!- We should have a coordinate '[' here." << endl;
				return;
			}

			// Read the number for the interval.
			input_repset.readchar();
			elemcell_coordinate[i] = input_repset.readnumber( 0, 1 );
			if ( input_repset.checkchar() == ',' ) {
				if ( input_repset.readnumber( 0, 1 ) != ( elemcell_coordinate[i] + 1 ) ) {
					cout << " -!- Error in line " << input_repset.linenumber() << "." << endl;
					cout << " -!- The coordinate is not ok." << endl;
					return;
				}
				elemcell_singlepoint[i] = 0;
				elemcell_embedding++;
			}
			else {
				elemcell_singlepoint[i] = 1;
			}
			if ( input_repset.checkchar() != closingbracket ) {
				cout << " -!- Error in line " << input_repset.linenumber() << "." << endl;
				cout << " -!- The coordinate is not ok." << endl;
				return;
			}

			// Read the closing bracket.
			input_repset.readchar();
			if ( i < ( dimension - 1 ) ) {
				if ( input_repset.checkchar() != 'x' ) {
					cout << " -!- Error in line " << input_repset.linenumber() << "." << endl;
					cout << " -!- The coordinate is not ok." << endl;
					return;
				}
				input_repset.readchar();
			}
		}
		elemcell.modify_coordinate( dimension, elemcell_coordinate, elemcell_singlepoint );
		repset.insert( elemcell );
	}

	if ( is_cubical ) {
		repset = repset.closedhull();
	}
	*this = repset;
	return;
}

void PTKrepresentableset::closedhull(const int index, const PTKelementarycell elemcell) {
	// This is a recursive procedure.

	if (index < myEmbedding) {

		// If the coordinate in position 'index' is degenerate, 
		//   then add it and verify the next 'index'.
		if (elemcell.mySinglepoint[index] == 1) {
			closedhull( index + 1, elemcell );
		}

		// If the coordinate in position 'index' is nondegenerate, 
		//   then add it and also his 'boundary', and verify the next 'index'.
		else {
			closedhull( index + 1, elemcell );
			PTKelementarycell elemcell_copy( elemcell );
			elemcell_copy.modify_coordinate( index, elemcell.myCoordinate[index], 1 );
			closedhull( index + 1, elemcell_copy );
			elemcell_copy.modify_coordinate( index, elemcell.myCoordinate[index] + 1, 1 );
			closedhull( index + 1, elemcell_copy );
		}
		return;
	}
	else {

		// At the end of the recusrion, add the elementary cell 
		//   to the closed hull.
		insert( elemcell );
	}
	return;
}

void PTKrepresentableset::openhull(const int index, const PTKelementarycell elemcell) {
	// This is a recursive procedure.
	
	if ( index < myEmbedding ) {

		// If the coordinate in position 'index' is nondegenerate, 
		//   then add it and verify the next 'index'.
		if ( elemcell.mySinglepoint[index] == 0 ) {
			openhull( index + 1, elemcell );
		}

		// If the coordinate in position 'index' is degenerate, 
		//   then add it and also his 'boundary', and verify the next 'index'.
		else {
			openhull( index + 1, elemcell );
			PTKelementarycell elemcell_copy( elemcell );
			elemcell_copy.modify_coordinate( index, elemcell.myCoordinate[index] - 1, 0 );
			openhull( index + 1, elemcell_copy );
			elemcell_copy.modify_coordinate( index, elemcell.myCoordinate[index], 0 );
			openhull( index + 1, elemcell_copy );
		}
		return;
	}
	else {

		// At the end of the recusrion, add the elementary cell 
		//   to the closed hull.
		insert( elemcell );
	}
	return;
}


void PTKrepresentableset::paint_border(int*& pixel, const int color, const int bmp_x, 
			const int bmp_y, const int BORDER_X, const int BORDER_Y) const {
	for (int i = 0; i < bmp_x; i++) {
		for (int j = 0; j < BORDER_Y; j++) {
			pixel[ ( j * bmp_x ) + i ] = color;
		}
	}
	for (int i = 0; i < bmp_x; i++) {
		for (int j = ( bmp_y - BORDER_Y ); j < bmp_y; j++) {
			pixel[ ( j * bmp_x ) + i ] = color;
		}
	}
	for (int i = 0; i < BORDER_X; i++) {
		for (int j = BORDER_Y; j < ( bmp_y - BORDER_Y ); j++) {
			pixel[ ( j * bmp_x ) + i ] = color;
		}
	}
	for (int i = ( bmp_x - BORDER_X ); i < bmp_x; i++) {
		for (int j = BORDER_Y; j < ( bmp_y - BORDER_Y ); j++) {
			pixel[ ( j * bmp_x ) + i ] = color;
		}
	}
	return;
}


void PTKrepresentableset::paint_axe(int*& pixel, const int color, const int position, const int bmp_x, 
			const int CELL_X, const int CELL_Y, const int nbintervalx, const int nbintervaly) const {
	for (int i = 0; i <= ( CELL_X * nbintervalx ); i++) {
		pixel[ ( position + i ) ] = color;
		pixel[ ( position + i + ( CELL_Y * nbintervaly * bmp_x ) ) ] = color;
		if ( ( ( i %CELL_X ) == 0 ) && ( i != 0 ) && ( i != CELL_X * nbintervalx ) ) {
			pixel[ ( position + i ) + bmp_x ] = color;
			pixel[ ( position + i ) - bmp_x ] = color;
			pixel[ ( position + i ) + bmp_x + ( CELL_Y * nbintervaly * bmp_x ) ] = color;
			pixel[ ( position + i ) - bmp_x + ( CELL_Y * nbintervaly * bmp_x ) ] = color;
		}
	}
	for (int j = 0; j <= ( CELL_Y * nbintervaly ); j++) {
		pixel[ ( position + ( j * bmp_x ) ) ] = color;
		pixel[ ( position + ( CELL_X * nbintervalx ) + ( j * bmp_x ) ) ] = color;
		if ( ( ( j %CELL_Y ) == 0 ) && ( j != 0 ) && ( j != CELL_Y * nbintervaly ) ) {
			pixel[ ( position + ( j * bmp_x ) ) + 1 ] = color;
			pixel[ ( position + ( j * bmp_x ) ) - 1 ] = color;
			pixel[ ( position + ( CELL_X * nbintervalx ) + ( j * bmp_x ) ) + 1 ] = color;
			pixel[ ( position + ( CELL_X * nbintervalx ) + ( j * bmp_x ) ) - 1 ] = color;
		}
	}
	return;
}


void PTKrepresentableset::paint_point(int*& pixel, const int color, const int color2, const int position, 
			const int bmp_x, const bool filled) const {
	int transparent(0);
	if ( filled ) {
		transparent = color;
	}
	else {
		transparent = color2;
	}
	pixel[position + 0 + (bmp_x * 3 ) ] = color;
	pixel[position + 1 + (bmp_x * 2 ) ] = color;
	pixel[position + 0 + (bmp_x * 2 ) ] = transparent;
	pixel[position - 1 + (bmp_x * 2 ) ] = color;
	pixel[position + 2 + (bmp_x * 1 ) ] = color;
	pixel[position + 1 + (bmp_x * 1 ) ] = transparent;
	pixel[position + 0 + (bmp_x * 1 ) ] = transparent;
	pixel[position - 1 + (bmp_x * 1 ) ] = transparent;
	pixel[position - 2 + (bmp_x * 1 ) ] = color;
	pixel[position + 3 + (bmp_x * 0 ) ] = color;
	pixel[position + 2 + (bmp_x * 0 ) ] = transparent;
	pixel[position + 1 + (bmp_x * 0 ) ] = transparent;
	pixel[position + 0 + (bmp_x * 0 ) ] = transparent;
	pixel[position - 1 + (bmp_x * 0 ) ] = transparent;
	pixel[position - 2 + (bmp_x * 0 ) ] = transparent;
	pixel[position - 3 + (bmp_x * 0 ) ] = color;
	pixel[position + 2 - (bmp_x * 1 ) ] = color;
	pixel[position + 1 - (bmp_x * 1 ) ] = transparent;
	pixel[position + 0 - (bmp_x * 1 ) ] = transparent;
	pixel[position - 1 - (bmp_x * 1 ) ] = transparent;
	pixel[position - 2 - (bmp_x * 1 ) ] = color;
	pixel[position + 1 - (bmp_x * 2 ) ] = color;
	pixel[position + 0 - (bmp_x * 2 ) ] = transparent;
	pixel[position - 1 - (bmp_x * 2 ) ] = color;
	pixel[position + 0 - (bmp_x * 3 ) ] = color;
	return;
}


void PTKrepresentableset::paint_line(int*& pixel, const int color, const int color2, const int position, 
			const int bmp_x, const int CELL, const bool filled, const bool orientation) const {
	int transparent(0);
	if ( filled ) {
		transparent = color;
	}
	else {
		transparent = color2;
	}
	if ( orientation == 0 ) {
		for (int i = 0; i <= CELL; i++) {
			pixel[ position + i + bmp_x ] = color;
			pixel[ position + i ] = transparent;
			pixel[ position + i - bmp_x ] = color;
		}
	}
	else if ( orientation == 1 ) {
		for (int i = 0; i <= CELL; i++) {
			pixel[ position + ( i * bmp_x ) + 1 ] = color;
			pixel[ position + ( i * bmp_x ) ] = transparent;
			pixel[ position + ( i * bmp_x ) - 1 ] = color;
		}
	}
	return;
}


void PTKrepresentableset::paint_square(int*& pixel, const int color, const int position, const int bmp_x, 
				const int CELL_X, const int CELL_Y) const {
	for (int i = 0; i <= CELL_X; i++) {
		for (int j = 0; j <= CELL_Y; j++) {
			pixel[ position + i + ( j * bmp_x ) ] = color;
		}
	}
	return;
}


ostream& operator<<(ostream& output, const PTKrepresentableset& repset) {

	// How-to-use-existing-code 101.
	repset.write( output, 0 );
	return output;
}


istream& operator>>(istream& input, PTKrepresentableset& repset) {

	// How-to-use-existing-code 101.
	repset.read( input );
	return input;
}
