// Copyright (C) 2004 by William D Kalies. All rights reserved.
//
//-----------------------------------------------------------------------------
// block.h:
//-----------------------------------------------------------------------------
/*! \file block.h
    \brief Block class.

    Version 3.0

    This file contains block class declarations.
    Last modified by BK on 1/22/02.
*/

#ifndef BLOCK_H
#define BLOCK_H

#include "bitcodes.h"
#include "cell.h"
#include "list.h"
#include "tree.h"

class complex;

//-----------------------------------------------------------------------------
// Block declarations
//-----------------------------------------------------------------------------

/*! \class block
    \brief class for block data at nodes in binary tree.

    Blocks have lists of cells of any dimension. 
    Last modified by BK on 10/1/00.
*/

class block : public node{
 private:
   block(const block&);              // disable copy construction
   block operator=(const block&);    // disable copy assignment
 public:
   bitcode* bc;
   cell_list cells[DIM+1];           // lists of interior cells at this node
   cell_list bdry_verts;
   static int counter;
   static int max;
   block() : node() {bc=new bitcode(0);}   // used for root node of cube tree only
   block(char bits, complex* c) : node()
     {++counter; ++max; bc=new bitcode(bits);}
   block(const bitcode& cube_code, complex* c) : node()
     {++counter; ++max; bc=new bitcode(cube_code.Bits());}
   ~block()
     {--counter; delete bc;}
   void CreateCells(complex*);
};

bitcode& BC(block* b);

/*! \fn Create(cell*, bitcode&, bitcode&, int, complex*, cell_list&);
    \brief Recursively creates a new cube and all its faces in any dimension.
 
    Last modified by BK on 5/10/00.
*/ 
int Create(cell**, bitcode&, bitcode&, int, complex*, cell_list&);

#endif
