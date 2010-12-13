#include <stdio.h>
#include <iostream>
#include <algorithm>
#include "tree.h"

Tree::Tree(const Box &b)
  : data(b.w)
{
  dim = b.v.size();				// dimension
  rootbox = b;					// bounding box
  root = new Node(0,0,0);		// root of the tree
  fulldepth = 0;				// set depth
  top();						// set recursive vars
}

Tree::Tree(const Tree &t)
  : data(t.data)
{
  dim = t.dim;					// dimension
  rootbox = t.rootbox;			// bounding box
  fulldepth = t.fulldepth;		// depth of the tree
  root = new Node(0,0,0);		// root of the tree
  top();						// set recursive vars
  copy_rec(root,t.root);
}

void Tree::copy_rec(Node *to, Node *from)
{
  //  printf("copying node %i (depth %i)\n",from->id,recdepth);

  if (recdepth == fulldepth)	// at a leaf
  {
	data.set_node(to);			// update entry for Node *
  }
  else if (from->children)		// node has kids
  {
	for (int i=0; i<pow(2,dim); i++)
	{
	  if (from->children[i])
	  {
		to->addchild(dim,i,from->children[i]->id);
		recdepth++;
		copy_rec(to->children[i], from->children[i]);
		recdepth--;
	  }
	}
  }
}

Tree::~Tree()
{
  cout << "in ~Tree()" << endl;
  del(root);
}

void Tree::del(Node *n)
{
  //  printf("in Node %i's d'tor\n",n->id);

  if (n->children)
  {
	for (int i=0; i<pow(2,dim); i++)
	{
	  if (n->children[i]) del(n->children[i]);
	}
  }
  delete n;	// recursively, all children are gone
}

void Tree::print()
{
  top();
  print_rec();
}

void Tree::print_rec()
{
  cout << "depth = " << recdepth << "  "
	   << "node->id = " << recnode->id << (recdepth==fulldepth ? " * " : "   ")
	   << "box: " << recboxes.back().v
	   << "," << recboxes.back().w
	   << endl;

  for (int i=0; i<pow(2,dim); i++)
  {
	if (down(i))
	{
	  print_rec();
	  up();
	}
  }  
}

// increase the depth by one
void Tree::subdivide()
{
  Box box;
  box.w = data.size_vector();
  data.set_size_vector(halve_vector(box.w)); // shrink boxes

  int oldlen = data.count();
  int num_children = pow(2,dim);
  data.resize(oldlen*num_children);

  // start from the end so we can write in place
  for (int i=oldlen-1; i>=0; i--)
  {
	data.node(i)->expand(dim);

	for (int c=num_children-1; c>=0; c--)
	{
	  int id = num_children*i+c;
	  //	  cout << "adding node " << id << " to old node " << i << endl;
	  data.set_node(id,data.node(i)->addchild(dim,c,id)); // add and wire simultaneously

	  box.v = data.corner_vector(i);			// ith corner vec
	  data.set_corner(id, box.get_corner(c));
	}
  }
  fulldepth++;
}  

void Tree::set_depth(int depth)
{
  if (depth < fulldepth)
	cerr << "Tree.set_depth: cannot go to a lower depth.  Use Tree.clear() first." << endl;
  fulldepth = depth;
}
  
// find box by point, returning its id or -1 if not found
int Tree::search(const Point &v)
{
  if (!rootbox.contains(v))
	return -1;

  top();						// reset recnode, recbox
  while (recdepth < fulldepth)
  {
	int i = recboxes.back().child_hit(v); // has to hit a child
	if (!down(i)) return -1;
  }
  return recnode->id;
}

// TODO: what does this need to return? success?
// currently returns -1 on failure to insert, new box num otherwise
int Tree::insert(const Point &v)
{
  if (!rootbox.contains(v))
	return -2;

  top();						// reset recnode, recbox

  bool inserted = false;
  while (recdepth < fulldepth)
  {
	int i = recboxes.back().child_hit(v);
	if (!down(i))
	{
	  recnode->addchild(dim,i);
	  inserted = true;
	}
  }

  if (inserted || fulldepth==0)	// add the new box! if depth==0, add the root box
  {
	data.add(recnode,recboxes.back().v);
	return recnode->id;
  }

  return -1;
}

