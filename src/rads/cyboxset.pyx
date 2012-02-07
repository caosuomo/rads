cimport numpy as np
import numpy as np
from cppdefs cimport *
from cyutils cimport *

cdef class UBoxSet:
	def __cinit__(self):
		pass

	cdef void init(self,cUniformBoxSet ubs):
		self.width = point2array(ubs.get_width())
		self.corners = np.zeros((ubs.size(),ubs.dim()),dtype=np.double)
		cdef Py_ssize_t i
		for i in range(len(self.corners)):
			self.corners[i] = point2array(ubs.get_corner(i))

	property dim:
		def __get__(self):
			return len(self.width)

	property size:
		def __get__(self):
			return self.corners.shape[0]

	property width:
		def __get__(self):
			return self.width
		
	property corners:
		def __get__(self):
			return self.corners
		
	def __repr__(self):
		return "width: " + self.width.__repr__() + "\ncorners:\n" + self.corners.__repr__()

cdef class BoxSet:
	def __cinit__(self):
		pass

	cdef void init(self,cBoxSet bs):
		self.corners = np.zeros((bs.size(),bs.dim()),dtype=np.double)
		self.widths = np.zeros((bs.size(),bs.dim()),dtype=np.double)
		cdef Py_ssize_t i
		cdef cBox b
		for i in range(bs.size()):
			b = bs.get_box(i)
			self.corners[i] = point2array(b.v)
			self.widths[i] = point2array(b.w)

	property dim:
		def __get__(self):
			return self.widths.shape[1]

	property size:
		def __get__(self):
			return self.widths.shape[0]

	property widths:
		def __get__(self):
			return self.widths
		
	property corners:
		def __get__(self):
			return self.corners
		
	def __repr__(self):
		return "width: " + self.width.__repr__() + "\ncorners:\n" + self.corners.__repr__()
