from cppdefs cimport *

cdef class Tree:
	cdef cTree *tree
	cdef vector[int] cinsert(self,cBox &b)
	cpdef int remove(self,np.ndarray a)
	cpdef void subdivide(self)
	cpdef int count(self)
	cpdef int depth(self)

