#ifndef _treedata_h
#define _treedata_h
#include <vector>
#include "node.h"
#include "boxset.h"
using namespace std;

class TreeData
{
private:
  vector<Point > boxes;
  vector<Node *> nodes;
  Point size;

  // helpers for remove
  void move_node(int from, int to);
  void move_box(int from, int to);

public:
  TreeData() {};
  TreeData(const Point &mysize);

  // manipulators
  void add(Node *n, const Point &v);
  void remove(vector<int> ids);
  void resize(int s);

  // setters
  void set_node(int id, Node *n) { nodes[id] = n; }
  void set_node(Node *n) { nodes[n->id] = n; }
  void set_corner(int id, const Point &v) { boxes[id] = v; }
  void set_size_vector(const Point &v) { size = v; }

  // accessors
  Point size_vector() const { return size; }
  Point corner_vector(int id) const { return boxes[id]; }
  Node *node(int id) const { return nodes[id]; }
  int count() const { return nodes.size(); }
  int dim() const { return size.size(); }
  UniformBoxSet get_boxes() const { return UniformBoxSet(size,boxes); } 
};

#endif
