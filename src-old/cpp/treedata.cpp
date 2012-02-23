#include <algorithm>
#include "treeutil.h"
#include "treedata.h"

TreeData::TreeData(const Point &mysize)
{
  size = mysize;
}

void TreeData::add(Node *n, const Point &v)
{
  n->id = count();
  boxes.push_back(v);
  nodes.push_back(n);
}

inline void TreeData::move_node(int from, int to)
{
  nodes[to] = nodes[from]; // put the pointer in the right place
  nodes[to]->id = to;	   // the new id is the array index
}

inline void TreeData::move_box(int from, int to)
{
  boxes[to] = boxes[from];
}

// assumes ids is sorted!
void TreeData::remove(vector<int> ids)
{
  int num_gone = ids.size();

  // update the lists and ids
  ids.push_back(nodes.size()); // add last elt for the loop

  for (int i=0; i<num_gone; i++)
  {
	for (int j = ids[i]; j < ids[i+1]-1; j++)
	{
	  move_node(j+1,j-i);
	  move_box(j+1,j-i);
	}
  }
  nodes.resize(nodes.size()-num_gone);
  boxes.resize(boxes.size()-num_gone);
}

void TreeData::resize(int s)
{
  nodes.resize(s);
  boxes.resize(s);
}
