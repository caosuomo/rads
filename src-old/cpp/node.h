#ifndef _node_h
#define _node_h

typedef unsigned int uint;

class Node
{
public:
  //  int coord;				// which coord this is split
  int id;						// node id, for the leaves only
  int childnum;					// this is parent->children[childnum]
  Node *parent;
  Node **children;
 
  Node(Node *myparent, int mychildnum, int myid=0)
  {
	parent = myparent;
	id = myid;
	childnum = mychildnum;
	children = 0;
  }

  ~Node()
  {
	if (children != 0)
	  delete [] children;
  }

  void expand(int dim)
  {
	children = new Node *[1<<dim];
	for (int i=0; i<(1<<dim); i++)
	  children[i] = 0;
  }

  Node *addchild(int dim, int mychildnum, int myid=0)
  {
	if (!children) expand(dim);
	children[mychildnum] = new Node(this, mychildnum, myid);
	return children[mychildnum];
  }

  void delchild(int child)
  {
	delete children[child];
	children[child] = 0;
  }

  bool haschild(int child)
  {
	return (children && children[child]);
  }
};

#endif
