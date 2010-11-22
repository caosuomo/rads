from libcpp.vector cimport vector
cimport numpy as np

cdef extern from "tree.h":
	cdef cppclass cPoint "Point":
		cPoint()
		cPoint(int)
		double& operator[] (int)
	cdef cppclass cBox "Box":
		cPoint v
		cPoint w
		cBox()
		cBox(cPoint &, cPoint&)
	
	cdef cppclass cTree "Tree":
		cTree(cBox &)
		cTree(cTree &)

		void subdivide()
  
		int search(cPoint &)
		vector[int] search(cBox &)

		int insert(cPoint &)
		vector[int] insert(cBox &)

		int remove(int)
		int remove(vector[int])

		int dimension()
		int depth()
		int count()
#		cUniformBoxSet boxes()
		cBox bounding_box()

	cTree *new_Tree "new Tree" (cBox&)
	cTree *new_Tree "new Tree" (cTree&)
	void del_Tree "delete" (cTree *tree)

cdef cPoint a2point(np.ndarray a):
	cdef cPoint p
	p = cPoint(a.shape[1])
	for i from 0 <= i < a.shape[1]:
		p[i] = a[i]
	return p
	
cdef cBox a2box(np.ndarray a):
	cdef cBox b
	b = cBox( a2point(a[0]), a2point(a[1]) )
	return b

cdef class Tree:
	cdef cTree *tree	   # hold a C++ instance which we're wrapping
	def __cinit__(self,np.ndarray a):
		self.tree = new_Tree(a2box(a))
	def __dealloc__(self):
		print "deleting tree!"
		del_Tree(self.tree)
	def insert(self,np.ndarray a):
		self.tree.insert(a2box(a))
	def subdivide(self):
		self.tree.subdivide()
	def count(self):
		return self.tree.count()
	def depth(self):
		return self.tree.depth()
		
	def __repr__(self):
		return "a Tree"
