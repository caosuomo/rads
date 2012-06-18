#include <iostream>
#include <cstdlib>
#include "treeutil.h"
#include "box.h"
#include "tree.h"

void test_remove(Tree &tree, int trials, int verbosity)
{
  for (int t=0; t<trials; t++)
  {
	int n = tree.count();
	if (n == 0) break;

	vector<int> ids;
	for (int i=0; i<n; i++)
	{
	  if ((double)rand()/RAND_MAX < 1.0/power(n,0.9))
		ids.push_back(i);
	}
	cout << "removing " << ids << endl;
	tree.remove(ids);
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
	  b.v[i] = ((double)rand()/RAND_MAX)*bbox.w[i]*(1-1.0/tree.dimension()) + bbox.v[i];
	  b.w[i] = ((double)rand()/RAND_MAX)*bbox.w[i]*(0.4);//(tree.dimension()-1)/tree.dimension();
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
  const int dim = 2;
  const int depth = 5;
  Point corner(dim,0), size(dim,8);
  Box bbox(corner,size);
  Tree tree(bbox);
  tree.insert(Point(dim,1));

  for (int d=0; d<depth; d++)
  {
	cout << "subdividing..." << endl;
	tree.subdivide();
  }

//   cout << "printing..." << endl;
//   tree.print();

  test_tree_box(tree, 1000, 1);
  //  test_remove(tree, 1000, 1);
  return 0;
}

// map test:
// 1. in matlab, compute transition matrix for henon at depth 16
// 2. print matrix, box centers in order
// 3. insert each box (in order!) into the tree, so the numbers are the same
// 4. compute the same transition matrix
// 5. check that the matrices are the same
