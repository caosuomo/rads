cimport numpy as np
import numpy as np
from cppdefs cimport *

#------------------------------
# Helpers
#------------------------------

cdef cPoint array2point(np.ndarray a)
cdef np.ndarray[double_t,ndim=1] point2array(cPoint p)
cdef cBox array2box(np.ndarray a)
cdef np.ndarray[np.long_t] vector2array_int(vector[int] v)
cdef np.ndarray[np.double_t] vector2array(vector[double] v)
