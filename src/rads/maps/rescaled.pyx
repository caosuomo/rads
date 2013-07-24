from rads.enclosure.cppdefs cimport *
from rads.enclosure.cymapper cimport Mapper

cdef extern from "rescaled_cpp.h":
	cdef cppclass cRescaled "RescaledMapper":
		cRescaled()
		cIPoint map_point (cIPoint &v)
	cRescaled *new_Rescaled "new RescaledMapper" ()
	void del_Rescaled "delete" (cRescaled *)

cdef class RescaledMapper(Mapper):

	def __cinit__(self):
		print "in RescaledMapper cinit"
		self.mapper = <cMapper *>new_Rescaled()

	def __dealloc__(self):
		del_Rescaled(<cRescaled *>self.mapper)
	
