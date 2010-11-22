from libcpp.vector cimport vector

cdef extern from "box.h":
	ctypedef struct cPoint "Point":
		pass
	ctypedef struct cIPoint "IPoint":
		pass
	
	cdef cppclass cBox "Box":
		cPoint v
		cPoint w
#		cBox()
		cBox(int)
#		cBox(cPoint &v, cPoint &w)
#		cBox(cIPoint &i)
		int size()
	cBox *new_Box "new Box" (int)
	void del_Box "delete" (cBox *)

cdef class Box:
	cdef cBox *box	   # hold a C++ instance which we're wrapping
	def __cinit__(self, int dim):
		self.box = new_Box(dim)

	def __dealloc__(self):
		del_Box(self.box)

	def size(self):
		return self.box.size()

	property v:
		def __get__(self): return self.box.v
#		def __set__(self, cPoint &v): self.box.v = v
	property w:
		def __get__(self): return self.box.w
#		def __set__(self, cPoint &w): self.box.w = w
		
	def __repr__(self):
		return "["+self.box.v+","+self.box.w+"]"
