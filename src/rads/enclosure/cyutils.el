;; This is an elisp-readable form of Cython annotation data -- a list of
;; annotations for each source line, where an annotation is a cons of
;; (score . code).

'(
  ;; line 1: cimport numpy as np
  (0 . "")
  ;; line 2: import numpy as np
  (0 . "")
  ;; line 3: from cppdefs cimport *
  (0 . "")
  ;; line 4: 
  (0 . "")
  ;; line 5: #------------------------------
  (0 . "")
  ;; line 6: # Helpers
  (0 . "")
  ;; line 7: #------------------------------
  (0 . "")
  ;; line 8: 
  (0 . "")
  ;; line 9: cdef cPoint array2point(np.ndarray a):
  (0 . "")
  ;; line 10: cdef cPoint p = cPoint(a.shape[0])
  (0 . "")
  ;; line 11: cdef Py_ssize_t i
  (0 . "")
  ;; line 12: for i in range(a.shape[0]):
  (0 . "")
  ;; line 13: p.set(i,<double>a[i])
  (0 . "")
  ;; line 14: return p
  (0 . "")
  ;; line 15: 
  (0 . "")
  ;; line 16: # cdef np.ndarray[double_t,ndim=1] point2array(cPoint &p):
  (0 . "")
  ;; line 17: # 	cdef np.ndarray[double_t,ndim=1] a = np.zeros((p.size()),dtype=np.double)
  (0 . "")
  ;; line 18: # 	cdef Py_ssize_t i
  (0 . "")
  ;; line 19: # 	for i in range(p.size()):
  (0 . "")
  ;; line 20: # 		a[i] = p[i]
  (0 . "")
  ;; line 21: # 	return a
  (0 . "")
  ;; line 22: 
  (0 . "")
  ;; line 23: cdef np.ndarray[double_t,ndim=1] point2array(cPoint p):
  (0 . "")
  ;; line 24: cdef np.ndarray a = np.zeros((p.size()),dtype=np.double)
  (0 . "")
  ;; line 25: cdef Py_ssize_t i
  (0 . "")
  ;; line 26: for i in range(p.size()):
  (0 . "")
  ;; line 27: a[i] = p[i]
  (0 . "")
  ;; line 28: return a
  (0 . "")
  ;; line 29: 
  (0 . "")
  ;; line 30: cdef cBox array2box(np.ndarray a):
  (0 . "")
  ;; line 31: cdef cBox b
  (0 . "")
  ;; line 32: b.v = array2point(a[0])
  (0 . "")
  ;; line 33: b.w = array2point(a[1])
  (0 . "")
  ;; line 34: return b
  (0 . "")
  ;; line 35: 
  (0 . "")
  ;; line 36: cdef np.ndarray[np.long_t] vector2array_int(vector[int] v):
  (0 . "")
  ;; line 37: cdef np.ndarray[np.long_t,ndim=1] a = np.zeros((v.size()),dtype=np.long)
  (0 . "")
  ;; line 38: cdef Py_ssize_t i
  (0 . "")
  ;; line 39: for i in range(v.size()):
  (0 . "")
  ;; line 40: a[i] = <np.long_t>v[i]
  (0 . "")
  ;; line 41: return a
  (0 . "")
  ;; line 42: 
  (0 . "")
  ;; line 43: cdef np.ndarray[np.double_t] vector2array(vector[double] v):
  (0 . "")
  ;; line 44: cdef np.ndarray[np.double_t,ndim=1] a = np.zeros((v.size()),dtype=np.double)
  (0 . "")
  ;; line 45: cdef Py_ssize_t i
  (0 . "")
  ;; line 46: for i in range(v.size()):
  (0 . "")
  ;; line 47: a[i] = <np.double_t>v[i]
  (0 . "")
  ;; line 48: return a
  (0 . "")
  ;; line 49: 
  (0 . "")
  ;; line 50: cdef np.ndarray[np.double_t] interval2array(cIVAL &v):
  (0 . "")
  ;; line 51: cdef np.ndarray[np.double_t,ndim=1] a = np.zeros(2,dtype=np.double)
  (0 . "")
  ;; line 52: a[0] = Inf(v)
  (0 . "")
  ;; line 53: a[1] = Sup(v)
  (0 . "")
  ;; line 54: return a
  (0 . "")
  ;; line 55: 
  (0 . "")
  ;; line 56: cdef cIVAL array2interval(np.ndarray a):
  (0 . "")
  ;; line 57: return cIVAL(a[0],a[1])
  (0 . "")
  ;; line 58: 
  (0 . "")
)
