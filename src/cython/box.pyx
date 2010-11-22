from libcpp.vector cimport vector

cdef extern from "box.h":
	ctypedef struct cPoint "Point":
		pass
	ctypedef struct cIPoint "IPoint":
		pass
	
	ctypedef cppclass cBox "Box":
		cPoint v;
		cPoint w;
		
		cBox()
		cBox(int dimension)
		cBox(cPoint &v, cPoint &w)
		cBox(cIPoint &i)
		int size()

cdef class Box:
	cdef cBox box	   # hold a C++ instance which we're wrapping
	def __cinit__(self, int dim):
		self.box = cBox(dim)

	def size():
		return self.box.size()

	property v:
		def __get__(self): return self.box.v
		def __set__(self, cPoint &v): self.box.v = v
	property w:
		def __get__(self): return self.box.w
		def __set__(self, cPoint &w): self.box.w = w
		
	def __repr__(self):
		return "["+self.box.v+","+self.box.w+"]"
