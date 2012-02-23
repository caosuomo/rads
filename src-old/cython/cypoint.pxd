cimport numpy as np

cdef extern from "point.h":
	cdef cppclass cPoint "Point":
		cPoint()
		cPoint(int)
		void set(int,double&)
		double& operator[] (int)
		int size()		
	cPoint *new_Point "new Point" (int)
	void del_Point "delete" (cPoint *)

cdef class Point:
	cdef cPoint *point	   # hold a C++ instance which we're wrapping
