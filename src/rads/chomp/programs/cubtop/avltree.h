/**************************************************************************

 avltree.hh

 Created by ANTOINE PINEAU
 Date : March 1998
 
 Modified by SYLVAIN BÉRUBÉ
 Date : November 2001

 Last modification by SYLVAIN BÉRUBÉ
 Date : January 2001

The class AVLtree is a data structure.

**************************************************************************/

#ifndef _AVLTREE_HH_
#define _AVLTREE_HH_

#include "avlnode.h"

#include <iostream>
#include <assert.h>
using namespace std;

template<class TYPE>
class AVLtree {
public:
	AVLtree();
	~AVLtree();
	const AVLtree<TYPE>& operator=(const AVLtree<TYPE>& tree);
	const bool operator==(const AVLtree<TYPE>& tree) const;
	const bool operator!=(const AVLtree<TYPE>& tree) const;
	void insert(const TYPE& data);
	void insert(const AVLtree<TYPE>& tree);
	void remove(const TYPE& data);
	void remove(const AVLtree<TYPE>& tree);
	const bool contains(const TYPE& data) const;
	const bool contains(const AVLtree<TYPE>& tree) const;
	const bool isempty() const;
	const TYPE& get_max() const;
	const TYPE& get_min() const;
	const TYPE& valeur(const TYPE& data) const;		//Verify utility of this function
	void empty_tree();
	const int get_numberofdata() const;
	void write(ostream& output) const;
protected:
	AVLnode<TYPE>* myRoot;
	int myNumberofdata;

	AVLtree(const AVLtree& tree);
	const bool insert(const TYPE& data, AVLnode<TYPE>*& node);
	void insert(const AVLnode<TYPE>*const node);
	const bool add_left(const TYPE& data, AVLnode<TYPE>*& node);
	const bool add_right(const TYPE& data, AVLnode<TYPE>*& node);
	void rotate_leftright(AVLnode<TYPE>*& node);
	void rotate_rightleft(AVLnode<TYPE>*& node);
	const bool remove(const TYPE& data, AVLnode<TYPE>*& node);
	void remove(const AVLnode<TYPE>*const node);
	const bool remove_left(const TYPE& data, AVLnode<TYPE>*& node);
	const bool remove_right(const TYPE& data, AVLnode<TYPE>*& node);
	const bool contains(const TYPE& data, const AVLnode<TYPE>* const& node) const;
	const TYPE& get_max(const AVLnode<TYPE>* const& node) const;
	const TYPE& get_min(const AVLnode<TYPE>* const& node) const;
	const TYPE& valeur(const TYPE& data, const AVLnode<TYPE>* const& node) const; //Verify utility of this function
	void empty_tree(AVLnode<TYPE>*& node);
	void copy_tree(const AVLnode<TYPE>* node);
	const bool compare_tree(const AVLnode<TYPE>* node) const;
	void write(ostream& output, AVLnode<TYPE>* node) const;
};

template<class TYPE>
ostream& operator<<(ostream& output, const AVLtree<TYPE>& tree);

template<class TYPE>
AVLtree<TYPE>::AVLtree()
:myRoot(NULL), myNumberofdata(0) {
}

template<class TYPE>
AVLtree<TYPE>::~AVLtree() {
	empty_tree();
}

template<class TYPE>
const AVLtree<TYPE>& AVLtree<TYPE>::operator=(const AVLtree<TYPE>& tree) {
	empty_tree();
	if (tree.myRoot != NULL) {
		copy_tree(tree.myRoot);
	}
	return *this;
}

template<class TYPE>
const bool AVLtree<TYPE>::operator==(const AVLtree<TYPE>& tree) const {
	if ( (myRoot == NULL) && (tree.myRoot == NULL) ) {
		return 1;
	}
	if (myNumberofdata != tree.myNumberofdata) {
		return 0;
	}
	if ( (myRoot == NULL) || (tree.myRoot == NULL) ) {
		return 0;
	}
	return ( compare_tree(tree.myRoot) );
}

template<class TYPE>
const bool AVLtree<TYPE>::operator!=(const AVLtree<TYPE>& tree) const {
	return ( !(*this == tree) );
}

template<class TYPE>
void AVLtree<TYPE>::insert(const TYPE& data) {
	insert(data, myRoot);
}

template<class TYPE>
void AVLtree<TYPE>::insert(const AVLtree<TYPE>& tree) {
	if (tree.myRoot != NULL) {
		insert(tree.myRoot->myData, myRoot);
		insert(tree.myRoot->myLeft);
		insert(tree.myRoot->myRight);		
	}
}

template<class TYPE>
void AVLtree<TYPE>::remove(const TYPE& data) {
	if ( contains(data, myRoot) ) {
		remove(data, myRoot);
	}
}

