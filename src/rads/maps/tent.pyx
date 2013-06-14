from rads.enclosure.cppdefs cimport *
from rads.enclosure.cymapper cimport Mapper

cdef extern from "tent_cpp.h":
	cdef cppclass cTent "TentMapper":
		cTent()
		cIPoint map_point (cIPoint &v)
	cTent *new_Tent "new TentMapper" ()
	void del_Tent "delete" (cTent *)

cdef class TentMapper(Mapper):

	def __cinit__(self):
		print "in TentMapper cinit"
		self.mapper = <cMapper *>new_Tent()

	def __dealloc__(self):
		del_Tent(<cTent *>self.mapper)
	
