cimport numpy as np
import numpy as np
from cppdefs cimport *
from cyutils cimport *

cdef class UBoxSet:
	def __cinit__(self):
		pass

	cdef init(self,cUniformBoxSet ubs):
		self.width = point2array(ubs.get_width())
		print self.width
		self.corners = np.zeros((ubs.size(),len(self.width)),dtype=np.double)
		cdef Py_ssize_t i
		for i in range(len(self.corners)):
			self.corners[i] = point2array(ubs.get_corner(i))
		
	def __repr__(self):
		return "width: " + self.width.__repr__() + "\ncorners:\n" + self.corners.__repr__()