template<class TYPE>
void AVLtree<TYPE>::remove(const AVLtree<TYPE>& tree) {
	if (*this == tree) {
		empty_tree();
		return;
	}
	if (tree.myRoot != NULL) {
		if ( contains(tree.myRoot->myData, myRoot) ) {
			remove(tree.myRoot->myData, myRoot);
		}
		remove(tree.myRoot->myLeft);
		remove(tree.myRoot->myRight);
	}
}

template<class TYPE>
const bool AVLtree<TYPE>::contains(const TYPE& data) const {
	return contains(data, myRoot);
}

template<class TYPE>
const bool AVLtree<TYPE>::contains(const AVLtree<TYPE>& tree) const {
	if (tree.myRoot == NULL) {
		return 1;
	}
	if (myNumberofdata < tree.myNumberofdata) {
		return 0;
	}
	if (myRoot == NULL) {
		return 0;
	}
	return ( compare_tree(tree.myRoot) );
}

template<class TYPE>
const bool AVLtree<TYPE>::isempty() const {
	return (myRoot == NULL);

}

template<class TYPE>
const TYPE& AVLtree<TYPE>::get_max() const {
	if (myRoot == NULL) {
		cout << endl << "Error AVLtree : get_max of an empty tree." << endl;
		assert(0);
	}
	return get_max(myRoot);
}

template<class TYPE>
const TYPE& AVLtree<TYPE>::get_min() const {
	if (myRoot == NULL) {
		cout << endl << "Error AVLtree : get_max of an empty tree." << endl;
		assert(0);
	}
	return get_min(myRoot);
}

template<class TYPE>
const TYPE& AVLtree<TYPE>::valeur(const TYPE& data) const {
	return valeur(data, myRoot);
}

template<class TYPE>
void AVLtree<TYPE>::empty_tree() {
	empty_tree(myRoot);
}

template<class TYPE>
const int AVLtree<TYPE>::get_numberofdata() const {
	return myNumberofdata;
}

template<class TYPE>
const bool AVLtree<TYPE>::insert(const TYPE& data, AVLnode<TYPE>*& node) {
	if (node == NULL) {
		node = new AVLnode<TYPE>(data);
		myNumberofdata++;
		return true;
	}
	if (data == node->myData) {
		return false;
	}
	if (data < node->myData) {
		return add_left(data, node);
	}
	else {
		return add_right(data, node);
	}
}

template<class TYPE>
void AVLtree<TYPE>::insert(const AVLnode<TYPE>*const node) {
	if (node != NULL) {
		insert(node->myData, myRoot);
		insert(node->myLeft);
		insert(node->myRight);		
	}
}

template<class TYPE>
const bool AVLtree<TYPE>::add_left(const TYPE& data, AVLnode<TYPE>*& node) {
	if (insert(data, node->myLeft)) {
		++(node->myIndex);
		if (node->myIndex == 0) {
			return 0;
		}
		if (node->myIndex == 1) {
			return 1;
		}
		if (node->myLeft->myIndex == -1) {
			rotate_rightleft(node->myLeft);
		}
		rotate_leftright(node);
	}
	return 0;
}

template<class TYPE>
const bool AVLtree<TYPE>::add_right(const TYPE& data, AVLnode<TYPE>*& node) {
	if (insert(data, node->myRight)) {
		--(node->myIndex);
		if (node->myIndex == 0) {
			return 0;
		}
		if (node->myIndex == -1) {
			return 1;
		}
		if (node->myRight->myIndex == 1) {
			rotate_leftright(node->myRight);
		}
		rotate_rightleft(node);
	}
	return 0;
}

template<class TYPE>
void AVLtree<TYPE>::rotate_leftright(AVLnode<TYPE>*& node) {
	AVLnode<TYPE>* temp(node->myLeft);
	int ia(temp->myIndex);
	int ib(node->myIndex);
	int nib(-ia - (0>-ia?0:-ia) - 1 + ib);
	int nia(ia - (0>-nib?0:-nib) - 1);
	temp->myIndex = nia;
	node->myIndex = nib;
	node->myLeft = temp->myRight;
	temp->myRight = node;
	node = temp;
}

template<class TYPE>
void AVLtree<TYPE>::rotate_rightleft(AVLnode<TYPE>*& node) {
	AVLnode<TYPE>* temp(node->myRight);
	int ia(temp->myIndex);
	int ib(node->myIndex);
	int nib(ib + (0>-ia?0:-ia) + 1);
	int nia(ia + (0>nib?0:nib) + 1);
	temp->myIndex = nia;
	node->myIndex = nib;
	node->myRight = temp->myLeft;
	temp->myLeft = node;
	node = temp;
}

