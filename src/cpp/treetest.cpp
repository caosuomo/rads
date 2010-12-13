#include <iostream>
#include <cstdlib>
#include "treeutil.h"
#include "box.h"
#include "tree.h"

void test_boxes()
{
//   vector<int> v(4);
//   cout << v[8] << endl;

  int dim = 2;
  Point v1(dim),v2(dim),w1(dim),w2(dim);

  v1[0] = 0;
  v1[1] = 0;

  w1[0] = 4;
  w1[1] = 3;

  v2[0] = 3;
  v2[1] = 1;

  w2[0] = 1;
  w2[1] = 1;

  Box b1(v1,w1);
  Box b2(v2,w2);
  Box b3 = b2;
  
  cout << "interval_intersect(2,4,3,5): "
	   << interval_intersect(2,4,3,5) << endl;
  
  cout << "b1.children_hit(b2): " << b1.children_hit(b2) << endl;
  cout << "b2.children_hit(b1): " << b2.children_hit(b1) << endl;

  cout << "b1.contains(v2): "
	   << b1.contains(v2) << endl;
  cout << "b2.contains(v1): "
	   << b2.contains(v1) << endl;

  cout << "b1.child_hit(v1): "
	   << b1.child_hit(v1) << endl;
  cout << "b1.child_hit(v2): "
	   << b1.child_hit(v2) << endl;
  cout << "b1.child_hit(w1): "
	   << b1.child_hit(w1) << endl;
}

void test_tree_vector(Tree &tree, int trials, int verbosity)
{
  Box bbox = tree.bounding_box();

  // search, remove, insert test!
  Point r(tree.dimension(),0);
  for (int t=0; t<trials; t++)
  {
	for (int i=0; i<r.size(); i++)
	  r[i] = ((double)rand()/RAND_MAX)*bbox.w[i] + bbox.v[i];

	int id = tree.search(r);
	switch (verbosity)
	{
	case 2:
	  cout << "search " << r << ": " << id << endl;
	  if (id >= 0)
		cout << "remove " << id << ": " << tree.remove(id) << endl;
	  else
		cout << "insert " << r << ": " << tree.insert(r) << endl;
	  break;
	case 1:
	  cout << "s" << id;
	  if (id >= 0)
		cout << "r" << tree.remove(id);
	  else
		cout << "i" << tree.insert(r);
	  break;
	case 0:
	  if (id >= 0)
		tree.remove(id);
	  else
		tree.insert(r);
	  break;
	}
  }

  if (verbosity==2)
  {
	cout << "printing..." << endl;
	tree.print();
  }
  else if (verbosity==1)
  {
	cout << endl;
  }
}

void test_tree_box(Tree &tree, int trials, int verbosity)
{
  Box bbox = tree.bounding_box();

  // search(Box), insert(Box) test!
  Box b(tree.dimension());
  for (int t=0; t<trials; t++)
  {
	for (int i=0; i<tree.dimension(); i++)
	{
	  b.v[i] = ((double)rand()/RAND_MAX)*bbox.w[i] + bbox.v[i] - bbox.w[i]/tree.dimension();
	  b.w[i] = ((double)rand()/RAND_MAX)*bbox.w[i]*(tree.dimension()-1)/tree.dimension();
	}
	vector<int> ids = tree.search(b);

	switch (verbosity)
	{
	case 2:
	  cout << "search " << b << ": " << ids << endl;
	  if (ids.size() > 4)
		cout << "remove " << ids << ": " << tree.remove(ids) << endl;
	  else
		cout << "insert " << b << ": " << tree.insert(b) << endl;
	  cout << "printing..." << endl;
	  tree.print();
	  break;
	case 1:
	  cout << "s" << ids;
	  if (ids.size() > 4)
		cout << "r" << tree.remove(ids);
	  else
		cout << "i" << tree.insert(b);
	  break;
	case 0:
	  if (ids.size() > 4)
		tree.remove(ids);
	  else
		tree.insert(b);
	  break;
	}
  }
  if (verbosity==1)
	cout << endl;
}

int main()
{
  cout << "hello, world" << endl;

  test_boxes();

  const int dim = 2;
  const int depth = 4;
  Point corner(dim,0), size(dim,pow(2,depth));
  Box bbox(corner,size);
  Tree tree(bbox);

  for (int d=0; d<depth; d++)
  {
	cout << "subdividing..." << endl;
	tree.subdivide();
  }
  cout << "printing..." << endl;
  tree.print();

  srand(5);						// keep it deterministic, silly
  test_tree_vector(tree,pow(2,dim*depth-1),0);
  test_tree_box(tree,20,0);

  Tree tree1 = tree;
  Tree tree2 = tree;

  srand(10);
  test_tree_vector(tree1,10,1);
  srand(10);
  test_tree_vector(tree2,10,1);

  srand(10);
  test_tree_box(tree1,10,1);
  srand(10);
  test_tree_box(tree2,10,1);
  return 0;
}

// map test:
// 1. in matlab, compute transition matrix for henon at depth 16
// 2. print matrix, box centers in order
// 3. insert each box (in order!) into the tree, so the numbers are the same
// 4. compute the same transition matrix
// 5. check that the matrices are the same
