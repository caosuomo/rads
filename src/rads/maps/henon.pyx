from rads.enclosure.cppdefs cimport *
from rads.enclosure.cymapper cimport Mapper

cdef extern from "henon_cpp.h":
	cdef cppclass cHenon "HenonMapper":
		cHenon()
		cIPoint map_point (cIPoint &v)
	cHenon *new_Henon "new HenonMapper" ()
	void del_Henon "delete" (cHenon *)

cdef class HenonMapper(Mapper):

	def __cinit__(self):
		print "in HenonMapper cinit"
		self.mapper = <cMapper *>new_Henon()

	def __dealloc__(self):
		del_Henon(<cHenon *>self.mapper)
	