template<class TYPE>
const bool AVLtree<TYPE>::remove(const TYPE& data, AVLnode<TYPE>*& node) {
	if (data < node->myData) {
		return remove_left(data, node);
	}
	if (data > node->myData) {
		return remove_right(data, node);
	}
	AVLnode<TYPE>* temp(node);
	if (node->myLeft == NULL) {
		node = node->myRight;
		delete temp;
		myNumberofdata--;
		return 1;
	}
	if (node->myRight == NULL) {
		node = node->myLeft;
		delete temp;
		myNumberofdata--;
		return 1;
	}
	node->myData = get_max(node->myLeft);
	return remove_left(node->myData, node);
}

template<class TYPE>
void AVLtree<TYPE>::remove(const AVLnode<TYPE>*const node) {
	if (node != NULL) {
		if (contains(node->myData, myRoot)) {
			remove(node->myData, myRoot);
		}
		remove(node->myLeft);
		remove(node->myRight);		
	}
}

template<class TYPE>
const bool AVLtree<TYPE>::remove_left(const TYPE& data, AVLnode<TYPE>*& node) {
	bool retour(0);
	if (remove(data, node->myLeft)) {
		--(node->myIndex);
		if (node->myIndex == -1) {
			return 0;
		}
		if (node->myIndex == 0) {
			return 1;
		}
		retour = (node->myRight->myIndex != 0);
		if (node->myRight->myIndex == 1) {
			rotate_leftright(node->myRight);
		}
		rotate_rightleft(node);
	}
	return retour;
}

template<class TYPE>
const bool AVLtree<TYPE>::remove_right(const TYPE& data, AVLnode<TYPE>*& node) {
	bool retour(0);
	if (remove(data, node->myRight)) {
		++(node->myIndex);
		if (node->myIndex == 1) {
			return 0;
		}
		if (node->myIndex == 0) {
			return 1;
		}
		retour = (node->myLeft->myIndex != 0);
		if (node->myLeft->myIndex == -1) {
			rotate_rightleft(node->myLeft);
		}
		rotate_leftright(node);
	}
	return retour;
}

template<class TYPE>
const bool AVLtree<TYPE>::contains(const TYPE& data, const AVLnode<TYPE>* const& node) const {
	if (node == NULL) {
		return 0;
	}
	if (data == node->myData) {
		return 1;
	}
	if (data < node->myData) {
		return contains(data, node->myLeft);
	}
	else {
		return contains(data, node->myRight);
	}
}

template<class TYPE>
const TYPE& AVLtree<TYPE>::get_max(const AVLnode<TYPE>* const& node) const {
	if (node->myRight == NULL) {
		return node->myData;
	}
	else {
		return get_max(node->myRight);
	}
}

template<class TYPE>
const TYPE& AVLtree<TYPE>::get_min(const AVLnode<TYPE>* const& node) const {
	if (node->myLeft == NULL) {
		return node->myData;
	}
	else {
		return get_min(node->myLeft);
	}
}

template<class TYPE>
const TYPE& AVLtree<TYPE>::valeur(const TYPE& data, const AVLnode<TYPE>* const& node) const {
	if (node == NULL) {
		cout << endl << "Error AVLtree : data can't be found." << endl;
		assert(0);
	}
	if (data == node->myData) {
		return node->myData;
	}
	if (data < node->myData) {
		return valeur(data, node->myLeft);
	}
	return valeur(data, node->myRight);
}

template<class TYPE>
void AVLtree<TYPE>::empty_tree(AVLnode<TYPE>*& node) {
	if (node != NULL) {
		empty_tree(node->myLeft);
		empty_tree(node->myRight);
		delete node;
		node = NULL;
	}
	myNumberofdata = 0;
}

template<class TYPE>
void AVLtree<TYPE>::copy_tree(const AVLnode<TYPE>* node) {
	if (node->myLeft != NULL) {
		copy_tree(node->myLeft);
	}
	insert(node->myData);
	if (node->myRight != NULL) {
		copy_tree(node->myRight);
	}
}

template<class TYPE>
const bool AVLtree<TYPE>::compare_tree(const AVLnode<TYPE>* node) const {
	if ( !(contains(node->myData) ) ) {
		return 0;
	}
	if (node->myLeft != NULL) {
		if ( !(compare_tree(node->myLeft) ) ) {
			return 0;
		}
	}
	if (node->myRight != NULL) {
		if ( !(compare_tree(node->myRight) ) ) {
			return 0;
		}
	}
	return 1;
}

template<class TYPE>
void AVLtree<TYPE>::write(ostream& output) const {
	write(output, myRoot);
}

template<class TYPE>
void AVLtree<TYPE>::write(ostream& output, AVLnode<TYPE>* node) const {
	if (node == NULL) {
		return;
	}
	write(output, node->myLeft);
	output << node->myData << endl;
	write(output, node->myRight);
}

template<class TYPE>
ostream& operator<<(ostream& output, const AVLtree<TYPE>& tree) {
	tree.write(output);
	return output;
}

#endif
