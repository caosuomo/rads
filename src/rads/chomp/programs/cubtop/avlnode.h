/**************************************************************************

 avlnode.hh

 Created by ANTOINE PINEAU
 Date : March 1998
 
 Modified by SYLVAIN BÉRUBÉ
 Date : November 2001

The class AVLnode is a friend of the class AVLtree.

**************************************************************************/

#ifndef _AVLNODE_HH_
#define _AVLNODE_HH_

#include <iostream>
using namespace std;

template<class TYPE>
class AVLtree;

template<class TYPE>
class AVLnode {
   friend class AVLtree<TYPE>;
   public:
      AVLnode(const TYPE& data);
      ~AVLnode();
      void write(ostream& output, int height);
   protected:
      TYPE myData;
      AVLnode<TYPE>* myLeft;
      AVLnode<TYPE>* myRight;
      int myIndex;
};

template<class TYPE>
AVLnode<TYPE>::AVLnode(const TYPE& data)
: myData(data), myLeft(NULL), myRight(NULL), myIndex(0) {
}

template<class TYPE>
AVLnode<TYPE>::~AVLnode() {
}

template<class TYPE>
void AVLnode<TYPE>::write(ostream& output, int height) {
   output << "<+" << height << "+>" << endl;
   output << "AVLnode : "<< this << endl;
   output << "Left : " << myLeft;
   output << ", Right : " << myRight;
   output << ", Index : " << myIndex << endl;
   output << "Data : " << endl << myData << endl;
   if (myLeft != NULL) {
      output << "L : ";
      myLeft->write(output, height + 1);
      output << endl;
   }
   if (myRight != NULL) {
      output << "R : ";
      myRight->write(output, height + 1);
      output << endl;
   }
   output << "<-" << height << "->";
}

#endif
