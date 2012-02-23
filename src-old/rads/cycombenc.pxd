from cppdefs cimport *

cdef class CombEnc:
	cdef cMapper * cmapper
	cdef cTree * ctree
	cdef object pymapper
	cdef object pytree
	cpdef update(self)
	cdef object adj
	cdef object mvm
	cdef void compute_adj(self,cUniformBoxSet &ubs)
	cdef void compute_mvm(self,cUniformBoxSet &ubs)
