from cppdefs cimport *

cdef class Mapper:
	cdef cMapper *mapper
	cdef cMapper *get_mapper(self)
	cpdef int get_dim(self)
#	cpdef char * get_name(self)
