/**************************************************************************

 ptkreprensetableset.hh

 Created by SYLVAIN BÉRUBÉ
 Date : December 2001

 Modified by SYLVAIN BÉRUBÉ and ANIK TRAHAN
 Date : January 2002

**************************************************************************/


#ifndef _PTKREPRESENTABLESET_
#define _PTKREPRESENTABLESET_

#include <iostream>
#include <fstream>
using namespace std;
#include "avltree.h"
#include "ptkelementarycell.h"
#include "ptkbitmap.h"
#include "pptextfile.h"


class PTKrepresentableset {
public:

	//  Default constructor (embedding = 0).
	PTKrepresentableset();

	//  Copy constructor.
	PTKrepresentableset(const PTKrepresentableset& repset);

	//  Constructor of an empty reprensetable set with embedding = (embedding).
	PTKrepresentableset(const int embedding);

	//  Default destructor.
	~PTKrepresentableset();

	//  Copy of the representable set.
	const PTKrepresentableset& operator=(const PTKrepresentableset& repset);

	//  Comparators
	const bool operator==(const PTKrepresentableset& repset) const;
	const bool operator!=(const PTKrepresentableset& repset) const;

	//  Insert new elements in the representable set.
	void insert(const PTKrepresentableset& repset);
	void insert(const PTKelementarycell& elemcell);

	//  Remove elements from the representable set.
	void remove(const PTKrepresentableset& repset);
	void remove(const PTKelementarycell& elemcell);

	//  Verify if the representable set contains a specific representable set.
	const bool contains(const PTKrepresentableset& repset) const;

	//  Verify if the representable set contains a specific elementary cell.
	const bool contains(const PTKelementarycell& elemcell) const;

	//  Get the number of elementary cell in the representable set.
	const int get_numberelemcell() const;

	//  Get the minimal and the maximal elementary cell 
	//    in the representable set.
	const PTKelementarycell get_min() const;
	const PTKelementarycell get_max() const;

	//  Get the minimals coordinates in the representable for each dimensions.
	int* get_coordmin() const;

	//  Get the maximals coordinates in the representable for each dimensions.
	int* get_coordmax() const;

	//  Verify if the representable set is empty.
	const bool isempty() const;

	//  Remove all elements in the representable set.
	void empty();

	//  Topological and set operations.
	
	//  Union of the representable sets.
	PTKrepresentableset set_union(const PTKrepresentableset& repset) const;

	//  Remove the representable set 'repset' from the representable set.
	PTKrepresentableset set_minus(const PTKrepresentableset& repset) const;

	//  Intersection of the representable sets.
	PTKrepresentableset set_intersection(const PTKrepresentableset& repset) const;

	//  Disjoint union of the representable sets.
	PTKrepresentableset set_disjointunion(const PTKrepresentableset& repset) const;

	//  Get the closed hull of the representable set.
	PTKrepresentableset closedhull() const;

	//  Get the open hull of the representable set.
	PTKrepresentableset openhull() const;

	//  Get the elementary cell of dimension 'k' of the representable set.
	PTKrepresentableset get_elemcelldimk(const int dimensionk) const;

	//  Get the proper faces of the representable set.
	PTKrepresentableset get_properfaces() const;

	//  Get the free faces of the representable set.
	PTKrepresentableset get_freefaces() const;

	//  Get the maximal faces of the representable set.
	PTKrepresentableset get_maximalfaces() const;

	//  Get the topological boundary of the representable set.
	PTKrepresentableset get_topologicalbound() const;

	//  Unclusion of a representable set into a new embedding.
	PTKrepresentableset inclusion(const int new_emb) const;

	// Get the bitmap of the representable set.
	const PTKbitmap get_bitmap() const;

	//  Send the representable set to an output device.	
	void write(ostream& output, const int format ) const;

	//  Get the representable set from a file.
	void read(istream& input);

protected:
	int myEmbedding;
	AVLtree<PTKelementarycell> myElemcell;

	//  Get the closed hull of the elementary cell.
	void closedhull(const int index, const PTKelementarycell elemcell);

	//  Get the open hull of the elementary cell.
	void openhull(const int index, const PTKelementarycell elemcell);

	//  Functions specific for the get_bitmap() function.
	void paint_border(int*& pixel, const int color, const int bmp_x, const int bmp_y, 
				const int BORDER_X, const int BORDER_Y) const;
	void paint_axe(int*& pixel, const int color, const int position, const int bmp_x, 
				const int CELL_X, const int CELL_Y, const int nbintervalx, const int nbintervaly) const;
	void paint_point(int*& pixel, const int color, const int color2, const int position, const int bmp_x, 
				const bool filled) const;
	void paint_line(int*& pixel, const int color, const int color2, const int position, const int bmp_x, 
				const int CELL, const bool filled, const bool orientation) const;
	void paint_square(int*& pixel, const int color, const int position, const int bmp_x, 
				const int CELL_X, const int CELL_Y) const;
};

ostream& operator<<(ostream& output, const PTKrepresentableset& repset);
istream& operator>>(istream& input, PTKrepresentableset& repset);

#endif
