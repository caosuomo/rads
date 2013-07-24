// Copyright (C) 2004 by William D Kalies. All rights reserved.
//
//-----------------------------------------------------------------------------
// vertex.h:
//-----------------------------------------------------------------------------
/*! \file vertex.h
    \brief Vertex class.

    Version 3.0

    This file contains declarations for vertex class.
    Last modified by BK on 1/22/02.
*/

#ifndef VERTEX_H
#define VERTEX_H

#include "cell.h"
#include "bitcodes.h"
#include "list.h"
#include "tree.h"

/*! \class vertex
    \brief Vertices of a cubical complex.

    Vertices are derived from cell, bitcode, and node.
    Last modified by BK on 6/25/00.
*/

class vertex : public cell {
 private:   
   cobdry_list* cobdry;              // vertices have a private coboundary list
   vertex(const vertex&);            // disable copy constructor
   vertex& operator=(const vertex&); // disable assignment operator
 public:
   bitcode* bc;
   static int counter;
   static int max;

   vertex(const bitcode& cube_code) 
     : cell(0)
     {++counter; ++max; bc=new bitcode(cube_code.Bits());
      for(int i=0; i<cube_code.Bits(); ++i) (*bc)(i,cube_code[i]); 
      cobdry=new cobdry_list;}       //!< New vertices have empty cobdry.
                                     
   ~vertex()                 
     {delete cobdry; delete bc; --counter;}  //!< Destructor destroys coboundary list.

   int CobdryIncidenceOf(cell*) const;
   void CobdryRemove(cell* c)
     {cobdry->Remove(c);}
//      assert(!(cobdry->Contains(c)));}
   void CobdryInsert(cell*, int);
   void Reduce(cell*);
   cell* FindInvertibleCobdryElement();
   void RemoveFromCobdry();
   cobdry_list* Cobdry()
     {return(cobdry);}
};

class vert : public node{
 public:
   cell* v;
   vert(cell* vv) : node(), v(vv) {}
   vert() : node(), v(NULL) {}
};

#endif
