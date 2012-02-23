from libcpp.vector cimport vector
cimport numpy as np
import numpy as np
from cppdefs cimport *
from cyutils cimport *
from cyboxset cimport UBoxSet

cdef class Tree:
	def __cinit__(self,np.ndarray a):
		self.tree = new_Tree(array2box(a))
	def __dealloc__(self):
		del_Tree(self.tree)

	def insert(self,np.ndarray a):
		if a.ndim == 2:
			return vector2array_int(self.tree.insert(array2box(a)))
		else:
			return self.tree.insert(array2point(a))

	def search(self,np.ndarray a):
		if a.ndim == 2:
			return vector2array_int(self.tree.search(array2box(a)))
		else:
			return self.tree.search(array2point(a))

	def boxes(self):
		b = UBoxSet()
		b.init(self.tree.boxes())
		return b

	cdef vector[int] cinsert(self,cBox &b):
		return self.tree.insert(b)

	cpdef int remove(self,np.ndarray a):
		return self.tree.remove(array2vector_int(a))

	cpdef void subdivide(self):
		self.tree.subdivide()
	
	cpdef int count(self):
		return self.tree.count()

	cpdef int depth(self):
		return self.tree.depth()
		
	def __repr__(self):
		self.tree.print_tree()
		return "Tree object"
