cimport numpy as np
import numpy as np
from cppdefs cimport *

cdef class UBoxSet:
	cdef np.ndarray corners
	cdef np.ndarray width
	cdef init(self,cUniformBoxSet ubs)
