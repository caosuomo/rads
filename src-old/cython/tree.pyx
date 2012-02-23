from libcpp.vector cimport vector
cimport numpy as np
import numpy as pnp
from cppdefs cimport *
from utils cimport *

#------------------------------
# Cython classes
#------------------------------

cdef class UBoxSet:
	cdef np.ndarray corners
	cdef np.ndarray width

	def __cinit__(self):
		pass

	cdef init(self,cUniformBoxSet ubs):
		self.width = point2array(ubs.get_width())
		print self.width
		self.corners = pnp.zeros((ubs.size(),len(self.width)),dtype=pnp.double)
		cdef Py_ssize_t i
		for i in range(len(self.corners)):
			self.corners[i] = point2array(ubs.get_corner(i))
		
	def __repr__(self):
		return "width: " + self.width.__repr__() + "\ncorners:\n" + self.corners.__repr__()


cdef class Tree:
	cdef cTree *tree
	def __cinit__(self,np.ndarray a):
		self.tree = new_Tree(array2box(a))
	def __dealloc__(self):
		del_Tree(self.tree)

	cdef vector[int] cinsert(self,cBox &b):
		return self.tree.insert(b)

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

	cpdef object subdivide(self):
		self.tree.subdivide()
	
	cpdef int count(self):
		return self.tree.count()

	cpdef int depth(self):
		return self.tree.depth()
		
	def __repr__(self):
		self.tree.print_tree()
		return "Tree object"
