#ifndef _tree_h
#define _tree_h

#include "treeutil.h"
#include "node.h"
#include "point.h"
#include "boxset.h"
#include "treedata.h"

class Tree
{
  // fixed vars
  int dim;						// dimension
  Node *root;					// root of the tree
  Box rootbox;					// bounding box

  // loop vars
  Node *recnode;				// node storage for recursion
  vector<Box> recboxes;			// box storage for recursion
  int recdepth;					// depth for recursion

  // state vars
  int fulldepth;				// height of the tree
  TreeData data;				// list of boxes, nodes

public:
  Tree(const Box &b);
  Tree(const Tree &t);
  ~Tree();

  // increase the depth by one, subdividing all boxes
  void subdivide();
  void set_depth(int depth);
  
  // find box(es), by point or interval
  int search(const Point &v);
  vector<int> search(const Box &b);

  // insert box(es), by point or interval
  // TODO: the Point could hit multiple boxes, but currently only one
  // will be inserted
  int insert(const Point &v);
  vector<int> insert(const Box &b);

  // remove box(es)
  int remove(int box);
  int remove(vector<int> boxvec);


  // tree state info
  int dimension() { return dim; };
  int depth() { return fulldepth; };
  int count() { return data.count(); };  // number of leaves
  UniformBoxSet boxes() { return data.get_boxes(); }		   // the leaf boxes
  Box bounding_box() { return rootbox; };   // size of the leaf boxes
  void print();

  // input / output
  //  void save(FILE *out);
  //  static Tree *load(FILE *in);

private:
  void del(Node *n);		// destructor helper
  // loop funcs:
  void top();				// reset to root of tree
  void up();				// ascend to the parent of recnode
  bool down(int i);			// descend to child i (return success)
  void search_rec(const Box &b, vector<int> &nums);
  void insert_rec(const Box &b, vector<int> &nums, bool ins);
  void print_rec();
  void copy_rec(Node *to, Node *from);
};

#endif
