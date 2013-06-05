from rads.enclosure.cppdefs cimport *
from rads.enclosure.cymapper cimport Mapper

cdef extern from "ricker_cpp.h":
	cdef cppclass cRicker "RickerMapper":
		cRicker()
		cIPoint map_point (cIPoint &curr)

	cRicker *new_Ricker "new RickerMapper" ()
	void del_Ricker "delete" (cRicker *)

cdef class RickerMapper(Mapper):

	def __cinit__(self):
		print "in RickerMapper cinit"
		self.mapper = <cMapper *>new_Ricker()

	def __dealloc__(self):
		del_Ricker(<cRicker *>self.mapper)
	
