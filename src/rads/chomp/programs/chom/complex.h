// Copyright (C) 2004 by William D Kalies. All rights reserved.
//
//-----------------------------------------------------------------------------
// complex.h:
//-----------------------------------------------------------------------------
/*! \file complex.h
    \brief Class declarations for cubical complexes.

    Version 3.0

    Last modified by BK on 1/22/02.
*/

#ifndef COMPLEX_H
#define COMPLEX_H

#include "common.h"
#include "cell.h"
#include "list.h" 
#include "tree.h"
#include "block.h"
#include "vertex.h"

/*! \class complex
    \brief Class for a cubical complex.

    A complex has storage for d-dimensional cells and vertices.

    Last modified by BK on 10/4/00.
*/

class complex{
 private:
   bitcode* previous_cube;
   int min[DIM];
   tree cubes;                      // main tree for cubical complex
   tree vertices;
   block* Merge(node*);
   void Simplify(block*);
 public:
//   complex() {}
   complex(block* nb) : previous_cube(NULL), cubes(nb)
     {vertices.Root=new node();}
   ~complex()
     {}
   int NewVertex(const bitcode&,cell**);
   cell* FindVertex(const bitcode&);
   void DeleteVertex(vertex* v)
     {vertices.Delete(*(v->bc));}
   bool InsertCube(block*);
   void FinalCube();
   void Homology(ofstream*, ofstream*);
   void Recurse(node*,list<block*>*,int);
   void Delete(node*,int);
   void PrintCubes()
     {cubes.Print();}
   void PrintVertices()
     {vertices.Print();}
};

#endif







