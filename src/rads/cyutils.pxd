cimport numpy as np
import numpy as np
from cppdefs cimport *

#------------------------------
# Helpers
#------------------------------

cdef cPoint array2point(np.ndarray a)
cdef cBox array2box(np.ndarray a)
cdef np.ndarray[np.double_t,ndim=1] point2array(cPoint p)
cdef np.ndarray[np.double_t,ndim=2] box2array(cBox b)
cdef np.ndarray[np.long_t] vector2array_int(vector[int] v)
#cdef vector[int] array2vector_int(np.ndarray[int_t,ndim=1] o)
cdef vector[int] array2vector_int(object o)
cdef np.ndarray[np.double_t] vector2array(vector[double] v)
cdef np.ndarray[np.double_t] interval2array(cIVAL &v)
cdef cIVAL array2interval(np.ndarray a)
