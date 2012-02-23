cimport numpy as np
import numpy as np
from cppdefs cimport *

cdef class UBoxSet:
	cdef np.ndarray corners
	cdef np.ndarray width
	cdef void init(self,cUniformBoxSet ubs)

cdef class BoxSet:
	cdef np.ndarray boxes
	cdef void init(self,cBoxSet ubs)
