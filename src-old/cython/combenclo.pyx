from libcpp.vector cimport vector

cdef vector[int] vect
cdef int i
for i in range(10):
	vect.push_back(i)
for i in range(10):
	print vect[i]

cdef extern from "tree.h":
	ctypedef struct c_Tree "Tree":
		int x0, y0, x1, y1
		int getLength()
		int getHeight()
		int getArea()
		void move(int dx, int dy)
		c_Tree add "operator+"(c_Tree right)
	c_Tree *new_Tree "new Tree" (int x0, int y0, int x1, int y1)
	void del_Tree "delete" (c_Tree *rect)

cdef class Tree:
	cdef c_Tree *thisptr	   # hold a C++ instance which we're wrapping
	def __cinit__(self, int x0, int y0, int x1, int y1):
		self.thisptr = new_Tree(x0, y0, x1, y1)
	def __dealloc__(self):
		del_Tree(self.thisptr)
	def getLength(self):
		return self.thisptr.getLength()
	def getHeight(self):
		return self.thisptr.getHeight()
	def getArea(self):
		return self.thisptr.getArea()
	def move(self, dx, dy):
		self.thisptr.move(dx, dy)
		
	def __add__(Tree left, Tree right):
		cdef c_Tree c = left.thisptr.add(right.thisptr[0])
		cdef Tree sum = Tree(c.x0, c.y0, c.x1, c.y1)
		return sum
		
	def __repr__(self):
		return "a Tree"

"""
cdef Tree r = Tree(1, 2, 3, 4)
print r
print "Original area:", r.getArea()
r.move(1,2)
print r
print "Area is invariante under rigid motions:", r.getArea()
r += Tree(0,0,1,1)
print r
print "Now the aread is:", r.getArea()
"""