vector<int> Tree::search(const Box &b)
{
  //  cout << "C++: tree.search(" << b << ")" << endl;

  vector<int> nums;

  if (fulldepth==0)
  {
    if (rootbox.intersects(b))
	  nums.push_back(root->id);		// add root
  }
  else
  {
	top();
	search_rec(b,nums);
  }
  
  return nums;
}
 
void Tree::search_rec(const Box &b, vector<int> &nums)
{
  if (recdepth == fulldepth)               // base case: at a leaf
  {
// 	cout << "C++: at leaf node ["
// 		 << recdepth << ","
// 		 << recnode->id << "]"
// 		 << endl;
    nums.push_back(recnode->id);
    return;
  }

  vector<int> children = recboxes.back().children_hit(b);

//   cout << "C++: children of node ["
// 	   << recdepth << ","
// 	   << recnode->id << "]:  "
// 	   << children << endl;

  for (int i=0; i<children.size(); i++)
  {
	if (down(children[i]))		// descend into children[i]th child
	{
	  search_rec(b,nums);
	  up();
	}
  }
}


vector<int> Tree::insert(const Box &b)
{
  vector<int> nums;

  if (!rootbox.intersects(b)) return nums;
  top();
  insert_rec(b,nums,false);
  return nums;
}
 
// bool inserted - whether this node was just inserted
void Tree::insert_rec(const Box &box, vector<int> &nums, bool inserted)
{
  if (recdepth == fulldepth)               // base case: at a leaf
  {
	if (inserted || fulldepth==0) // a child was added to a node, or at root
	{
	  data.add(recnode,recboxes.back().v);
      //	  nums.push_back(recnode->id);
	}
    return;
  }

  vector<int> children = recboxes.back().children_hit(box);
  for (int i=0; i<children.size(); i++)
  {
   	inserted = false;			// IMPORTANT: reset to false each time
	if (!down(children[i]))		// try to go to children[i]th child
	{							
	  recnode->addchild(dim,children[i]); // failed, so add the child
	  inserted = true;					  // just inserted this child
	  down(children[i]);		// now go to children[i]th child
	}

	insert_rec(box,nums,inserted);
	up();
  }
}

// remove box(es)
int Tree::remove(int id)
{
  remove(vector<int> (1,id));
  return 0;
}

int Tree::remove(vector<int> idvec)
{
  if (idvec.size() == 0)
	return 0;					// successfully removed nothing!
  sort(idvec.begin(),idvec.end());
  if (idvec.back() > count())
	return -1;		// exit softly if ids are out of range
  
  // do the actual deletions
  for (int i=0; i<idvec.size(); i++)
  {
	Node *node = data.node(idvec[i]);
	node->parent->delchild(node->childnum);
	// TODO: should nodes keep a child count and get destroyed if it's
	// 0?  or is it okay to have a lot of 'dangling' intermediate nodes?
  }

  // update the lists and ids
  data.remove(idvec);
  return 0;
}


inline void Tree::top()
{
  recnode = root;
  recboxes.clear();
  recboxes.push_back(rootbox);
  recdepth = 0;
}

inline void Tree::up()
{
  recnode = recnode->parent;
  recboxes.pop_back();
  recdepth--;
}

bool Tree::down(int i)
{
  if (!recnode->haschild(i))
	return false;
  recnode = recnode->children[i];
  
  // scale down the last box and add it to the end
  recboxes.push_back(recboxes.back().scale_down(i));
  recdepth++;
  return true;
}

/*
void Tree::subdivide()
{
  Data olddata = data;  
  data.reset(data.count()*(1<<dim));

  for (int b=0; b<olddata.count(); b++)
  {
	Node *node = olddata.node(b);
	node->expand(dim);
	for (int c=0; c<(1<<dim); c++)
	{
	  int id = (1<<dim)*b+c;
	  Node *child = olddata[i]->addchild(c,id);
	  
	  nodevec[id] =  // add and wire simultaneously

	}
  }

  scaleboxdown(boxsize);
  fulldepth++;
}  
*/
