from libcpp.vector cimport vector
cimport numpy as np
import numpy as np
from cppdefs cimport *
from cyutils cimport *
from cyboxset cimport UBoxSet

cdef class Tree:
	def __cinit__(self,np.ndarray box,int depth=0,full=False):
		self.tree = new_Tree(array2box(box))
		if full:
			self.insert(self.bbox)
			for i in range(depth):
				self.subdivide()
		else:
			self.set_depth(depth)
			
	def __dealloc__(self):
		del_Tree(self.tree)

	def insert(self,np.ndarray a):
		if a.ndim == 2:
			return vector2array_int(self.tree.insert(array2box(a)))
		else:
			return self.tree.insert(array2point(a))

	def insert_box(self,np.ndarray a):
		return vector2array_int(self.tree.insert(array2box(a)))

	def remove(self,ids):
		return self.tree.remove(array2vector_int(ids))

	def search(self,np.ndarray a):
		if a.ndim == 2:
			return vector2array_int(self.tree.search(array2box(a)))
		else:
			return self.tree.search(array2point(a))

	def search_box(self,np.ndarray a):
		return vector2array_int(self.tree.search(array2box(a)))

	def boxes(self):
		b = UBoxSet()
		b.init(self.tree.boxes())
		return b

	cpdef object subdivide(self,int subdivs=1):
		for i in range(subdivs):
			self.tree.subdivide()

	cpdef object set_depth(self,int d):
		self.tree.set_depth(d)
	
	cdef cTree * get_tree(self):
		return self.tree

	property bbox:
		def __get__(self):
			return box2array(self.tree.bounding_box())
		
	property dim:
		def __get__(self):
			return self.tree.dimension()
		
	property depth:
		def __get__(self):
			return self.tree.depth()
		
	property size:
		def __get__(self):
			return self.tree.count()
		
	def __repr__(self):
		self.tree.print_tree()
		return "Tree object"
