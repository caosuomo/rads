from libcpp.vector cimport vector

cdef extern from "point.h":
	ctypedef cppclass cPointBase[T] "PointBase":
		vector[T] v

		cPointBase()
		cPointBase(int size)
		cPointBase(int size, T &)
		T& operator[] (int i)
		int size()
		
	ctypedef cPointBase[double] Point
	ctypedef cPointBase[INTERVAL] IPoint
