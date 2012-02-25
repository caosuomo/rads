cimport numpy as np
import numpy as np
from cppdefs cimport *

#------------------------------
# Helpers
#------------------------------

cdef cPoint array2point(np.ndarray a):
	cdef cPoint p = cPoint(a.shape[0])
	cdef Py_ssize_t i
	for i in range(a.shape[0]):
		p.set(i,<double>a[i])
	return p

# cdef np.ndarray[np.double_t,ndim=1] point2array(cPoint &p):
# 	cdef np.ndarray[np.double_t,ndim=1] a = np.zeros((p.size()),dtype=np.double)
# 	cdef Py_ssize_t i
# 	for i in range(p.size()):
# 		a[i] = p[i]
# 	return a

cdef np.ndarray[np.double_t,ndim=1] point2array(cPoint p):
	cdef np.ndarray a = np.zeros((p.size()),dtype=np.double)
	cdef Py_ssize_t i
	for i in range(p.size()):
		a[i] = p[i]
	return a

cdef cBox array2box(np.ndarray a):
	cdef cBox b
	b.v = array2point(a[0])
	b.w = array2point(a[1])
	return b

cdef np.ndarray[np.double_t,ndim=2] box2array(cBox b):
	cdef np.ndarray a = np.zeros((2,b.size()),dtype=np.double)
	a[0] = point2array(b.v)
	a[1] = point2array(b.w)
	return a

cdef np.ndarray[np.long_t] vector2array_int(vector[int] v):
	cdef np.ndarray[np.long_t,ndim=1] a = np.zeros((v.size()),dtype=np.long)
	cdef Py_ssize_t i
	for i in range(v.size()):
		a[i] = <np.long_t>v[i]
	return a

cdef np.ndarray[np.double_t] vector2array(vector[double] v):
	cdef np.ndarray[np.double_t,ndim=1] a = np.zeros((v.size()),dtype=np.double)
	cdef Py_ssize_t i
	for i in range(v.size()):
		a[i] = <np.double_t>v[i]
	return a

cdef np.ndarray[np.double_t] interval2array(cIVAL &v):
	cdef np.ndarray[np.double_t,ndim=1] a = np.zeros(2,dtype=np.double)
	a[0] = Inf(v)
	a[1] = Sup(v)
	return a

cdef cIVAL array2interval(np.ndarray a):
	return cIVAL(a[0],a[1])

cdef vector[int] array2vector_int(object lst):
	cdef vector[int] v
	cdef Py_ssize_t i
	for i in range(len(lst)):
		v.push_back(<int?>lst[i])
	return v
	
