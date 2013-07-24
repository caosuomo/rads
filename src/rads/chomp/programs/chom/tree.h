// Copyright (C) 2004 by William D Kalies. All rights reserved.
//
//-----------------------------------------------------------------------------
// tree.h:
//-----------------------------------------------------------------------------
/*! \file tree.h
    \brief tree class.

    Version 3.0

    This file contains declarations for the base tree class.
    Author: Madjid Allili.
    Last modified by BK on 1/22/02.
*/

#ifndef TREE_H
#define TREE_H

#include "bitcodes.h"

class node{
 public:
   node *Left,*Right,*Parent;
   static int counter;
   static int max;
   node():Left(NULL),Right(NULL),Parent(NULL){++counter; ++max;}
   node(node *L, node *R):Left(L),Right(R),Parent(NULL){++counter; ++max;}
   node(node *P):Left(NULL),Right(NULL),Parent(P){++counter; ++max;}
   ~node() {--counter;}
   friend class tree;
};

class tree{
 public:
   node* Root;
   tree(): Root(NULL){}
   tree(node* n): Root(n) {}
   node* Find(const bitcode&);
   node* Insert(const bitcode&, node*);
   void Delete(const bitcode&);
   void Print(node*);
   void Print()
    {Print(Root);}
};

#endif
