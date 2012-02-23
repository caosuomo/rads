from cppdefs cimport *

cdef class Tree:
	cdef cTree *tree
	cpdef object subdivide(self,int subdivs=*)
	cpdef object set_depth(self,int)
	cdef cTree * get_tree(self)
